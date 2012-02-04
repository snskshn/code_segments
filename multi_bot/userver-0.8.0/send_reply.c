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
#include "trace.h"
#include "stats.h"
#include "syscalls.h"
#include "options.h"
#include "add_to_reply.h"
#include "reply_status.h"
#include "aio_layer.h"

#define MAX_FAKE_WRITE (200)

extern int do_writev(int sd, struct rep *rep);
extern int do_write(int sd, struct rep *rep);
extern int do_sendfile(int sd, struct rep *rep);
extern void process_writev_errors(int sd, int err);

int do_write_cached(struct info *ip);
int do_write_not_cached(struct info *ip);
#ifdef HAVE_AIO_LAYER
int do_aio_write_cached(struct info *ip);
#endif /* HAVE_AIO_LAYER */

/* ---------------------------------------------------------------------- */
int
send_reply(int sd, struct rep *rep)
{
  int rc = 0;
  struct info *ip;
 
  assert(sd > 2);
  ip = info_ptr(sd);
  assert(ip);
  set_fsm_state(ip, FSM_WRITING_REPLY);

  PRINT_TIME(sd, &tnow, &tprev, "send_reply: entered ");

  if (
#ifdef HAVE_SSL
      ip->ssl == 0 &&
#endif /* HAVE_SSL */
      options.use_sendfile &&
      ip->type == INFO_CLIENT &&
      ip->req.method != HTTP_RETURN_ERROR) {
#ifdef HAVE_SENDFILE
    if (options.use_aio_write) {
      printf("aio_sendfile not supported\n");
    }
    rc = do_sendfile(sd, rep);
    return(rc);
#else
    printf("send_reply: sendfile not supported\n");
    exit(-1);
#endif
  }

  /* set_fsm_state(ip, FSM_WRITING_REPLY); */

  if (rep->use_cache) {
    if (options.use_aio_write) {
#ifdef HAVE_AIO_LAYER
      rc = do_aio_write_cached(ip);
#else
      printf("send_reply: AIO layer not compiled in and/or supported?\n");
      exit(1);
#endif /* HAVE_AIO_LAYER */
    } else {
      rc = do_write_cached(ip);
    }
  } else {
    rc = do_write_not_cached(ip);
  }
  PRINT_TIME(sd, &tnow, &tprev, "send_reply: returning rc = %d", rc);
  return rc;
} /* send_reply */

#ifdef HAVE_AIO_LAYER
int
do_aio_write_cached(struct info *ip)
{
  struct rep *rep = &ip->rep;
  int bytes_to_write = 0;
  struct aio_vec aio_vec[2];
  struct aio_vec *hdr = 0;
  struct aio_vec *body = 0;
  int sd = ip->sd;
  int i;
  int trace_fd = -1;
  int rc = 0;
  int save_errno = 0;
  int vec_count = 0;
 

  if (options.skip_header) {
    body = &aio_vec[0];
    body->base = rep->iovec[1].iov_base;
    body->len = rep->iovec[1].iov_len;
#ifdef HAVE_ENCRYPTION
    if (body->base == rep->cache->buf) {
      rep->cache->file_reg_handle;
    } else {
      rep->cache->encrypt_buf_handle;
    }
#else
    body->reg_handle = rep->cache->file_reg_handle;
#endif /* HAVE_ENCRYPTION */
    vec_count = 1;
    bytes_to_write = body->len;

    PRINT_TIME(sd, &tnow, &tprev, 
	       "do_aio_write_cached: body len %10d hdr  base %p  reg_handle %p", 
	       body->len, body->base, body->reg_handle);
    PRINT_TIME(sd, &tnow, &tprev, 
	       "do_aio_write_cached: total_len = %lu nwritten = %d", 
	       rep->total_len, rep->nwritten);
  } else {
    /* Here
     * The iovec[0] entry points at the header in the cache
     * the iovec[1] entry points at the mmapped file
     */
    hdr  = &aio_vec[0];
    body = &aio_vec[1];

    hdr->base = rep->iovec[0].iov_base;
    hdr->len = rep->iovec[0].iov_len;
    hdr->reg_handle = rep->cache->header_reg_handle;

    body->base = rep->iovec[1].iov_base;
    body->len = rep->iovec[1].iov_len;
#ifdef HAVE_ENCRYPTION
    if (body->base == rep->cache->buf) {
      rep->cache->file_reg_handle;
    } else {
      rep->cache->encrypt_buf_handle;
    }
#else
    body->reg_handle = rep->cache->file_reg_handle;
#endif /* HAVE_ENCRYPTION */

    vec_count = 2;

    bytes_to_write = hdr->len + body->len;

    PRINT_TIME(sd, &tnow, &tprev, 
	       "do_aio_write_cached: hdr  len %10d hdr  base %p  reg_handle %p", 
	       hdr->len, hdr->base, hdr->reg_handle);
    PRINT_TIME(sd, &tnow, &tprev, 
	       "do_aio_write_cached: body len %10d hdr  base %p  reg_handle %p", 
	       body->len, body->base, body->reg_handle);
    PRINT_TIME(sd, &tnow, &tprev, 
	       "do_aio_write_cached: total_len = %lu nwritten = %d", 
	       rep->total_len, rep->nwritten);

    PRINT_TIME(sd, &tnow, &tprev, "do_aio_write_cached: bytes_to_write %d", 
	       bytes_to_write);

    for (i=0; i<vec_count; i++) {
      PRINT_TIME(sd, &tnow, &tprev, "do_aio_write_cached: iovec len[%d] = %d "
	  "base[%d] = 0x%x", i, rep->iovec[i].iov_len, i, 
	  rep->iovec[i].iov_base);
    }
  }

  for (i=0; i<vec_count; i++) {
    PRINT_TIME(sd, &tnow, &tprev, "do_aio_write_cached: aio_vec len[%d] = %d "
	"base[%d] = %p handle[%d] = %p", i, aio_vec[i].len, i, aio_vec[i].base,
	i, aio_vec[i].reg_handle);
  }

  assert(bytes_to_write >= 1);

  num_socket_writev_calls++;

  PRINT_TIME(sd, &tnow, &tprev, "do_write: aio_sock_writev aio_vec = %p "
      "count = %d", aio_vec, vec_count);
  TRACE(EVT_AIO_SOCK_WRITEV,
    trace_fd = sd;
    rc = aio_sock_writev(sd, aio_vec, vec_count);
  );
  save_errno = errno;
  DEBG(MSG_WRITEV, "rc = %d\n", rc);

    
  PRINT_TIME(sd, &tnow, &tprev, 
      "do_aio_write_cached: returning rc = %d", rc);
  return(rc);
} /* do_aio_write_cached */
#endif /* HAVE_AIO_LAYER */

int
post_sock_writev(int sd, int nwritten, int err)
{
  struct info *ip;
  struct rep *rep;
  struct iovec *hdr_iov;
  struct iovec *body_iov;
  int bytes_to_write = 0;

  assert(sd);
  ip = info_ptr(sd);
  rep = &ip->rep;

  hdr_iov = &(rep->iovec[0]);
  body_iov = &(rep->iovec[1]);
  bytes_to_write = rep->total_len;

  if (nwritten < 0) {
    if (err == EAGAIN) { 
      num_socket_writev_eagain++;
      PRINT_TIME(sd, &tnow, &tprev, 
	  "post_sock_writev: EAGAIN would block -  returning 0");
      return 0;
    } else {
      process_writev_errors(sd, err);
    }

    /* fprintf(stderr, "write failed with sd = %d\n", sd); */
    /* perror ("write"); */
    info_close_rep_fd(ip);
    PRINT_TIME(sd, &tnow, &tprev, "post_sock_writev: write failed -  returning -1");
    return -1;
  }

  if (nwritten == 0) {
    num_socket_writev_zero++;
#ifdef HAVE_AIO_LAYER
    /* The aio layer may return 0 bytes indicating that the client side has
     * terminated the connection
     */
    info_close_rep_fd(ip);
    PRINT_TIME(sd, &tnow, &tprev, "post_sock_writev: write failed -  returning -1");
    return -1;
#endif /* HAVE_AIO_LAYER */
  } else {
    num_socket_writev_successful++;
    num_socket_writev_bytes += nwritten;
    num_socket_write_bytes_total += nwritten;
  }

  rep->nwritten += nwritten;

  assert(hdr_iov->iov_len >= 0);
  assert(body_iov->iov_len >= 0);

  if (nwritten < bytes_to_write) {
    PRINT_TIME(sd, &tnow, &tprev, 
	       "post_sock_writev: hdr  len %10d hdr  base 0x%0x", 
	       hdr_iov->iov_len, hdr_iov->iov_base);
    PRINT_TIME(sd, &tnow, &tprev, 
	       "post_sock_writev: body len %10d body base 0x%0x", 
	       body_iov->iov_len, body_iov->iov_base);

    PRINT_TIME(sd, &tnow, &tprev, 
	       "post_sock_writev: rep->iovec[0] (hdr)    len %10d hdr  base 0x%0x", 
	       rep->iovec[0].iov_len, rep->iovec[0].iov_base);
    PRINT_TIME(sd, &tnow, &tprev, 
	       "post_sock_writev: rep->iovec[1] (body)   len %10d body base 0x%0x", 
	       rep->iovec[1].iov_len, rep->iovec[1].iov_base);

    PRINT_TIME(sd, &tnow, &tprev, "post_sock_writev: returning %d", nwritten);
    printf("post_sock_writev: WARNING - aio_sock_writev wrote %d of "
	"%d bytes", nwritten, bytes_to_write);
    return(nwritten);
  }

  PRINT_TIME(sd, &tnow, &tprev, "do_write_cached: returning 0");
  return 0;
} /* post_sock_writev */

int
do_write_cached(struct info *ip)
{
  struct rep *rep = &ip->rep;
  int nwritten = 0;
  int bytes_to_write = 0;
  struct iovec *hdr_iov = 0;
  struct iovec *body_iov = 0;
  int remainder = 0;
  int sd = ip->sd;
 

  /* This has been modified to use writev.
   * The iovec[0] entry points at the header in the cache
   * the iovec[1] entry points at the mmapped file
   */

  /* For now we rely on the writev call as being
   * reasonably well implemented. Once the header is done
   * it's iovec will have length set to zero - and the writev
   * call will be smart enough not to try to write it
   * and will just continue with writing the body
   *
   * This may be fixed/changed easily in the do_writev call.
   * By just recognizing that the header has been written
   * and changing the base and the count.
   */

  hdr_iov  = &rep->iovec[0];
  body_iov = &rep->iovec[1];

  do {

    /* here we write as many bytes as we can each time until the
     * write would block - then we return 
     * or until all bytes have been written
     */
    PRINT_TIME(sd, &tnow, &tprev, 
	       "do_write_cached: total_len = %lu nwritten = %d", 
	       rep->total_len, rep->nwritten);
    PRINT_TIME(sd, &tnow, &tprev, 
	       "do_write_cached: hdr  len %10d hdr  base 0x%0x", 
	       hdr_iov->iov_len, hdr_iov->iov_base);
    PRINT_TIME(sd, &tnow, &tprev, 
	       "do_write_cached: body len %10d body base 0x%0x", 
	       body_iov->iov_len, body_iov->iov_base);

    bytes_to_write = hdr_iov->iov_len + body_iov->iov_len;
    assert(bytes_to_write >= 1);
    PRINT_TIME(sd, &tnow, &tprev, "do_write_cached: bytes_to_write %d", 
	       bytes_to_write);

    nwritten = do_writev(sd, rep);

    if (nwritten < 0) {
      PRINT_TIME(sd, &tnow, &tprev, "do_write_cached: writev "
		 "error returning %d", nwritten);
      return(nwritten);
    }

    /* some bytes have been written so we need to update the iovecs */
    if (hdr_iov->iov_len != 0) {
      if (nwritten <= (int) hdr_iov->iov_len) {
	hdr_iov->iov_len -= nwritten;
	/* on HP-UX iov_base is void */
	/* hdr_iov->iov_base += nwritten; */
	hdr_iov->iov_base = (void *) (((char *) hdr_iov->iov_base) + 
				      nwritten);
      } else {
	remainder = nwritten - hdr_iov->iov_len;
	/* on HP-UX iov_base is void */
	hdr_iov->iov_base = (void *) (((char *) hdr_iov->iov_base) + 
				      hdr_iov->iov_len);
	hdr_iov->iov_len = 0;

	body_iov->iov_len -= remainder;
	/* on HP-UX iov_base is void */
	/* body_iov->iov_base += remainder; */
	body_iov->iov_base = (void *) (((char *) body_iov->iov_base) + 
				       remainder);
      }
    } else {
      body_iov->iov_len -= nwritten;
      /* on HP-UX iov_base is void */
      /* body_iov->iov_base += nwritten; */
      body_iov->iov_base = (void *) (((char *) body_iov->iov_base) + 
				     nwritten);
    }

    rep->nwritten += nwritten;

    assert(hdr_iov->iov_len >= 0);
    assert(body_iov->iov_len >= 0);

    if (nwritten < bytes_to_write) {
      PRINT_TIME(sd, &tnow, &tprev, 
		 "do_write_cached: hdr  len %10d hdr  base 0x%0x", 
		 hdr_iov->iov_len, hdr_iov->iov_base);
      PRINT_TIME(sd, &tnow, &tprev, 
		 "do_write_cached: body len %10d body base 0x%0x", 
		 body_iov->iov_len, body_iov->iov_base);

      PRINT_TIME(sd, &tnow, &tprev, 
		 "do_write_cached: rep->iovec[0] (hdr)    len %10d hdr  base 0x%0x", 
		 rep->iovec[0].iov_len, rep->iovec[0].iov_base);
      PRINT_TIME(sd, &tnow, &tprev, 
		 "do_write_cached: rep->iovec[1] (body)   len %10d body base 0x%0x", 
		 rep->iovec[1].iov_len, rep->iovec[1].iov_base);

      PRINT_TIME(sd, &tnow, &tprev, "do_write_cached: returning %d", nwritten);
      return(nwritten);
    }
  } while (rep->nwritten < rep->total_len);

  PRINT_TIME(sd, &tnow, &tprev, "do_write_cached: returning 0");
  return 0;
} /* do_write_cached */



/* Non caching case */

int
do_write_not_cached(struct info *ip)
{
  struct rep *rep = &ip->rep;
  int nwritten = 0;
  int bytes_to_write = 0;
  int rc = 0;
  int reply_size = -1;
  int sd = ip->sd;
  int len = 0;
  char tmp[MAX_FAKE_WRITE];

  /* Loop reading from the file and writing to the socket
   * until reading from the file fails or until the number
   * of bytes written leaves us with the situation where the 
   * buffer is full but not all of the bytes have been written
   */

  /* NOT sure how we keep from writing 8196 bytes out each time even
   * for files that are much smaller */

  /* Ah - eventually we can't write all of the bytes and
   * rep->end will be pointing at the end of the buffer
   * leaving no room for more data in the buffer - so we 
   * return and wait until the write socket buffer gets drained
   */

  /* If the file read fails we exit because rep->fd = -1 */

  do {

    /* rep->cur is where the next write to the socket will come from
     * rep->end points to the end of the current data
     */

    reply_size = rep->buf_size;

    DEBG(MSG_SEND_REPLY, "fd = %d end = %p buf+reply_size = %p\n",
	rep->fd, rep->end, rep->buf + reply_size);
    DEBG(MSG_SEND_REPLY, "rep->buf = %p cur = %p end = %p\n",
	rep->buf, rep->cur, rep->end);

    /* If there is space in the buffer and there is more data to send
     * then put the next chunk of data into the buffer 
     */

    /* Is there room in the buffer for more data then try to */
    /* get more from the file. */
    if (ip->req.method != HTTP_RETURN_ERROR &&
	rep->end < rep->buf + reply_size) {
      /*
       * Call add_to_reply_buffer() to add more data into the reply buffer.
       * The return value is the number of bytes added to the buffer.
       */
      rc = add_to_reply_buffer(ip);

      if (rc < 0) {
	return rc;
      } else if (rc == 0) {
	if (rep->end == rep->buf) {
	  /*
	   * The buffer is empty, yet add_to_reply_buffer() failed to add
	   * to it.  There must be nothing more to write at this time.
	   */
	  break;
	}
      }
    } /* if */

    bytes_to_write = rep->end - rep->cur;
    if (bytes_to_write > 0) {
      /* set_fsm_state(ip, FSM_WRITING_REPLY); */
      nwritten = do_write(sd, rep);
      /* if we don't write all the bytes asked the socket buffer is full */
      if (nwritten < bytes_to_write) {
	PRINT_TIME(sd, &tnow, &tprev, "do_write_not_cached: returning %d", 
		   nwritten);
	return nwritten;
      }

      DEBG(MSG_SEND_REPLY, "do_write_not_cached: nwritten = %d  "
	   "rep->nwritten = %d total_len = %d\n",
	   (int) nwritten, (int) rep->nwritten, (int) rep->total_len);
    }

    DEBG(MSG_SEND_REPLY, "do_write_not_cached: at while\n")
    DEBG(MSG_SEND_REPLY, "do_write_not_cached: rep->nwritten = %d\n", rep->nwritten);
    DEBG(MSG_SEND_REPLY, "do_write_not_cached: rep->total_len = %d\n", rep->total_len);
    // DEBG(MSG_SEND_REPLY, "do_write_not_cached: rep->hdr_len  = %d\n", rep->hdr_len);
    DEBG(MSG_SEND_REPLY, "do_write_not_cached: rep->buf = %p\n", rep->buf);
    DEBG(MSG_SEND_REPLY, "do_write_not_cached: rep->cur = %p\n", rep->cur);
    DEBG(MSG_SEND_REPLY, "do_write_not_cached: rep->end = %p\n", rep->end);
    DEBG(MSG_SEND_REPLY, "do_write_not_cached: buf+reply_size = %p\n",
	rep->buf + reply_size);
#ifdef OLDWAY
  } while (rep->fd > 0 && rep->end < rep->buf + reply_size);
#else
  } while (rep->fd > 0 && rep->nwritten < rep->total_len);
#endif

  /* This is a hack. It assumes that the write can be done all at once */
  if (options.fake_writing) {
     // reply_status_fill(ip, HTTP_OK, "Fake Reply", ip->req.close ? 0 : 1);
     // rc = write(ip->sd, ip->rep.buf, ip->rep.total_len);
     len = snprintf(tmp,  MAX_FAKE_WRITE,
          HTTP_OK_STR_ID_LEN, ip->req.close? 0: 1, server_ident, 0UL);
     rc = write(ip->sd, tmp, len);
     // ip->rep.end = ip->rep.buf;
     // ip->rep.cur = ip->rep.buf;
     // assert(rc == ip->rep.total_len);
  }

  /* if not all of the bytes get written then the end point
   * is not reset to the beginning and it is equal to the
   * end of the buffer - since we can't add to the buffer we
   * must stop and wait until we can write more.
   */

  PRINT_TIME(sd, &tnow, &tprev, "do_write_not_cached: returning 0");
  return 0;
} /* do_write_not_cached */
