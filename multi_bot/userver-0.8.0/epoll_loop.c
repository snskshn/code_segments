/*
    userver -- (pronounced you-server or micro-server).
    This file is part of the userver, a high-performance web server designed for
    performance experiments.

    Copyright (C) 2005-2010 Tim Brecht
    Based on the file originally Copyright (C) 2004  Hewlett-Packard Company

    Authors: Tim Brecht <brecht@cs.uwaterloo.ca>
    See AUTHORS file for list of contributors to the project.
  
    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of the
    License, or (at your option) any later version.
  
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.
  
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
    02111-1307 USA
*/


#ifdef HAVE_EPOLL

#define EXTRA_FDS     (50)

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <time.h>
/* Needed for syscall definitions */
#include <asm/unistd.h>

/* Change these when glibc with epoll is installed */
/* #include <sys/epoll.h> */
#ifdef IA64
/* Uncomment this if you can point to the real deal */
/* #include <asm-ia64/unistd.h> */
#endif /* IA64 */
#ifdef PROVIDE_EPOLL_HEADERS
#include "sys_epoll.h"
#else
#include <sys/epoll.h>
#endif

//#define LOCAL_DEBUG

#ifdef LOCAL_DEBUG
#include <sys/mman.h>
#endif /* LOCAL_DEBUG */

#include "interest_set.h"
#include "options.h"
#include "debug.h"
#include "stats.h"
#include "cntl_intr.h"
#include "cntl_conns.h"
#include "lru.h"
#include "q.h"
#include "interactive_mode.h"
#include "do_new_connections.h"
#include "trace.h"
#include "util.h"
#include "sock_listener.h"

#define MAX_PROCS   (1000)

void check_and_record_epoll_errors(int epoll_errno);
int process_epoll_events(int num);
static int max_usable_sd;

/* this array is used to track all the open descriptors
 * and what state they are in
 */
struct epoll_event *epoll_array = 0;

/* this gets filled in by the kernel by epoll_wait indicating new events */
struct epoll_event *epoll_results = 0;
int *epoll_map = 0;
int epoll_fd = -1;
int epoll_fd_array[MAX_PROCS];
int max_epoll_fd = 0;
int max_epoll_entry = 0;
struct epoll_event_ctlv *new_epoll_ctlv_evts = 0;
int new_epoll_ctlv_count = 0;

void
epoll_loop_init(void)
{
  int i;
  int entries = max_fds + EXTRA_FDS;
  int size = entries * sizeof(struct epoll_event);

  printf("&epoll_array = %p\n", &epoll_array);
  printf("&epoll_results = %p\n", &epoll_results);

  epoll_array = (struct epoll_event *) alloc_aligned(cache_line_size, size,
      ALLOC_MALLOC, "epoll_loop_init: (epoll_array)");
  printf("epoll_loop_init: epoll_array has %d entries and used %d bytes\n",
    max_fds, size);
  printf("epoll_loop_init: epoll_array = %p\n", epoll_array);
  printf("epoll_loop_init: &epoll_array[0] = %p\n", &epoll_array[0]);

  epoll_results = (struct epoll_event *) alloc_aligned(cache_line_size, size,
      ALLOC_MALLOC, "epoll_loop_init: (epoll_results)");
  printf("epoll_loop_init: epoll_results has %d entries and used %d bytes\n",
	    max_fds, size);

  size = entries * sizeof(int);
  epoll_map = (int *) alloc_aligned(cache_line_size, size, ALLOC_MALLOC,
      "epoll_loop_init: (epoll_map)");
  printf("epoll_loop_init: epoll_map has %d entries and used %d bytes\n",
	    max_fds, size);
 
#ifdef HAVE_EPOLL_CTLV
  size = entries * sizeof(struct epoll_event_ctlv);
  new_epoll_ctlv_evts = alloc_aligned(cache_line_size, size, ALLOC_MALLOC,
      "epoll_loop_init: (new_epoll_ctlv_evts)");
  printf("epoll_loop_init: new_epoll_ctlv_evts has %d entries "
	   "and used %d bytes\n", max_fds, size);
  new_epoll_ctlv_count = 0;
#endif /* HAVE_EPOLL_CTLV */

  /* initialize the arrays */
  for (i=0; i<max_fds; i++) {
    epoll_map[i] = -1;
    epoll_array[i].data.fd = -1;
    epoll_array[i].events = 0;
#ifdef HAVE_EPOLL_CTLV
    new_epoll_ctlv_evts[i].op = -1;
    new_epoll_ctlv_evts[i].res = 0;
    new_epoll_ctlv_evts[i].event.data.fd = -1;
    new_epoll_ctlv_evts[i].event.events = -1;
#endif /* HAVE_EPOLL_CTLV */
  }

  interest_set_init(DECLARE_WITH_EPOLL);


  for (i=0; i<numprocs; i++) {
#ifdef PROVIDE_EPOLL_HEADERS
    epoll_fd_array[i] = sys_epoll_create(max_fds);
#else
    epoll_fd_array[i] = epoll_create(max_fds);
#endif
    if (epoll_fd_array[i] < 0) {
      perror("epoll_loop_init: epoll_create : ");
      printf("epoll_loop_init: epoll_create failed\n");
      exit(1);
    } else {
      printf("epoll_loop_init: epoll_fd_array[%d] = %d\n", i, epoll_fd_array[i]);
    }
  }

  if (numprocs == 1) {
    epoll_fd = epoll_fd_array[0];
  }
}

void
epoll_loop (void)
{
  extern int sigio_pending();
  extern int my_proc_id;
  int n;
  int rc;
  int epoll_errno = 0;
  int trace_fd = 0;
  int save_errno;
  int timeout_msec = 0;
  int do_continue = 0;
  int did_something = 0;

  /* to stop compiler from complaining */
  save_errno = 0;

  errno = 0;
  // epoll_fd = epoll_fd_array[my_proc_id];

  printf("Entered server/epoll loop\n");
  printf("Server copy %d using epoll_fd = %d\n", my_proc_id, epoll_fd);
  assert(epoll_fd > 0);
  fflush(stdout);
  fflush(stderr);


  while (1) {

      do_continue = server_loop_top();
      if (do_continue) {
         continue;
      }

      TRACE_START_TIME(EVT_EVENT_PHASE);
      do {
	   errno = 0;

	  /* if using SIGIOs for new connections and we disable interrupts
	   * while in epoll we can't call
	   * epoll until there is something to do otherwise we 
	   * get stuck waiting 
	   */

          if (options.select_timeout < 0) {
              /* epoll_wait call should block indefinitely */
              timeout_msec = -1;
          } else {
	    if ((options.get_connections == OPT_CONN_WITH_SIGIO) && 
		(options.dont_intr_select)) {
	      /* if using SIGIOs for new connections and epoll doesn't get
	       * interrupted we don't want to wait for a time out when in epoll
	       */
	      timeout_msec = 0;
	    } else {
	      timeout_msec = options.select_timeout * 1000;
	    }
          }

          max_usable_sd = max_epoll_entry;

	  /* used in order to see what the average is */
	  total_usable_sd += max_usable_sd;

	  if (options.dont_intr_select) {
	    notification_off();
	  }

#ifdef HAVE_EPOLL_CTLV
	    if (options.use_epoll_ctlv) {

	      if (new_epoll_ctlv_count != 0) {
		num_epoll_ctlv_calls++;
		if (new_epoll_ctlv_count > num_max_count_to_epoll_ctlv) {
		    num_max_count_to_epoll_ctlv = new_epoll_ctlv_count;
		}
		num_total_count_to_epoll_ctlv += new_epoll_ctlv_count;


		TRACE(EVT_EPOLL_CTLV,
		  trace_fd = new_epoll_ctlv_count;
		  rc = sys_epoll_ctlv(epoll_fd, EPOLL_CTLV, 
		       new_epoll_ctlv_count, new_epoll_ctlv_evts);
		  save_errno = errno;
		);
		new_epoll_ctlv_count = 0;
		if (rc < 0) {
		  perror("epoll_array_change: epoll_ctlv failed ");
		  exit(1);
		}
	      } else {
                num_epoll_ctlv_no_change++;
	      }
	    }
#endif /* HAVE_EPOLL_CTLV */


	    /* Debugging 
            info_listener_print_status();
	    */

	    PRINT_TIME(epoll_fd, &tnow, &tprev, 
		"epoll_loop: calling epoll_wait max = %d\n",
		max_fds);
	    num_epoll_calls++;
	    epoll_errno = 0;


/* Done this way because nesting these macros doesn't work */
#ifdef PROVIDE_EPOLL_HEADERS
	    TRACE(EVT_EPOLL_WAIT,
	      trace_fd = 0;
	      /* get the kernel to fill epoll_results with info 
	       * about what is ready
	       * the max number of fds/events that the array
	       * can handle is max_fds
	       */

	      rc = sys_epoll_wait(epoll_fd, epoll_results, max_fds, timeout_msec);
	      epoll_errno = errno;
	    );
#else
	    TRACE(EVT_EPOLL_WAIT,
	      trace_fd = 0;
	      /* get the kernel to fill epoll_results with info 
	       * about what is ready
	       * the max number of fds/events that the array
	       * can handle is max_fds
	       */

	      rc = epoll_wait(epoll_fd, epoll_results, max_fds, timeout_msec);
	      epoll_errno = errno;
	    );
#endif /* PROVIDE_EPOLL_HEADERS */

	    n = rc;
	    PRINT_TIME(epoll_fd, &tnow, &tprev, 
		"epoll_loop: epoll_wait returned "
		"n = %d epoll_errno = %d", n, epoll_errno);

#ifdef OLDWAY
	    DEBG(MSG_SERVER_LOOP, 
		"epoll_wait returns n = %d epoll_errno = %d\n",
		n, epoll_errno);
#endif

	  if (options.dont_intr_select) {
	    notification_on();
	  }

	  if (n == -1 && epoll_errno == EINTR) {
	     PRINT_TIME(epoll_fd, &tnow, &tprev, 
		 "epoll_loop: epoll was interrupted");
             num_epoll_interrupted++;
	  }
      } while (n == -1 && epoll_errno == EINTR);

      TRACE_END_TIME(EVT_EVENT_PHASE, n, 0);

      gettimeofday (&lru_now, NULL);

      /* this indicates that epoll timed out */
      if (n == 0) {
	num_epoll_timed_out++;
      } else if (n < 0) {
	check_and_record_epoll_errors(epoll_errno);
	if (epoll_errno == EINTR) {
	  /* make the call again */
	  continue;
	}
      } else { /* n > 0 */
        /* epoll was successful and has n descriptors to process */
        num_epoll_successful++;
        num_event_calls_successful++;
        num_total_fds_from_epoll += n;
        num_total_fds_from_events = num_total_fds_from_epoll;
	if (n > num_max_fds_from_epoll) {
	   num_max_fds_from_epoll = n;
	}
	PRINT_TIME(epoll_fd, &tnow, &tprev, "epoll_loop: calling "
	    "process_epoll_events");
	did_something = process_epoll_events(n);
	if (!did_something) {
	  num_epoll_waits_nothing_to_do++;
	}
      }

      if (options.close_idle_conns) {
        close_idle_conns(&lru_now);
      }

      do_event_loop_end(&lru_now);
    } /* while */
}

void
check_and_record_epoll_errors(int epoll_errno)
{
  /* handle all of the various errors */
  switch (epoll_errno) {

    case 0:
      num_epoll_failed_noerror++;
      break;

    case EAGAIN:
      num_epoll_eagain++;
      break;

    case EINTR:
      printf("Bizarre failure\n");
      printf("Should exit the epoll loop when getting EINTR\n");
      exit(1);
      break;

    default :
      num_epoll_eother++;
      fprintf(stderr, "epoll_wait failed: %d\n", epoll_errno);
      break;
  } /* switch */
} /* check_and_record_epoll_errors */

#endif /* HAVE_EPOLL */
