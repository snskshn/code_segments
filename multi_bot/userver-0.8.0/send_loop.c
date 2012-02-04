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


#ifdef SEND

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

#include "options.h"
#include "options.h"
#include "debug.h"
#include "stats.h"
#include "cntl_intr.h"
#include "cntl_conns.h"
#include "lru.h"
#include "q.h"
#include "interactive_mode.h"
#include "sigstuff.h"
#include "do_new_connections.h"
#include "trace.h"
#include "interest_set.h"
#include "sock_listener.h"

#include "send.h"

void check_and_record_select_errors(int select_errno);
int bufs_empty();

unsigned int send_readfds = 0;

#define LOOP_MAX  1000000

#define WAIT_TIMEOUT_SEC    (150)
#define WAIT_TIMEOUT_MSEC   (WAIT_TIMEOUT_SEC * 100)
#define WAIT_TIMEOUT        (WAIT_TIMEOUT_MSEC)
/* Don't wait for timeout */
/* #define WAIT_TIMEOUT        (0) */

void
send_loop_init(void)
{
  interest_set_init(DECLARE_WITH_SEND);
}

void
send_loop(void)
{
#ifdef ARRAY_OF_BUFS
  int buf_index;
#endif /* ARRAY_OF_BUFS */
  /* fd_set readable, writable; */
  int rc;
  int loop_counter = 0;
  int timeout;
  extern int interactive_skip;
  int trace_fd = 0;
  struct timeval tv;
  unsigned int tmp_read_set = 0;
  int select_errno = 0;
  int n = 0;

  tv.tv_sec = options.select_timeout;
  tv.tv_usec = 0;

  printf("Entered server/send loop\n");
  fflush(stdout);
  fflush(stderr);

  while (1) {

    if (options.interactive) {
      interactive_mode();
    }

    if (options.send_polls_for_accepts) {
      notification_off();
      PRINT_TIME(NOFD, &tnow, &tprev, "send_loop: poll for new connections");
      do_new_connections(SOCK_LISTENER_USE_ALL, FROM_SEND_LOOP);
      if (options.accepts_only) {
	DEBG(MSG_SERVER_LOOP, "send_loop: back to top of while\n");
	notification_on();
	continue;
      }
      notification_on();
    }

    if (options.send_selects_for_accepts) {
    do {
      tmp_read_set = send_readfds;
#ifdef ONE_LISTENER
      TRACE(EVT_SELECT,
         trace_fd = sock_listener_max;
         rc = select(sock_listener_max + 1, (fd_set *) &tmp_read_set, 0, 0, &tv);
         select_errno = errno;
      );
#else
      /* TODO: Not sure about this at all */
      TRACE(EVT_SELECT,
         trace_fd = sock_listener_min;
         rc = select(sock_listener_max + 1, (fd_set *) &tmp_read_set, 0, 0, &tv);
         select_errno = errno;
      );
#endif /* ONE_LISTENER */
      n = rc;
      PRINT_TIME(NOFD, &tnow, &tprev, "send_loop: select returns %d", rc);
      if (rc < 0) {
        if (n == -1 && select_errno == EINTR) {
	  PRINT_TIME(NOFD, &tnow, &tprev, 
	      "select_loop: select was interrupted");
	  num_select_interrupted++;
        }
      }
    } while (n == -1 && select_errno == EINTR);

  /* this indicates that select timed out */
  if (n == 0) {
    num_select_timed_out++;
  } else if (n < 0) {
    check_and_record_select_errors(select_errno);
  } else { /* n > 0 */
    /* select was successful and has n descriptors to process */
    num_select_successful++;
    num_total_fds_from_select += n;
    if (n > num_max_fds_from_select) {
       num_max_fds_from_select = n;
    }
    PRINT_TIME(NOFD, &tnow, &tprev, "send_loop: calling process_sds");
#ifdef ONE_LISTENER
    process_sds(n, (fd_set *) &tmp_read_set, 0, server_sd);
#else
    /* TODO: not sure about this at all */
    process_sds(n, (fd_set *) &tmp_read_set, 0, sock_listener_max);
#endif /* ONE_LISTENER */
  }
    }

    /* Problems with repeatedly printing this when not 
     * blocking for events 
     */
    if (options.send_block_for_events) {
#ifdef ARRAY_OF_BUFS
      for (buf_index=0; buf_index<EVT_BUFS; buf_index++) {
        PRINT_TIME(NOFD, &tnow, &tprev, 
	  "send_loop: buf_index = %d head = %d tail = %d",
	  buf_index, ecb->head[buf_index], ecb->tail[buf_index]);
      }
#else
      PRINT_TIME(NOFD, &tnow, &tprev, "send_loop: head = %d tail = %d",
	  ecb->head, ecb->tail);
#endif /* ARRAY_OF_BUFS */
      PRINT_TIME(NOFD, &tnow, &tprev, "send_loop: num_queued = %d",
	  ecb->num_queued);
    }

    /* wait for any event - indefinitely. */
    if ((options.send_block_for_events) && (bufs_empty())) {
      timeout = WAIT_TIMEOUT;
      PRINT_TIME(NOFD, &tnow, &tprev, "send_loop: wait for any "
	  "event -- timeout = %d", timeout);
      PRINT_TIME(NOFD, &tnow, &tprev, "send_loop: queue = 0x%x "
	  "notify = 0x%x", ecb->queue, ecb->notify);

      /* TODO: check out efficiency of calling send_handler asynchronously
       *       e.g., 0xffffffff - instead of ecb->notify will call 
       *       handler when any evt arrives 
       */

      /* NOTE: -1 instead of timeout doesn't seem to work */
      TRACE(EVT_WAIT_EVTS,
        rc = evtctl(EVT_ANY, WAIT_FOR_EVENT, (void *) ecb->notify,
	  (int) &timeout);
        trace_fd = 0;
      )
      if (rc < 0) {
        printf("evtctl failed errno = %d\n", errno);
  exit(1);
      }
      PRINT_TIME(NOFD, &tnow, &tprev, "send_loop: evtctl returns %d", rc);
      PRINT_TIME(NOFD, &tnow, &tprev, "send_loop: remaining timeout = %d "
	  "elapsed = %d\n",
     timeout, (WAIT_TIMEOUT - timeout));
    } else {
      if (options.use_getpid) {
        PRINT_TIME(NOFD, &tnow, &tprev, "send_loop: calling getpid");
        /* try inserting quick syscall to get events delivered  */
        getpid();
        PRINT_TIME(NOFD, &tnow, &tprev, 
	    "send_loop: done call to getpid\n");
      }
    }

    if (!(bufs_empty()) && (!interactive_skip)) {
      PRINT_TIME(NOFD, &tnow, &tprev, "send_loop: notify = 0x%x "
	  "queue = 0x%x", ecb->notify, ecb->queue);
#ifdef ARRAY_OF_BUFS
      for (buf_index=0; buf_index<EVT_BUFS; buf_index++) {
        PRINT_TIME(NOFD, &tnow, &tprev, 
	  "send_loop: buf_index = %d head = %d "
	  "tail = %d idle = %d", buf_index, ecb->head[buf_index], 
	  ecb->tail[buf_index], num_idle);
      }
#else
      PRINT_TIME(NOFD, &tnow, &tprev, "send_loop: head = %d "
	  "tail = %d idle = %d", ecb->head, ecb->tail, num_idle);
#endif /* ARRAY_OF_BUFS */

      PRINT_TIME(NOFD, &tnow, &tprev, "send_loop: calling send_handler");

    /* Trying to more accurately trace time */
    /* and this includes lots of other things that are traced. */
    send_handler(NULL);

      notification_off();
        if (options.process_sds_order == OPT_PROCESS_SDS_LIFO) {
    q_sync(Q_ADD_TO_FRONT);
        } else if (options.process_sds_order == OPT_PROCESS_SDS_FIFO) {
    q_sync(Q_ADD_TO_REAR);
        }
      notification_on();

    } else {
      loop_counter++;
      if (loop_counter >= LOOP_MAX) {
	loop_counter = 0;
	printf(".");
	fflush(stdout);
	/* PRINT_TIME(NOFD, &tnow, &tprev, "send_loop: noevents"); */
      }
    }

    gettimeofday(&lru_now, NULL);
    PRINT_TIME(NOFD, &tnow, &tprev, "send_loop: sec = %ld usec = %ld",
	lru_now.tv_sec, lru_now.tv_usec);

    if (options.close_idle_conns) {
      close_idle_conns(&lru_now);
    }

    do_event_loop_end(&lru_now);
  } /* while */
} /* send_loop */

#ifndef ARRAY_OF_BUFS
int
bufs_empty(void)
{
  int ret_val = 0;
  if (ecb->head == ecb->tail) {
    ret_val = 1;
  } else {
    ret_val = 0;
  }
  PRINT_TIME(NOFD, &tnow, &tprev, 
      "bufs_empty: (non array) returns %d\n", ret_val);
  return(ret_val);
}
#endif /* ARRAY_OF_BUFS */


#endif /* SEND */
