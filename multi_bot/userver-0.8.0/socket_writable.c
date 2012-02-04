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


#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>


#include "debug.h"
#include "trace.h"
#include "state.h"
#include "stats.h"
#include "options.h"
#include "syscalls.h"
#include "cntl_intr.h"
#include "interest_set.h"
#include "lru.h"
#include "post_sock.h"
#include "logging.h"

#ifdef CALL_STATS
#include "call_stats.h"
#endif /* CALL_STATS */

int
socket_writable(struct info *ip)
{
  int x = 0;

  PRINT_TIME(ip->sd, &tnow, &tprev, "socket_writable: entered ");
  notification_off();

  if (send_reply(ip->sd, &ip->rep) < 0) {
    if (ip->type == INFO_CLIENT) {
      if (!ip->rep.use_cache) {
	info_close_rep_fd(ip);
      } else {
	assert(ip->rep.cache);
	cache_dec_ref_count(ip->rep.cache);
      }
    }

    num_send_reply_failed++;
    PRINT_TIME(ip->sd, &tnow, &tprev, 
               "socket_writable: send_reply failed ");
    notification_on();
    return do_close (ip, REASON_SEND_REPLY_FAILED);
  }

  notification_on();

  if (!options.use_socket_aio) {
    x = post_socket_writable(ip->sd);
  }

  PRINT_TIME(ip->sd, &tnow, &tprev, "socket_writable: nwritten = %d "
             "total_len = %d", ip->rep.nwritten, ip->rep.total_len);
  PRINT_TIME(ip->sd, &tnow, &tprev, "socket_writable: returning 0");
  return 0;
}

int
post_socket_writable(int sd)
{
  struct info *ip;
  int peer_sd;
  struct info *peer_ip;
  notification_off();
  int rc = 0;
  void *start_addr = 0;

  ip = info_ptr(sd);
  assert(ip);

  PRINT_TIME(ip->sd, &tnow, &tprev, "post_socket_writable: nwritten %d "
             "total_len %d", ip->rep.nwritten, ip->rep.total_len);

  lru_access(ip->sd);

  switch (ip->type) {
    case INFO_CLIENT:

      if (ip->skip == OPT_SKIP_DO_MADVISE) {
         if (ip->rep.nwritten > ip->skip_info + OPT_SKIP_MADVISE_SIZE) {
            assert(ip->rep.cache != 0);
            start_addr = (void *) ip->rep.cache->buf + ip->skip_info; 
            rc = madvise(start_addr, OPT_SKIP_MADVISE_SIZE, MADV_DONTNEED);
            ip->skip_info += OPT_SKIP_MADVISE_SIZE;

            PRINT_TIME(ip->sd, &tnow, &tprev, "post_socket_writable: madvise rc = %d nwritten = %zd "
                 "total_len = %zd skip_info = %zd ", rc, ip->rep.nwritten, ip->rep.total_len, ip->skip_info);
            PRINT_TIME(ip->sd, &tnow, &tprev, "post_socket_writable: buf = %p start_addr = %p",
                  ip->rep.cache->buf, start_addr);

            if (rc < 0) {
              perror("post_socket_writable: madvise failed");
              assert(rc >= 0);
            }
         }
      }

      if (ip->rep.nwritten >= ip->rep.total_len) {
	/*
	 * IMPORTANT:
	 * If you change this block of code in any way, there is a section
	 * of code in socket_readable.c for MOD_SPECWEB99 that needs to
	 * be changed in a parallel fashion (i.e. that code is almost a
	 * duplicate of this code).  Look for the do_spec_dyn_req: label.
	 */
	PRINT_TIME(ip->sd, &tnow, &tprev, "post_socket_writable: done reply");

	/* Possibly log the client's request */
	if( options.use_logging ) log_request(ip);

#ifdef CALL_STATS
        if (options.call_stats) {
           call_stats_finished(ip->req.num, ip->rep.total_len, ip->rep.nwritten,
             ip->req.client_id, ip->req.call_id, ip->rep.http_status);
        }
#endif /* CALL_STATS */
        
	
	num_replies++;

	/* done with this reply */
	info_close_rep_fd(ip);
#ifndef AVOID_BUF_REUSE
	/* Because sendfile can return before it is finished
	 * sending all of the bytes requested of it,
	 * we need to avoid reusing the buffer until we know
	 * that the sendfile has completed.
	 * It seems that currently the only way to know this
	 * is through an application level handshake.
	 * In our case that happens when the client sends the next
	 * request on the connection or when the client closes their end.
	 * NOTE: this technique can lead to problems because idle connections
	 * can lead to buffers not being reclaimed. But, presumably that
	 * can get taken care of by closing idle connections.
	 *
	 * Bottom line: we can't permit reuse of the buffer now 
	 *    this will be done later when we complete a read on the socket.
	 */
	info_reset_rep_buf(ip);
#endif /* AVOID_BUF_REUSE */

	// update reply stats for this cache item / entry
	if (ip->rep.use_cache) {
	  ip->rep.cache->reply_count++;
	  cache_dec_ref_count(ip->rep.cache);
	  ip->rep.use_cache = 0;
	}

	ip->req.num = -1;
	ip->req.type = REQ_INVALID;
	ip->req.state = PARSE_METHOD;
	ip->req.skip_lws = 0;
	ip->req.method = HTTP_NONE;
	ip->req.end = ip->req.cur = ip->req.buf;
	ip->req.tmp_len = 0;
	ip->req.cookie_len = 0;
	ip->req.content_len = 0;
	ip->req.padding_len = 0;
	ip->req.ifmodsince = (time_t) 0;

        ip->skip = 0;
        ip->skip_info = 0;

	/* If HTTP 1.0 is being used we close down the connection,
	 * otherwise we wait for the client to close it or we
	 * later close connections on an lru basis.
	 */
	if (ip->req.close) {
	  PRINT_TIME(ip->sd, &tnow, &tprev, "post_socket_writable: closing %d", 
		     ip->sd);
	  notification_on();
	  return do_close(ip, REASON_HTTP_1_0);
	}

#ifdef FIXHUP
        /* if we are in FSM_FCGI_CLEARING_BUFFER state that means that something
	 * bad has happened to the client socket (typically it timed out and it
	 * is no longer in the interest set anyway 
	 */
	if (get_fsm_state(ip->sd) != FSM_FCGI_CLEARING_BUFFER) {
	  /* read again to get EOF or other indication that socket was closed */
	  set_fsm_state(ip, FSM_READING_NEXT_REQUEST);
	  interest_set_change(ip->sd, ISET_NOT_WRITABLE | ISET_READABLE);
        } else {
          printf("Don't think this should happen anymore\n");
          assert(0);
          exit(1);
        }
#else
	/* read again to get EOF or other indication that socket was closed */
	set_fsm_state(ip, FSM_READING_NEXT_REQUEST);
	interest_set_change(ip->sd, ISET_NOT_WRITABLE | ISET_READABLE);
#endif
  
      } else {
	num_socket_send_again++;
      }
      break;

    case INFO_FASTCGI:
      peer_sd = ip->rep.fd;
      peer_ip = info_ptr(peer_sd);
      assert(peer_ip != NULL);
      if (peer_ip->fsm_state == FSM_FCGI_WAITING_FOR_REPLY) {
	interest_set_change(ip->sd, ISET_NOT_WRITABLE | ISET_READABLE);
      }
      break;

    default:
      assert(0);
  }

  notification_on();
  PRINT_TIME(ip->sd, &tnow, &tprev, "post_socket_writable: nwritten = %d "
             "total_len = %d", ip->rep.nwritten, ip->rep.total_len);
  PRINT_TIME(ip->sd, &tnow, &tprev, "post_socket_writable: returning 0");
  return 0;
}
