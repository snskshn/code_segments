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


#include <stdlib.h>
#include <errno.h>
#include "common.h"
#include "cntl_intr.h"
#include "cntl_conns.h"
#include "debug.h"
#include "trace.h"
#include "options.h"
#include "stats.h"
#include "syscalls.h"
#include "lru.h"
#include "state.h"
#include "q.h"
#include "do_new_connections.h"
#include "interest_set.h"
#include "sock_listener.h"
#include "trace.h"
#include "aio_layer.h"
#include "post_sock.h"
#include "info.h"

#ifdef CALL_STATS
#include "call_stats.h"
#endif /* CALL_STATS */

#ifdef SEND_NOT_NEEDED
#include <unistd.h>
#include <fcntl.h>
#endif /* SEND_NOT_NEEDED */

#ifdef SEND
#include "send.h"
#endif /* SEND */

#ifdef HAVE_AIO_LAYER
void aio_sock_purge_events(int sd);
#endif


/* Pointer to the info struct for the connection
 * that is getting closed, plus we are now going
 * to try adding a reason for the closing.
 *
 */



int
do_close(struct info *ip, int reason)
{
  int i = 0;
  int sd = ip->sd;
  int rc;
  int err = 0;
#ifdef HAVE_AIO_LAYER
  int trace_fd = -1;
#endif /* HAVE_AIO_LAYER */
#ifdef SEND_NOT_NEEDED
  int flags;
#endif /* SEND_NOT_NEEDED */

  PRINT_TIME(sd, &tnow, &tprev, "do_close: entered reason = %d", reason);
  notification_off();

#ifdef CALL_STATS
  if (ip->req.num == -1) {
    /* If the request number is -1. It means that this is a connection
     * that has not received and actual request yet so we can ignore it
     * (as far as tracking stats is concerned 
     */
  } else {
    if (reason == REASON_CLIENT_CLOSED || reason == REASON_HTTP_1_0) {
      /* do nothing */
      /* might want to check that stats have already been recorded. */
    } else {
      call_stats_failed(ip->req.num, ip->rep.total_len, ip->rep.nwritten, ip->req.client_id, 
         ip->req.call_id, ip->rep.http_status, reason);
    }
  }
#endif

  lru_closed(sd);
#ifdef FIXHUP
  if (get_fsm_state(sd) != FSM_FCGI_CLEARING_BUFFER) {
    interest_set_change(sd, ISET_NOT_READABLE | ISET_NOT_WRITABLE | ISET_DONE);
  } else {
    /* when the client is clearing the FCGI buffer it is not in the interest set */
    // PRINT_TIME(sd, &tnow, &tprev, "was clearing buffer so no interest set changes needed");
    interest_set_change(sd, ISET_NOT_READABLE | ISET_NOT_WRITABLE | ISET_DONE);
  }
#else
  interest_set_change(sd, ISET_NOT_READABLE | ISET_NOT_WRITABLE | ISET_DONE);
#endif

  num_closed_from_state(ip->fsm_state);
  PRINT_TIME(sd, &tnow, &tprev, "do_close: closing from state %s", 
     fsm_string[ip->fsm_state]);

  if (get_fsm_state(sd) != FSM_CONN_ERROR) {
    set_fsm_state(ip, FSM_CLOSING);
  }

  if (ip->rep.use_cache) {
    if (ip->rep.fd > 0) {
      cache_dec_ref_count(ip->rep.cache);
      ip->rep.fd = -1;
    }
  }


  if ((options.process_sds_order == OPT_PROCESS_SDS_LIFO) ||
      (options.process_sds_order == OPT_PROCESS_SDS_FIFO)) {
    q_to_delete(sd);
  }

  if (options.use_max_open_sd) {
    FD_CLR(sd, &openfds);
    assert(sd <= max_opensd);
    if (sd == max_opensd) {
      for (i=sd; i>=0; i--) {
	if (FD_ISSET(i, &openfds)) {
	  break;
	}
      }
      max_opensd = i;
      assert(max_opensd < sd);
    }
  }

#ifdef SEND_NOT_NEEDED
  if (options.send_io_events || !options.auto_accept) {
    if ((flags = fcntl (sd, F_GETFL, 0)) < 0) {
        perror("fcntl F_GETFL failed\n");
	exit (1);
    }
    PRINT_TIME(sd, &tnow, &tprev, "do_close: flags were = 0x%x", flags);
    /* Turn off asynchronous delivery of new events 
     * for some reason we still get events on the socket even
     * after it has been closed
     */
    if (fcntl(sd, F_SETFL, (flags & (~O_ASYNC))) < 0) {
      perror("fcntl F_SETFL ~O_ASYNC failed\n");
      exit(1);
    }
    if ((flags = fcntl (sd, F_GETFL, 0)) < 0) {
        perror("fcntl F_GETFL failed\n");
	exit (1);
    }
    PRINT_TIME(sd, &tnow, &tprev, "do_close: flags now  = 0x%x", flags);
  }
#endif /* SEND_NOT_NEEDED */

  if (options.use_aio_close) {
    if (!options.aio_dont_close) {
#ifdef HAVE_AIO_LAYER
      /* num_aio_sock_close_calls++; */
      num_close_calls++;
      num_sock_close_calls++;
      if (get_fsm_state(sd) == FSM_CONN_ERROR) {
#ifdef AIO_PURGE_EVENTS
	aio_sock_purge_events(sd);
#endif
	TRACE(EVT_AIO_SOCK_DESTROY,
	  trace_fd = sd;
	  PRINT_TIME(sd, &tnow, &tprev, "do_close: aio_sock_destroy sd = %d",
	    sd);
	  // printf("do_close: aio_sock_destroy sd = %d\n", sd);
	  rc = aio_sock_destroy(sd);
	);
	if (rc < 0) {
	  num_aio_sock_destroy_failed++;
	}

	/* aio_sock_destroy is synchronous so we need to do the 
	 * post_sock_close
	 */
	post_sock_close(sd, rc, 0);

      } else {
	TRACE(EVT_AIO_SOCK_CLOSE,
	  trace_fd = sd;
	  PRINT_TIME(sd, &tnow, &tprev, "do_close: aio_sock_close sd = %d", sd);
	  rc = aio_sock_close(sd);
	);
	if (rc < 0) {
	  num_aio_sock_close_failed++;
	}
      }
#else
      printf("do_close: aio not handled\n");
      exit(1);
#endif /* HAVE_AIO_LAYER */
    }
  } else {

#ifdef HAVE_SSL
    if (ip->ssl) {
      rc = SSL_shutdown(ip->ssl);
      PRINT_TIME(sd, &tnow, &tprev, "do_close: SSL_shutdown returns = %d", rc);

      /* Not sure how to handle the various returns values should be handled */  
      switch(rc) {
        case -1:
          printf("SSL_shutdown failed\n");
          exit(1);
          break;
      }
    
      /* Is no return value */
      SSL_free(ip->ssl);
      PRINT_TIME(sd, &tnow, &tprev, "do_close: SSL_free returned");

      /* Need to ensure that this isn't treated as an SSL connection anymore */
      ip->ssl = 0;
    }
#endif

    rc = close_sock(sd);
    err = errno;

    notification_on();
    post_sock_close(sd, rc, err);
  }

  PRINT_TIME(sd, &tnow, &tprev, "do_close: returning");
  return 0;

} /* do_close */

void
post_sock_close(int sd, int rc, int err)
{
  struct info *ip = info_ptr(sd);
  assert(ip);

  notification_off();

  if (rc < 0) {
    num_sock_close_failed++;
    switch(err) {
      case ENOTCONN:
        num_sock_close_enotconn++;
	break;

      default:
        num_sock_close_failed_other++;
	break;
    };
  }

  /* reset this for the next time this fd gets reused */
  set_fsm_state(ip, FSM_CLOSED);

  PRINT_TIME(sd, &tnow, &tprev, "post_sock_close: conn_num = %d requests = %d ",
      ip->conn_num, ip->req.num);

  /* TODO: check that this doesn't break a bunch of stuff */ 

  info_listener[ip->parent_sd].cur_conns--;
  PRINT_TIME(sd, &tnow, &tprev, "post_sock_close: parent = %d cur_conns = %d",
      ip->parent_sd, info_listener[ip->parent_sd].cur_conns);

  ip->conn_num = -1;
  ip->req.num = -1;
  ip->req.buf = ip->req.static_buf;
  ip->req.buf_size = options.read_buffer_size;
  ip->req.end = ip->req.cur = ip->req.buf;
  ip->rep.is_corked = 0;
  ip->rep.http_status = 0;
  info_reset_rep_buf(ip);
  ip->parent_sd = -1;
  ip->sd = -1;
  ip->rep.use_cache = 0;
  info_delete(sd);
  ip->sd_state = 0;
  ip->rep.cache = 0;

  ip->skip = 0;
  ip->skip_info = 0;

#ifdef CALL_STATS
  ip->req.client_id = 0;
  ip->req.call_id = 0;
#endif /* CALL_STATS */

  num_idle++;
  cur_conns--;

  PRINT_TIME(sd, &tnow, &tprev, "post_sock_close: cur_conns = %d", cur_conns);

#ifdef SEND_AUTO_ACCEPT_CONTROL
  ecb->completed++;
#endif 
  assert(num_idle <= maxconns);

  if (should_enable_new_conns()) {
    socket_new_conn_on();
#ifdef SEND
    if (options.auto_accept) {
      send_release_queued_evts();
    }
#endif /* SEND */
  }

  /* pick up any new connections that have been attempted */
  if (options.get_connections_on_close && should_enable_new_conns()) {
    notification_on();
    /* TODO: think about if this should be ALL / ANY / or whatever */
    do_new_connections(SOCK_LISTENER_USE_ALL, FROM_DO_CLOSE);
    notification_off();
  }

  notification_on();
  PRINT_TIME(sd, &tnow, &tprev, "post_sock_close: returning");
}
