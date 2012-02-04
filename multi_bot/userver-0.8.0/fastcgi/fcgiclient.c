/*
    userver -- (pronounced you-server or micro-server).
    This file is part of the userver, a high-performance web server designed for
    performance experiments.
          
    This file is Copyright (C) 2004  Craig Barkhouse

    Authors: Craig Barkhouse <cabarkho@uwaterloo.ca>
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
#include <errno.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../fcgi.h"


void
hexdump(const void *buf, int len)
{
  int offset = 0;
  int line_offset;

  while (offset < len) {
    printf("%08X:", offset);
    for (line_offset = 0; line_offset < 16; line_offset++) {
      if ((line_offset % 4) == 0)
	printf(" ");
      if (offset + line_offset < len)
	printf(" %02X", ((unsigned char *) buf)[offset + line_offset]);
      else
	printf("   ");
    }
    printf("  ");
    for (line_offset = 0; line_offset < 16; line_offset++) {
      if (offset + line_offset >= len)
	break;
      printf("%c", isprint(((unsigned char *) buf)[offset + line_offset]) ?
	  ((unsigned char *) buf)[offset + line_offset] : '.');
    }
    offset += 16;
    printf("\n");
  }
}


#define REENTRANCY_THRESHOLD 5

const char *
str(const char *format, ...)
{
  static char buf[REENTRANCY_THRESHOLD][1024];
  static int bufind = -1;
  va_list argptr;

  bufind = (bufind + 1) % REENTRANCY_THRESHOLD;

  va_start(argptr, format);
  vsprintf(buf[bufind], format, argptr);

  return buf[bufind];
}


int fcgi_connect_un(int sd, char *path)
{
  struct sockaddr_un sun;

  if (strlen(path) >= sizeof(sun.sun_path)) {
    printf("fcgi_connect_un: path too long\n");
    return -1;
  }

  sun.sun_family = AF_UNIX;
  strcpy(sun.sun_path, path);

  return connect(sd, (struct sockaddr *) &sun, SUN_LEN(&sun));
}


int fcgi_connect_in(int sd, char *host)
{
  struct sockaddr_in sin;
  char *ptr;
  char hostbuf[128];
  int hostlen;
  int port = 8888;

  if ((ptr = strchr(host, ':')) != NULL) {
    /*
     * We don't want to destroy the caller's data by setting *ptr to '\0',
     * so we make a local copy of the hostname portion.
     */
    if ((hostlen = ptr - host) >= sizeof(hostbuf)) {
      printf("fcgi_connect_in: hostname too long\n");
      return -1;
    }
    strncpy(hostbuf, host, hostlen);
    hostbuf[hostlen] = '\0';
    host = hostbuf;
    port = atoi(ptr + 1);
  }

  memset(&sin, '\0', sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_port = htons(port);
  if (host[0] == '\0') {
    sin.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  } else {
    inet_aton(host, &sin.sin_addr);
  }

  return connect(sd, (struct sockaddr *) &sin, sizeof(sin));
}


int fcgi_connect(char *addr)
{
  int sd;

  if (addr == NULL) {
    printf("fcgi_connect: no addr\n");
    return -1;
  }

  if (strchr(addr, '/') != NULL) {
    sd = socket(PF_UNIX, SOCK_STREAM, 0);
    if (sd >= 0) {
      if (fcgi_connect_un(sd, addr) < 0)
	return -1;
    }
  } else {
    sd = socket(PF_INET, SOCK_STREAM, 0);
    if (sd >= 0) {
      if (fcgi_connect_in(sd, addr) < 0)
	return -1;
    }
  }

  return sd;
}


int
build_FCGI_hdr(char *buf, int bufsize, unsigned char type,
    unsigned short requestId, unsigned short contentLength)
{
  FCGI_Header *hdr;

  if (bufsize >= sizeof(FCGI_Header)) {
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

  return sizeof(FCGI_Header);
}


int
build_FCGI_padding(char *buf, int bufsize, unsigned short contentLength)
{
  unsigned char paddingLength = FCGI_PADDING_LEN(contentLength);

  if (bufsize >= paddingLength)
    memset(buf, '\0', paddingLength);

  return paddingLength;
}


int
build_FCGI_len(char *buf, int bufsize, int len)
{
  if (len <= 127) {
    if (bufsize >= 1)
      *buf = len;
    return 1;
  }

  if (bufsize >= 4) {
    *buf++ = 0x80 | ((len & 0x7F000000) >> 24);
    *buf++ = (len & 0x00FF0000) >> 16;
    *buf++ = (len & 0x0000FF00) >> 8;
    *buf = len & 0x000000FF;
  }
  return 4;
}


int
build_FCGI_namevalue_pair(char *buf, int bufsize, const char *name,
    const char *value)
{
  int namelen, valuelen;
  int len = 0;

  namelen = strlen(name);
  len += build_FCGI_len(buf + len, bufsize - len, namelen);
  valuelen = strlen(value);
  len += build_FCGI_len(buf + len, bufsize - len, valuelen);

  if (bufsize >= len + namelen)
    memcpy(buf + len, name, namelen);
  len += namelen;

  if (bufsize >= len + valuelen)
    memcpy(buf + len, value, valuelen);
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

  if (bufsize >= len + contentLength)
    memcpy(buf + len, contentData, contentLength);
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
  body.appStatusB0 = appStatus & 0x000000FF;
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

  ap = arglist;
  while ((name = va_arg(ap, char *)) != NULL) {
    value = va_arg(ap, char *);
    contentLength += build_FCGI_namevalue_pair(NULL, 0, name, value);
  }

  len += build_FCGI_hdr(buf, bufsize, FCGI_PARAMS, requestId, contentLength);

  if (bufsize >= len + contentLength) {
    ap = arglist;
    while ((name = va_arg(ap, char *)) != NULL) {
      value = va_arg(ap, char *);
      len += build_FCGI_namevalue_pair(buf + len, bufsize - len, name, value);
    }
  }

  len += build_FCGI_padding(buf + len, bufsize - len, contentLength);

  return len;
}


int
build_FCGI_PARAMS(char *buf, int bufsize, unsigned short requestId, ...)
{
  va_list arglist;

  va_start(arglist, requestId);
  return build_FCGI_PARAMS_list(buf, bufsize, requestId, arglist);
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

  ap = arglist;
  while ((name = va_arg(ap, char *)) != NULL) {
    contentLength += build_FCGI_namevalue_pair(NULL, 0, name, "");
  }

  len += build_FCGI_hdr(buf, bufsize,
      FCGI_GET_VALUES, 0, contentLength);

  if (bufsize >= len + contentLength) {
    ap = arglist;
    while ((name = va_arg(ap, char *)) != NULL) {
      len += build_FCGI_namevalue_pair(buf + len, bufsize - len, name, "");
    }
  }

  len += build_FCGI_padding(buf + len, bufsize - len, contentLength);

  return len;
}


int
build_FCGI_GET_VALUES(char *buf, int bufsize, ...)
{
  va_list arglist;

  va_start(arglist, bufsize);
  return build_FCGI_GET_VALUES_list(buf, bufsize, arglist);
}


int
build_FCGI_GET_VALUES_RESULT_list(char *buf, int bufsize, va_list arglist)
{
  va_list ap;
  int contentLength = 0;
  char *name, *value;
  int len = 0;

  ap = arglist;
  while ((name = va_arg(ap, char *)) != NULL) {
    value = va_arg(ap, char *);
    contentLength += build_FCGI_namevalue_pair(NULL, 0, name, value);
  }

  len += build_FCGI_hdr(buf, bufsize,
      FCGI_GET_VALUES_RESULT, 0, contentLength);

  if (bufsize >= len + contentLength) {
    ap = arglist;
    while ((name = va_arg(ap, char *)) != NULL) {
      value = va_arg(ap, char *);
      len += build_FCGI_namevalue_pair(buf + len, bufsize - len, name, value);
    }
  }

  len += build_FCGI_padding(buf + len, bufsize - len, contentLength);

  return len;
}


int
build_FCGI_GET_VALUES_RESULT(char *buf, int bufsize, ...)
{
  va_list arglist;

  va_start(arglist, bufsize);
  return build_FCGI_GET_VALUES_RESULT_list(buf, bufsize, arglist);
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


#define DELAYED_WRITE

typedef enum { GET, POST } REQUEST_METHOD;

int
main(int argc, char *argv[])
{
  time_t start;
  int sd;
  char *addr = "192.168.10.109:8888";
  char query_string[1024];
  REQUEST_METHOD request_method = POST;
  char reqbuf[1024];
  char *req;
  char repbuf[16384];
  char *rep;
  int len, ret;
  int i, j;

  /*
   * Build query_string.  Make sure timestamp is last!
   */
  time(&start);
  sprintf(query_string,
      "do=1&util=ping&host=192.168.10.1&user=Tim%%20Brecht&timestamp=%s",
      ctime(&start));
  query_string[strlen(query_string) - 1] = '\0';  // strip trailing LF

  if (argc > 1)
    addr = argv[1];

  if ((sd = fcgi_connect(addr)) < 0) {
    perror("connect");
    return 2;
  }

for (j = 0; j < 5; j++) {

  /*
   * REQUEST step 1: begin request
   */

  req = reqbuf;
  len = build_FCGI_BEGIN_REQUEST(req, reqbuf + sizeof(reqbuf) - req, 1, FCGI_RESPONDER, FCGI_KEEP_CONN);
#ifdef DELAYED_WRITE
  req += len;
#else
  printf("sending %d bytes:\n", len);
  hexdump(req, len);
  ret = write(sd, req, len);
  if (ret != len) {
    perror("write");
    return 2;
  }
#endif

  /*
   * REQUEST step 2a: send global parameters
   */

  len = build_FCGI_PARAMS(req, reqbuf + sizeof(reqbuf) - req, 1,
      // CGI global variables
      "SERVER_SOFTWARE", "userver/0.4.6",  // should be VERSION in userver
      "SERVER_NAME", "serverhost.serverdomain",
      "GATEWAY_INTERFACE", "CGI/1.1",
      // CGI request-specific variables
      "SERVER_PROTOCOL", "HTTP/1.1",
      "SERVER_PORT", "6800",
      "REMOTE_HOST", "remotehost.remotedomain",
      "REMOTE_ADDR", "1.2.3.4",
      NULL);
#ifdef DELAYED_WRITE
  req += len;
#else
  printf("sending %d bytes:\n", len);
  hexdump(req, len);
  ret = write(sd, req, len);
  if (ret != len) {
    perror("write");
    return 2;
  }
#endif

  /*
   * REQUEST step 2b: send request-specific parameters
   */

  switch (request_method) {
    case GET:
      len = build_FCGI_PARAMS(req, reqbuf + sizeof(reqbuf) - req, 1,
	  "REQUEST_METHOD", "GET",
	  "QUERY_STRING", query_string,
	  NULL);
#ifdef DELAYED_WRITE
      req += len;
#else
      printf("sending %d bytes:\n", len);
      hexdump(req, len);
      ret = write(sd, req, len);
      if (ret != len) {
	perror("write");
	return 2;
      }
#endif
      break;
    case POST:
      len = build_FCGI_PARAMS(req, reqbuf + sizeof(reqbuf) - req, 1,
	  "REQUEST_METHOD", "POST",
	  "CONTENT_LENGTH", str("%d", strlen(query_string)),
	  NULL);
#ifdef DELAYED_WRITE
      req += len;
#else
      printf("sending %d bytes:\n", len);
      hexdump(req, len);
      ret = write(sd, req, len);
      if (ret != len) {
	perror("write");
	return 2;
      }
#endif
      break;
  }

  /*
   * REQUEST step 2c: finish sending parameters
   */

#if 1
  len = build_FCGI_PARAMS(req, reqbuf + sizeof(reqbuf) - req, 1, NULL);
#ifdef DELAYED_WRITE
  req += len;
#else
  printf("sending %d bytes:\n", len);
  hexdump(req, len);
  ret = write(sd, req, len);
  if (ret != len) {
    perror("write");
    return 2;
  }
#endif
#endif

  /*
   * REQUEST step 3: send stdin stream
   */

  switch (request_method) {
    case POST:
      len = build_FCGI_STDIN_str(req, reqbuf + sizeof(reqbuf) - req, 1, query_string);
#ifdef DELAYED_WRITE
      req += len;
#else
      printf("sending %d bytes:\n", len);
      hexdump(req, len);
      ret = write(sd, req, len);
      if (ret != len) {
	perror("write");
	return 2;
      }
#endif
      break;
  }

  /*
   * REQUEST step 4: finish request
   */

  len = build_FCGI_STDIN(req, reqbuf + sizeof(reqbuf) - req, 1, 0, NULL);
#ifdef DELAYED_WRITE
  req += len;
#else
  printf("sending %d bytes:\n", len);
  hexdump(req, len);
  ret = write(sd, req, len);
  if (ret != len) {
    perror("write");
    return 2;
  }
#endif

#ifdef DELAYED_WRITE
  len = req - reqbuf;
  req = reqbuf;
  printf("(delayed) sending %d bytes:\n", len);
  hexdump(req, len);
  ret = write(sd, req, len);
  if (ret != len) {
    perror("write");
    return 2;
  }
#endif

  /*
   * READ REPLY
   */

  while (1) {
    rep = repbuf;
    len = read(sd, rep, sizeof(repbuf));
    if (len < 0) {
      perror("read");
      return 3;
    }
    if (len == 0)
      return 4;
    printf("read %d bytes:\n", len);
#if 1
    hexdump(repbuf, len);
#else
    for (i = 0; i < len; i++) {
      if (isprint(repbuf[i]) || repbuf[i] == '\r' || repbuf[i] == '\n' || repbuf[i] == '\t')
	putchar(repbuf[i]);
    }
#endif
    break;
  }
}

  printf("finished\n");

  return 0;
}
