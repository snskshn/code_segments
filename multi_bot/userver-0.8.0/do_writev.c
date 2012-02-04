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
#include <sys/uio.h>
#include <errno.h>
#include "common.h"
#include "debug.h"
#include "trace.h"
#include "stats.h"
#include "syscalls.h"
#include "state.h"
#include "options.h"

void process_writev_errors(int sd, int err);

/* ---------------------------------------------------------------------- */
int
do_writev(int sd, struct rep *rep)
{
  int nwritten;
  int rc;
  struct info *ip;
  int save_errno;
  int trace_fd = 0;
  int i = 0;
  int vec_count = 0;
  struct iovec *start_iov = 0;
  int towrite = 0;

#ifdef DEBUG_OUTPUT_BUFS
  char outstr[100];
#endif
 
  assert(sd > 2);
  ip = info_ptr(sd);
  assert(ip);

  /* This is a slight optimization for when the header */
  /* has already been written. */
  if (rep->iovec[0].iov_len != 0) {
	  start_iov = &rep->iovec[0];
    /* If the file is empty, then just write the header */
    if( rep->iovec[1].iov_len == 0 ) {
      vec_count = 1;
    } else {
      vec_count = 2;
    }
    
  } else {
	  start_iov = &rep->iovec[1];
	  vec_count = 1;
    /* If we get here, the file should be non-empty! */
    assert( rep->iovec[1].iov_len > 0 );
  }

  for (i=0; i<2; i++) {
    PRINT_TIME(sd, &tnow, &tprev, "do_writev: len[%d] = %10d base[%d] = 0x%x",
        i, rep->iovec[i].iov_len, i, rep->iovec[i].iov_base);
  }
  PRINT_TIME(sd, &tnow, &tprev, "do_writev: start_iov->iov_len = %d "
     "start_iov->iov_base = 0x%x", start_iov->iov_len, start_iov->iov_base);

  for (i=0; i<vec_count; i++) {
    towrite += start_iov[i].iov_len;
  }
  PRINT_TIME(sd, &tnow, &tprev, "do_writev: towrite = %d", towrite);

  num_socket_writev_calls++;

#ifdef DEBUG_OUTPUT_BUFS
  for (i=0; i<vec_count; i++) {
    sprintf(outstr, "do_writev vec = %d\n", i); 
    print_buf(outstr, sd, start_iov[i].iov_base, start_iov[i].iov_len);
  }
#endif

  TRACE(EVT_WRITEV_SOCK,
    trace_fd = sd;
    rc = writev(sd, start_iov, vec_count);
  );
  save_errno = errno;
  nwritten = rc;
  DEBG(MSG_WRITEV, "nwritten = %d\n", (int) nwritten);

#ifdef HAVE_EPOLL
  if (options.epoll_edge_triggered && nwritten >= 0) {
    if (nwritten < towrite) {
      sd_state_not_writable(info_ptr(sd));
    }
  }
#endif /* HAVE_EPOLL */

  if (nwritten < 0) {
    if (save_errno == EAGAIN) { 
      num_socket_writev_eagain++;
      PRINT_TIME(sd, &tnow, &tprev, 
	  "do_writev: EAGAIN would block -  returning 0");
#ifdef HAVE_EPOLL
      if (options.epoll_edge_triggered) {
	sd_state_not_writable(ip);
      }
#endif /* HAVE_EPOLL */
      return 0;
    } else {
      process_writev_errors(sd, save_errno);
    }

    /* fprintf(stderr, "write failed with sd = %d\n", sd); */
    /* perror ("write"); */
    info_close_rep_fd(ip);
    PRINT_TIME(sd, &tnow, &tprev, "do_writev: write failed -  returning -1");
    return -1;
  }

  if (nwritten == 0) {
    num_socket_writev_zero++;
  } else {
    num_socket_writev_successful++;
    num_socket_writev_bytes += nwritten;
    num_socket_write_bytes_total += nwritten;
  }
    
  PRINT_TIME(sd, &tnow, &tprev, 
      "do_writev: returning nwritten = %d", nwritten);
  return(nwritten);
} /* do_writev */

/*----------------------------------------------------------------------*/
void
process_writev_errors(int sd, int err)
{
  switch (err)
  {
    case EAGAIN:
      num_socket_writev_eagain++;
      PRINT_TIME(sd, &tnow, &tprev, 
	  "do_writev: EAGAIN would block -  returning 0");
      break;

    case ECONNRESET:
      PRINT_TIME(sd, &tnow, &tprev, "do_writev: ECONNRESET ");
      num_socket_writev_reset++;
      break;

    case EPIPE:
      PRINT_TIME(sd, &tnow, &tprev, "do_writev: EPIPE ");
      num_socket_writev_epipe++;
      break;

    default:
      num_socket_writev_failed_others++;
      PRINT_TIME(sd, &tnow, &tprev, "do_writev: other failure ");
      DEBG(MSG_WRITEV, "do_writev: write failed err = %d\n", err);
      break;
  } /* switch */
}
