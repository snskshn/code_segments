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
#include <sys/poll.h>

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

void check_and_record_poll_errors(int poll_errno);
void process_poll_events(int num);
static int max_usable_sd;

struct pollfd *poll_array = 0;
int *poll_map = 0;;
int max_poll_fd = 0;
int max_poll_entry = 0;

void
poll_loop_init(void)
{
  int i;
  int size = max_fds * sizeof(struct pollfd);
  poll_array = (struct pollfd *) alloc_aligned(cache_line_size, size,
      ALLOC_MALLOC, "poll_loop_init: (poll_array)");
  printf("poll_loop_init: poll_array has %d entries and used %d bytes\n",
	    max_fds, size);

  size = max_fds * sizeof(int);
  poll_map = (int *) alloc_aligned(cache_line_size, size, ALLOC_MALLOC,
      "poll_loop_init: (poll_map)");
  printf("poll_loop_init: poll_map has %d entries and used %d bytes\n",
	    max_fds, size);

  /* initialize the array */
  for (i=0; i<max_fds; i++) {
    poll_array[i].fd = -1;
    poll_array[i].events = 0;
    poll_array[i].revents = 0;
    poll_map[i] = -1;
  }

  interest_set_init(DECLARE_WITH_POLL);
}

void
poll_loop (void)
{
  extern int sigio_pending();
  int n;
  int rc;
  int poll_errno = 0;
  int trace_fd = 0;
  int timeout_msec = 0;
  int do_continue = 0;

  printf("Entered server/poll loop\n");
  fflush(stdout);
  fflush(stderr);

  while (1) {

      do_continue = server_loop_top();
      if (do_continue) {
         continue;
      }

      TRACE_START_TIME(EVT_EVENT_PHASE);

      do {

	  /* if using SIGIOs for new connections and we disable interrupts
	   * while in poll we can't call
	   * poll until there is something to do otherwise we 
	   * get stuck waiting 
	   */

          if (options.select_timeout < 0) {
              /* poll_wait call should block indefinitely */
              timeout_msec = -1;
          } else {
	    if ((options.get_connections == OPT_CONN_WITH_SIGIO) && 
		(options.dont_intr_select)) {
	      /* if using SIGIOs for new connections and poll doesn't get
	       * interrupted we don't want to wait for a time out when in poll
	       */
	      timeout_msec = 0;
	    } else {
	      timeout_msec = options.select_timeout * 1000;
	    }
          }

          max_usable_sd = max_poll_entry;

	  /* used in order to see what the average is */
	  total_usable_sd += max_usable_sd;

	  if (options.dont_intr_select) {
	    notification_off();
	  }

	    PRINT_TIME(NOFD, &tnow, &tprev, 
		"poll_loop: calling poll max = %d", max_usable_sd);
	    num_poll_calls++;
	    poll_errno = 0;
	    TRACE(EVT_POLL,
	      trace_fd = 0;
	      rc = poll(poll_array, max_usable_sd + 1, timeout_msec);
	      poll_errno = errno;
	    );
	    n = rc;
	    PRINT_TIME(NOFD, &tnow, &tprev, "\n");
	    PRINT_TIME(NOFD, &tnow, &tprev, "poll_loop: poll returned "
		"n = %d poll_errno = %d", n, poll_errno);

	    DEBG(MSG_SERVER_LOOP, "poll returns n = %d poll_errno = %d\n",
		n, poll_errno);

	  if (options.dont_intr_select) {
	    notification_on();
	  }

	  if (n == -1 && poll_errno == EINTR) {
	     PRINT_TIME(NOFD, &tnow, &tprev, 
		 "poll_loop: poll was interrupted");
             num_poll_interrupted++;
	  }
      } while (n == -1 && poll_errno == EINTR);

      TRACE_END_TIME(EVT_EVENT_PHASE, n, 0);

      gettimeofday (&lru_now, NULL);

      /* this indicates that poll timed out */
      if (n == 0) {
	num_poll_timed_out++;
      } else if (n < 0) {
	check_and_record_poll_errors(poll_errno);
	if (poll_errno == EINTR) {
	  /* make the call again */
	  continue;
	}
      } else { /* n > 0 */
        /* poll was successful and has n descriptors to process */
        num_poll_successful++;
        num_event_calls_successful++;
        num_total_fds_from_poll += n;
        num_total_fds_from_events = num_total_fds_from_poll;
	if (n > num_max_fds_from_poll) {
	   num_max_fds_from_poll = n;
	}
	PRINT_TIME(NOFD, &tnow, &tprev, "poll_loop: calling "
	    "process_poll_events");
	process_poll_events(n);
      }

      if (options.close_idle_conns) {
        close_idle_conns(&lru_now);
      }

      do_event_loop_end(&lru_now);

    } /* while */
}

void
check_and_record_poll_errors(int poll_errno)
{
  /* handle all of the various errors */
  switch (poll_errno) {

    case 0:
      num_poll_failed_noerror++;
      break;

    case EAGAIN:
      num_poll_eagain++;
      break;

    case EINTR:
      printf("Bizarre failure\n");
      printf("Should exit the poll loop when getting EINTR\n");
      exit(1);
      break;

    default :
      num_poll_eother++;
      fprintf(stderr, "poll failed: %d\n", poll_errno);
      break;
  } /* switch */
} /* check_and_record_poll_errors */

