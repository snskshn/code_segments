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


#ifdef HAVE_SENDFILE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#if defined(__linux__) || defined(SUNOS) 
#include <sys/sendfile.h>
#endif
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <fcntl.h>

#include "common.h"
#include "debug.h"
#include "trace.h"
#include "stats.h"
#include "syscalls.h"
#include "state.h"
#include "options.h"


/* 
 * NOTE: this was moved out of send_reply to clean things up and
 * because it hadn't really been used - so this would need to be tested
 */

extern void process_write_errors(int sd, int err);
void process_sendfile_errors(int sd, int err);

void fcntl_fail(char * err) {
  printf("Error: fcntl() failed: %s\n", err);
  exit(1);
}



/* ---------------------------------------------------------------------- */

int 
do_sendfile(int sd, struct rep *rep)
{
  struct info *ip;
  int trace_fd = 0;
  int rc = 0;
  int save_errno = 0;
  int hdr_len = 0;	/* length of the http header */
  int file_len = 0;	/* length of file contents */

#if defined(__linux__)
  int file_fd = 0;	/* file fd */
  int value = 0;
  int nwritten = 0;
  int towrite = 0;
  int oldflags = 0, newflags = 0;         /* For use with fcntl */
  off_t tmp_offset = 0;

  ip = info_ptr(sd);
  assert(ip != NULL);
  assert(rep == &ip->rep);

  /* Calculate some things:
   *   hdr_len - number of bytes to send using write()
   *   file_len - number of bytes to send using sendfile()
   *   file_fd - fd to pass to sendfile()
   *   rep->offset - offset to pass to sendfile()
   */
  switch (ip->req.type) {
    case REQ_STATIC:
      hdr_len = (rep->end - rep->cur);
      file_len = rep->total_len - hdr_len;
      file_fd = rep->fd;
      break;

    case REQ_FASTCGI:
      hdr_len = 0;
      file_len = rep->total_len - hdr_len;
      file_fd = dyn_bufpool->track->mmap_fd;
      PRINT_TIME(sd, &tnow, &tprev, "do_sendfile: rep->cur = %p dyn_bufpool->track->adjusted_addr = %p",
         rep->cur, dyn_bufpool->track->adjusted_addr);
      if( rep->offset == 0 ) rep->offset = rep->cur - (char *) dyn_bufpool->track->adjusted_addr;
      break;

    default:
      printf("do_sendfile(): invalid ip->req.type %d\n", ip->req.type);
      assert(0);
  }

  PRINT_TIME(sd, &tnow, &tprev, "do_sendfile: hdr_len = %d file_len = %d file_fd = %d offset = %d", 
        hdr_len, file_len, file_fd, rep->offset);
  PRINT_TIME(sd, &tnow, &tprev, "do_sendfile: use_cache = %d", rep->use_cache);

  /*
   * If there is static header data in the buffer, send that first using
   * write().  Later, we'll send the main content using sendfile().
   */
  if (hdr_len > 0) {

    if (options.skip_header) {
      rep->cur = rep->end;
      hdr_len = 0;
    } else {
      PRINT_TIME(sd, &tnow, &tprev, "do_sendfile: write buffer data first");
      PRINT_TIME(sd, &tnow, &tprev, "rep->cur = %p rep->end = %p diff = %d",
		 rep->cur, rep->end, (int)(rep->end - rep->cur));

      /* Only do the TCP_CORK if the file has something in it */
      if ((options.use_tcp_cork && !rep->is_corked) && file_len > 0 ) {
	value = 1;
	TRACE(EVT_TCP_CORK,
	      trace_fd = sd;
	      rc = setsockopt(sd, SOL_TCP, TCP_CORK, &value, sizeof(value));
	      );
	if (rc < 0) {
	  printf("do_sendfile: unable to TCP_CORK\n");
	  exit(-1);
	}
	rep->is_corked = 1;
	num_socket_corked++;
      }

      num_socket_write_calls++;
      towrite = rep->end - rep->cur;

#ifdef DEBUG_OUTPUT_BUFS
      print_buf("do_sendfile (header)", sd, rep->cur, (rep->end - rep->cur));
#endif

      /* Write the header */
      TRACE(EVT_WRITE_SOCK,
	    trace_fd = sd;
	    rc = write(sd, rep->cur, rep->end - rep->cur);
	    save_errno = errno;
	    );

      nwritten = rc;
      PRINT_TIME(sd, &tnow, &tprev, "do_sendfile: write returns nwritten = %d "
                 "errno = %d", nwritten, save_errno);

      PRINT_TIME(sd, &tnow, &tprev, "do_sendfile: towrite = %d nwritten = %d", towrite, nwritten);
#ifdef HAVE_EPOLL
      if (options.epoll_edge_triggered && nwritten >= 0) {
	if (nwritten < towrite) {
	  sd_state_not_writable(info_ptr(sd));
	}
      }
#endif /* HAVE_EPOLL */

      if (nwritten <= 0) {
	process_write_errors(sd, save_errno);
	if (save_errno == EAGAIN) {
	  PRINT_TIME(sd, &tnow, &tprev, 
		     "do_sendfile: EAGAIN would block -  returning 0");
#ifdef HAVE_EPOLL
	  if (options.epoll_edge_triggered) {
	    sd_state_not_writable(info_ptr(sd));
	  }
#endif /* HAVE_EPOLL */
	  return 0;
	}

	info_close_rep_fd(ip);
	PRINT_TIME(sd, &tnow, &tprev, 
		   "do_sendfile: write failed -  returning -1");
	return -1;
      } else {
	num_socket_write_successful++;
	num_socket_write_bytes += (double) nwritten;
	num_socket_write_bytes_total += (double) nwritten;
      }


      /* set_fsm_state(ip, FSM_WRITING_REPLY); */
      rep->cur += nwritten;
      if (rep->cur == rep->end) {
	rep->end = rep->cur = rep->buf;
      }
      rep->nwritten += nwritten;
      hdr_len -= nwritten;
    }
  }

  /*
   * Assuming there is no more static header data to send, go ahead and
   * send the main content using sendfile().
   */
  if (hdr_len == 0 && file_len > 0) {

    PRINT_TIME(sd, &tnow, &tprev,
               "do_sendfile: calling sendfile file_fd = %d", file_fd);
    num_socket_sendfile_calls++;
    PRINT_TIME(sd, &tnow, &tprev, "do_sendfile: offset = %d", rep->offset);

    towrite = rep->total_len - rep->nwritten;
    PRINT_TIME(sd, &tnow, &tprev, "do_sendfile: rep->offset = %d bytes remaining = %d",
	rep->offset, rep->total_len - rep->nwritten);

#ifdef DEBUG_OUTPUT_BUFS
    print_sendfile_buf("do_sendfile (body)", sd, file_fd, 
         rep->offset, rep->total_len - rep->nwritten);
#endif

    if (options.blocking_sendfile) {
      // Turn off non-blocking for this socket
      oldflags = fcntl(sd, F_GETFL, 0);
      if (oldflags < 0 )
        fcntl_fail("F_GETFL failed: Could not retrieve socket flags");
      newflags = fcntl(sd, F_SETFL, oldflags & ~O_NONBLOCK);
      if (newflags < 0 )
        fcntl_fail("F_SETFL failed: Could not set socket to blocking");
    }

    tmp_offset = (off_t) rep->offset; 
    TRACE(EVT_SENDFILE,
      trace_fd = sd;
      /* 0 means use current file postion */
      /* ~0UL is saying send all the bytes remaining */
      /* This doesn't work when we cache open files */
      /* rc = sendfile(sd, file_fd, 0, ~0UL); */

      /* Note that the kernel is supposed to update the offset here */
      /* ~0UL is max_unsigned long and will try to write all 
       * remaining bytes 
       */

      /* NOTE: This causes problems with TCP_NODELAY */
      /* Interestingly the kernel thinks that there are more
       * bytes that are to be written so it delays sending
       * a partial packet even if TCP_NODELAY is set (i.e., Nagle's
       * algorithm is disabled
       * rc = sendfile(sd, file_fd, (off_t *) &(rep->offset), ~0UL);
       */
/*
      rc = sendfile(sd, file_fd, (off_t *) &(rep->offset), 
		    rep->total_len - rep->nwritten);
*/
      rc = sendfile(sd, file_fd, &tmp_offset, 
		    rep->total_len - rep->nwritten);
#if defined(TRACE_ON)
      tmp_extra1 = (int) tmp_offset - rep->offset;
#endif
    );
    save_errno = errno;

    rep->offset = (int) tmp_offset;

    nwritten = rc;
    PRINT_TIME(sd, &tnow, &tprev, "do_sendfile: nwritten = %d", nwritten);

    PRINT_TIME(sd, &tnow, &tprev, "do_sendfile: towrite = %d nwritten = %d", towrite, nwritten);
#ifdef HAVE_EPOLL
    if (options.epoll_edge_triggered && nwritten >= 0) {
      if (nwritten < towrite) {
	sd_state_not_writable(info_ptr(sd));
      }
    }
#endif /* HAVE_EPOLL */

    if (options.blocking_sendfile) {
      // Make the socket non-blocking again
      newflags = fcntl(sd, F_SETFL, oldflags);
      if (newflags < 0 ) 
        fcntl_fail("F_SETFL failed: Could not set socket non-blocking");
    }

    if (nwritten == 0) {
      printf("sendfile was to send %d bytes but sent 0\n", towrite);
      printf("Suspected kernel bug\n");
      assert(nwritten != 0);
      exit(1);
      num_socket_sendfile_zero++;
    } else  if (nwritten < 0) {
      process_sendfile_errors(sd, save_errno);
      if (save_errno == EAGAIN) {
        PRINT_TIME(sd, &tnow, &tprev, 
                   "do_sendfile: EAGAIN would block -  returning 0");
#ifdef HAVE_EPOLL
      if (options.epoll_edge_triggered) {
	sd_state_not_writable(info_ptr(sd));
      }
#endif /* HAVE_EPOLL */
        return 0;
      }

      info_close_rep_fd(ip);
      PRINT_TIME(sd, &tnow, &tprev, 
                 "do_sendfile: write failed -  returning -1");
      return -1;
    }

    num_socket_sendfile_successful++;
    num_socket_sendfile_bytes += (double) nwritten;
    num_socket_write_bytes_total += (double) nwritten;


    if (options.use_tcp_cork && rep->is_corked) {
      /* Uncork the TCP queue now that the header has been sent */
      value = 0;
      TRACE(EVT_TCP_CORK,
            trace_fd = sd;
            rc = setsockopt(sd, SOL_TCP, TCP_CORK, &value, sizeof(value));
            );
      if (rc < 0) {
        printf("do_sendfile: unable to TCP_CORK\n");
        exit(-1);
      }
      rep->is_corked = 0;
      num_socket_uncorked++;
    }

    /* set_fsm_state(ip, FSM_WRITING_REPLY); */
    rep->nwritten += nwritten;
  }
  return nwritten;


#elif defined(HPUX)

#define SEND_REMAINDER (0)
  struct iovec hdrtrl[2];
  struct iovec *hdr_iov = 0;
  int remainder = 0;
  int nwritten = 0;

  ip = info_ptr(sd);
  assert(ip != NULL);
  assert(rep == &ip->rep);

  PRINT_TIME(sd, &tnow, &tprev, "do_sendfile: rep->cur = %p rep->end = %p "
             "diff = %d", rep->cur, rep->end, rep->end - rep->cur);

  /* NOTE: in this case we don't do anything special for the zero byte
   * file case because it it's one system call either way.
   */
  hdr_len = (rep->end - rep->cur);
  file_len = rep->total_len - hdr_len;

  PRINT_TIME(sd, &tnow, &tprev, "do_sendfile: hdr_len = %d file_len = %d",
             hdr_len, file_len);

  /* Set up the header which is sent before the file */
  if (rep->cur < rep->end) {
    /* on HP-UX sendfile includes iovs for a header and trailer */
    /* if they are not null they are sent before and after the file */
    hdrtrl[0].iov_base = rep->cur;
    hdrtrl[0].iov_len = rep->end - rep->cur;
  } else {
    hdrtrl[0].iov_base = 0;
    hdrtrl[0].iov_len = 0;
  }

  /* This is the trailer which is sent after the file */
  hdrtrl[1].iov_base = 0;
  hdrtrl[1].iov_len = 0;

  PRINT_TIME(sd, &tnow, &tprev, "do_sendfile: hdrtrl[0].iov_base = %p "
             "len = %d", hdrtrl[0].iov_base, hdrtrl[0].iov_len);


  /* XXX: Not sure if this works */
  PRINT_TIME(sd, &tnow, &tprev, "do_sendfile: offset = %d", rep->offset);
  num_socket_sendfile_calls++;
  TRACE(EVT_SENDFILE,
        trace_fd = sd;
        rc = sendfile(sd, rep->fd, rep->offset, SEND_REMAINDER, hdrtrl, 0);
        save_errno = errno;
        );
  nwritten = rc;
  PRINT_TIME(sd, &tnow, &tprev, "do_sendfile: nwritten = %d", nwritten);

  if (nwritten <= 0) {
    process_sendfile_errors(sd, save_errno);
    if (save_errno == EAGAIN) {
      PRINT_TIME(sd, &tnow, &tprev, 
                 "do_sendfile: EAGAIN would block -  returning 0");
      return 0;
    }

    info_close_rep_fd(ip);
    PRINT_TIME(sd, &tnow, &tprev, 
               "do_sendfile: write failed -  returning -1");
    return -1;
  }

  num_socket_sendfile_successful++;
  num_socket_sendfile_bytes += (double) nwritten;
  num_socket_write_bytes_total += (double) nwritten;


  hdr_iov = &hdrtrl[0];

  /* some bytes have been written so we need to update the iovecs */
  if (hdr_iov->iov_len != 0) {
    if (nwritten <= hdr_iov->iov_len) {
      rep->cur += nwritten;
    } else {
      remainder = nwritten - hdr_iov->iov_len;
      /* some of these were the last of the header bytes */
      rep->cur += hdr_iov->iov_len;
      hdr_iov->iov_len = 0;
      /* the remainder were sent from the file */
      rep->offset += remainder;
    }
    if (rep->cur == rep->end) {
      rep->end = rep->cur = rep->buf;
    }
  } else {
    rep->offset += nwritten;
  }

  rep->nwritten += nwritten;

  return nwritten;


#elif defined(FreeBSD)

#define SEND_REMAINDER (0)

  struct iovec hdrtrl[2];
  struct sf_hdtr hdtr;
  struct iovec *hdr_iov = 0;
  int remainder = 0;
  off_t nwritten = 0;
  int flags = 0;

  ip = info_ptr(sd);
  assert(ip != NULL);
  assert(rep == &ip->rep);

  PRINT_TIME(sd, &tnow, &tprev, "do_sendfile: FreeBSD");
  PRINT_TIME(sd, &tnow, &tprev, "do_sendfile: rep->cur = %p rep->end = %p "
             "diff = %d", rep->cur, rep->end, rep->end - rep->cur);

  /* NOTE: in this case we don't do anything special for the zero byte
   * file case because it it's one system call either way.
   */
  hdr_len = (rep->end - rep->cur);
  file_len = rep->total_len - hdr_len;

  PRINT_TIME(sd, &tnow, &tprev, "do_sendfile: hdr_len = %d file_len = %d",
             hdr_len, file_len);

  /* Set up the header which is sent before the file */
  if (rep->cur < rep->end) {
    /* on FreeBSD sendfile uses header and trailer pointers arrays */
    hdrtrl[0].iov_base = rep->cur;
    hdrtrl[0].iov_len = rep->end - rep->cur;
  } else {
    hdrtrl[0].iov_base = 0;
    hdrtrl[0].iov_len = 0;
  }

  /* This is the trailer which is sent after the file */
  hdrtrl[1].iov_base = 0;
  hdrtrl[1].iov_len = 0;

  hdtr.headers = &hdrtrl[0];
  hdtr.hdr_cnt = 1;
  hdtr.trailers = &hdrtrl[1];
  hdtr.trl_cnt = 1;

  PRINT_TIME(sd, &tnow, &tprev, "do_sendfile: hdrtrl[0].iov_base = %p "
             "len = %d", hdrtrl[0].iov_base, hdrtrl[0].iov_len);


  /* XXX: Not sure if this works */
  PRINT_TIME(sd, &tnow, &tprev, "do_sendfile: offset = %d", rep->offset);
  num_socket_sendfile_calls++;
  nwritten = 0;

  flags = 0;
  if (options.cache_miss_skip) {
    /* in this case skip_info counts how many times this request
     * has been skipped
     */
    if (ip->skip_info > options.cache_miss_skip) {
      PRINT_TIME(sd, &tnow, &tprev, 
                 "do_sendfile: ip->skip_info = %d clearing", ip->skip_info);
      flags = 0;
      ip->skip_info = 0;
    } else {

#if defined (DARWIN)
      printf("MAC OS X doesn't seem to have SF_NODISKIO\n");
      exit(1);
#else
      /* Tell sendfile not to block on disk I/0 */
      flags = SF_NODISKIO;
      /* we'll see if it gets skipped when sendfile returns */
#endif
    }
  }

  PRINT_TIME(sd, &tnow, &tprev, "do_sendfile: flags = %d", flags);

#if defined (DARWIN)
  /* On MAC OS X we 0 indicated send until the end of the file 
   * and after the call nwritten will contain the number of bytes written.
   */
  nwritten = 0;
  TRACE(EVT_SENDFILE,
        trace_fd = sd;
        rc = sendfile(rep->fd, sd, rep->offset, 
                      &nwritten, &hdtr, flags);
#if defined (TRACE_ON)
        tmp_extra1 = nwritten;
#endif
        save_errno = errno;
        );
#else
  /* On FreeBSD sendfile returns 0 on success -1 on failure
   * the total number of bytes written is returned in nwritten
   */
  TRACE(EVT_SENDFILE,
        trace_fd = sd;
        rc = sendfile(rep->fd, sd, rep->offset, 
                      SEND_REMAINDER, &hdtr, &nwritten, flags);
        tmp_extra1 = nwritten;
        save_errno = errno;
        );
#endif

  PRINT_TIME(sd, &tnow, &tprev, "do_sendfile: rc = %d nwritten = %d save_errno = %d", 
             rc, nwritten, save_errno);

#ifdef OLDWAY
  if (save_errno == EBUSY) {
     PRINT_TIME(sd, &tnow, &tprev, "do_sendfile: got EBUSY");
  }

  if ((rc < 0) && (save_errno == EBUSY)) {
     process_sendfile_errors(sd, save_errno);
     assert(options.cache_miss_skip);
     /* sendfile didn't send the request because it would require disk I/O */
     ip->skip_info++;
     num_cache_skips++;
     // return 0;
  }

  if (nwritten == 0 && rc < 0) {
    process_sendfile_errors(sd, save_errno);
#ifdef OLDWAY
    /* On BSD EAGAIN is something different, it is like EWOULDBLOCK */
    if (save_errno == EAGAIN) {
      PRINT_TIME(sd, &tnow, &tprev, 
                 "do_sendfile: EAGAIN would block -  returning 0");
      return 0;
    }
#endif

    info_close_rep_fd(ip);
    PRINT_TIME(sd, &tnow, &tprev, 
               "do_sendfile: write failed -  returning -1");
    return -1;
  }
#else

  if (rc < 0) {
    switch(errno) {
      case EBUSY:
        /* Want to track how many EBUSYs we get */
        process_sendfile_errors(sd, save_errno);
        assert(options.cache_miss_skip);
        /* sendfile didn't send the request because it would require disk I/O */
        ip->skip_info++;
        num_cache_skips++;
        break;

      case EAGAIN:
        /* Do nothing, on BSD EAGAIN indicates EWOULDBLOCK */
        /* But it's probably doing something */
        break;

      default:
        process_sendfile_errors(sd, save_errno);
        info_close_rep_fd(ip);
        PRINT_TIME(sd, &tnow, &tprev, 
          "do_sendfile: write failed -  returning -1");
        return -1;
        break;
    }
  }

  assert(nwritten >= 0);

  /* if nothing got written no need to update the things below. */
  if (nwritten == 0) {
    assert(save_errno == EAGAIN || save_errno == EBUSY);
    PRINT_TIME(sd, &tnow, &tprev, "do_sendfile: sendfile nwritten = %d rc = %d save_errno = %d",
      nwritten, rc, save_errno);
    return 0;
  }
#endif /* OLDWAY */

  num_socket_sendfile_successful++;
  num_socket_sendfile_bytes += (double) nwritten;
  num_socket_write_bytes_total += (double) nwritten;

  hdr_iov = &hdrtrl[0];

  /* some bytes have been written so we need to update the iovecs */
  if (hdr_iov->iov_len != 0) {
    if (nwritten <= hdr_iov->iov_len) {
      rep->cur += nwritten;
    } else {
      remainder = nwritten - hdr_iov->iov_len;
      /* some of these were the last of the header bytes */
      rep->cur += hdr_iov->iov_len;
      hdr_iov->iov_len = 0;
      /* the remainder were sent from the file */
      rep->offset += remainder;
    }
    if (rep->cur == rep->end) {
      rep->end = rep->cur = rep->buf;
    }
  } else {
    rep->offset += nwritten;
  }

  rep->nwritten += nwritten;

  PRINT_TIME(sd, &tnow, &tprev, "do_sendfile: rep->offset = %d bytes remaining = %d",
	rep->offset, rep->total_len - rep->nwritten);

  return nwritten;


#else

#error sendfile not implemented on this architecture

#endif

}

/*----------------------------------------------------------------------*/
void
process_sendfile_errors(int sd, int err)
{
  switch (err)
    {
    case EAGAIN:
      num_socket_sendfile_eagain++;
      PRINT_TIME(sd, &tnow, &tprev, "do_sendfile: EAGAIN would block");
      break;

    case ECONNRESET:
      PRINT_TIME(sd, &tnow, &tprev, "do_sendfile: ECONNRESET ");
      num_socket_sendfile_reset++;
      break;


#ifdef __FreeBSD__
    case ENOTCONN:
      PRINT_TIME(sd, &tnow, &tprev, "do_sendfile: ENOTCONN ");
      num_socket_sendfile_enotconn++;
      break;
#endif

    case EPIPE:
      PRINT_TIME(sd, &tnow, &tprev, "do_sendfile: EPIPE ");
      num_socket_sendfile_epipe++;
      break;

    case EBUSY:
      PRINT_TIME(sd, &tnow, &tprev, "do_sendfile: EBUSY ");
      num_socket_sendfile_ebusy++;
      break;

    default:
      num_socket_sendfile_failed_others++;
      PRINT_TIME(sd, &tnow, &tprev, "do_sendfile: other failure ");
      DEBG(MSG_WRITEV, "do_sendfile: sendfile failed err = %d\n", err);
      printf("do_sendfile: sendfile failed errno = %d\n", err);
      break;
    } /* switch */
}

#endif /* HAVE_SENDFILE */

