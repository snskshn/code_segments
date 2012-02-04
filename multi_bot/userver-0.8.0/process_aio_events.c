/*
    userver -- (pronounced you-server or micro-server).
    This file is part of the userver, a high-performance web server designed for
    performance experiments.

    This file Copyright (C) 2004  Hewlett-Packard Company

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


#ifdef HAVE_AIO_LAYER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "interest_set.h"
#include "debug.h"
#include "options.h"
#include "cntl_conns.h"
#include "lru.h"
#include "q.h"
#include "sock_listener.h"
#include "aio_layer.h"
#include "post_sock.h"
#include "state.h"
#include "options.h"
#include "trace.h"

/* #define LOCAL_DEBUG */
#include "local_debug.h"

int purge_counts[SOCK_MAX_COMPL];

/* These need to correspond to the completion types defined in aio_layer.h */
char *compl_type_str[] = {
  "sock_invalid_completion",
  "sock_accept_completion",
  "sock_read_completion",
  "sock_write_completion",
  "sock_close_completion",
};


#define do_aio_state_invalid(type, sd) \
  printf("process_aio_events: got %s completion in invalid state\n",  \
      compl_type_str[type]); \
  printf("process_aio_events: state = %s", fsm_state_string(sd)); \
  assert(0); \
  exit(1);

#define do_aio_state_default(type, sd) \
  printf("process_aio_events: got %s completion in unexpected state\n",  \
      compl_type_str[type]); \
  printf("process_aio_events: state = %s", fsm_state_string(sd)); \
  assert(0); \
  exit(1);

/*----------------------------------------------------------------------*/

void do_sock_accept_compl(int listen_sd, struct info *ip, int sd, int err);
void do_sock_read_compl(struct info *ip, int result, int err);
void do_sock_write_compl(struct info *ip, int result, int err);
void do_sock_close_compl(int sd, int result, int err);

/* Global */
int g_cur_completion_evt = -1;
int g_num_completion_evts = -1;
compl_evt_t *g_completions = 0;

int num_found_purged = 0;

void
process_aio_events(int n, compl_evt_t *completions)
{
  int sd = -1;
  /* q_nodeptr p = NULL; */
  int i;
  int num_evts = n;
  compl_evt_t *evt = 0;
  int result = 0;
  int err = 0;
  compl_t evt_type;
  int listen_sd = SOCK_LISTENER_UNKNOWN;
  struct info *ip = 0;
#ifdef DEBUG_ON
  int save_n = n;
#endif


  TRACE_START_TIME(EVT_WORK_PHASE);

  PRINT_TIME(NOFD, &pnow, &pprev, "");
  PRINT_TIME(NOFD, &pnow, &pprev, "process_aio_events: entered "
      "(time process_aio_events only)");
  PRINT_TIME(NOFD, &pnow, &pprev, "process_aio_events: n = %d", n);

  assert(n > 0);
  g_num_completion_evts = num_evts;
  g_completions = completions;

  for (i=0; i<num_evts; i++) {
    g_cur_completion_evt = i;
    evt = &completions[i];
    evt_type = evt->type;
    sd = (int) evt->handle;
    result = evt->result;
    err = evt->result_errno;

    PRINT_TIME(sd, &tnow, &tprev, "process_aio_events: index = %d "
	"type = %d [%s]", i, evt_type, compl_type_str[evt_type])
    PRINT_TIME(sd, &tnow, &tprev, "process_aio_events: result = %d err = %d ",
	result, err);

    /*
     * conns_off_if_needed();
     */

    /* TODO: fix this as this requires the underlying aio layer to 
     * set up the data field with the listener socket
     *
     * TODO: on other systems we may end up having to use the data 
     * field for the
     * new client socket (since it's created prior to the accept call).
     * WAIT: that should be hidden from this layer.
     * aside from the fact that this data field may not be valid anymore.
     */

    if (evt_type < 0) {
      /* This is a purged event that we are skipping */
      PRINT_TIME(sd, &tnow, &tprev, "process_aio_events: skipping "
	  "purged event type = %d", evt_type);
      n--;
      num_found_purged++;
      continue;
    }

    switch(evt_type) {

      case SOCK_ACCEPT_COMPL:
        listen_sd = (int) evt->handle;
        sd = evt->result;

        if (!info_map_valid(sd)) {
	  ip = info_add(sd);
          set_fsm_state(ip, FSM_INITIAL_STATE);
	} else {
          ip = info_ptr(sd);
        }

	PRINT_TIME(sd, &tnow, &tprev, "process_aio_events: doing "
	    "accept completion");
	PRINT_TIME(sd, &tnow, &tprev, "process_aio_events: state = %s",
	      fsm_state_string(sd));

	switch (get_fsm_state(sd)) {
	  /* Got SOCK_ACCEPT_COMPL */
          case FSM_INITIAL_STATE:
          case FSM_CONNECTING:
	    do_sock_accept_compl(listen_sd, ip, sd, err);
	    break;

	  /* Got SOCK_ACCEPT_COMPL */
	  case FSM_READING_REQUEST:
	  case FSM_WRITING_REPLY:
	  case FSM_READING_NEXT_REQUEST:
	  case FSM_READ_ARRIVED_WHILE_WRITING:
	  case FSM_CLOSING:
	    /* accept completion arrives after a read completion
	     * we've already assumed the connection was accepted
	     * (otherwise we wouldn't be getting data on it
	     * so now we just ignore the accept completion
	     * because we've previously completed the accept.
	     */

	    /* Do nothing */
	    break;

	  /* Got SOCK_ACCEPT_COMPL */
	  case FSM_CLOSED:
	  case FSM_CONN_ERROR:
	    do_aio_state_invalid(evt_type, sd);
	    break;

	  default:
	    do_aio_state_default(evt_type, sd);
	    break;

	}
	/* Got SOCK_ACCEPT_COMPL */
	break;

      case SOCK_READ_COMPL:
        sd = (int) evt->handle;
	ip = info_ptr(sd);
	/* Believe that this shouldn't happen because bad events
	 * should get purged
	 */
	assert(ip);

	PRINT_TIME(sd, &tnow, &tprev, "process_aio_events: doing "
	    "read completion");
	PRINT_TIME(sd, &tnow, &tprev, "process_aio_events: state = %s",
	      fsm_state_string(sd));

	switch (get_fsm_state(sd)) {

	  /* Got SOCK_READ_COMPL */
	  case FSM_INITIAL_STATE:
	    do_aio_state_invalid(evt_type, sd);
	    break;

	  /* Got SOCK_READ_COMPL */
	  case FSM_CONNECTING:
	    /* if we preposted a read before the accept and the
	     * read arrives before the accept we need to do some work
	     * to pretend that the accept has arrived and then
	     * later eat the accept completion event
	     */

	    assert(options.aio_read_before_accept);
	    listen_sd = ip->parent_sd;

	    /* we haven't done the post_accept stuff yet do it now */
	    do_sock_accept_compl(listen_sd, ip, sd, err);

            /* now handle the read completion */
	    do_sock_read_compl(ip, result, err);
	    break;

	  /* Got SOCK_READ_COMPL */
	  case FSM_READING_REQUEST:
	  case FSM_READING_NEXT_REQUEST:
	    /* This is in order completion */
	    do_sock_read_compl(ip, result, err);
	    break;

	  /* Got SOCK_READ_COMPL */
	  case FSM_READ_ARRIVED_WHILE_WRITING:
	    do_aio_state_invalid(evt_type, sd);
	    break;

	  /* Got SOCK_READ_COMPL */
	  case FSM_WRITING_REPLY:
	    assert(options.aio_read_before_write);
	    /* for now we just make note of the fact
	     * that the read has completed and we
	     * do the work necessary after the pending
	     * write completion arrives and has been handled
	     */
	    set_fsm_state(ip, FSM_READ_ARRIVED_WHILE_WRITING);
	    ip->save_result = result;
	    ip->save_err = err;
	    break;

	  /* Got SOCK_READ_COMPL */
	  case FSM_CLOSING:
	  case FSM_CLOSED:
	    do_aio_state_invalid(evt_type, sd);
	    break;

	  /* Got SOCK_READ_COMPL */
	  case FSM_CONN_ERROR:
	    /* Do nothing just skip the event */
	    /* Read completion arrives after the connection has been reset */
	    break;

	  /* Got SOCK_READ_COMPL */
	  default:
	    do_aio_state_default(evt_type, sd);
	    break;

	}
	/* Got SOCK_READ_COMPL */
	break;

      case SOCK_WRITE_COMPL:
	/* this is really a writev completion but there may not be
	 * a separate event completion type for a writev.
	 */
        sd = (int) evt->handle;
	ip = info_ptr(sd);
	assert(ip);

	PRINT_TIME(sd, &tnow, &tprev, "process_aio_events: doing "
	    "write completion");
	PRINT_TIME(sd, &tnow, &tprev, "process_aio_events: state = %s",
	      fsm_state_string(sd));

	switch (get_fsm_state(sd)) {

	  /* Got SOCK_WRITE_COMPL */
	  case FSM_INITIAL_STATE:
	  case FSM_CONNECTING:
	  case FSM_READING_REQUEST:
	  case FSM_READING_NEXT_REQUEST:
	    do_aio_state_invalid(evt_type, sd);
	    break;

	  /* Got SOCK_WRITE_COMPL */
	  case FSM_READ_ARRIVED_WHILE_WRITING:
	    PRINT_TIME(sd, &tnow, &tprev, "process_aio_events: got "
		"write completion after a read completion has arrived");

	    do_sock_write_compl(ip, result, err);

	    /* if there is an outstanding read completion
	     * event that has arrived but hasn't been processed
	     * we should do it now
	     */
	    PRINT_TIME(sd, &tnow, &tprev, "process_aio_events: now "
		  "finishing up the outstanding read completion");
	    do_sock_read_compl(ip, ip->save_result, ip->save_err);
	    break;

	  /* Got SOCK_WRITE_COMPL */
	  case FSM_WRITING_REPLY:
	    do_sock_write_compl(ip, result, err);
	    break;

	  /* Got SOCK_WRITE_COMPL */
	  case FSM_CLOSING:
	  case FSM_CLOSED:
	    do_aio_state_invalid(evt_type, sd);
	    break;

	  /* Got SOCK_WRITE_COMPL */
	  case FSM_CONN_ERROR:
	    /* Do nothing just skip the event */
	    /* Read completion arrives after the connection has been reset */
	    break;

	  /* Got SOCK_WRITE_COMPL */
	  default:
	    do_aio_state_default(evt_type, sd);
	    break;

	}
	/* Got SOCK_WRITE_COMPL */
	break;

      case SOCK_CLOSE_COMPL:

	PRINT_TIME(sd, &tnow, &tprev, "process_aio_events: doing "
	    "close completion");
	PRINT_TIME(sd, &tnow, &tprev, "process_aio_events: state = %s",
	      fsm_state_string(sd));

	switch (get_fsm_state(sd)) {

	  /* Got SOCK_CLOSE_COMPL */
	  case FSM_INITIAL_STATE:
	  case FSM_CONNECTING:
	  case FSM_READING_REQUEST:
	  case FSM_READING_NEXT_REQUEST:
	  case FSM_READ_ARRIVED_WHILE_WRITING:
	  case FSM_WRITING_REPLY:
	    do_aio_state_invalid(evt_type, sd);
	    break;

	  /* Got SOCK_CLOSE_COMPL */
	  case FSM_CLOSING:
	    do_sock_close_compl(sd, result, err);
	    break;

	  /* Got SOCK_CLOSE_COMPL */
	  case FSM_CLOSED:
	    do_aio_state_invalid(evt_type, sd);
	    break;

	  /* Got SOCK_CLOSE_COMPL */
	  case FSM_CONN_ERROR:
	    /* Do nothing just skip the event */
	    /* Read completion arrives after the connection has been reset */
	    break;


	  /* Got SOCK_CLOSE_COMPL */
	  default:
	    do_aio_state_default(evt_type, sd);
	    break;
	}
        /* Got SOCK_CLOSE_COMPL */
        break;

      default:
	printf("process_aio_events: event type invalid = %d\n", evt_type);
	exit(-1);
	break;
    } /* switch */

    n--;
    do_extra_accepts(n, options.extra_freq);
  } /* for */

  assert(n == 0);

  PRINT_TIME(NOFD, &pnow, &pprev, "process_aio_events: returning "
      "(time process_aio_only)");

  TRACE_END_TIME(EVT_WORK_PHASE, save_n, 0);
} /* process_aio_events */


/*----------------------------------------------------------------------*/
void
do_sock_read_compl(struct info *ip, int result, int err)
{
  int sd = ip->sd;

  PRINT_TIME(sd, &tnow, &tprev, "do_sock_read_compl: "
      "calling post_sock_read");

  /* there was an error on this connection so it's been 
   * closed after initiating the write 
   */
  if ((ip->fsm_state == FSM_CLOSING) || (ip->fsm_state == FSM_CLOSED) ||
      (ip->fsm_state == FSM_CONN_ERROR)) {
    /* note that the sd could be -1 here */
    PRINT_TIME(sd, &tnow, &tprev, "do_sock_read_compl: state = %s so returning",
	fsm_state_to_string(sd));
    return;
  }

  post_sock_read(sd, result, err);

  if (result == 0) {
    PRINT_TIME(sd, &tnow, &tprev, "do_sock_read_compl: got EOF");
    /* do_close is called by post_sock_read */
    /* so we should hopefully get a close completion */
  } else if (result < 0) {
    PRINT_TIME(sd, &tnow, &tprev, "do_sock_read_compl: got error = %d", err);
    /* post_sock_read handles the error */
  } else {
    /* if we don't have a complete request we need to aio_read again */
    if (ip->req.state != PARSE_DONE) {
      PRINT_TIME(sd, &tnow, &tprev, "do_sock_read_compl: "
	"parsing not done - calling do_readable");
      do_readable(sd);
    } else {
      /* TODO: if we were to prepost a read it would happen around here
       * it could get a bit tricky because I think that the FSM states
       * would all get altered quite a bit and/or we might have
       * problems with assertions getting thrown.
       */
      if (options.aio_read_before_write) {
	PRINT_TIME(sd, &tnow, &tprev, "do_sock_read_compl: "
	  "preinitiating a read before a write");
	do_readable(sd);
      }

      PRINT_TIME(sd, &tnow, &tprev, "do_sock_read_compl: "
	"parsing is done - calling do_writable");
      do_writable(sd);
    }
  }
} /* do_sock_read_compl */

/*----------------------------------------------------------------------*/
void
do_sock_write_compl(struct info *ip, int result, int err)
{
  int sd = ip->sd;
  int rc = 0;

  /* there was an error on this connection so it's been 
   * closed after initiating the write 
   */
  if ((ip->fsm_state == FSM_CLOSING) || (ip->fsm_state == FSM_CLOSED) ||
      (ip->fsm_state == FSM_CONN_ERROR)) {
    PRINT_TIME(sd, &tnow, &tprev, "do_sock_write_compl: state = %s so returning",
	fsm_state_string(sd));
    return;
  }

  if (!options.use_sendfile && options.caching_on) {
    PRINT_TIME(sd, &tnow, &tprev, "do_sock_write_compl: "
	"calling post_sock_writev");

    rc = post_sock_writev(sd, result, err);
    if (rc >= 0) {
      PRINT_TIME(sd, &tnow, &tprev, "do_sock_write_compl: "
	  "calling post_socket_writable");
      post_socket_writable(sd);

      if (!options.aio_read_before_write) {
	if (fsm_state_reading(sd)) {
	  PRINT_TIME(sd, &tnow, &tprev, "do_sock_write_compl: "
	    "calling do_readable sd = %d", sd);
	  do_readable(sd);
	} else {
	  if (get_fsm_state(sd) != FSM_CLOSING) {
	     printf("do_sock_write_compl: state = %s when we think it "
		     "should be FSM_CLOSING\n", 
		     fsm_string[get_fsm_state(sd)]);
	     assert(get_fsm_state(sd) == FSM_CLOSING);
	     exit(1);
	  } 
	}
      }
    } else {
      PRINT_TIME(sd, &tnow, &tprev, "do_sock_write_compl: write "
                 "completion error now closing");
      set_fsm_state(ip, FSM_CONN_ERROR);
      do_close(ip, REASON_AIO_WRITE_COMPL_ERROR);
    }
  } else {
    PRINT_TIME(sd, &tnow, &tprev, "do_sock_write_compl: "
	"calling post_sock_write");
    rc = post_sock_write(sd, result, err);

    if (rc >= 0) {
      PRINT_TIME(sd, &tnow, &tprev, "do_sock_write_compl: "
	  "calling post_socket_writable");
      post_socket_writable(sd);

      if (!options.aio_read_before_write && fsm_state_reading(sd)) {
	PRINT_TIME(sd, &tnow, &tprev, "do_sock_write_compl: "
	  "calling do_readable sd = %d", sd);
	do_readable(sd);
      } else if (fsm_state_writing(sd)) {
	/* there is still data to be written */
	PRINT_TIME(sd, &tnow, &tprev, "do_sock_write_compl: "
	  "calling do_writable sd = %d", sd);
	do_writable(sd);
      } else if (fsm_state_reading(sd)) {
	 if (!options.aio_read_before_write) {
           printf("do_sock_write_compl: state = %s is readable but "
	       "aio_read_before_write "
	       "not used\n", fsm_state_string(sd));
	   exit(1);
	 }
      } else {
	/* This happens when the request is a 1.0 request and 
	 * we've initiated a close after the write
	 */
	if (get_fsm_state(sd) != FSM_CLOSING) {
	   printf("do_sock_write_compl: state = %s when we think it "
		   "should be FSM_CLOSING\n", fsm_state_string(sd));
	   assert(get_fsm_state(sd) == FSM_CLOSING);
	   exit(1);
	} 
      }
    } else {
      PRINT_TIME(sd, &tnow, &tprev, "do_sock_write_compl: write "
                 "completion error now closing");
      do_close(ip, REASON_AIO_WRITE_COMPL_ERROR);
    }
  }
} /* do_sock_write_compl */


/*----------------------------------------------------------------------*/
void
do_sock_accept_compl(int listen_sd, struct info *ip, int new_sd, int err)
{
  /* there was an error on this connection so it's been 
   * closed after initiating the write 
   */
  if ((ip->fsm_state == FSM_CLOSING) || (ip->fsm_state == FSM_CLOSED) ||
      (ip->fsm_state == FSM_CONN_ERROR)) {
    PRINT_TIME(new_sd, &tnow, &tprev, "do_sock_accept_compl: state = %s "
      "so returning", fsm_state_string(new_sd));
    return;
  }

  if ((listen_sd < sock_listener_min) || 
      (listen_sd > sock_listener_max)) {
    listen_sd = SOCK_LISTENER_UNKNOWN;
  }

  assert(listen_sd != SOCK_LISTENER_UNKNOWN);

  PRINT_TIME(listen_sd, &tnow, &tprev, "do_sock_accept_compl: "
      "calling post_sock_accept new sd = %d", new_sd);
  post_sock_accept(listen_sd, ip, new_sd, err);

  /* if the read hasn't been posted before the accept do so now */
  if (!options.aio_read_before_accept) {
    PRINT_TIME(new_sd, &tnow, &tprev, "do_sock_accept_compl: "
	"calling do_readable new_sd = %d", new_sd);
    assert(fsm_state_reading(new_sd));
    do_readable(new_sd);
  }
} /* do_sock_accept_compl */

/*----------------------------------------------------------------------*/
void
do_sock_close_compl(int sd, int result, int err)
{
  int cur_state = get_fsm_state(sd);
  /* there was an error on this connection so it's been 
   * closed after initiating the write 
   */
  if ((cur_state == FSM_CLOSED) || (cur_state == FSM_CONN_ERROR)) {
    PRINT_TIME(sd, &tnow, &tprev, "do_sock_close_compl: state = %s so "
      "returning", fsm_state_string(sd));
    return;
  }

  PRINT_TIME(sd, &tnow, &tprev, "do_sock_close_compl: "
    "calling post_sock_close");
  post_sock_close(sd, result, err);
}
/*----------------------------------------------------------------------*/

void
aio_sock_purge_events(int sd)
{
  int i;
  compl_evt_t *evt = 0;
  for (i=g_cur_completion_evt+1; i<g_num_completion_evts; i++) {
    evt = &g_completions[i];
    if (evt->handle == sd) {
      /* A negative type indicates that it is not longer valid */
      PRINT_TIME(sd, &tnow, &tprev, "Purging type = %d for sd = %d\n", 
	  evt->type, sd);
      purge_counts[evt->type]++;
      evt->type = -evt->type;
    }
  }
}

void
aio_sock_print_purged(void)
{
  int i;
  int total = 0;
  printf("Purge counts:\n");
  for (i=0; i<SOCK_MAX_COMPL; i++) {
    total += purge_counts[i];
    printf("  purged %10d evts of type %s\n", 
	purge_counts[i], compl_type_str[i]);
  }
  printf("Total events purged = %d\n", total);
  printf("Total found purged and skipped = %d\n", num_found_purged);
}

void
aio_sock_purge_counts_init(void)
{
  int i;
  printf("Initializing purge counts:\n");
  // ldbg("aio_sock_purge_counts_init: info = %p\n", info);
  for (i=0; i<SOCK_MAX_COMPL; i++) {
    purge_counts[i] = 0;
  }
  // ldbg("aio_sock_purge_counts_init: info = %p\n", info);
}
#endif /* HAVE_AIO_LAYER */
