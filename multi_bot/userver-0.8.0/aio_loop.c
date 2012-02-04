/*
    userver -- (pronounced you-server or micro-server).
    This file is part of the userver, a high-performance web server designed for
    performance experiments.

    This file Copyright (C) 2004  Hewlett-Packard Company

    Authors: Tim Brecht <brecht@cs.uwaterloo.ca>
             Done while working part-time for HP
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

#ifdef HAVE_AIO_LAYER

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

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
#include "aio_layer.h"
#include "sock_listener.h"
#include "util.h"

void check_and_record_aio_wait_errors(int aio_wait_errno);
void process_aio_events(int num, compl_evt_t *completions);
void handle_completions(int queue, int n);
void initiate_new_aio_accepts();
int sock_accept_more(int listen_sd);

/* this array is used to track all the open descriptors
 * and what state they are in
 */
compl_evt_t *completions = 0;
int completions_max = -1;

int accept_cq = -1;
int read_cq   = -1;
int write_cq  = -1;
int all_cq = -1;

#define MAX_COMPL_QUEUES    (5)

int complqs[MAX_COMPL_QUEUES];
int complq_evts[MAX_COMPL_QUEUES];
int complq_count = 0;


void
aio_loop_init(void)
{
  extern void aio_sock_purge_counts_init();
  int size;
  unsigned int all_mask;

  /* create space for 4 events per fd -- this is plenty big */
  completions_max = max_fds * 4;

  /* allocate space for get event completion events */
  size = completions_max * sizeof(compl_evt_t);
  completions = (compl_evt_t *) alloc_aligned(cache_line_size, size, 
	 ALLOC_MALLOC, "aio_loop_init: (completions)");
  printf("aio_loop_init: completions has %d entries and used %d bytes\n",
	  completions_max, size);

  /* initialize the array */
  memset(completions, 0, size);

  printf("aio_loop_init: aio_complq_count = %d\n", options.aio_complq_count);

  switch (options.aio_complq_count) {

    case 1:
      /* TODO: figure out how to size these completion queues 
       * For 3 queues we used max_fds for each so now make this 3 x max_fds 
       * now make them equal to the maximum number of open fds
       */
      size = max_fds * 3;
      all_mask = SOCK_ACCEPT_MASK | SOCK_READ_MASK | 
	         SOCK_WRITE_MASK | SOCK_CLOSE_MASK;

      PRINT_TIME(NOFD, &tnow, &tprev, "aio_init: creating ONE "
	  "completion queue");
      PRINT_TIME(NOFD, &tnow, &tprev, "aio_init: size = %d type = 0x%x", 
	 size, all_mask);

      all_cq = aio_sock_create_compl_q(size, all_mask);
      complqs[complq_count++] = all_cq;

      /* TODO: ensure that this return value is possible */
      if (all_cq < 0) {
	printf("aio_loop_init: aio_sock_create_compl_q failed with "
	    "return = %d errno = %d\n", all_cq, errno);
	exit(1);
      }
      break;

    case 3:
      /* TODO: figure out how to size this completion queue */
      /* For now make it equal to the maximum number of open fds */
      /* size = options.accept_compl_qlen */
      size = max_fds;

      PRINT_TIME(NOFD, &tnow, &tprev, "aio_init: creating accept "
	  "completion queue");
      PRINT_TIME(NOFD, &tnow, &tprev, "aio_init: size = %d type = 0x%x", 
	 size, SOCK_ACCEPT_MASK);
      accept_cq = aio_sock_create_compl_q(size, SOCK_ACCEPT_MASK);
      complqs[complq_count++] = accept_cq;

      /* TODO: ensure that this return value is possible */
      if (accept_cq < 0) {
	printf("aio_loop_init: aio_sock_create_compl_q failed with "
	    "return = %d errno = %d\n", accept_cq, errno);
	exit(1);
      }

      /* TODO: figure out how to size these completion queues */
      /* For now make them equal to the maximum number of open fds */
      /* size = options.read_compl_qlen */
      size = max_fds;

      PRINT_TIME(NOFD, &tnow, &tprev, "aio_init: creating read "
	  "completion queue");
      PRINT_TIME(NOFD, &tnow, &tprev, "aio_init: size = %d type = 0x%x", 
	 size, SOCK_READ_MASK);
      read_cq = aio_sock_create_compl_q(size, SOCK_READ_MASK);
      complqs[complq_count++] = read_cq;

      /* TODO: ensure that this return value is possible */
      if (read_cq < 0) {
	printf("aio_loop_init: aio_sock_create_compl_q failed with "
	    "return = %d errno = %d\n", read_cq, errno);
	exit(1);
      }

      /* TODO: figure out how to size these completion queues */
      /* For now make them equal to the maximum number of open fds */
      /* size = options.read_compl_qlen */
      size = max_fds;

      PRINT_TIME(NOFD, &tnow, &tprev, "aio_init: creating write "
	  "completion queue");
      PRINT_TIME(NOFD, &tnow, &tprev, "aio_init: size = %d type = 0x%x", 
	 size, SOCK_WRITE_MASK);
      write_cq = aio_sock_create_compl_q(size, SOCK_WRITE_MASK);
      complqs[complq_count++] = write_cq;

      /* TODO: ensure that this return value is possible */
      if (write_cq < 0) {
	printf("aio_loop_init: aio_sock_create_compl_q failed with "
	    "return = %d errno = %d\n", write_cq, errno);
	exit(1);
      }
      break;

    default:
      printf("aio_loop_init: Specified completion queue count = %d "
	  "not implemented yet\n", options.aio_complq_count);
      exit(1);
      break;
  }

  printf("aio_loop_init: read_cq = %d write_cq = %d "
         "accept_cq = %d all_cq = %d\n", 
	 read_cq, write_cq, accept_cq, all_cq);

  interest_set_init(DECLARE_WITH_AIO);

  aio_sock_purge_counts_init();
}


void
aio_loop(void)
{
  int i;
  int n = 0;
  int rc = 0;
  int aio_wait_errno = 0;
  int trace_fd = 0;
  int do_continue = 0;
  struct timeval timeout;
  struct timeval *timeo_ptr;

  int n_total = 0;
  int n_write_compl = 0;
  int n_read_compl = 0;
  int n_accept_compl = 0;
  int n_all_compl = 0;
  int n_tmp = 0;
  int completion_count = strlen(options.aio_completion_order);
  int init_prepost_done = 0;


  printf("Entered server/aio loop\n");
  fflush(stdout);
  fflush(stderr);

  PRINT_TIME(NOFD, &tnow, &tprev, "aio_loop: complq_count = %d",
      complq_count);

  while (1) {

    do_continue = server_loop_top();
    if (do_continue) {
       continue;
    }

    /* TODO: this may be redundant with the above code */
    if (!init_prepost_done) {
      initiate_new_aio_accepts();
      if (options.aio_initial_prepost_only) {
        init_prepost_done = 1;
      }
    }

    /* Figure out how many events of each type there are to process 
     * as well as the total number of events
     */

    n = 0;
    n_total = 0;
    n_write_compl = 0;
    n_read_compl = 0;
    n_accept_compl = 0;
    n_all_compl = 0;
    n_tmp = 0;

    for (i=0; i<complq_count; i++) {
      n_tmp = aio_sock_num_events(complqs[i]);
      PRINT_TIME(NOFD, &tnow, &tprev, "aio_loop: i = %d n_tmp = %d "
	  "n_total = %d", i, n_tmp, n_total);

      if (complqs[i] == read_cq) {
         n_read_compl += n_tmp;	
      } else if (complqs[i] == write_cq) {
         n_write_compl += n_tmp;	
      } else if (complqs[i] == accept_cq) {
         n_accept_compl += n_tmp;	
      } else if (complqs[i] == all_cq) {
         /* we use  n_total here */
	n_all_compl += n_tmp;
      }

      n_total += n_tmp;
    }

    PRINT_TIME(NOFD, &tnow, &tprev, "aio_loop: writes = %d "
	"reads = %d accepts = %d n_all_compl = %d total = %d",
	n_write_compl, n_read_compl, n_accept_compl, n_all_compl, n_total);

    /* if there aren't any events to process then we should
     * block and wait until something happens 
     */

    if (n_total == 0) {

      if (options.aio_dont_wait) {
	num_aio_dont_wait++;
      } else {

	do {

          if (options.select_timeout < 0) {
            /* aio_wait call should block indefinitely */
            timeo_ptr = (struct timeval *) 0;
          } else {
            /* in most interfaces the timeout can change to reflect
             * the remaining time, so we reset it each time
             */
	    timeout.tv_sec = options.select_timeout;
	    timeout.tv_usec = 0;
            timeo_ptr = &timeout;
          }

	  PRINT_TIME(NOFD, &tnow, &tprev, "aio_loop: calling aio_wait "
	      "evt_mask = 0x%x, comp_array = %p max = %d, timeo_ptr = %p", 
	      SOCK_ALL_MASK, 0, 1, timeo_ptr);
	  PRINT_TIME(NOFD, &tnow, &tprev, 
	      "aio_loop: calling aio_wait evt_buf = 0 max = 0");
	  num_aio_wait_calls++;
	  errno = 0;
	  aio_wait_errno = 0;
	  /* 0 for max count means don't return any events only a count */
	  TRACE(EVT_AIO_WAIT,
	    trace_fd = 0;
	    rc = aio_wait(SOCK_ALL_MASK, 0, 0, timeo_ptr);
	    aio_wait_errno = errno;
	  );
	  n = rc;
	  PRINT_TIME(NOFD, &tnow, &tprev, "\n");
	  PRINT_TIME(NOFD, &tnow, &tprev, "aio_loop: aio_wait returned "
	      "n = %d aio_wait_errno = %d", n, aio_wait_errno);

	  if (n == AIO_WAIT_ERROR && aio_wait_errno == EINTR) {
	     PRINT_TIME(NOFD, &tnow, &tprev, 
		 "aio_loop: aio was interrupted");
	     num_aio_wait_interrupted++;
	     printf("aio_loop: aio_wait failed with rc = %d\n", n);
	  }
	} while (n == AIO_WAIT_ERROR && aio_wait_errno == EINTR);


	/* this indicates that the aio_wait call timed out */
	if (n == AIO_WAIT_TIMEOUT) {
	  /* printf("aio_loop: aio_wait timed out\n"); */
	  num_aio_wait_timed_out++;
	} else if (n < 0) {
	  printf("aio_loop: aio_wait failed with rc = %d\n", n);
	  check_and_record_aio_wait_errors(aio_wait_errno);
	} else { /* n >= 0 */
	  /* if n == 0 here we have been woken from the wait
	   * because there are events but we get them a different way
	   */
	  
	  /* aio was successful and has n descriptors to process */
	  num_aio_wait_successful++;
	  num_total_events_from_aio_wait += n;
	  if (n > num_max_events_from_aio_wait) {
	     num_max_events_from_aio_wait = n;
	  }
	}
      } /* dont_wait_poll */

      gettimeofday (&lru_now, NULL);

      if (options.close_idle_conns) {
         /* Don't close idle connections when using aio */
	 printf("We don't close idle connections with AIO because\n");
	 printf("preposted accepts may cause the socket/connection to appear idle\n");
	 exit(1);
         /* close_idle_conns(&lru_now); */
      }

      print_running_stats(&lru_now);

    } else {

      switch (options.aio_complq_count) {
	case 1:
	  assert(n_total == n_all_compl);
	  handle_completions(all_cq, n_all_compl);
	  break;

	case 3:
	  /* I know that this could code would be simpler with
	   * function pointers or other approaches but this is
	   * easier to trace with debuggers and to figure out
	   * which functions are called when
	   */
	  for (i=0; i<completion_count; i++) {

	    PRINT_TIME(NOFD, &tnow, &tprev, "i = %d order = %c",
		i, options.aio_completion_order[i]);

	    switch(options.aio_completion_order[i]) {

	      case OPT_AIO_READ_COMPL_EVTS:
		if (n_read_compl > 0) {
		  handle_completions(read_cq, n_read_compl);
		}
		break;

	      case OPT_AIO_WRITE_COMPL_EVTS:
		if (n_write_compl > 0) {
		  handle_completions(write_cq, n_write_compl);
		}
		break;

	      case OPT_AIO_ACCEPT_COMPL_EVTS:
		if (n_accept_compl > 0) {
		  handle_completions(accept_cq, n_accept_compl);
		}
		break;

	      case OPT_AIO_ALL_COMPL_EVTS:
		if (n_all_compl > 0) {
		  handle_completions(all_cq, n_all_compl);
		}
		break;

	      default:
		printf("aio_loop: specified completion order %c unknown\n",
	                options.aio_completion_order[i]);
		exit(1);
		break;
	    }
	  }

	  break;

	default:
	  printf("aio_loop_init: Specified completion queue count = %d "
	      "not implemented yet\n", options.aio_complq_count);
	  exit(1);
	  break;
      } 

      gettimeofday (&lru_now, NULL);

      if (options.close_idle_conns) {
         /* Don't close idle connections when using aio */
	 printf("We don't close idle connections with AIO because\n");
	 printf("preposted accepts may cause the socket/connection to appear idle\n");
	 exit(1);
         /* close_idle_conns(&lru_now); */
      }

      do_event_loop_end(&lru_now);
    }
  } /* while */

} /* aio_loop */

void
handle_completions(int queue, int n)
{
  int trace_fd = 0;
  int rc = 0;
  int max = 0;
  int thold = 0;

  assert(n >= 0);

  PRINT_TIME(NOFD, &tnow, &tprev, "handle_completions: queue = %d n = %d\n",
      queue, n);

  /* This is where we actually process the events */
  if (n == 0) {
    return;
  } 
  
  if (queue == write_cq) {
      thold = options.aio_write_events_limit;
  } else if (queue == read_cq) {
      thold = options.aio_read_events_limit;
  } else if (queue == accept_cq) {
      thold = options.aio_accept_events_limit;
  } else if (queue == all_cq) {
      thold = 0;
      thold += options.aio_write_events_limit;
      thold += options.aio_read_events_limit;
      thold += options.aio_accept_events_limit;
  } else {
      printf("handle_completions: unknown completion queue = %d\n",
	  queue);
      exit(1);
  }

  if (thold) {
    max = thold;
  } else {
    max = completions_max;
  }

  num_aio_sock_getevents_calls++;
  errno = 0;
  TRACE(EVT_AIO_SOCK_GETEVENTS,
    trace_fd = queue;
    rc = aio_sock_getevents(queue, max, completions);
  );
  n = rc;
  PRINT_TIME(NOFD, &tnow, &tprev, "handle_completions: after "
      "aio_sock_getevents queue = %d max = %d n = %d", queue, max, n);

  if (n < 0) {
    num_aio_sock_getevents_failed++;
    printf("handle_completions: aio_sock_getevents returns < 0 when it should not\n");
    exit(1);
  } else if (n == 0) {
    /* We can't be doing this as it will kill performance */	   
    /* printf("handle_completions: aio_sock_getevents returns 0 events\n"); */
    /* Note that this is behaviour is fine */
    /* exit(1); */
  } else {
    num_aio_sock_getevents_successful++;
    num_event_calls_successful++;
    num_total_events_from_aio_sock_getevents += n;
    num_total_fds_from_events += n;
    if (n > num_max_events_from_aio_sock_getevents) {
      num_max_events_from_aio_sock_getevents = n;
    }
    process_aio_events(n, completions);
  }

} /* handle_completions */


void
initiate_new_aio_accepts(void)
{
  int i = 0;
  int thold = options.aio_accept_thold;
  int diff = 0;

  /* number of potentially open connections - actual connections */
  if (thold == 0) {
    diff = -1;
  } else {
    diff = (maxconns - num_idle) - cur_conns;
  }

  /* Initiate new connection accepts.
   * The number of accepts initiated will be controlled by the
   * -m / --accept-count option
   */

  PRINT_TIME(NOFD, &tnow, &tprev, 
      "initiate_new_aio_accepts: sock_listener_min = %d sock_listener_max = %d",
       sock_listener_min, sock_listener_max);
  PRINT_TIME(NOFD, &tnow, &tprev, 
      "initiate_new_aio_accepts: num_idle = %d cur_conns = %d "
      "diff = %d thold = %d", num_idle, cur_conns, diff, thold);
  if (diff < thold) {
    for (i=sock_listener_min; i<=sock_listener_max; i++) {
      if (sock_is_listener(i)) {
	PRINT_TIME(NOFD, &tnow, &tprev, "aio_loop: listener = %d", i);
	if (options.use_socket_aio) {
	  if (sock_accept_more(i)) {
	    do_new_connections(i, FROM_AIO_SOCK_ACCEPT_INIT);
	  }
	} else {
	  do_new_connections(i, FROM_AIO_SOCK_ACCEPT_INIT);
	}
      }
    }
  }
}

#endif /* HAVE_AIO_LAYER */

