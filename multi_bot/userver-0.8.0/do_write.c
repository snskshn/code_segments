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


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "common.h"
#include "debug.h"
#include "local_debug.h"
#include "trace.h"
#include "stats.h"
#include "syscalls.h"
#include "post_sock.h"
#include "aio_layer.h"
#include "state.h"
#include "util.h"
#include "options.h"

void process_write_errors(int sd, int err);

/* ---------------------------------------------------------------------- */
int
do_write(int sd, struct rep *rep)
{
  int nwritten;
  int rc;
  struct info *ip;
  int save_errno;
  int trace_fd = 0;
  int towrite = 0;
 
  assert(sd > 2);
  ip = info_ptr(sd);
  assert(ip);

  PRINT_TIME(sd, &tnow, &tprev, "calling write with %d bytes",
             (rep->end - rep->cur));

  DEBG(MSG_SEND_REPLY, "do_write: rep->buf = %p\n", rep->buf);
  DEBG(MSG_SEND_REPLY, "do_write: rep->cur = %p\n", rep->cur);
  DEBG(MSG_SEND_REPLY, "do_write: rep->end = %p\n", rep->end);

  if (options.fake_writing) {
    nwritten = rep->end - rep->cur;
#if 0
    if (rep->total_len - rep->nwritten < nwritten) {
      nwritten = rep->total_len - rep->nwritten;
    }
#endif
    save_errno = 0;
    rc = post_sock_write(sd, nwritten, save_errno);
    PRINT_TIME(sd, &tnow, &tprev, "faking write returning rc = %d", rc);
    return(rc);
  }

  num_socket_write_calls++;

#ifdef DEBUG_OUTPUT_BUFS
  print_buf("do_write:", sd, rep->cur, (rep->end - rep->cur));
#endif

  if (options.use_aio_write) {
#ifdef HAVE_AIO_LAYER
    PRINT_TIME(sd, &tnow, &tprev, "do_write: aio_sock_write buf = %p "
      "len = %d handle = %p", rep->cur, (rep->end - rep->cur), rep->write_reg_handle);
    TRACE(EVT_AIO_SOCK_WRITE,
      trace_fd = sd;
      rc = aio_sock_write(sd, rep->cur, (rep->end - rep->cur), 
                          rep->write_reg_handle);
    );
    save_errno = errno;
    PRINT_TIME(sd, &tnow, &tprev, 
	"do_write: aio_sock_write returns %d\n", rc);

#else
    printf("do_write: aio not handled\n");
    exit(1);
#endif /* HAVE_AIO_LAYER */
  } else {

    towrite = rep->end - rep->cur;
#ifdef HAVE_SSL
    if (ip->ssl) {
    TRACE(EVT_WRITE_SOCK,
      trace_fd = sd;
      rc = SSL_write(ip->ssl, rep->cur, rep->end - rep->cur);
    );
    save_errno = errno;

    PRINT_TIME(sd, &tnow, &tprev, "do_write: done SSL_write rc = %d", rc);
	} else {
    TRACE(EVT_WRITE_SOCK,
      trace_fd = sd;
      rc = write(sd, rep->cur, rep->end - rep->cur);
	  );
    save_errno = errno;
    PRINT_TIME(sd, &tnow, &tprev, "do_write: done write rc = %d", rc);
	}
#else
    TRACE(EVT_WRITE_SOCK,
      trace_fd = sd;
      rc = write(sd, rep->cur, rep->end - rep->cur);
    );
    save_errno = errno;
    PRINT_TIME(sd, &tnow, &tprev, "do_write: done write rc = %d", rc);
#endif

    nwritten = rc;
    DEBG(MSG_WRITABLE, "nwritten = %d\n", (int) nwritten);

#ifdef HAVE_EPOLL
    if (options.epoll_edge_triggered && nwritten >= 0) {
      if (nwritten < towrite) {
	sd_state_not_writable(info_ptr(sd));
      }
    }
#endif /* HAVE_EPOLL */

    rc = post_sock_write(sd, nwritten, save_errno);
  }

  PRINT_TIME(sd, &tnow, &tprev, "do_write: returning rc = %d", rc);
  return(rc);
} /* do_write */

/* Call post write with info about the sd, and return codes */
int
post_sock_write(int sd, int nwritten, int err)
{
  struct info *ip; 
  struct rep *rep;

  assert(sd);
  ip = info_ptr(sd);
  assert(ip);
  rep = &ip->rep;

  if (nwritten < 0) {
    // printf("post_sock_write: write failed sd=%d err=%d\n", sd, err);
    // perror("write");
    if (err == EAGAIN) { 
      num_socket_write_eagain++;
      PRINT_TIME(sd, &tnow, &tprev, 
	  "post_sock_write: EAGAIN would block -  returning 0");
#ifdef HAVE_EPOLL
      if (options.epoll_edge_triggered) {
	sd_state_not_writable(ip);
      }
#endif /* HAVE_EPOLL */
      return 0;
    } else {
      process_write_errors(sd, errno);
    }

    PRINT_TIME(sd, &tnow, &tprev, "post_sock_write: write failed -  returning -1");
    info_close_rep_fd(ip);
    return -1;
  }

  if (nwritten == 0) {
    num_socket_write_zero++;
#ifdef HAVE_AIO_LAYER
    /* fprintf(stderr, "write failed with sd = %d\n", sd); */
    info_close_rep_fd(ip);
    PRINT_TIME(sd, &tnow, &tprev, "post_sock_write: write failed -  returning -1");
    return -1;
#endif /* HAVE_AIO_LAYER */
  } else {
    num_socket_write_successful++;
    num_socket_write_bytes += nwritten;
    num_socket_write_bytes_total += nwritten;

    switch (ip->type) {
      case INFO_FASTCGI:
	assert(ip->appserver != NULL);
	ip->appserver->stats.num_successful_writes++;
	ip->appserver->stats.total_bytes_written += nwritten;
	break;
      default:
	break;
    }
  }

  rep->cur += nwritten;
  if (rep->cur == rep->end) {
    rep->end = rep->cur = rep->buf;
  }
  rep->nwritten += nwritten;

  PRINT_TIME(sd, &tnow, &tprev, "post_sock_write: "
      "returning nwritten = %d", nwritten);
  return(nwritten);

} /* post_sock_write */

/*----------------------------------------------------------------------*/
void
process_write_errors(int sd, int err)
{
  switch (err)
  {
    case EAGAIN:
      num_socket_write_eagain++;
      PRINT_TIME(sd, &tnow, &tprev, 
	  "do_write: EAGAIN would block -  returning 0");
      break;

    case ECONNRESET:
      PRINT_TIME(sd, &tnow, &tprev, "do_write: ECONNRESET ");
      num_socket_write_reset++;
      break;

    case EPIPE:
      PRINT_TIME(sd, &tnow, &tprev, "do_write: EPIPE ");
      num_socket_write_epipe++;
      break;

    default:
      num_socket_write_failed_others++;
      PRINT_TIME(sd, &tnow, &tprev, "do_write: other failure ");
      DEBG(MSG_WRITABLE, "do_write: write failed err = %d\n", err);
      break;
  } /* switch */
}
