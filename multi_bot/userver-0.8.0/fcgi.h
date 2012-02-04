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

#ifndef FCGI_H
#define FCGI_H

#include <stdarg.h>

#include "fastcgi.h"
#include "info.h"


/*
 * The following variable stores the number of bytes we set aside at the
 * beginning of each FastCGI reply buffer so that we have room to prepend
 * HTTP headers.
 */
extern int fcgi_HTTP_hdr_len;


#ifndef FCGI_ALIGN
#define FCGI_ALIGN 8
#endif

#define FCGI_PADDING_LEN(contentLength) \
  ((FCGI_ALIGN - ((contentLength) % FCGI_ALIGN)) % FCGI_ALIGN)

#define FCGI_HDR_TYPE(hdr) \
  (hdr)->type

#define FCGI_HDR_REQUESTID(hdr) \
  (((unsigned short) (hdr)->requestIdB1 << 8) + (hdr)->requestIdB0)

#define FCGI_HDR_CONTENT_LEN(hdr) \
  (((unsigned short) (hdr)->contentLengthB1 << 8) + (hdr)->contentLengthB0)

#define FCGI_HDR_PADDING_LEN(hdr) \
  (hdr)->paddingLength


/* generic routines for building FastCGI messages */
int build_FCGI_hdr(char *buf, int bufsize, unsigned char type,
    unsigned short requestId, unsigned short contentLength);
int build_FCGI_padding(char *buf, int bufsize, unsigned short contentLength);
int build_FCGI_len(char *buf, int bufsize, int len);
int build_FCGI_namevalue_pair(char *buf, int bufsize, const char *name,
    const char *value);
int build_FCGI_request(char *buf, int bufsize, unsigned char type,
    unsigned short requestId, unsigned short contentLength,
    const void *contentData);
int build_FCGI_request_str(char *buf, int bufsize, unsigned char type,
    unsigned short requestId, const char *contentStr);
int build_FCGI_BEGIN_REQUEST(char *buf, int bufsize, unsigned short requestId,
    unsigned short role, unsigned char flags);
int build_FCGI_ABORT_REQUEST(char *buf, int bufsize, unsigned short requestId);
int build_FCGI_END_REQUEST(char *buf, int bufsize, unsigned short requestId,
    unsigned long appStatus, unsigned char protocolStatus);
int build_FCGI_PARAMS_list(char *buf, int bufsize, unsigned short requestId,
    va_list arglist);
int build_FCGI_PARAMS(char *buf, int bufsize, unsigned short requestId, ...);
int build_FCGI_END_PARAMS(char *buf, int bufsize, unsigned short requestId);
int build_FCGI_STDIN(char *buf, int bufsize, unsigned short requestId,
    unsigned short contentLength, const void *contentData);
int build_FCGI_STDIN_str(char *buf, int bufsize, unsigned short requestId,
    const char *contentStr);
int build_FCGI_END_STDIN(char *buf, int bufsize, unsigned short requestId);
int build_FCGI_STDOUT(char *buf, int bufsize, unsigned short requestId,
    unsigned short contentLength, const void *contentData);
int build_FCGI_STDOUT_str(char *buf, int bufsize, unsigned short requestId,
    const char *contentStr);
int build_FCGI_STDERR(char *buf, int bufsize, unsigned short requestId,
    unsigned short contentLength, const void *contentData);
int build_FCGI_STDERR_str(char *buf, int bufsize, unsigned short requestId,
    const char *contentStr);
int build_FCGI_DATA(char *buf, int bufsize, unsigned short requestId,
    unsigned short contentLength, const void *contentData);
int build_FCGI_DATA_str(char *buf, int bufsize, unsigned short requestId,
    const char *contentStr);
int build_FCGI_GET_VALUES_list(char *buf, int bufsize, va_list arglist);
int build_FCGI_GET_VALUES(char *buf, int bufsize, ...);
int build_FCGI_GET_VALUES_RESULT_list(char *buf, int bufsize, va_list arglist);
int build_FCGI_GET_VALUES_RESULT(char *buf, int bufsize, ...);
int build_FCGI_UNKNOWN_TYPE(char *buf, int bufsize, unsigned char type);

/* uServer-specific routines */
int build_FCGI_SET_VALUES_list(char *buf, int bufsize, va_list arglist);
int build_FCGI_SET_VALUES(char *buf, int bufsize, ...);
struct info *fcgi_find_idle(struct info *peer_ip);
int fcgi_go_idle(struct info *ip);
int fcgi_service_request(struct info *ip, int peer_sd);
int fcgi_service_next_request(struct info *ip);
int fcgi_process_request(int peer_sd);
#ifdef FIXHUP
int fcgi_find_next_request(struct info *client_ip, app_t *app);
#endif
int fcgi_add_to_reply_buffer_httpd(struct info *ip);

#define DO_FCGI_GET_SERVER_VALUES
#ifdef DO_FCGI_GET_SERVER_VALUES
int fcgi_get_server_values(appserver_t *appserver);
#endif
int fcgi_set_server_values(appserver_t *appserver);

#endif /* FCGI_H */
