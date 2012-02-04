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


/*
 * Title   : reply_status.c
 * Author  : Tim Brecht
 * Date    : Sat Mar  8 14:45:13 EST 2003
 * 
 * NOTE: this server is designed so that these types of responses
 *       should really never happen. As a result they are not
 *       implemented efficiently, or very cleanly, etc.
 */

#include <string.h>
#include <stdlib.h>

#include "reply_status.h"
#include "debug.h"
#include "trace.h"
#include "reply_status.h"
#include "interest_set.h"
#include "stats.h"

#define STATUS_BUF_LEN (200)

int
reply_status_fill(struct info *ip, int status, char *str, int http_version)
{
  int len = 0;
  char tmp_buf[STATUS_BUF_LEN];
  time_t now;            /* Seconds since the epoch */
  struct tm * gmt_time;  /* Greenwich Mean Time */

  assert(ip != NULL);
  assert(ip->type == INFO_CLIENT);

  /* we won't be reading any more file or dynamic data */
  info_close_rep_fd(ip);

  /* we'll construct an HTTP reply in rep.static_buf */
  info_reset_rep_buf(ip);

  /* if we were caching, then we aren't anymore! */
  ip->rep.use_cache = 0;

  /* Store the status code for future reference (e.g. logging) */
  ip->rep.http_status = status;

  switch (status) {
    case HTTP_OK:
      /* NOTE/WARNING: so far this is only used to send a response
       * to escape requests
       */
      len = snprintf(ip->rep.buf, ip->rep.buf_size,
	  HTTP_OK_STR_ID_LEN, http_version, server_ident, 0UL);
      assert(len > 0);
      assert(len < ip->rep.buf_size);
      break;

    case HTTP_NOT_MODIFIED:
      now = time(NULL);
      gmt_time = gmtime( &now );
      len = strftime(tmp_buf, STATUS_BUF_LEN, "%a, %d %b %Y %T GMT", gmt_time);
      assert(len != 0);

      if( http_version == 1 ) {
	assert(HTTP_NOT_MODIFIED_STRLEN < ip->rep.buf_size);
        memcpy(ip->rep.buf, HTTP_11_NOT_MODIFIED_STR, HTTP_NOT_MODIFIED_STRLEN);
      } else {
	assert(HTTP_NOT_MODIFIED_STRLEN < ip->rep.buf_size);
        memcpy(ip->rep.buf, HTTP_10_NOT_MODIFIED_STR, HTTP_NOT_MODIFIED_STRLEN);
      }      
      memcpy(ip->rep.buf + HTTP_NOT_MODIFIED_STRLEN, tmp_buf, len);
      memcpy(ip->rep.buf + HTTP_NOT_MODIFIED_STRLEN + len, "\r\n\r\n", 4 );
      len += HTTP_NOT_MODIFIED_STRLEN + 4;
      assert(len < ip->rep.buf_size);
      break;

    case HTTP_NOT_FOUND:
      /* tack on the uri because it's nice/helpful for debugging */
      len = snprintf(tmp_buf, sizeof(tmp_buf), HTTP_NOT_FOUND_STR,
	  ip->req.uri);
      assert(len > 0);
      assert((unsigned int) len < sizeof(tmp_buf));
      len = snprintf(ip->rep.buf, ip->rep.buf_size,
	  HTTP_HDR_STR_ID_LEN "%s", http_version, status,
	  HTTP_NOT_FOUND_IDENT, server_ident, (unsigned long) len, tmp_buf);
      assert(len > 0);
      assert(len < ip->rep.buf_size);
      num_reply_http_not_found_count++;
      break;

    case HTTP_BAD_REQUEST:
      len = snprintf(tmp_buf, sizeof(tmp_buf), HTTP_BAD_REQUEST_STR);
      assert(len > 0);
      assert((unsigned int) len < sizeof(tmp_buf));
      len = snprintf(ip->rep.buf, ip->rep.buf_size,
	  HTTP_HDR_STR_ID_LEN "%s", http_version, status,
	  HTTP_BAD_REQUEST_IDENT, server_ident, (unsigned long) len, tmp_buf);
      assert(len > 0);
      assert(len < ip->rep.buf_size);
      num_reply_http_bad_request++;
      break;

    case HTTP_FORBIDDEN:
      /* tack on the uri because it's nice/helpful for debugging */
      len = snprintf(tmp_buf, sizeof(tmp_buf), HTTP_FORBIDDEN_STR,
	  ip->req.uri);
      assert(len > 0);
      assert((unsigned int) len < sizeof(tmp_buf));
      len = snprintf(ip->rep.buf, ip->rep.buf_size,
	  HTTP_HDR_STR_ID_LEN "%s", http_version, status,
	  HTTP_FORBIDDEN_IDENT, server_ident, (unsigned long) len, tmp_buf);
      assert(len > 0);
      assert(len < ip->rep.buf_size);
      num_reply_http_forbidden++;
      break;

    case HTTP_UNAUTHORIZED:
      /* tack on the uri because it's nice/helpful for debugging */
      len = snprintf(tmp_buf, sizeof(tmp_buf), HTTP_UNAUTHORIZED_STR,
	  ip->req.uri);
      assert(len > 0);
      assert((unsigned int) len < sizeof(tmp_buf));
      len = snprintf(ip->rep.buf, ip->rep.buf_size,
	  HTTP_HDR_STR_ID_LEN "%s", http_version, status,
	  HTTP_UNAUTHORIZED_IDENT, server_ident, (unsigned long) len, tmp_buf);
      assert(len > 0);
      assert(len < ip->rep.buf_size);
      num_reply_http_unauthorized++;

      break;

    case HTTP_SERVICE_UNAVAILABLE:
      len = snprintf(tmp_buf, sizeof(tmp_buf), HTTP_SERVICE_UNAVAILABLE_STR);
      assert(len > 0);
      assert((unsigned int) len < sizeof(tmp_buf));
      len = snprintf(ip->rep.buf, ip->rep.buf_size,
	  HTTP_HDR_STR_ID_LEN "%s", http_version, status,
	  HTTP_SERVICE_UNAVAILABLE_IDENT, server_ident, (unsigned long) len, tmp_buf);
      assert(len > 0);
      assert(len < ip->rep.buf_size);
      num_reply_http_service_unavailable++;
      break;

    case HTTP_NOT_IMPLEMENTED:
      len = snprintf(tmp_buf, sizeof(tmp_buf), HTTP_NOT_IMPLEMENTED_STR);
      assert(len > 0);
      assert((unsigned int) len < sizeof(tmp_buf));
      len = snprintf(ip->rep.buf, ip->rep.buf_size,
	  HTTP_HDR_STR_ID_LEN "%s", http_version, status,
	  HTTP_NOT_IMPLEMENTED_IDENT, server_ident, (unsigned long) len, tmp_buf);
      assert(len > 0);
      assert(len < ip->rep.buf_size);
      num_reply_http_not_implemented++;
      break;

    default:
      printf("reply_status_fill: unknown status = %d\n", status);
      assert(0);
      break;
  }

  ip->rep.end += len;
  ip->rep.total_len = len;
  ip->rep.nwritten = 0;
  ip->rep.offset = 0;

  PRINT_TIME(ip->sd, &tnow, &tprev,
      "reply_status_fill: rep.buf = %s",
      ip->rep.buf);
  PRINT_TIME(ip->sd, &tnow, &tprev,
      "reply_status_fill: rep.cur = %p rep.end = %p",
      ip->rep.cur, ip->rep.end);

  /* the following acts as a flag to say that this function was called */
  ip->req.method = HTTP_RETURN_ERROR;

  /* we're ready to write this reply back to the client */
  interest_set_change(ip->sd, ISET_WRITABLE);

  return 1;
}
