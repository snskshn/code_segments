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


#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <time.h>

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
#include "interest_set.h"
#include "util.h"
#include "trace.h"

#ifdef SEND
#include "send.h"

void send_process_new_evts();
#endif /* SEND */

void check_and_record_select_errors(int select_errno);
static int max_usable_sd;

fd_set *readable = 0;
fd_set *writable = 0;
fd_set *rdfds = 0;
fd_set *wrfds = 0;

void malloc_fd_set(fd_set **s, char *str);

void
select_loop_init(void)
{
  printf("&readable = %p\n", &readable);
  malloc_fd_set(&readable, "readable");
  malloc_fd_set(&writable, "writable");
  malloc_fd_set(&rdfds, "rdfds");
  malloc_fd_set(&wrfds, "wrfds");

  interest_set_init(DECLARE_WITH_FDSET);
}

void
malloc_fd_set(fd_set **s, char *str)
{
  int size = 0;

  size = sizeof(fd_set);

  *s = (fd_set *) alloc_aligned(cache_line_size, size, ALLOC_MMAP,
      "select_loop_init: new fd_set");

  memset(*s, 0, size);
  printf("malloc_fd_set: size = %d s = %p *s = %p\n", size, s, *s);

}

void
select_loop(void)
{
  int n;
  int rc = 0;
  int select_errno = 0;
  int trace_fd = 0;
  int do_continue = 0;

  printf("Entered server/select loop\n");
  fflush(stdout);
  fflush(stderr);

  while (1) {
      struct timeval tv;
      struct timeval *timeo_ptr;

      do_continue = server_loop_top();

      if (do_continue) {
         continue;
      }


      TRACE_START_TIME(EVT_EVENT_PHASE);
      notification_off();
        if (options.use_memcpy) {
	  memcpy(readable, rdfds, sizeof(fd_set));
	  memcpy(writable, wrfds, sizeof(fd_set));
	} else {
	  *readable = *rdfds;
	  *writable = *wrfds;
	}
      notification_on();

      do {

	  /* if using SIGIOs for new connections and we disable interrupts
	   * while in select we can't call
	   * select until there is something to do otherwise we 
	   * get stuck waiting 
	   */

          if (options.select_timeout < 0) {
            /* select call should block indefinitely */
            timeo_ptr = (struct timeval *) 0;
          } else {
	    if ((options.get_connections == OPT_CONN_WITH_SIGIO) && 
		(options.dont_intr_select)) {
	      /* if using SIGIOs for new connections and select doesn't get
	       * interrupted we don't want to wait for a time out 
	       * when in select
	       */
	      tv.tv_sec =  0;
	      tv.tv_usec = 0;
	    } else {
	      tv.tv_sec = options.select_timeout;
	      tv.tv_usec = 0;
	    }
            timeo_ptr = &tv;
          }
  

          if (options.use_max_open_sd) {
	    max_usable_sd = max_opensd;
	  } else {
            max_usable_sd = max_sd;
	  }
	  PRINT_TIME(NOFD, &tnow, &tprev, "select_loop: max_usable_sd = %d",
	      max_usable_sd);

	  /* used in order to see what the average is */
	  total_usable_sd += max_usable_sd;

	  if (options.dont_intr_select) {
	    notification_off();
	  }
#ifdef SEND
	    /* Because we don't get notified when new events arrive
	     * when notification is off we need to check if new 
	     * events have been delivered while notification was off
	     * and to handle those events prior to calling select  
	     */
	    if (options.get_connections == OPT_CONN_WITH_SEND_EVTS) {
	      send_process_new_evts();
	      PRINT_TIME(NOFD, &tnow, &tprev, "select_loop: notify = 0x%x "
		  "queue = 0x%x", ecb->notify, ecb->queue);
	      PRINT_TIME(NOFD, &tnow, &tprev, "select_loop: head = %d "
		  "tail = %d idle = %d", ecb->head, ecb->tail, num_idle);
	    }
#endif /* SEND */
	    PRINT_TIME(NOFD, &tnow, &tprev, "select_loop: calling select");
	    num_select_calls++;
	    select_errno = 0;
	    errno = 0;

#ifdef DEBUG_FDSET
	    print_fdset("select rdfds:", rdfds, max_usable_sd);
	    print_fdset("select wrfds:", wrfds, max_usable_sd);
#endif
	    TRACE(EVT_SELECT,
	      trace_fd = 0;
	      rc = select(max_usable_sd + 1, readable, writable, 0, timeo_ptr);
	      select_errno = errno;
	    );
	    n = rc;
	    PRINT_TIME(NOFD, &tnow, &tprev, "\n");
	    PRINT_TIME(NOFD, &tnow, &tprev, "select_loop: select "
		"returned n = %d select_errno = %d", n, select_errno);
#ifdef SEND
	    if (options.get_connections == OPT_CONN_WITH_SEND_EVTS) {
	      PRINT_TIME(NOFD, &tnow, &tprev, "select_loop: notify = 0x%x "
		  "queue = 0x%x", ecb->notify, ecb->queue);
	      PRINT_TIME(NOFD, &tnow, &tprev, "select_loop: head = %d "
		  "tail = %d", ecb->head, ecb->tail);
	    }
#endif /* SEND */
	    DEBG(MSG_SERVER_LOOP, "select returns n = %d "
		"select_errno = %d\n", n, select_errno);

	  if (options.dont_intr_select) {
	    notification_on();
	  }

	  if (n == -1 && select_errno == EINTR) {
	     PRINT_TIME(NOFD, &tnow, &tprev, "select_loop: select was "
		 "interrupted");
             num_select_interrupted++;

	     /* if the select call was interrupted because of a SIGIO
	      * or a SEND event 
	      * then we need to copy the file descriptors that will be
	      * used in the select call - because the interrupt handler
	      * can change them. Note: depending on options used it
	      * might be sufficient to only copy rdfds
	      */
	     if ((options.get_connections == OPT_CONN_WITH_SIGIO) ||
	         (options.get_connections == OPT_CONN_WITH_SEND_EVTS)) {
		notification_off();
		  if (options.use_memcpy) {
		    memcpy(readable, rdfds, sizeof(fd_set));
		    memcpy(writable, wrfds, sizeof(fd_set));
		  } else {
		    *readable = *rdfds;
		    *writable = *wrfds;
		  }
		notification_on();
	     }
	  }
      } while (n == -1 && select_errno == EINTR);

      TRACE_END_TIME(EVT_EVENT_PHASE, n, 0);

      gettimeofday (&lru_now, NULL);

      /* this indicates that select timed out */
      if (n == 0) {
	num_select_timed_out++;
      } else if (n < 0) {
	check_and_record_select_errors(select_errno);
	if (select_errno == EINTR) {
	  /* make the call again */
	  continue;
	}
      } else { /* n > 0 */
        /* select was successful and has n descriptors to process */
        num_select_successful++;
        num_event_calls_successful++;
        num_total_fds_from_select += n;
        num_total_fds_from_events = num_total_fds_from_select;
	if (n > num_max_fds_from_select) {
	   num_max_fds_from_select = n;
	}
	PRINT_TIME(NOFD, &tnow, &tprev, "select_loop: calling process_sds");
	process_sds(n, readable, writable, max_usable_sd);
      }

      if (options.close_idle_conns) {
        close_idle_conns(&lru_now);
      }

      do_event_loop_end(&lru_now);
    } /* while */
}

void
check_and_record_select_errors(int select_errno)
{
  /* handle all of the various errors */
  switch (select_errno) {

    case 0:
      num_select_failed_noerror++;
      break;

    case EAGAIN:
      num_select_eagain++;
      break;

    case EINTR:
      printf("Bizarre failure\n");
      printf("Should exit the select loop when getting EINTR\n");
      exit(1);
      break;

    default :
      num_select_eother++;
      fprintf(stderr, "select failed: %d\n", select_errno);

      /* If this is empty we should block indefinitely waiting for 
       * a new connection */
      fprintf (stderr, "printing fd set info:\n");
      print_fdset("rdfds:", rdfds, max_usable_sd);
      print_fdset("wrfds:", wrfds, max_usable_sd);
      /* exit (1); */
      break;

  } /* switch */
} /* check_and_record_select_errors */

#ifdef SEND
void
send_process_new_evts(void)
{
  int rc = 0;
  /* used for trace data */
  int trace_fd = 0;

  while ((ecb->head < ecb->tail) && (num_idle > options.free_fd_thold)) {
    PRINT_TIME(NOFD, &tnow, &tprev, "send_process_new_evts: head = %d "
	"tail = %d idle = %d", ecb->head, ecb->tail, num_idle);

    /* OUT_OF_DATE: Notification will be off when we are in the handler but
     * if we don't turn it on here the handler thinks we're getting
     * a race condition
     */
    TRACE(EVT_SENDHANDLER_CALL,
        trace_fd = 0;
	send_handler(NULL);
        rc = send_handler_evts_done;
    );
  } /* while */
}
#endif /* SEND */
