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


/*----------------------------------------------------------------------*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#include <errno.h>
#include <stdio.h>
/* #include <asm/delay.h> */

#include "reply_status.h"
#include "debug.h"
#include "trace.h"
#include "stats.h"
#include "options.h"
#include "interest_set.h"
#include "lru.h"
#include "aio_layer.h"
#include "post_sock.h"
#include "state.h"
#include "cache.h"
#include "syscalls.h"
#include "cntl_intr.h"
#include "fcgi.h"
#include "util.h"
#include "logging.h"

#ifdef CALL_STATS
#include "call_stats.h"
#endif /* CALL_STATS */


// #define LOCAL_DEBUG
#include "local_debug.h"

#ifdef HAVE_ENCRYPTION
#include <rpc/des_crypt.h>
#define CONTENTLEN      "content-length:"
#define CONTENTLEN_LEN  15
static char des_key[8];
static int des_key_initialized = 0;
static int get_content_length(char *ptr, int len);
static int encrypt_buf(const char *inbuf, char *outbuf, int len, int withhdr);
#endif                          /* HAVE_ENCRYPTION */

#define READ_BUF_SIZE          (1024)
#define DEFAULT_NAME           "index.html"

/*----------------------------------------------------------------------*/
int special_uri(char *uri, int sd);
void sleep_usecs(int usecs);
char *http_content_type(char *uri);
#ifdef MOD_SPECWEB99
extern int spec_dyn_req(struct info *);
#endif


/*----------------------------------------------------------------------*/
int work_around_set_value(cacheinfo * cache)
{
  int ret_val = 0;
  if (cache) {
    ret_val = (int) (cache->header_len + cache->buf_len);
  }
  return (ret_val);
}

/*----------------------------------------------------------------------*/
int socket_readable(struct info *ip)
{
  int sd = ip->sd;
  struct req *req = &ip->req;
  char *buf = 0;
  int max_bytes = 0;
  int nread = -1;
  int rc = 0;
  /* used for trace data */
  int trace_fd = sd;

  int err = 0;

  PRINT_TIME(sd, &tnow, &tprev, "socket_readable: entered ip = %p", ip);
  assert(sd > 2);

  ldbg("socket_readable: sd = %d\n", sd);
  ldbg("socket_readable: info = %p\n", info);
  ldbg("socket_readable: &info[sd] = %p\n", &info[sd]);
  ldbg("socket_readable: ip = %p\n", ip);
  ldbg("socket_readable: ip->conn_num = %d\n", ip->conn_num);
  ldbg("socket_readable: ip->req.num = %d\n", ip->req.num);

#ifdef AVOID_BUF_REUSE
  /* Added because it looks like there is a goofy
   * decision made in the Linux kernel that says
   * sendfile can return before it's finished doing
   * all that is requested even if the socket is in blocking mode 
   *
   * NOTE: this call assumes that info_reset_rep_buf is idempotent.
   *   Which as far as I can tell it is.
   */
  if (ip->type == INFO_CLIENT) {
        PRINT_TIME(sd, &tnow, &tprev, "client is calling info_reset_rep_buf");
  	info_reset_rep_buf(ip);
  }
#endif /* AVOID_BUF_REUSE */

  num_socket_read_calls++;

  /*
   * Determine where we would like to read into (buf) and the maximum number
   * of bytes to read (max_bytes).
   */
  switch (ip->type) {
  case INFO_CLIENT:
    /* read as much as our read buffer will allow */
    buf = req->end;
    max_bytes = (req->buf + req->buf_size) - req->end;
    break;
  case INFO_FASTCGI:
    /*
     * For FastCGI, we don't blindly read as much as we can.  We expect
     * FastCGI headers and reply data on the same socket, in more or less
     * alternating fashion.  Our goal is to take note of the headers, for
     * state, while assembling the reply data in a contiguous fashion.
     * Thus, we can't afford to read a header followed by data, since then
     * we would have to move the data to restore contiguity.  On the other
     * hand, we can afford to read data followed by a header; once we've
     * inspected the header, we won't need it anymore, and can overwrite
     * it with future data.
     */
    buf = req->end;
    max_bytes = req->content_len + req->padding_len + FCGI_HEADER_LEN;
    PRINT_TIME(sd, &tnow, &tprev, "socket_readable: buf = %p max_bytes = %d", 
        buf, max_bytes);
    switch (req->state) {
    case FCGI_READING_STDOUT:
    case FCGI_READING_STDERR:
      break;
    case FCGI_READING_HEADER:
    case FCGI_READING_END_REQUEST:
    case FCGI_READING_GET_VALUES_RESULT:
    case FCGI_READING_UNKNOWN_TYPE:
      /*
       * In all of these cases, we may have already read part of what
       * we want to read.  The number of bytes we've read so far is
       * stored in tmp_len.  Adjust buf and max_bytes accordingly.
       */
      buf += req->tmp_len;
      max_bytes -= req->tmp_len;
      PRINT_TIME(sd, &tnow, &tprev, "socket_readable: FCGI_READING_UNKNOWN_TYPE "
                 "buf = %p max_bytes = %d", buf, max_bytes);
      break;
    default:
      assert(0);
    }

    if (buf == 0) {
      printf("socket_readable: buf = %p when it shouldn't be\n", buf);
      exit(1);
    }

    if (buf + max_bytes > req->buf + req->buf_size) {
	printf("socket_readable: ERROR: Encountered a FastCGI reply that was too large!\n");
      printf("       Try increasing --dyn-buffer-size.\n");
      printf("  Note: that the buffer must currently be large enough to hold\n");
      printf("  an entire response from the app server -- because the userver\n");
      printf("  fills in the Content-Length, which isn't known until all of the\n");
      printf("  contents are in the buffer\n");
      printf("buf = %p\n", buf);
      printf("req->buf = %p\n", req->buf);
      printf("req->static_buf = %p\n", req->static_buf);
      printf("req->dyn_buf = %p\n", req->dyn_buf);
      printf("req->cur = %p\n", req->cur);
      printf("req->end = %p\n", req->end);
      printf("req->buf_size = %d\n", req->buf_size);
      printf("req->uri = %s\n", req->uri);
      printf("req->cookie = %s\n", req->cookie);
      printf("req->tmp = %s\n", req->tmp);

      assert(0);
    }
    break;
  default:
    assert(0);
  }
  ldbg("socket_readable(): sd=%d buf=%p max_bytes=%d\n", sd, buf, max_bytes);

  /*
   * We are about to call read(), and we'll be passing in max_bytes as the
   * maximum number of bytes to read.  If this number isn't positive, then
   * we've already messed up somehow.
   */
  assert(max_bytes > 0);

  if (options.use_aio_read) {
#ifdef HAVE_AIO_LAYER
    PRINT_TIME(sd, &tnow, &tprev, "socket_readable: aio_sock_read buf = %p "


               "len = %d handle = %p", buf, max_bytes, req->read_reg_handle);

    TRACE(EVT_AIO_SOCK_READ, trace_fd = sd;
          rc = aio_sock_read(sd, buf, max_bytes, req->read_reg_handle);
          err = errno;
        );
#else
    printf("socket_readable: aio not handled\n");
    exit(1);
#endif                          /* HAVE_AIO_LAYER */
  } else {

#ifdef HAVE_SSL
    if (ip->ssl) {
	TRACE(EVT_READ_SOCK, trace_fd = sd;
          rc = SSL_read(ip->ssl, buf, max_bytes);
          err = errno;
        );
    }  else {
	TRACE(EVT_READ_SOCK, trace_fd = sd;
          rc = read(sd, buf, max_bytes);
          err = errno;
        );
    }			   
#else
    TRACE(EVT_READ_SOCK, trace_fd = sd;
          rc = read(sd, buf, max_bytes);
          err = errno;
        );
#endif

  nread = rc;

#ifdef DEBUG_READ_BUFS
    if (nread >= 0) {
      print_buf("socket_readable (just read)", sd, buf, nread);
    }
#endif

    PRINT_TIME(sd, &tnow, &tprev, "socket_readable: max_bytes = %d nread = %d",
               max_bytes, nread);

#ifdef HAVE_EPOLL
    if (options.epoll_edge_triggered && nread >= 0) {
      if (nread < max_bytes) {
        sd_state_not_readable(info_ptr(sd));
      }
    }
#endif /* HAVE_EPOLL */

    /* When we've moved fully to this interface
     * we may wish to not pass the buffer as 
     * it can be obtained from the sd.
     * That way the post_sock_read call only requires
     * the sd and info that will be obtained
     * as return values from the call
     *
     */
    rc = post_sock_read(sd, nread, err);
  }

  PRINT_TIME(sd, &tnow, &tprev, "socket_readable: returning rc = %d nread = %d", rc, nread);
  return rc;
}

/*----------------------------------------------------------------------*/
int post_sock_read(int sd, int nread, int err)
{
  struct info *ip;
  struct req *req;
  int peer_sd;
  struct info *peer_ip;
  struct rep *peer_rep;
  FCGI_Header *hdr;
  FCGI_EndRequestBody *endreq;
  FCGI_UnknownTypeBody *unknown;
  char *ptr;
  char *name, *value;
  int namelen, valuelen;
  int sharedmem_datalen, sharedmem_size;
  int rc;

  ip = info_ptr(sd);
  assert(ip);
  req = &ip->req;

  PRINT_TIME(sd, &tnow, &tprev, "post_sock_read: nread = %d err = %d", nread, err);

#ifdef TRACK_STUCK_CONNS
  ip->read_count++;
  ip->read_nread = nread;
  ip->read_errno = err;
#endif /* TRACK_STUCK_CONNS */

  if (nread <= 0) {
    if (nread < 0) {
      switch (err) {
      case EAGAIN:
        num_socket_read_eagain++;
        ldbg("read() sd %d returned err EAGAIN\n", sd);
        PRINT_TIME(sd, &tnow, &tprev,
                   "post_sock_read: " "read failed - EAGAIN");
#ifdef HAVE_EPOLL
        if (options.epoll_edge_triggered) {
          sd_state_not_readable(ip);
        }
#endif /* HAVE_EPOLL */
        rc = -EAGAIN;
        goto post_sock_read_done;

      case ECONNRESET:
        num_socket_read_reset++;
        ldbg("read() sd %d returned err ECONNRESET\n", sd);
        PRINT_TIME(sd, &tnow, &tprev,
                   "post_sock_read: " "read failed - ECONNRESET");
        break;

      default:
        num_socket_read_failed_others++;
        ldbg("read() sd %d returned err %d\n", sd, err);
        PRINT_TIME(sd, &tnow, &tprev,
                   "post_sock_read: " "read failed - others");
        break;
      }
    } else {                    /* nread == 0 */
      if (ip->fsm_state == FSM_READING_REQUEST) {
        num_socket_read_nothing_first++;
      } else {
        num_socket_read_nothing_other++;
      }
      ldbg("read() sd %d returned 0\n", sd);
      PRINT_TIME(sd, &tnow, &tprev,
                 "post_sock_read: " "read failed ZERO bytes");
#ifdef HAVE_EPOLL
      if (options.epoll_edge_triggered) {
        sd_state_not_readable(ip);
      }
#endif /* HAVE_EPOLL */
    }

    PRINT_TIME(sd, &tnow, &tprev, "read failed");

    switch (ip->type) {
    case INFO_CLIENT:
#ifdef FIXHUP
#ifdef OLDWAY
      if (nread < 0) {
        if (ip->req.type == REQ_FASTCGI) {
          /* for a dynamic request -- the request might be in the queue already so
           * we can't just close down the connection. Instead we make note that the
           * client is gone (CLEARING/DRAINING) and eventually we'll try to do
           * something on the client socket and it will get closed down then.
           */ 
          convert_to_hupped(ip);
        } else if (ip->req.type == REQ_STATIC || ip->req.type == REQ_INVALID) {
          /* for a static request */
          set_fsm_state(ip, FSM_CONN_ERROR);
          do_close(ip, REASON_READ_CONN_ERROR);
        } else {
          printf("UNHANDLED REQUEST TYPE = %d\n", ip->req.type);
          assert(0);
        }
      } else {
        /* nread == 0 case */
        do_close(ip, REASON_READ_CONN_ERROR);
      }
#else
      if (nread < 0) {
          set_fsm_state(ip, FSM_CONN_ERROR);
      }
      do_close(ip, REASON_READ_CONN_ERROR);
#endif 
#else
      if (nread < 0) {
        set_fsm_state(ip, FSM_CONN_ERROR);
      }
      do_close(ip, REASON_READ_CONN_ERROR);
#endif
      break;
    case INFO_FASTCGI:
      //TODO: reconnect FastCGI socket?
      printf("Fastcgi connection broken? sd = %d\n", ip->sd);
      assert(0);
      break;
    default:
      assert(0);
    }
    rc = 0;
    goto post_sock_read_done;
  } /* if (nread <= 0) ... */

  PRINT_TIME(sd, &tnow, &tprev, "post_sock_read: read was successful");
  num_socket_read_bytes += (double) nread;
  num_socket_read_successful++;

  if (options.close_after_read) {
    do_close(ip, REASON_CLOSE_AFTER_READ);
    rc = 0;
    goto post_sock_read_done;
  }

  notification_off();

  lru_access(sd);

  PRINT_TIME(sd, &tnow, &tprev, "post_sock_read: ip type %d state %d", ip->type,
             get_fsm_state(sd));

  switch (ip->type) {
  case INFO_CLIENT:
    req->end += nread;
    assert(req->end <= req->buf + req->buf_size);

    if (req->state == PARSE_DONE) {
      /*
       * We've already finished reading and parsing the HTTP request, so we
       * must be reading additional content now.
       */
      switch (req->type) {
      case REQ_FASTCGI:
        assert(ip->fsm_state == FSM_FCGI_WRITING_STDIN);
        /* in this case the peer_sd is the client sd */
        peer_sd = ip->rep.fd;

        interest_set_change(sd, ISET_NOT_READABLE);
        interest_set_change(peer_sd, ISET_WRITABLE);
        break;

#ifdef MOD_SPECWEB99
      case REQ_SPECWEB99:
        if (req->end >= req->cur + req->content_len) {
          goto do_spec_dyn_req;
        }
        break;
#endif

      default:
        /*
         * Other types of requests (STATIC in particular) shouldn't have
         * any content.
         */
        assert(0);
      }
    } else {
      /*
       * We're still reading and parsing the HTTP request.
       */
      if (parse_bytes(&ip->req) < 0) {
        num_failed_parse++;
        notification_on();

        reply_status_fill(ip, HTTP_BAD_REQUEST, req->uri, req->close ? 0 : 1);
        req->state = PARSE_DONE;
      }

      if (req->state == PARSE_DONE) {

        ldbg("*** DONE PARSING\n");
        ldbg("*** method [%s]\n", info_req_method_str(req->method));
        ldbg("*** uri [%s]\n", nice_str(req->uri));
        ldbg("*** ver 1.%d\n", req->close ? 0 : 1);
        ldbg("*** query [%s]\n", nice_str(req->query_string));
        ldbg("*** cookie [%s]\n", nice_str(req->cookie));
        ldbg("*** ifmodsince 0x%08lx [%s]\n", (unsigned long) req->ifmodsince,
             http_time_to_str(req->ifmodsince));
        ldbg("*** content_len %d\n", req->content_len);
        ldbg("*** buf still has %d bytes\n", (int) (req->end - req->cur));
        PRINT_TIME(sd, &tnow, &tprev, "post_sock_read: parsing done uri = [%s]",
                   req->uri);

        //TODO:
        // Handle dynamic requests to invalid paths here by returning
        // 404 Not Found.  Currently they cause parse_bytes() to fail and
        // we return 400 Bad Request above.


        num_requests++;
        req->num = num_requests;
#ifdef TRACK_STUCK_CONNS
        ip->session_len++;
#endif /* TRACK_STUCK_CONNS */

#ifdef CALL_STATS
        if (options.call_stats) {
          call_stats_request(req->num, req->uri);
        }
#endif /* CALL_STATS */


        switch (req->type) {
        case REQ_STATIC:
          interest_set_change(sd, ISET_NOT_READABLE | ISET_WRITABLE);

          if (options.close_after_parse) {
            notification_on();
            do_close(ip, REASON_CLOSE_AFTER_PARSE);
            /*
             * return 0 because we don't want --full-read options to call
             * socket_readable again
             */
            rc = 0;
            goto post_sock_read_done;
          }

          switch (options.acting_as) {
          case OPT_ACTING_AS_HTTPD:
            rc = setup_http(ip);
            if (rc != SETUP_HTTP_CONTINUE) {
              goto post_sock_read_done;
            }
            break;
            /* TODO: 
             * Add call to handle bittorrent here.
             */
          case OPT_ACTING_AS_BITTORRENT:
          default:
            printf("post_sock_read: unknown value for option.acting_as = %d\n",
                   options.acting_as);
            assert(0);
          }                     /* switch (options.acting_as) */
          break;

        case REQ_FASTCGI:
          PRINT_TIME(sd, &tnow, &tprev, "calling fcgi_process_request");
          if (fcgi_process_request(sd) < 0) {
            /*
             * Some sort of error occurred.  However, the FastCGI code
             * should have already called reply_status_fill(), so there's
             * nothing more for us to do.
             */
            assert(req->method == HTTP_RETURN_ERROR);
            printf("unable to process FastCGI request sd %d\n", sd);
          }
          break;

#ifdef MOD_SPECWEB99
        case REQ_SPECWEB99:
          if (req->end >= req->cur + req->content_len) {
            goto do_spec_dyn_req;
          }
          if (req->cur + req->content_len > req->buf + req->buf_size) {
            printf
                ("ERROR: Encountered a SPECweb99 POST request that was too large!\n");
            printf("       Try increasing --read-buffer-size.\n");
            assert(0);
          }
          break;
#endif /* MOD_SPECWEB99 */

        default:
          printf("post_sock_read: unknown request type [%s]\n",
                 req_type_str(req->type));
          assert(0);
        }                       /* switch (req->type) */

        /* pick up any new connections that have been attempted */
        if (options.call_writable_from_readable) {
          notification_on();
          PRINT_TIME(sd, &tnow, &tprev,
                     "post_sock_read: calling socket_writable ", sd, &tnow,
                     &tprev);
          socket_writable(ip);
          notification_off();
        }
      } else {
        PRINT_TIME(sd, &tnow, &tprev, "post_sock_read: parsing not done");
      }
    }
    break;

  case INFO_FASTCGI:
    assert(ip->appserver != NULL);
    ip->appserver->stats.num_successful_reads++;
    ip->appserver->stats.total_bytes_read += nread;
    do {
      ldbg("%s content_len %d padding_len %d tmp_len %d nread %d\n",
           info_req_state_str(req->state), req->content_len, req->padding_len,
           req->tmp_len, nread);
      switch (req->state) {
      case FCGI_READING_HEADER:
        req->tmp_len += nread;
        nread = 0;
        assert(req->tmp_len <=
               req->content_len + req->padding_len + FCGI_HEADER_LEN);
        if (req->tmp_len ==
            req->content_len + req->padding_len + FCGI_HEADER_LEN) {
          hdr =
              (FCGI_Header *) (req->end + req->content_len + req->padding_len);
#ifdef LOCAL_DEBUG
          if (req->padding_len > 0) {
            ldbg("discarded %d bytes of padding preceding header\n",
                 req->padding_len);
          }
#endif                          /* LOCAL_DEBUG */
          switch (FCGI_HDR_TYPE(hdr)) {
          case FCGI_STDOUT:
          case FCGI_STDERR:
            ldbg("got FCGI_STDOUT/STDERR header\n");
            lhd(hdr, FCGI_HEADER_LEN);
            req->content_len = FCGI_HDR_CONTENT_LEN(hdr);
            req->padding_len = FCGI_HDR_PADDING_LEN(hdr);
            if (req->content_len > 0) {
              if (FCGI_HDR_TYPE(hdr) == FCGI_STDOUT) {
                req->state = FCGI_READING_STDOUT;
              } else {
                req->state = FCGI_READING_STDERR;
              }
            }
            break;
          case FCGI_END_REQUEST:
            ldbg("got FCGI_END_REQUEST header\n");
            lhd(hdr, FCGI_HEADER_LEN);
            req->content_len = FCGI_HDR_CONTENT_LEN(hdr);
            assert(req->content_len == sizeof(FCGI_EndRequestBody));
            req->padding_len = FCGI_HDR_PADDING_LEN(hdr);
            req->state = FCGI_READING_END_REQUEST;
            break;
          case FCGI_GET_VALUES_RESULT:
            ldbg("got FCGI_GET_VALUES_RESULT header\n");
            lhd(hdr, FCGI_HEADER_LEN);
            req->content_len = FCGI_HDR_CONTENT_LEN(hdr);
            req->padding_len = FCGI_HDR_PADDING_LEN(hdr);
            if (req->content_len > 0) {
              req->state = FCGI_READING_GET_VALUES_RESULT;
            }
            break;
          case FCGI_UNKNOWN_TYPE:
            ldbg("got FCGI_UNKNOWN_TYPE header\n");
            lhd(hdr, FCGI_HEADER_LEN);
            req->content_len = FCGI_HDR_CONTENT_LEN(hdr);
            assert(req->content_len == sizeof(FCGI_UnknownTypeBody));
            req->padding_len = FCGI_HDR_PADDING_LEN(hdr);
            req->state = FCGI_READING_UNKNOWN_TYPE;
            break;
          default:
            printf("unrecognized FastCGI header type [%d],read from %s, sd = %d\n",
                   FCGI_HDR_TYPE(hdr),ip->appserver->path,sd);
            lhd(hdr, FCGI_HEADER_LEN);
            assert(0);
          }
          req->tmp_len = 0;
        }
        break;

      case FCGI_READING_END_REQUEST:
        if (req->tmp_len + nread >= (int) sizeof(FCGI_EndRequestBody)) {
          endreq = (FCGI_EndRequestBody *) req->end;
          ldbg("got FCGI_END_REQUEST body\n");
          lhd(endreq, sizeof(FCGI_EndRequestBody));
          nread -= sizeof(FCGI_EndRequestBody) - req->tmp_len;
          req->tmp_len = sizeof(FCGI_EndRequestBody);
          req->state = FCGI_READING_HEADER;

          /* process the FCGI_END_REQUEST */
          switch (endreq->protocolStatus) {
          case FCGI_REQUEST_COMPLETE:
            break;
          case FCGI_CANT_MPX_CONN:
            printf("unhandled FastCGI protocol status [CANT_MPX_CONN]\n");
            assert(0);
          case FCGI_OVERLOADED:
            printf("unhandled FastCGI protocol status [OVERLOADED]\n");
            assert(0);
          case FCGI_UNKNOWN_ROLE:
            printf("unhandled FastCGI protocol status [UNKNOWN_ROLE]\n");
            assert(0);
          default:
            printf("unrecognized FastCGI protocol status [%d]\n",
                   endreq->protocolStatus);
            assert(0);
          }

          interest_set_change(ip->sd, ISET_NOT_READABLE);

          /* Get the client */
          peer_sd = ip->rep.fd;
          peer_ip = info_ptr(peer_sd);
          assert(peer_ip != NULL);

#ifdef FIXHUP
          peer_ip = info_ptr(peer_sd);
          assert(peer_ip);
          if (is_hungup(peer_ip)) {
             /* need to add it back to interest set */
             PRINT_TIME(peer_sd, &tnow, &tprev, "client got hup so adding to interest set");
             interest_set_change(peer_sd, ISET_WRITABLE | ISET_NOT_READABLE | ISET_INIT);
          } else {
             PRINT_TIME(peer_sd, &tnow, &tprev, "changing interest set");
             interest_set_change(peer_sd, ISET_WRITABLE | ISET_NOT_READABLE);
          }

          rc = setup_http(peer_ip);
          if (rc != SETUP_HTTP_CONTINUE) {
            printf("fcgi_do_readable(): setup_http() returned %d\n", rc);
            //TODO
          }
#else
          interest_set_change(peer_sd, ISET_WRITABLE);
          rc = setup_http(peer_ip);
          if (rc != SETUP_HTTP_CONTINUE) {
            printf("fcgi_do_readable(): setup_http() returned %d\n", rc);
            //TODO
          }
#endif
        } else {
          req->tmp_len += nread;
          nread = 0;
        }
        break;

      case FCGI_READING_STDOUT:
      case FCGI_READING_STDERR:
        ldbg("got FCGI_STDOUT/STDERR data\n");
        ip->appserver->stats.num_successful_data_reads++;
        /*
         * The (remaining) number of bytes of this STDOUT/STDERR
         * message is stored in req->content_len.  If nread is at
         * least req->content_len, then we've read all of the data,
         * so the next thing we expect is another FastCGI header.
         * Otherwise, there's still more data to read.
         */
        if (nread >= (int) req->content_len) {
          ip->appserver->stats.total_data_bytes_read += req->content_len;
          req->end += req->content_len;
          nread -= req->content_len;
          req->content_len = 0;
          req->state = FCGI_READING_HEADER;
        } else {
          ip->appserver->stats.total_data_bytes_read += nread;
          req->end += nread;
          req->content_len -= nread;
          nread = 0;
        }
        break;

      case FCGI_READING_GET_VALUES_RESULT:
        if (req->tmp_len + nread >= req->content_len) {
          ldbg("got FCGI_GET_VALUES_RESULT body\n");
          lhd(req->end, req->content_len);
          nread -= req->content_len - req->tmp_len;
          req->tmp_len = req->content_len;
          req->state = FCGI_READING_HEADER;

          /* process each of the name/value pairs */
          /* TBB - SOMEONE SHOULD CHECK THIS */
          ptr = req->end;
          while ((char *) ptr < req->end + req->content_len) {
            if (ptr[0] & 0x80) {
              namelen = ((ptr[0] & 0x7F) << 24)
                  + (ptr[1] << 16)
                  + (ptr[2] << 8)
                  + ptr[3];
              ptr += 4;
            } else {
              namelen = ptr[0];
              ptr++;
            }
            if (ptr[0] & 0x80) {
              valuelen = ((ptr[0] & 0x7F) << 24)
                  + (ptr[1] << 16)
                  + (ptr[2] << 8)
                  + ptr[3];
              ptr += 4;
            } else {
              valuelen = ptr[0];
              ptr++;
            }
            name = (char *) ptr;
            ptr += namelen;
            value = (char *) ptr;
            ptr += valuelen;
            ldbg("FCGI_GET_VALUES_RESULT: name %.*s value %.*s\n",
                 namelen, name, valuelen, value);

            /*
             * By this point, 'name' points to a non-terminated string
             * of length 'namelen', and similarly 'value' points to a
             * non-terminated string of length 'valuelen'.  Let's see
             * what we've got.
             */
            if (!strncmp(name, "SHAREDMEM_DATALEN", namelen)) {
              sharedmem_datalen = 0;
              while (valuelen > 0) {
                sharedmem_datalen *= 10;
                sharedmem_datalen += *value - '0';
                value++;
                valuelen--;
              }
              ldbg("SHAREDMEM_DATALEN is %d\n", sharedmem_datalen);
              peer_sd = ip->rep.fd;
              peer_ip = info_ptr(peer_sd);
              assert(peer_ip != NULL);
              peer_rep = &peer_ip->rep;
              sharedmem_size = options.dyn_buffer_size - fcgi_HTTP_hdr_len;
              ldbg("sharedmem_size is %d\n", sharedmem_size);
              if (sharedmem_datalen > sharedmem_size) {
                printf("SHAREDMEM_DATALEN %d > %d\n",
                       sharedmem_datalen, sharedmem_size);
                exit(1);
              }
              peer_rep->end = peer_rep->cur + sharedmem_datalen;
            } else {
              /* ignore */
            }
          }

          /* go idle unless we are currently servicing a client */
          peer_sd = ip->rep.fd;
          if (peer_sd == -1) {
            interest_set_change(ip->sd, ISET_NOT_READABLE);
          }
        } else {
          req->tmp_len += nread;
          nread = 0;
        }
        break;

      case FCGI_READING_UNKNOWN_TYPE:
        if (req->tmp_len + nread >= (int) sizeof(FCGI_UnknownTypeBody)) {
          unknown = (FCGI_UnknownTypeBody *) req->end;
          ldbg("got FCGI_UNKNOWN_TYPE body type=%d\n", unknown->type);
          lhd(unknown, sizeof(FCGI_UnknownTypeBody));
          nread -= sizeof(FCGI_UnknownTypeBody) - req->tmp_len;
          req->tmp_len = sizeof(FCGI_UnknownTypeBody);
          req->state = FCGI_READING_HEADER;

          /* process the FCGI_UNKNOWN_TYPE */
          switch (unknown->type) {
          case FCGI_SET_VALUES:
            printf("The appserver [%s] does not support FCGI_SET_VALUES!\n",
                   ip->appserver->path);
            printf
                ("Note:  This is a uServer-specific extension to the FastCGI protocol.\n");
            break;
          default:
            printf("The appserver [%s] does not support record type %d!\n",
                   ip->appserver->path, unknown->type);
            break;
          }
          exit(1);
        } else {
          req->tmp_len += nread;
          nread = 0;
        }
        break;

      default:
        assert(0);
      }                         /* switch (req->state) */
    } while (nread > 0);
    ldbg("%s content_len %d padding_len %d tmp_len %d nread %d\n",
         info_req_state_str(req->state),
         req->content_len, req->padding_len, req->tmp_len, nread);
    break;
  default:
    assert(0);
  }                             /* switch (ip->type) */

  rc = 1;

#ifdef MOD_SPECWEB99
  goto post_sock_read_done;

do_spec_dyn_req:
  /*
   * Any code to trace the amount of time spent handling a
   * dynamic request should be put into spec_dyn_req().
   *
   * FIXME: Instead of calling statically linked function
   * spec_dyn_req():
   * 1. Search the application servers under req->app for one
   *    whose ->path exists.  Treat that ->path as a module name,
   *    and do a dlopen() to dynamically load the module.
   * 2. Obtain an entry point (function pointer(?)) into this
   *    module (dlsym()?)
   * 3. Maintain a mapping of modules currently loaded, so for
   *    subsequent request we can directly invoke the entry
   *    function without having to load the module.
   * 4. Provide a mechanism for unloading modules (dlclose()).
   *
   * See the file spec_dyn_req.c for FIXMEs for SPECweb99
   * dynamic requests.
   *
   * Note that all dynamic writes to sockets are currently done
   * without using any write mechanisms in the userver, hence
   * they are blocking (even on sockets).
   *
   */
  rc = spec_dyn_req(ip);

  /*
   * The following code cleans things up when we are finished
   * writing the reply.  It is basically ripped right out of
   * post_socket_writable(), so if that code changes in any
   * significant way, this code should be updated.
   */

  // START of code ripped from post_socket_writable()
  PRINT_TIME(ip->sd, &tnow, &tprev, "post_sock_read: done reply");

  /* Possibly log the client's request */
  if( options.use_logging ) {
		log_request(ip);
	}

  num_replies++;

  /* done with this reply */
  info_close_rep_fd(ip);
#ifndef AVOID_BUF_REUSE
	/* TODO: Not sure if this is correct */
  info_reset_rep_buf(ip);
#endif /* AVOID_BUF_REUSE */

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

  /* read again to get EOF or other indication that socket
   * was closed */
  set_fsm_state(ip, FSM_READING_NEXT_REQUEST);

  interest_set_change(ip->sd, ISET_NOT_WRITABLE | ISET_READABLE);
  // END of code ripped from post_socket_writable()
#endif /* MOD_SPECWEB99 */

post_sock_read_done:

  notification_on();
  PRINT_TIME(sd, &tnow, &tprev, "post_sock_read: read %d bytes", nread);
  PRINT_TIME(sd, &tnow, &tprev, "post_sock_read: returning ip = %p", ip);
  return rc;
}                               /* post_sock_read */

/*----------------------------------------------------------------------*/
int special_uri(char *uri, int sd)
{
  struct info *ip = info_ptr(sd);
  int ret = 0;
  int call_exit = 0;
  int version = 0;

  assert(ip);

  if (ip->req.close) {
    version = 0;
  } else {
    version = 1;
  }

  if (strcmp(uri, "esc_stats_clear") == 0) {
    ret = 1;
    stats_clear();
  } else if (strcmp(uri, "esc_stats_print") == 0) {
    ret = 1;
    stats_print();
  } else if (strcmp(uri, "esc_stats_print_and_clear") == 0) {
    ret = 1;
    stats_print();
    stats_clear();
  } else if (strcmp(uri, "esc_stats_print_and_exit") == 0) {
    ret = 1;
    call_exit = 1;
    stats_print();
  } else if (strcmp(uri, "esc_exit") == 0) {
    ret = 1;
    call_exit = 1;
  } else if (strcmp(uri, "esc_start_timer") == 0) {
    gettimeofday(&timer_start, NULL);
    ret = 1;
  } else if (strcmp(uri, "esc_stop_timer") == 0) {
    gettimeofday(&timer_end, NULL);
    ret = 1;
  }

  /* need to reply to client to permit it to continue */
  if (ret == 1) {
    printf("Got special server escape request = %s\n", uri);
    reply_status_fill(ip, HTTP_OK, "Got escape request", version);

    /* may cause problems with HTTP/1.1 */

    if (call_exit) {
      printf("Exiting due to special escape request\n");
      do_close(ip, REASON_ESC_SEQUENCE);
      exit(0);
    }
  }
  /* if */
  return (ret);
}

/*----------------------------------------------------------------------*/
void sleep_usecs(int usecs)
{
  struct timeval tval;

  tval.tv_sec = usecs / 1000000;
  tval.tv_usec = usecs % 1000000;
  select(0, NULL, NULL, NULL, &tval);

}

/*----------------------------------------------------------------------*/
/* For now only recognize a few simple types */
char *http_content_type(char *uri)
{
  char *tmp = 0;
  int from_end = 0;
  int x = strlen(uri);
  char *ret = "text/html";

  tmp = uri + x;
  while (tmp != uri) {
    if (*tmp == '.') {
      break;
    }
    tmp--;
    from_end++;
  }

  ldbg("http_content_type: uri = [%s] tmp = [%s] from_end = %d\n", uri, tmp,
       from_end);
  switch (from_end) {

  case 3:
    if (strcmp(tmp, ".ps") == 0) {
      ret = "application/postscript";
    }
    break;

  case 4:
    if (strcmp(tmp, ".gif") == 0) {
      ret = "image/gif";
    } else if (strcmp(tmp, ".jpg") == 0) {
      ret = "image/jpeg";
    } else if (strcmp(tmp, ".txt") == 0) {
      ret = "text/plain";
    } else if (strcmp(tmp, ".pdf") == 0) {
      ret = "application/pdf";
    } else if (strcmp(tmp, ".htm") == 0) {
      ret = "text/html";
    }
    break;

  case 5:
    if (strcmp(tmp, ".html") == 0) {
      ret = "text/html";
    }
    break;
  }

  return (ret);
}


/*----------------------------------------------------------------------*/
int setup_http(struct info *ip)
{
  struct rep *rep;
  struct req *req;
  struct stat stat;
  int trace_fd = -1;
  cacheinfo *cache = NULL;
  int sd = ip->sd;
  int rc = -1;
  int err = 0;
  int version = 0;
  int uri_len = 0;
  struct info *peer_ip;
  struct req *peer_req;
  char *type = 0;
  int hdr_len = 0;
  char save_char = 0;
  int extra_hdr_len;
  int i;
#ifdef HAVE_ENCRYPTION
  int n, encrypted;
#endif                          /* HAVE_ENCRYPTION */

  assert(ip != NULL);
  rep = &ip->rep;
  req = &ip->req;

  if (req->close) {
    version = 0;
  } else {
    version = 1;
  }

  uri_len = strlen(req->uri);

  PRINT_TIME(sd, &tnow, &tprev, "setup_http: entered uri_len = %d", uri_len);

  set_fsm_state(ip, FSM_WRITING_REPLY);


  /* check to see if we should add index.html */
  if (options.doc_root_len == uri_len || req->uri[uri_len - 1] == '/') {
    strcat(req->uri, DEFAULT_NAME);
    PRINT_TIME(sd, &tnow, &tprev, "setup_http: rewrote uri = [%s]", req->uri);
  }

  /* Be default we don't skip anything */
  ip->skip = 0;
  ip->skip_info = 0;

  if (options.victim_str[0] != '\0') {
    /* if the specified victim_str matches any portion of the uri it is a vicitim */
    if (strstr(req->uri, options.victim_str) != 0) {
      if (options.victim_skip == 0) {
         printf("A victim for skipping was specified but the --victim-skip value is 0\n");
         exit(1);
      }
      ip->skip = options.victim_skip;
      ldbg("Victim string is %s and skip count is = %d\n", options.victim_str, ip->skip);
    } 
  }

  /* special quick check to see if request is an escape */
  if (req->uri[options.doc_root_len] == 'e') {
    /* returns 0 if not a special uri */
    if ((rc = special_uri(req->uri + options.doc_root_len, sd)) > 0) {
      notification_on();
      PRINT_TIME(sd, &tnow, &tprev,
                 "setup_http: " "returning after special_uri");
      return 1;
    }
  }

  switch (req->method) {

  case HTTP_GET:
  case HTTP_HEAD:
  case HTTP_POST:
    //TODO: HEADs probably shouldn't be counted in num_good_gets
    num_good_gets++;

    /* at this point we have the full request */
    if (options.delay && options.state_delay == OPT_NO_STATE_DELAY) {
      PRINT_DELAY(sd, &tnow, &tprev, "before delay sleep_usecs");
      sleep_usecs(options.delay);
      PRINT_DELAY(sd, &tnow, &tprev, "after delay sleep_usecs");
    }
    /* if */
    switch (req->type) {
    case REQ_STATIC:
      if (options.caching_on && req->type == REQ_STATIC) {
        PRINT_TIME(sd, &tnow, &tprev, "setup_http: calling cache_add");
        TRACE(EVT_CACHE_ADD, trace_fd = sd;
              cache = cache_add(req->uri, version);
              rc = work_around_set_value(cache);
              /* cache ? rc = (int) (cache->header_len + cache->buf_len) : rc = 0; */
            );
      }

      if (cache) {
        PRINT_TIME(sd, &tnow, &tprev, "setup_http: is in cache");

        /* Handle if-modified-since requests */
        if (req->ifmodsince > 0) {
          num_reqs_if_modified_since++;
          rc = check_if_modified_since(cache, req->ifmodsince);
          if (rc == 0) {
            /* Just send the 304 header */
            reply_status_fill(ip, HTTP_NOT_MODIFIED, req->uri, version);
            num_reply_http_not_modified++;
            return SETUP_HTTP_CONTINUE;
          }
        }

        rep->use_cache = 1;
        rep->cache = cache;
        rep->fd = cache->fd;
	rep->http_status = HTTP_OK;
        assert(cache->ref_count >= 1);

        /* Need to reset these each time so they start by pointing
         * at the beginning of respective buffers
         */
        rep->iovec[0].iov_base = cache->header;
        rep->iovec[0].iov_len = cache->header_len;
        rep->iovec[1].iov_base = cache->buf;
        rep->iovec[1].iov_len = cache->buf_len;

#ifdef HAVE_ENCRYPTION
        encrypted = 0;

        /* If encryption is compiled in, then determine if the option is
         * enabled and whether or not this file needs to be encrypted.
         * If it's to be encrypted then encrypt it into the encryption
         * buffer and the change iovec pointer in the reply to point to
         * the encryption buffer.
         */
        if (options.encrypt_data) {
          num_encrypt_tot_reqs++;
          if ((options.encrypt_max_file_size == 0UL) ||
              ((unsigned long) cache->buf_len <=
               options.encrypt_max_file_size)) {
            num_encrypt_size_ok++;
            if (options.encrypt_percent < 100) {
              n = (rand() % 100) + 1;
              if (n <= options.encrypt_percent)
                encrypted = 1;
            } else {
              encrypted = 1;
            }
            if (encrypted) {
              num_encrypt_encrypted++;
              rep->fd = cache->encryptfd;
              n = encrypt_buf(cache->buf, cache->encryptbuf, cache->buf_len,
                              options.skip_header);
              assert(n >= 0);
              num_encrypt_bytes += (long long) n;
              rep->iovec[1].iov_base = cache->encryptbuf;
            }
          }
        }
#endif                          /* HAVE_ENCRYPTION */

#ifdef HAVE_AIO_LAYER
#ifdef HAVE_ENCRYPTION
        if (encrypted) {
          cache->encrypt_buf_handle;
        } else {
          cache->file_reg_handle;
        }
#else
        rep->write_reg_handle = cache->file_reg_handle;
#endif                          /* HAVE_ENCRYPTION */
        PRINT_TIME(sd, &tnow, &tprev, "setup_http: write_reg_handle = %p",
                   rep->write_reg_handle);
#endif                          /* HAVE_AIO_LAYER */

        rep->buf = cache->header;
        rep->end = rep->cur = rep->buf;
        rep->total_len = cache->buf_len;
        if (!options.skip_header) {
          hdr_len = cache->header_len;
          rep->end += hdr_len;
        }
      } else {
        /* Not using caching */
        PRINT_TIME(sd, &tnow, &tprev,
                   "setup_http: is not in cache or not using cache");
        rep->use_cache = 0;
        rep->cache = NULL;

        TRACE(EVT_OPEN_FILE, trace_fd = sd;
              rc = open(req->uri, O_RDONLY);
              err = errno;
            );

        PRINT_TIME(sd, &tnow, &tprev, "setup_http: " "open returns %d", rc);
        rep->fd = rc;

        if (rep->fd < 0) {
          PRINT_TIME(sd, &tnow, &tprev,
                     "setup_http: "
                     "open failed - returning after calling do_close");
          PRINT_TIME(sd, &tnow, &tprev, "setup_http: " "errno = %d", err);
          num_failed_open++;
          /* perror (req->uri); */
          reply_status_fill(ip, HTTP_NOT_FOUND, req->uri, version);
          notification_on();
          return SETUP_HTTP_CONTINUE;
        }

        TRACE(EVT_FSTAT, trace_fd = rep->fd;
              rc = fstat(rep->fd, &stat);
            );

        if (rc < 0) {
          num_failed_stat++;
          /* perror (req->uri); */
          info_close_rep_fd(ip);
          notification_on();
          PRINT_TIME(sd, &tnow, &tprev,
                     "setup_http: "
                     "fstat failed - returning after calling do_close");
          return do_close(ip, REASON_FSTAT_FAILED);
        }

        /* Check that the request is for a file and not for a directory */
        if (S_ISDIR(stat.st_mode)) {
          PRINT_TIME(sd, &tnow, &tprev, "setup_http: " "file stat failed");

#ifdef HAVE_DIRECTORY_LISTING
          directory_list(ip);
#else
          reply_status_fill(ip, HTTP_FORBIDDEN, req->uri, version);
          return SETUP_HTTP_CONTINUE;
#endif                          /* HAVE_DIRECTORY_LISTING */
        } else {
          rep->total_len = stat.st_size;
        }


        /* Handle if-modified-since requests */
        if (req->ifmodsince > 0) {
          num_reqs_if_modified_since++;
          num_extra_fstat++;
          if (stat.st_mtime <= req->ifmodsince) {
            /* Hasn't been modified since, so send 304 hdr */
            reply_status_fill(ip, HTTP_NOT_MODIFIED, req->uri, version);
            num_reply_http_not_modified++;
            return SETUP_HTTP_CONTINUE;
          }
        }

        rep->end = rep->cur = rep->buf;
        if (!options.skip_header) {
          if (options.content_type) {
            type = http_content_type(req->uri);
            PRINT_TIME(sd, &tnow, &tprev, "setup_http: uri = [%s] type = [%s]",
                       req->uri, type);
	    /* TODO: why isn't this using reply_status_fill */
            hdr_len =
                snprintf(rep->cur, rep->buf_size, HTTP_OK_STR_ID_LEN_TYPE, version,
                        server_ident, (unsigned long int) rep->total_len, type);
	    assert(hdr_len > 0);
	    if (hdr_len >= rep->buf_size) {
              printf("ERROR setup_http: write header failed\n");
              printf("ERROR setup_http: snprintf returns %d buffer len = %d\n",
			      hdr_len, rep->buf_size);
              printf("ERROR setup_http: --reply-buffer-size = %d too small, must be > %d\n",
			      rep->buf_size, hdr_len);
	      exit(1);
	    }
          } else {
	    /* TODO: why isn't this using reply_status_fill */
            hdr_len =
                snprintf(rep->cur, rep->buf_size, HTTP_OK_STR_ID_LEN, version, server_ident,
                        (unsigned long int) rep->total_len);
	    assert(hdr_len > 0);
	    if (hdr_len >= rep->buf_size) {
              printf("ERROR setup_http: write header failed\n");
              printf("ERROR setup_http: snprintf returns %d buffer len = %d\n",
			      hdr_len, rep->buf_size);
              printf("ERROR setup_http: --reply-buffer-size = %d too small, must be > %d\n",
			      rep->buf_size, hdr_len);
	      exit(1);
	    }
          }
	  rep->http_status = HTTP_OK;
          rep->end += hdr_len;
        }
      }
      PRINT_TIME(sd, &tnow, &tprev,
                 "setup_http: buf = %p cur = %p end = %p end-cur = %d",
                 rep->buf, rep->cur, rep->end, rep->end - rep->cur);
      PRINT_TIME(sd, &tnow, &tprev, "setup_http: total_len = %d hdr_len = %d",
                 rep->total_len, hdr_len);
      break;

    case REQ_FASTCGI:
      PRINT_TIME(sd, &tnow, &tprev,
                 "setup_http: not using cache for FastCGI response");
      rep->use_cache = 0;
      rep->cache = NULL;

      peer_ip = info_ptr(rep->fd);
      assert(peer_ip != NULL);
      if (!(peer_ip->appserver->flags & APPSERVER_SHAREDMEM)) {
        /*
         * In the absence of shared memory, the FastCGI request buffer
         * and the client reply buffer are actually the same buffer.  The
         * FastCGI peer has only been maintaining its own cur and end
         * pointers, however, so we need to update our own pointers
         * before doing anything.
         */
        peer_req = &peer_ip->req;
        rep->cur = peer_req->cur;
        rep->end = peer_req->end;
      }
      rep->total_len = rep->end - rep->cur;

      /*
       * The FastCGI application typically sends some headers of its own,
       * such as Content-Type.  It then must send a blank line to delimit
       * the end of headers.
       * What we do here is as follows.  We scan the FastCGI response
       * until we hit "\r\n\r\n", counting up the number of bytes along
       * the way.  We then strip those bytes from the FastCGI response.
       * Having done so, we now know the "true" content length, and can
       * construct the complete set of headers.
       * This could probably be improved slightly by leaving the extra
       * header bytes in the FastCGI response buffer, thus saving a
       * memcpy().  However, it would complicate our handling of HEAD
       * requests (see below).
       */
      extra_hdr_len = 0;
#define ALLOW_LFLF              // kludge: recognize "\n\n" as well as "\r\n\r\n"
#ifdef ALLOW_LFLF
      for (i = 1; i < (int) rep->total_len; i++) {
        if (rep->cur[i] == '\n' &&
            ((i >= 3 && rep->cur[i - 3] == '\r' && rep->cur[i - 2] == '\n'
              && rep->cur[i - 1] == '\r')
             || rep->cur[i - 1] == '\n')) {
          extra_hdr_len = i + 1;
          break;
        }
      }
#else
      for (i = 3; i < rep->total_len; i++) {
        if (rep->cur[i - 3] == '\r' && rep->cur[i - 2] == '\n'
            && rep->cur[i - 1] == '\r' && rep->cur[i] == '\n') {
          extra_hdr_len = i + 1;
          break;
        }
      }
#endif
      if (extra_hdr_len == 0) {
        //TODO - FastCGI application didn't send blank line
        printf("*** the FastCGI app didn't send a blank line ***\n");
      }
      rep->total_len -= extra_hdr_len;
      if (options.skip_header) {
        rep->cur += extra_hdr_len;
      } else {
        /*
         * In fcgi_service_request(), we've computed the maximum length of
         * the HTTP reply headers that we need to prepend, and left that
         * amount of space (by incrementing cur).  The actual headers
         * typically won't need all of that space, because the content
         * length typically won't require 10 digits to express (this is
         * why we decrease hdr_len by a bit).  Fill in that space with
         * HTTP reply headers.  We use snprintf() for this, to be safe,
         * since we can't afford for any extra data to get overwritten.
         * As it is, there's no way to avoid one character getting
         * overwritten by the trailing nul.  So, we save that character
         * in save_char for subsequent restoration.
         */
        hdr_len = rep->cur - rep->buf;
        hdr_len -= max_ulong_digits() - ulong_digits(rep->total_len);
        save_char = rep->cur[0];
        rep->cur -= hdr_len;
        rc = snprintf(rep->cur, hdr_len + 1, HTTP_OK_STR_ID_LEN_NOBLANK,
                      version, server_ident, (unsigned long) rep->total_len);
        if (rc != hdr_len) {
           printf("rc = %d hdr_len = %d\n", rc, hdr_len);
        }
        assert(rc == (int) hdr_len);
        rep->cur[hdr_len] = save_char;
        hdr_len += extra_hdr_len;
	rep->http_status = HTTP_OK;

#ifdef QUEUE_BUFS
      //THIS IS A HACK
      //For some reason, we sometimes enter this branch after the buffer has been
      //transferred to another flow. This causing seg faults and other errors.
      //I currently can't figure out why this is, but the transfer should not
      //be happening if the FCGI request still isn't done. So, in the interest of
      //time, this 'done_write' flag indicates that it has passed this stage and
      //so we can transfer the buffer.
                                                                                                                                                             
      ip->done_write =1;
#endif

//#define HDR_CHECK
#ifdef HDR_CHECK
          printf("===================== HEADER INFO ===================\n");
          printf("HEADER: [%s]\n", rep->cur);
          printf("rep->buf = [%s]\n", rep->buf);
          printf("rep->cur = %p\n", rep->cur);
          printf("rep->buf = %p\n", rep->buf);
          printf("hdr_len (calc 1) = %d\n", rep->total_len);
          printf("hdr_len (calc 2) = %d\n", 
            max_ulong_digits() - ulong_digits(rep->total_len));
          printf("hdr_len = %d\n", hdr_len);
          printf("extra_hdr_len = %d\n", extra_hdr_len);
          printf("rep->total_len = %d\n", rep->total_len);
          printf("save_char = %c\n", save_char);
          printf("===================== HEADER INFO ===================\n");
#endif
      }
      break;

    default:
      assert(0);

    }                           /* switch (req->type) */

    /*
     * By now, any HTTP headers should have been written into req->buf,
     * and the following variables should be accurate:
     *   rep->use_cache, req->total_len, req->cur, req->end
     * and unless --skip-header is used
     *   hdr_len
     */

    /* for HEAD requests, omit the content */
    if (req->method == HTTP_HEAD) {
      info_close_rep_fd(ip);
      rep->total_len = 0;
      if (options.skip_header) {
        rep->end = rep->cur;
      } else {
        rep->end = rep->cur + hdr_len;
      }
    }

    if (!options.skip_header) {
      /*
       * Up to this point, rep->total_len meant the number of bytes in the
       * file or in the dynamic response (e.g. FastCGI).  Now, we amend it to
       * also include our prepended HTTP reply headers.  Thus, it is the
       * total number of bytes that we will send to the client.
       */
      rep->total_len += hdr_len;
    }

    rep->nwritten = 0;
    rep->offset = 0;
    DEBG(MSG_READABLE, "rep->buf = %p rep->cur = %p " "rep->end = %p\n",
         rep->buf, rep->cur, rep->end);
    break;

  case HTTP_NOT_IMPL:
    reply_status_fill(ip, HTTP_NOT_IMPLEMENTED, req->uri, version);
    /* Close the connection after sending this response. */
    req->close = 1;
    break;

  case HTTP_RETURN_ERROR:
    /*
     * We should have already called reply_status_fill() to write the
     * HTTP response.
     * Close the connection after sending this response.
     */
    req->close = 1;
    break;

  default:
    assert(0);

  }                             /* switch (req->method) */

  PRINT_TIME(sd, &tnow, &tprev, "setup_http: rep.total_len = %d",
             rep->total_len);

  return SETUP_HTTP_CONTINUE;

} /* setup_http */

/*----------------------------------------------------------------------*/

#ifdef HAVE_ENCRYPTION

/* Maybe not so quick but definitely a dirty routine to encrypt data to
 * be sent. The purpose is to spend some cycles and touch some memory.
 *   - The encryption routine works on blocks of 8 bytes. This routine
 *     assumes the extra space is there if needed.
 *   - There is no protection against encrypting in-flight data. I.e.
 *     we can be encrypting the data yet again while TCP/IP is still
 *     trying to push out the previous values.
 * We rely on the ecb_crypt() function being on the system. It uses DES
 * Electronic Code Book encoding versus Cipher Block Chaining, which is
 * faster (and less secure, but who cares). 
 */
static int encrypt_buf(const char *inbuf, char *outbuf, int len, int withhdr)
{
  int start = 0;
  int contentln = len;
  int elen, offset, total;
  char *p = NULL;

  /* Initialize our bugus -- and fixed -- DES key if necessary */
  if (!des_key_initialized) {
    memcpy(des_key, "-userver", 8);
    des_setparity(des_key);
    des_key_initialized = 1;
  }

  /* If header is embedded in the file then ned to parse the header
   * to find the length of the actual data.
   */
  if (withhdr) {

    /* First look for HTTP content-lenth token. */
    if (len <= CONTENTLEN_LEN)
      return -1;                /* header too short */
    for (offset = CONTENTLEN_LEN - 1; offset < len + CONTENTLEN_LEN;) {
      p = memchr(inbuf + offset, ':', len - offset);
      if (p == NULL) {
        break;
      }
      if (strncasecmp(p - CONTENTLEN_LEN + 1, CONTENTLEN, CONTENTLEN_LEN) != 0) {
        offset = p - inbuf + 1;
        p = NULL;
        continue;
      }
      break;
    }
    if (p == NULL) {
      /* Didn't find the content length! */
      return -1;
    }

    /* Get the length of the content */
    ++p;
    contentln = get_content_length(p, p - inbuf);
    if (contentln < 0)
      return -1;

    start = len - contentln;
    if (start < 0)
      return -1;
  }

  /* Copy all to the output buffer */
  memcpy(outbuf, inbuf, len);

  if (contentln == 0)
    return 0;

  /* Calculate how much to encrypt and pad w/ 0 */
  if (contentln & 0x7) {
    elen = 8 - (contentln & 0x7);
    memset(outbuf + contentln, 0, elen);
    contentln += elen;
  }

  /* Encrypt the data in the output buffer, possibly in chunks */
  total = 0;
  p = outbuf + start;
  while (total < contentln) {
    elen = contentln - total;
    if (elen > DES_MAXDATA)
      elen = DES_MAXDATA;
    if (ecb_crypt(des_key, p, elen, DES_ENCRYPT | DES_SW) != DESERR_NONE)
      return -1;
    p += elen;
    total += elen;
  }
  return contentln;
}

/* Get the length following the content-type token */
static int get_content_length(char *ptr, int len)
{
  int x;
  char ascval[16];
  char *end = ptr + len - 1;

  for (; (ptr <= end) && isspace((int) *ptr); ptr++);
  if (ptr > end)
    return -1;
  for (x = 0; (ptr + x <= end) && isdigit((int) *(ptr + x)); x++);
  if (x > 15)
    return -1;
  memcpy(ascval, ptr, x);
  ascval[x] = '\0';
  return atoi(ascval);
}

#endif /* HAVE_ENCRYPTION */

