/*
    userver -- (pronounced you-server or micro-server).
    This file is part of the userver, a high-performance web server designed for
    performance experiments.
          
    Copyright (C) 2005-2010  Tim Brecht
    Original Copyright (C) 2004  Craig Barkhouse

    Original Author: Craig Barkhouse <cabarkho@uwaterloo.ca>
    Additional Authors: Tim Brecht <brecht@cs.uwaterloo.ca>
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
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// #define LOCAL_DEBUG
#include "local_debug.h"
#include "myassert.h"
#include "common.h"
#include "info.h"
#include "interest_set.h"
#include "state.h"
#include "options.h"
#include "util.h"
#include "stats.h"
#include "fcgi.h"
#include "sock_listener.h"
#include "reply_status.h"
#include "debug.h"

/*
 * Notes on the build_FCGI_* family of functions
 * ---------------------------------------------
 *
 * These functions are used to construct a FastCGI message in a buffer,
 * typically just before sending that message over a socket.  Included
 * here are functions to build every possible type of FastCGI message,
 * although userver only ever builds some of these types.
 *
 * It should be noted that FastCGI messages generally vary in length, so
 * you don't know exactly how long they will be until you build them.
 * Each function takes a pointer (buf) and a size (bufsize) as its first
 * two arguments.  No function will never write beyond the specified bounds,
 * i.e. beyond buf+bufsize-1.  Each function returns the number of bytes
 * that the complete message would occupy, whether enough room is present
 * or not.  Thus, the correct way to check for success is to make sure the
 * return value is <= bufsize.  One trick you an use to calculate how large
 * a message will be, perhaps so that you can allocate just enough memory
 * for it, is to call a build_FCGI_* function with bufsize == 0.  Nothing
 * will be written, and the required size will be returned.
 *
 */


int
build_FCGI_hdr(char *buf, int bufsize, unsigned char type,
    unsigned short requestId, unsigned short contentLength)
{
  FCGI_Header *hdr;

  if (bufsize >= FCGI_HEADER_LEN) {
    hdr = (FCGI_Header *) buf;
    hdr->version = FCGI_VERSION_1;
    hdr->type = type;
    hdr->requestIdB1 = (requestId & 0xFF00) >> 8;
    hdr->requestIdB0 = (requestId & 0x00FF);
    hdr->contentLengthB1 = (contentLength & 0xFF00) >> 8;
    hdr->contentLengthB0 = (contentLength & 0x00FF);
    hdr->paddingLength = FCGI_PADDING_LEN(contentLength);
    hdr->reserved = 0;
  }

  return FCGI_HEADER_LEN;
}


int
build_FCGI_padding(char *buf, int bufsize, unsigned short contentLength)
{
  unsigned char paddingLength = FCGI_PADDING_LEN(contentLength);

  if (bufsize >= paddingLength) {
    memset(buf, '\0', paddingLength);
  }

  return paddingLength;
}


int
build_FCGI_len(char *buf, int bufsize, int len)
{
  if (len <= 127) {
    if (bufsize >= 1) {
      *buf = len;
    }
    return 1;
  }

  if (bufsize >= 4) {
    *buf++ = 0x80 | ((len & 0x7F000000) >> 24);
    *buf++ = (len & 0x00FF0000) >> 16;
    *buf++ = (len & 0x0000FF00) >> 8;
    *buf   = (len & 0x000000FF);
  }
  return 4;
}


int
build_FCGI_namevalue_pair(char *buf, int bufsize, const char *name,
    const char *value)
{
  int namelen, valuelen;
  int len = 0;

  assert(value != NULL);
  namelen = strlen(name);
  len += build_FCGI_len(buf + len, bufsize - len, namelen);
  valuelen = strlen(value);
  len += build_FCGI_len(buf + len, bufsize - len, valuelen);

  if (bufsize >= len + namelen) {
    memcpy(buf + len, name, namelen);
  }
  len += namelen;

  if (bufsize >= len + valuelen) {
    memcpy(buf + len, value, valuelen);
  }
  len += valuelen;

  return len;
}


int
build_FCGI_request(char *buf, int bufsize, unsigned char type,
    unsigned short requestId, unsigned short contentLength,
    const void *contentData)
{
  int len = 0;

  len += build_FCGI_hdr(buf, bufsize, type, requestId, contentLength);

  if (bufsize >= len + contentLength) {
    memcpy(buf + len, contentData, contentLength);
  }
  len += contentLength;

  len += build_FCGI_padding(buf + len, bufsize - len, contentLength);

  return len;
}


int
build_FCGI_request_str(char *buf, int bufsize, unsigned char type,
    unsigned short requestId, const char *contentStr)
{
  return build_FCGI_request(buf, bufsize,
      type, requestId, strlen(contentStr), contentStr);
}


int
build_FCGI_BEGIN_REQUEST(char *buf, int bufsize, unsigned short requestId,
    unsigned short role, unsigned char flags)
{
  FCGI_BeginRequestBody body;

  memset(&body, '\0', sizeof(body));
  body.roleB1 = (role & 0xFF00) >> 8;
  body.roleB0 = (role & 0x00FF);
  body.flags = flags;

  return build_FCGI_request(buf, bufsize,
      FCGI_BEGIN_REQUEST, requestId, sizeof(body), &body);
}


int
build_FCGI_ABORT_REQUEST(char *buf, int bufsize, unsigned short requestId)
{
  return build_FCGI_request(buf, bufsize,
      FCGI_ABORT_REQUEST, requestId, 0, NULL);
}


int
build_FCGI_END_REQUEST(char *buf, int bufsize, unsigned short requestId,
    unsigned long appStatus, unsigned char protocolStatus)
{
  FCGI_EndRequestBody body;

  memset(&body, '\0', sizeof(body));
  body.appStatusB3 = (appStatus & 0xFF000000) >> 24;
  body.appStatusB2 = (appStatus & 0x00FF0000) >> 16;
  body.appStatusB1 = (appStatus & 0x0000FF00) >> 8;
  body.appStatusB0 = (appStatus & 0x000000FF);
  body.protocolStatus = protocolStatus;

  return build_FCGI_request(buf, bufsize,
      FCGI_BEGIN_REQUEST, requestId, sizeof(body), &body);
}


int
build_FCGI_PARAMS_list(char *buf, int bufsize, unsigned short requestId,
    va_list arglist)
{
  va_list ap;
  int contentLength = 0;
  char *name, *value;
  int len = 0;

  /* Do a "mock build" to determine the contentLength. */
  va_copy(ap, arglist);
  while ((name = va_arg(ap, char *)) != NULL) {
    value = va_arg(ap, char *);
    if (value != NULL) {
      contentLength += build_FCGI_namevalue_pair(NULL, 0, name, value);
    }
  }
  va_end(ap);

  len += build_FCGI_hdr(buf, bufsize,
      FCGI_PARAMS, requestId, contentLength);

  if (bufsize >= len + contentLength) {
    va_copy(ap, arglist);
    while ((name = va_arg(ap, char *)) != NULL) {
      value = va_arg(ap, char *);
      if (value != NULL) {
	len += build_FCGI_namevalue_pair(buf + len, bufsize - len,
	    name, value);
      }
    }
    va_end(ap);
  }

  len += build_FCGI_padding(buf + len, bufsize - len, contentLength);

  return len;
}


int
build_FCGI_PARAMS(char *buf, int bufsize, unsigned short requestId, ...)
{
  va_list arglist;
  int len;

  va_start(arglist, requestId);
  len = build_FCGI_PARAMS_list(buf, bufsize, requestId, arglist);
  va_end(arglist);

  return len;
}


int
build_FCGI_STDIN(char *buf, int bufsize, unsigned short requestId,
    unsigned short contentLength, const void *contentData)
{
  return build_FCGI_request(buf, bufsize,
      FCGI_STDIN, requestId, contentLength, contentData);
}


int
build_FCGI_STDIN_str(char *buf, int bufsize, unsigned short requestId,
	const char *contentStr)
{
  return build_FCGI_request(buf, bufsize,
      FCGI_STDIN, requestId, strlen(contentStr), contentStr);
}


int
build_FCGI_STDOUT(char *buf, int bufsize, unsigned short requestId,
    unsigned short contentLength, const void *contentData)
{
  return build_FCGI_request(buf, bufsize,
      FCGI_STDOUT, requestId, contentLength, contentData);
}


int
build_FCGI_STDOUT_str(char *buf, int bufsize, unsigned short requestId,
	const char *contentStr)
{
  return build_FCGI_request(buf, bufsize,
      FCGI_STDOUT, requestId, strlen(contentStr), contentStr);
}


int
build_FCGI_STDERR(char *buf, int bufsize, unsigned short requestId,
    unsigned short contentLength, const void *contentData)
{
  return build_FCGI_request(buf, bufsize,
      FCGI_STDERR, requestId, contentLength, contentData);
}


int
build_FCGI_STDERR_str(char *buf, int bufsize, unsigned short requestId,
	const char *contentStr)
{
  return build_FCGI_request(buf, bufsize,
      FCGI_STDERR, requestId, strlen(contentStr), contentStr);
}


int
build_FCGI_DATA(char *buf, int bufsize, unsigned short requestId,
    unsigned short contentLength, const void *contentData)
{
  return build_FCGI_request(buf, bufsize,
      FCGI_DATA, requestId, contentLength, contentData);
}


int
build_FCGI_DATA_str(char *buf, int bufsize, unsigned short requestId,
	const char *contentStr)
{
  return build_FCGI_request(buf, bufsize,
      FCGI_DATA, requestId, strlen(contentStr), contentStr);
}


int
build_FCGI_GET_VALUES_list(char *buf, int bufsize, va_list arglist)
{
  va_list ap;
  int contentLength = 0;
  char *name;
  int len = 0;

  /* Do a "mock build" to determine the contentLength. */
  va_copy(ap, arglist);
  while ((name = va_arg(ap, char *)) != NULL) {
    contentLength += build_FCGI_namevalue_pair(NULL, 0, name, "");
  }
  va_end(ap);

  len += build_FCGI_hdr(buf, bufsize,
      FCGI_GET_VALUES, FCGI_NULL_REQUEST_ID, contentLength);

  if (bufsize >= len + contentLength) {
    va_copy(ap, arglist);
    while ((name = va_arg(ap, char *)) != NULL) {
      len += build_FCGI_namevalue_pair(buf + len, bufsize - len, name, "");
    }
    va_end(ap);
  }

  len += build_FCGI_padding(buf + len, bufsize - len, contentLength);

  return len;
}


int
build_FCGI_GET_VALUES(char *buf, int bufsize, ...)
{
  va_list arglist;
  int len;

  va_start(arglist, bufsize);
  len = build_FCGI_GET_VALUES_list(buf, bufsize, arglist);
  va_end(arglist);

  return len;
}


int
build_FCGI_GET_VALUES_RESULT_list(char *buf, int bufsize, va_list arglist)
{
  va_list ap;
  int contentLength = 0;
  char *name, *value;
  int len = 0;

  /* Do a "mock build" to determine the contentLength. */
  va_copy(ap, arglist);
  while ((name = va_arg(ap, char *)) != NULL) {
    value = va_arg(ap, char *);
    if (value != NULL) {
      contentLength += build_FCGI_namevalue_pair(NULL, 0, name, value);
    }
  }
  va_end(ap);

  len += build_FCGI_hdr(buf, bufsize,
      FCGI_GET_VALUES_RESULT, FCGI_NULL_REQUEST_ID, contentLength);

  if (bufsize >= len + contentLength) {
    va_copy(ap, arglist);
    while ((name = va_arg(ap, char *)) != NULL) {
      value = va_arg(ap, char *);
      if (value != NULL) {
	len += build_FCGI_namevalue_pair(buf + len, bufsize - len,
	    name, value);
      }
    }
    va_end(ap);
  }

  len += build_FCGI_padding(buf + len, bufsize - len, contentLength);

  return len;
}


int
build_FCGI_GET_VALUES_RESULT(char *buf, int bufsize, ...)
{
  va_list arglist;
  int len;

  va_start(arglist, bufsize);
  len = build_FCGI_GET_VALUES_RESULT_list(buf, bufsize, arglist);
  va_end(arglist);

  return len;
}


int
build_FCGI_UNKNOWN_TYPE(char *buf, int bufsize, unsigned char type)
{
  FCGI_UnknownTypeBody body;

  memset(&body, '\0', sizeof(body));
  body.type = type;

  return build_FCGI_request(buf, bufsize,
      FCGI_BEGIN_REQUEST, 0, sizeof(body), &body);
}


int
build_FCGI_SET_VALUES_list(char *buf, int bufsize, va_list arglist)
{
  va_list ap;
  int contentLength = 0;
  char *name, *value;
  int len = 0;

  /* Do a "mock build" to determine the contentLength. */
  va_copy(ap, arglist);
  while ((name = va_arg(ap, char *)) != NULL) {
    value = va_arg(ap, char *);
    if (value != NULL) {
      contentLength += build_FCGI_namevalue_pair(NULL, 0, name, value);
    }
  }
  va_end(ap);

  len += build_FCGI_hdr(buf, bufsize,
      FCGI_SET_VALUES, FCGI_NULL_REQUEST_ID, contentLength);

  if (bufsize >= len + contentLength) {
    va_copy(ap, arglist);
    while ((name = va_arg(ap, char *)) != NULL) {
      value = va_arg(ap, char *);
      if (value != NULL) {
	len += build_FCGI_namevalue_pair(buf + len, bufsize - len,
	    name, value);
      }
    }
    va_end(ap);
  }

  len += build_FCGI_padding(buf + len, bufsize - len, contentLength);

  return len;
}


int
build_FCGI_SET_VALUES(char *buf, int bufsize, ...)
{
  va_list arglist;
  int len;

  va_start(arglist, bufsize);
  len = build_FCGI_SET_VALUES_list(buf, bufsize, arglist);
  va_end(arglist);

  return len;
}
int mark_num_idle=0;

#ifdef FIXHUP
struct info *
fcgi_find_idle(struct info *client_ip)
{
  appserver_t *appserver = NULL;
  struct info *appserver_ip = NULL;

  assert(client_ip);
  assert(is_client_conn(client_ip));
#ifdef AVOID_BUF_REUSE 
  int appserver_sd = -1;
  
  /* first try to find an appserver in the idle_app_q */
  /* Because the AVOID_BUF_REUSE case was added later idle appserver could 
   * be either in the idle_app_q or in the data structures used previously
   */
  appserver_sd =  queue_get(idle_app_q);
  PRINT_TIME(client_ip->sd, &tnow, &tprev, "queue_get returns %d", appserver_sd);
  if (appserver_sd >= 0) {
    appserver_ip = info_ptr(appserver_sd);
    assert(appserver_ip != NULL);
    assert(is_app_conn(appserver_ip));
    assert(appserver_ip->rep.fd == -1);
    mark_num_idle--;
    PRINT_TIME(client_ip->sd, &tnow, &tprev, "found idle app server in idle_app_q sd = %d", appserver_ip->sd);
    return(appserver_ip);
  } else {

    assert(client_ip->req.app != NULL);
    /* if the client hasn't already been reset */
    if (client_ip->req.type != REQ_INVALID) {
      assert(client_ip->req.app->req_type == client_ip->req.type);
    }

    // num_busy = 0;
    appserver = client_ip->req.app->server;
    assert(appserver != NULL);
    do {
      appserver_ip = info_ptr(appserver->sd);
      assert(appserver_ip != NULL);
      assert(is_app_conn(appserver_ip));
      if (appserver_ip->rep.fd == -1) {
	mark_num_idle--;
        break;
      } else {
        PRINT_TIME(client_ip->sd, &tnow, &tprev, "app server sd = %d is busy with client = %d", 
          appserver_ip->sd, appserver_ip->rep.fd);
      }
      // num_busy++;
      appserver = appserver->next;
    } while (appserver != NULL);

    // assert(num_busy > 0);
    if (appserver == NULL) {
      /* were not able to find an appserver */
      appserver_ip = NULL;
    }
  }

  if (appserver_ip == NULL) {
    PRINT_TIME(client_ip->sd, &tnow, &tprev, "no idle appserver");
  } else {
    PRINT_TIME(client_ip->sd, &tnow, &tprev, "returning appserver_ip->sd = %d", appserver_ip->sd);
    assert(appserver_ip->sd > 0);
  }

  return(appserver_ip);
#else
  int num_busy = 0;

    if (client_ip == NULL) {
      PRINT_TIME(client_ip->sd, &tnow, &tprev, "nothing is idle");
      return NULL;
    }

  assert(client_ip != NULL);
  assert(client_ip->req.app != NULL);
  assert(client_ip->req.app->req_type == client_ip->req.type);
                                                                                                                                                             
  num_busy = 0;
  appserver = client_ip->req.app->server;
  assert(appserver != NULL);
  do {
    appserver_ip = info_ptr(appserver->sd);
    assert(appserver_ip != NULL);
    assert(appserver_ip->type == INFO_FASTCGI);
    if (appserver_ip->rep.fd == -1) {
      PRINT_TIME(client_ip->sd, &tnow, &tprev, "found idle app server %d", appserver_ip->sd);
      mark_num_idle--;
      return appserver_ip;
    }
    num_busy++;
    appserver = appserver->next;
  } while (appserver != NULL);
  assert(num_busy > 0);

  //printf("num busy = %d\n", num_busy);
  return NULL;
#endif 
}
#else
struct info *
fcgi_find_idle(struct info *peer_ip)
{

 appserver_t *appserver;
  struct info *ip;
  int num_busy;

#ifdef AVOID_BUF_REUSE 
  int sd;
  sd =  queue_get(idle_app_q);
  PRINT_TIME(sd, &tnow, &tprev, "queue_get returns %d", sd);
  if (sd >= 0) {
    ip = info_ptr(sd);
    assert(ip != NULL);
    assert(ip->type == INFO_FASTCGI);
    if (ip->rep.fd == -1) {
        //printf("dequeued an idle app server %d\n", mark_num_idle);
        mark_num_idle--;
      PRINT_TIME(sd, &tnow, &tprev, "found idle app server ip->sd = %d", ip->sd);
      return ip;
    }
  }

  if (peer_ip == NULL) {
        PRINT_TIME(sd, &tnow, &tprev, "nothing is idle");
	//printf("nothing is idle %d\n", mark_num_idle);
	return NULL;
  }
#endif


  assert(peer_ip != NULL);
  assert(peer_ip->req.app != NULL);
  assert(peer_ip->req.app->req_type == peer_ip->req.type);
                                                                                                                                                             
  num_busy = 0;
  appserver = peer_ip->req.app->server;
  assert(appserver != NULL);
  do {
    ip = info_ptr(appserver->sd);
    assert(ip != NULL);
    assert(ip->type == INFO_FASTCGI);
    if (ip->rep.fd == -1) {
	//printf("found an idle app server %d\n", mark_num_idle);
	mark_num_idle--;
      return ip;
    }
    num_busy++;
    appserver = appserver->next;
  } while (appserver != NULL);
  assert(num_busy > 0);

  //printf("num busy = %d\n", num_busy);
  return NULL;
}
#endif /* FIXHUP */


int
fcgi_go_idle(struct info *ip)
{
  assert(ip != NULL);
#ifdef FIXHUP
  assert(is_app_conn(ip));
#endif /* FIXHUP */

  ip->rep.fd = -1;
  ip->req.buf = ip->req.static_buf;
  ip->req.buf_size = options.read_buffer_size;
  ip->req.end = ip->req.cur = ip->req.buf;
  interest_set_change(ip->sd, ISET_NOT_READABLE | ISET_NOT_WRITABLE);
mark_num_idle++;
//printf("make app server idle # %d, with sd %d\n", mark_num_idle, ip->sd);

  PRINT_TIME(ip->sd, &tnow, &tprev, "app server going idle");
#ifdef AVOID_BUF_REUSE 
  queue_put(idle_app_q, ip->sd);
#endif
  return 0;
}


int fcgi_HTTP_hdr_len = -1;

/*
 * Return values:
 *   -1  can't service request, returning error to client
 *    0  service delayed
 *    1  service started
 *
 *    ip - is the appserver socket
 *    peer_sd is the client socket
 *
 *    Note that this routine doesn't actually do any processing
 *    it just associates the client and appserver sockets
 */
int
fcgi_service_request(struct info *ip, int peer_sd)
{
  struct req *req;
  struct info *peer_ip;
  struct req *peer_req;
  app_t *app;

  PRINT_TIME(ip->sd, &tnow, &tprev, "fcgi_service_request: peer_sd = %d", peer_sd);

  assert(ip != NULL);
  assert(ip->type == INFO_FASTCGI);
  req = &ip->req;
  peer_ip = info_ptr(peer_sd);
  assert(peer_ip != NULL);
  assert(peer_ip->type == INFO_CLIENT);
  peer_req = &peer_ip->req;
  assert(peer_req->type == REQ_FASTCGI);


//printf("servicing using server %p\n", peer_ip);

  /* allocate a dynamic buffer to store the FastCGI reply */
  assert(peer_ip->rep.buf == peer_ip->rep.static_buf);
  peer_ip->rep.dyn_buf = (char *) bufpool_alloc(dyn_bufpool, peer_sd);
  if (peer_ip->rep.dyn_buf == NULL) {
    /*
     * We're out of dynamic buffers.  Push the request back to the head of
     * the queue, and block the application.  We will unblock the application
     * once buffer space becomes available (see info_reset_rep_buf()).
     */
    app = ip->app;
    queue_push(app->req_q, peer_sd);
    app_block(app);
    //TODO: keep a stat for this?
    PRINT_TIME(ip->sd, &tnow, &tprev, "fcgi_service_request: no dynamic buffers");
   
		// printf("fcgi_service_request(): out of dynamic buffers\n");
		out_of_buffers_count++;
 
		if (!options.no_exit_on_no_buffers) {
		  printf("fcgi_service_request(): out of dynamic buffers\n");
			exit(-1);
		}

		
    return 0;
  }
  peer_ip->rep.buf = peer_ip->rep.dyn_buf;
  peer_ip->rep.buf_size = options.dyn_buffer_size;
  peer_ip->rep.end = peer_ip->rep.cur = peer_ip->rep.buf;

  /*
   * Unless we are using shared memory, we unify our request buffer with
   * the client's reply buffer.  This is so that the data that we accumulate
   * by reading from the FastCGI socket is already in the right place for
   * writing out to the client.
   */
  if (!(ip->appserver->flags & APPSERVER_SHAREDMEM)) {
    req->buf = peer_ip->rep.buf;
    req->buf_size = peer_ip->rep.buf_size;
  }
  req->end = req->cur = req->buf;

  /*
   * Unless we are omitting HTTP headers, leave some blank space at the
   * beginning of the buffer, large enough to hold the HTTP reply headers
   * that we will later prepend (see setup_http()).
   */
  if (!options.skip_header) {
    ldbg("fcgi_service_request: peer_ip->rep.cur = %p\n", peer_ip->rep.cur);
    peer_ip->rep.end = peer_ip->rep.cur += fcgi_HTTP_hdr_len;
    ldbg("fcgi_service_request: fcgi_HTTP_hdr_len = %d peer_ip->rep.cur = %p\n",
        fcgi_HTTP_hdr_len, peer_ip->rep.cur);
    req->end = req->cur += fcgi_HTTP_hdr_len;
  }

  /* update stats */
  ip->appserver->stats.num_serviced++;

  /* join the two info structs */
  ip->rep.fd = peer_sd;
  peer_ip->rep.fd = ip->sd;

  /* now get the ball rolling */
  interest_set_change(ip->sd, ISET_WRITABLE);
  set_fsm_state(peer_ip, FSM_FCGI_WRITING_SET_VALUES);

  return 1;
}


/*
 * Return values:
 *   -1  can't service request, returning error to client
 *    0  service delayed, or nothing to do
 *    1  service started
 *
 *    This is what the appserver side of the socket calls to do a request.
 */
int
fcgi_service_next_request(struct info *ip)
{
  int next_sd;
  app_t *app;
  int ret;
#ifdef FIXHUP
  struct info *client_ip = 0;
#endif /* FIXHUP */

  assert(ip != NULL);
#ifdef FIXHUP 
  assert(is_app_conn(ip));
#else
  assert(ip->type == INFO_FASTCGI);
#endif /* FIXHUP */

  /*
   * Flush any stale outgoing data.  Stale incoming data will be flushed by
   * fcgi_service_request().
   */
  ip->rep.end = ip->rep.cur = ip->rep.buf;

  /*
   * Make sure the application isn't blocked.
   */
  app = ip->app;
  if (app_is_blocked(app)) {
    //printf("fcgi_service_next_request(): app is blocked\n");
    PRINT_TIME(ip->sd, &tnow, &tprev, "app is blocked");
    fcgi_go_idle(ip);
    return 0;
  }

#ifdef FIXHUP
  assert(app);
  assert(ip->rep.fd > 0);
  client_ip = info_ptr(ip->rep.fd);
  assert(client_ip);
  next_sd = fcgi_find_next_request(client_ip, app);
  PRINT_TIME(ip->sd, &tnow, &tprev, "fcgi_find_next_request returns next_sd = %d", next_sd);
  if (next_sd < 0) {
    PRINT_TIME(ip->sd, &tnow, &tprev, "no more requests");
    fcgi_go_idle(ip);
    return 0;
  } 
#else
  /*
   * Make sure there is a request in the queue.
   */
  next_sd = queue_get(app->req_q);
  PRINT_TIME(ip->sd, &tnow, &tprev, "queue_get returns next_sd = %d", next_sd);
  if (next_sd < 0) {
    PRINT_TIME(ip->sd, &tnow, &tprev, "no more requests");
    fcgi_go_idle(ip);
    return 0;
  }
#endif /* FIXHUP */

  /*
   * Now try to service the request.  If this fails (due to lack of buffer
   * space)), push the request back to the head of the queue.  We will
   * unblock the queue once buffer space becomes available.
   */
	//printf("servicing req\n");
  PRINT_TIME(ip->sd, &tnow, &tprev, "try to service the next request next_sd = %d", next_sd);
  ret = fcgi_service_request(ip, next_sd);
  if (ret != 1) {
    //printf("fcgi_service_next_request(): can't service request\n");
    PRINT_TIME(ip->sd, &tnow, &tprev, "can't service request - going idle");
    fcgi_go_idle(ip);
    //TODO: keep a stat for this?
  }
  return ret;
}


/*
 * Really doesn't process the request but tries to link
 * a client request with an appserver that can later handle
 * that request.
 *
 * Return values:
 *   -1  can't service request, returning error to client
 *    0  service delayed
 *    1  service started
 *
 *    Try to find an appserver to handle the request coming
 *    from the client sd. In this case the peer_sd is the client side.
 *    If there are no other clients waiting for service and there
 *    is an idle appserver we go ahead and link the client and
 *    the appserver for later processing.
 *    If there are other clients waiting or there are no available
 *    appservers then we queue the request to be handled later.
 *
 */
#ifdef FIXHUP
 /* I'm not sure that when a EPOLLHUP occurs if requests that
  * have been queued are being handled properly.
  * Especially in cases where there are no buffers left to be used
  * to handle the request
  */
#endif

#ifdef FIXHUP
int
fcgi_process_request(int client_sd)
{
  struct info *appserver_ip = 0;
  struct info *client_ip = 0;
  struct req *client_req = 0;
  app_t *app = 0;
  app_stats_t *stats = 0;
  int qsize = 0;
  int retval = -1;
  int next_sd = -1;       /* what is the sd for the next request to handle */

  PRINT_TIME(client_sd, &tnow, &tprev, "entered ");

  /* Ensure that this really is a client sd and that it is still alive */
  client_ip = info_ptr(client_sd);
  assert(client_ip != NULL);
  assert(is_client_conn(client_ip));
  assert(!is_hungup(client_ip));

  /* Ensure that it is an FCGI request */
  client_req = &client_ip->req;
  assert(client_req->type == REQ_FASTCGI);

  /* get the application type and update stats */
  app = client_req->app;
  stats = &app->stats;
  stats->num_requests++;
  qsize = queue_size(app->req_q);
  stats->total_req_queue_size += qsize;
  if (qsize > stats->max_req_queue_size) {
    stats->max_req_queue_size = qsize;
  }

  /*
   * We will either service the request, if possible, or queue it for later
   * servicing.  In either case, we are not expecting anything more from
   * the client just yet.
   */
  interest_set_change(client_sd, ISET_NOT_READABLE | ISET_NOT_WRITABLE);

  /*
   *
   * If we aren't behind any other requests, and there is an application
   * server that is idle, then we can try to service the request immediately.
   * Otherwise we queue the current request. But we should still check
   * if there are other requests to handle and to handle them.
   *
   */

  /* if the queue is not empty we are legitimately queueing this request
   * behind the other stuff in the queue
   * if it is empty, we are only queuing it briefly to be dequeued shortly.
   * I think this makes the code simpler to write and to follow
   */
  PRINT_TIME(client_sd, &tnow, &tprev, "app->req_q len = %d", qsize);
  if (!queue_is_empty(app->req_q)) {
    PRINT_TIME(client_sd, &tnow, &tprev, "about to actually queue the request");
    stats->num_queued++;
  } else {
    PRINT_TIME(client_sd, &tnow, &tprev, "about to add request to queue for simplicity");
  }

  /*
   * Queue the request for later.
   */
  if (queue_put(app->req_q, client_sd) < 0) {
    PRINT_TIME(client_sd, &tnow, &tprev, "queue was full so reject request");
    reply_status_fill(client_ip, HTTP_SERVICE_UNAVAILABLE, client_req->uri,
	client_req->close ? 0 : 1);
    /*
     * If you want to forcibly close the connection when this condition
     * occurs, just uncomment the following line.  I don't think we
     * necessarily should close the connection.  --CAB
     */
    //client_req->close = 1;
    //TODO: keep a stat for this?
    set_fsm_state(client_ip, FSM_WRITING_REPLY);
    retval = -1;
  } else {
    /* the request has been successfully queued */
    set_fsm_state(client_ip, FSM_FCGI_REQUEST_QUEUED);

    /* See if there are any available appservers */
    appserver_ip = fcgi_find_idle(client_ip);

    if (appserver_ip != NULL) {
      PRINT_TIME(client_sd, &tnow, &tprev, "appserver_ip->sd = %d", appserver_ip->sd);
      /* now get a request out of the queue, there must be at least one 
       * because we just queued on above
       */
      next_sd = fcgi_find_next_request(client_ip, app);
      if (next_sd < 0) {
         printf("Tried to find the next request to process but failed even though one was just added\n");
         assert(0);
         exit(1);
      } else {
         PRINT_TIME(client_sd, &tnow, &tprev, "appserver_ip->sd = %d doing next_sd = %d", 
           appserver_ip->sd, next_sd);
         retval = fcgi_service_request(appserver_ip, next_sd);
      }
    } else {
      PRINT_TIME(client_sd, &tnow, &tprev, "no idle appservers so delay handling request"); 
      retval = 0;
    }
  }

  PRINT_TIME(client_sd, &tnow, &tprev, "returning retval = %d", retval);
  return(retval);
}
#else
int
fcgi_process_request(int peer_sd)
{
  struct info *ip = 0;
  struct info *peer_ip = 0;
  struct req *peer_req;
  app_t *app;
  app_stats_t *stats;
  int qsize;

  PRINT_TIME(peer_sd, &tnow, &tprev, "entered ");

  peer_ip = info_ptr(peer_sd);
  assert(peer_ip != NULL);
  peer_req = &peer_ip->req;
  assert(peer_req->type == REQ_FASTCGI);
  app = peer_req->app;
  stats = &app->stats;
  stats->num_requests++;
  qsize = queue_size(app->req_q);
  stats->total_req_queue_size += qsize;
  if (qsize > stats->max_req_queue_size) {
    stats->max_req_queue_size = qsize;
  }

  /*
   * We will either service the request, if possible, or queue it for later
   * servicing.  In either case, we are not expecting anything more from
   * the client just yet.
   */
  interest_set_change(peer_sd, ISET_NOT_READABLE);
  /*
   * If we aren't behind any other requests, and there is an application
   * server that is idle, then we can try to service the request immediately.
   *
   * After this call ip points at the appserver socket
   */
  if (queue_is_empty(app->req_q) && (ip = fcgi_find_idle(peer_ip)) != NULL) {
    PRINT_TIME(peer_sd, &tnow, &tprev, "found idle %d\n", ip->sd);
    /* have the appserver socket (ip) process the request from the client (peer_sd) */
    return fcgi_service_request(ip, peer_sd);
  }

  /*
   * Queue the request for later.
   */
  PRINT_TIME(peer_sd, &tnow, &tprev, "queuing the request");
  if (queue_put(app->req_q, peer_sd) < 0) {
    PRINT_TIME(peer_sd, &tnow, &tprev, "queue was full so reject request");
    //printf("fcgi_process_request(): can't queue request\n");
    reply_status_fill(peer_ip, HTTP_SERVICE_UNAVAILABLE, peer_req->uri,
	peer_req->close ? 0 : 1);
    /*
     * If you want to forcibly close the connection when this condition
     * occurs, just uncomment the following line.  I don't think we
     * necessarily should close the connection.  --CAB
     */
    //peer_req->close = 1;
    //TODO: keep a stat for this?
    set_fsm_state(peer_ip,FSM_WRITING_REPLY);
    return -1;
  }

  stats->num_queued++;
  return 0;
}
#endif /* FIXHUP */


int
fcgi_add_to_reply_buffer_httpd(struct info *ip)
{
  struct req *req;
  struct rep *rep;
  int sd, peer_sd;
  unsigned short requestId;
  struct info *peer_ip;
  struct info_listen_struct *ilp;
  struct req *peer_req;
  appserver_t *appserver;
  struct sockaddr_in *listen_addr, *peer_addr;
  unsigned max_bytes;
  unsigned content_bytes;
  unsigned num_bytes, bytes_added;
  int sharedmem_offset;
  int done;

  req = &ip->req;
  rep = &ip->rep;
  sd = ip->sd;
  peer_sd = ip->rep.fd;
  requestId = peer_sd;
  assert(get_fsm_state(peer_sd) != FSM_WRITING_REPLY);
  peer_ip = info_ptr(peer_sd);
  assert(peer_ip != NULL);
  assert(peer_ip->req.app != NULL);

  if (peer_ip->fsm_state == FSM_FCGI_WAITING_FOR_REPLY) {
    /* return 0, meaning there's nothing more to do right now */
    return 0;
  }

  peer_req = &peer_ip->req;
  assert(peer_req->type == REQ_FASTCGI);
  assert(peer_req->app != NULL);
  appserver = peer_req->app->server;
  assert(appserver != NULL);

  done = 0;
  bytes_added = 0;
  max_bytes = (rep->buf + rep->buf_size) - rep->end;
  //dpariag rep->offset = 0;
  do {
    num_bytes = 0;
    ldbg("fcgi_add_to_reply_buffer_httpd(): state %s\n",
	fsm_state_to_string(peer_ip->fsm_state));

    switch (peer_ip->fsm_state) {
      case FSM_FCGI_WRITING_SET_VALUES:
	if (appserver->flags & APPSERVER_SHAREDMEM) {
	  sharedmem_offset =
	      peer_ip->rep.buf - dyn_bufpool->buf + fcgi_HTTP_hdr_len;
	  num_bytes = build_FCGI_SET_VALUES(rep->end,
	      max_bytes,
	      "SHAREDMEM_OFFSET", format_str("%d", sharedmem_offset),
	      NULL);
	  if (num_bytes <= max_bytes) {
	    set_fsm_state(peer_ip, FSM_FCGI_WRITING_BEGIN_REQUEST);
	  }
	} else {
	  /* nothing to do */
	  set_fsm_state(peer_ip, FSM_FCGI_WRITING_BEGIN_REQUEST);
	}
	break;

      case FSM_FCGI_WRITING_BEGIN_REQUEST:
	num_bytes = build_FCGI_BEGIN_REQUEST(rep->end,
	    max_bytes,
	    requestId,
	    FCGI_RESPONDER,
	    FCGI_KEEP_CONN);
	if (num_bytes <= max_bytes) {
	  set_fsm_state(peer_ip, FSM_FCGI_WRITING_PARAMS_1);
	}
	break;

      case FSM_FCGI_WRITING_PARAMS_1:
	ilp = &info_listener[peer_ip->parent_sd];
	listen_addr = &ilp->addr.u.sa_in;
	peer_addr = &peer_ip->addr.u.sa_in;
	num_bytes = build_FCGI_PARAMS(rep->end,
	    max_bytes,
	    requestId,
	    // request-independent parameters
	    "SERVER_SOFTWARE", VERSION,
	    "SERVER_NAME", options.hostname,
	    "GATEWAY_INTERFACE", "CGI/1.1",
	    // request-specific parameters
	    "SERVER_PROTOCOL", peer_req->close ? "HTTP/1.0" : "HTTP/1.1",
	    "SERVER_PORT", format_str("%d", ntohs(listen_addr->sin_port)),
	    "SCRIPT_NAME", peer_req->uri + options.doc_root_len,
             /* Added for support for php */
	    "SCRIPT_FILENAME", peer_req->uri + options.doc_root_len,
	    "REMOTE_ADDR", format_str("%s", inet_ntoa(peer_addr->sin_addr)),
	    "HTTP_COOKIE", peer_req->cookie_len > 0 ? peer_req->cookie : NULL,
	    NULL);
	if (num_bytes <= max_bytes) {
	  set_fsm_state(peer_ip, FSM_FCGI_WRITING_PARAMS_2);
	}
	break;

      case FSM_FCGI_WRITING_PARAMS_2:
	switch (peer_req->method) {
	  case HTTP_POST:
	    num_bytes = build_FCGI_PARAMS(rep->end,
		max_bytes,
		requestId,
		"REQUEST_METHOD", "POST",
		//TODO: CONTENT_TYPE?
		"CONTENT_LENGTH", format_str("%d", peer_req->content_len),
#ifdef OLDWAY
		"QUERY_STRING", peer_req->query_string,
#else
		"QUERY_STRING", "Non-null query string",
#endif
		NULL);
	    break;
	  case HTTP_GET:
	  case HTTP_HEAD:
	    num_bytes = build_FCGI_PARAMS(rep->end,
		max_bytes,
		requestId,
		"REQUEST_METHOD", "GET",
		"QUERY_STRING", peer_req->query_string,
		NULL);
	    break;
	  default:
	    printf("invalid request method [%d]\n", peer_req->method);
	    assert(0);
	}
	if (num_bytes <= max_bytes) {
	  set_fsm_state(peer_ip, FSM_FCGI_WRITING_END_PARAMS);
	}
	break;

      case FSM_FCGI_WRITING_END_PARAMS:
	num_bytes = build_FCGI_PARAMS(rep->end,
	    max_bytes,
	    requestId,
	    NULL);
	if (num_bytes <= max_bytes) {
	  set_fsm_state(peer_ip, FSM_FCGI_WRITING_STDIN);
	}
	break;

      case FSM_FCGI_WRITING_STDIN:
	if (peer_ip->req.content_len > 0) {
	  content_bytes = peer_ip->req.end - peer_ip->req.cur;
	  if (content_bytes > (unsigned) peer_ip->req.content_len) {
	    /*
	     * The client has sent more content than we were expecting (from
	     * the Content-Length header).  We won't consider this an error.
	     * For one thing, when testing userver with a telnet client,
	     * there will always be an extra LF or CRLF at the end.  -CAB
	     */
	    ldbg("ignoring %d extra content bytes\n",
		content_bytes - peer_ip->req.content_len);
	    content_bytes = peer_ip->req.content_len;
	  }
	  if (content_bytes > 0) {
	    num_bytes = build_FCGI_STDIN(rep->end,
		max_bytes,
		requestId,
		content_bytes,
		peer_ip->req.cur);
	    if (num_bytes > max_bytes) {
	      /*
	       * We didn't have enough room to build an FCGI_STDIN message
	       * using all of the available content.  Try again with the
	       * content size reduced by an intelligent amount.
	       */
	      content_bytes -= (num_bytes - max_bytes);
	      if (content_bytes > 0) {
		num_bytes = build_FCGI_STDIN(rep->end,
		    max_bytes,
		    requestId,
		    content_bytes,
		    peer_ip->req.cur);
	      }
	    }
	  }
	  if (num_bytes <= max_bytes) {
	    peer_ip->req.content_len -= content_bytes;
	    peer_ip->req.cur += content_bytes;
	    if (peer_ip->req.cur == peer_ip->req.end) {
	      peer_ip->req.end = peer_ip->req.cur = peer_ip->req.buf;
	    }
	    assert(peer_ip->req.content_len >= 0);
	  }
	  if (peer_ip->req.content_len > 0) {
	    ldbg("waiting for more content from client\n");
	    interest_set_change(sd, ISET_NOT_WRITABLE);
	    interest_set_change(peer_sd, ISET_READABLE);
	    done = 1;
	  }
	} else {
	  set_fsm_state(peer_ip, FSM_FCGI_WRITING_END_STDIN);
	}
	break;

      case FSM_FCGI_WRITING_END_STDIN:
	num_bytes = build_FCGI_STDIN(rep->end,
	    max_bytes,
	    requestId,
	    0,
	    NULL);
	if (num_bytes <= max_bytes) {
	  interest_set_change(sd, ISET_NOT_WRITABLE);
	  set_fsm_state(peer_ip, FSM_FCGI_WAITING_FOR_REPLY);
	}
	break;

      case FSM_FCGI_WAITING_FOR_REPLY:
	req->state = FCGI_READING_HEADER;
	req->tmp_len = 0;
	req->content_len = 0;
	req->padding_len = 0;
	done = 1;
	break;

      default:
	printf("invalid fsm_state [%d]\n", peer_ip->fsm_state);
	assert(0);
    }

    if ((int) num_bytes > rep->buf_size) {
      printf("fcgi_add_to_reply_buffer_httpd(): we will never have enough room!\n");
      interest_set_change(sd, ISET_NOT_WRITABLE);
      //TODO: return some kind of error to the client
      do_close(peer_ip, REASON_NO_SPACE);
      return -1;
    } else if (num_bytes > max_bytes) {
      ldbg("fcgi_add_to_reply_buffer_httpd(): we currently don't have enough room\n");
      done = 1;
    } else {
      /* we successfully added some bytes */
      bytes_added += num_bytes;
      rep->end += num_bytes;
      max_bytes -= num_bytes;
    }
  } while (!done);

  return bytes_added;
}


#ifdef DO_FCGI_GET_SERVER_VALUES
int
fcgi_get_server_values(appserver_t *appserver)
{
  char buf[1024];
  int num_bytes;
  int ret;

  assert(appserver != NULL);

  num_bytes = build_FCGI_GET_VALUES(buf, sizeof(buf),
      /* standard FastCGI variables defined in fastcgi.h */
      FCGI_MAX_CONNS,
/*
      FCGI_MAX_REQS,
      FCGI_MPXS_CONNS,
*/
      /* last parameter must be NULL */
      NULL);
  ldbg("sending FCGI_GET_VALUES message to sd %d\n", appserver->sd);
  lhd(buf, num_bytes);
  ret = write(appserver->sd, buf, num_bytes);
  if (ret != num_bytes) {
    perror("write");
    return 2;
  }

  interest_set_change(appserver->sd, ISET_READABLE);

  return 0;
}
#endif /* DO_FCGI_GET_SERVER_VALUES */


int
fcgi_set_server_values(appserver_t *appserver)
{
  char buf[1024];
  int sharedmem_size;
  int num_bytes;
  int ret;

  assert(appserver != NULL);

  if (appserver->flags & APPSERVER_SHAREDMEM) {
    assert(options.dyn_use_shared_mem);
    sharedmem_size = options.dyn_buffer_size - fcgi_HTTP_hdr_len;
    num_bytes = build_FCGI_SET_VALUES(buf, sizeof(buf),
	"SHAREDMEM_FILENAME", dyn_bufpool->track->mmap_file,
	"SHAREDMEM_SIZE", format_str("%d", sharedmem_size),
	NULL);
    ldbg("sending FCGI_SET_VALUES message to sd %d\n", appserver->sd);
    lhd(buf, num_bytes);
    ret = write(appserver->sd, buf, num_bytes);
    if (ret != num_bytes) {
      perror("write");
      return 2;
    }
    interest_set_change(appserver->sd, ISET_READABLE);
  } else {
    PRINT_TIME(NOFD, &tnow, &tprev, "Only fcgi_set_server_values only sends data if using shared memory");
  }

  return 0;
}

#ifdef FIXHUP
/* Because we are now handling the case where clients might disappear
 * on us (timeout and we get e.g., HUP) we provide a more general
 * routine for getting the next client request to process
 */
int
fcgi_find_next_request(struct info *client_ip, app_t *app)
{
  int next_client_sd = -1;
  struct info *next_client_ip = 0;

  assert(client_ip);
  assert(is_client_conn(client_ip));
  assert(app);

  /*
   * see if there is a request in the queue.
   */
  next_client_sd = queue_get(app->req_q);
  PRINT_TIME(client_ip->sd, &tnow, &tprev, "queue_get returns next_client_sd = %d", next_client_sd);
  if (next_client_sd > 0) {
    /* I've made changes elsewhere that mean that we should never get something out of this
     * queue that is not valid -- however we now need to check if the client still
     * wants the result or not 
     */
    next_client_ip = info_ptr(next_client_sd);
    assert(next_client_ip != 0);
    assert(is_client_conn(next_client_ip));

    /* It might be the case that the client has had
     * their connection reset in which case they might
     * not need to have anything done 
     *
     * NOTE: need to loop here incase there are multiple clients
     * that are no longer around.
     */
    while (is_hungup(next_client_ip)) {
      PRINT_TIME(client_ip->sd, &tnow, &tprev, "client for next request timed out so skipping next_client_sd = %d", next_client_sd);

      /* now that this is out of the queue it can be removed */
      /* need to do something here to get this client to clear/finish up */
      /* possibly the same thing that gets done when we finish handling the request */
      unblock_hupped_client(next_client_ip);

      /* now see if there is another client in the queue */
      next_client_sd = queue_get(app->req_q);
      PRINT_TIME(client_ip->sd, &tnow, &tprev, "queue_get returns next_client_sd = %d", next_client_sd);
      if (next_client_sd > 0) {
	next_client_ip = info_ptr(next_client_sd);
      } else {
	PRINT_TIME(client_ip->sd, &tnow, &tprev, "no more requests left");
	break;
      }
    }
  } else {
    PRINT_TIME(client_ip->sd, &tnow, &tprev, "nothing in the application request");
  }

  PRINT_TIME(client_ip->sd, &tnow, &tprev, "returning next_client_sd = %d", next_client_sd);
  return(next_client_sd);
}
#endif /* FIXHUP */
