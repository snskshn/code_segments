/*
    userver -- (pronounced you-server or micro-server).
    This file is part of the userver, a high-performance web server designed for
    performance experiments.
          
    This file is Copyright (C) 2005  Craig Barkhouse

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
#include <stdio.h>
#include <string.h>
#include <time.h>

#ifdef __SUNPRO_C
#include <ctype.h>
#endif

#include "fcgiapp.h"


FCGX_Request request;

int count = 0;


typedef struct argvector {
  int argc;
  char **argv;
  char *buf;
  int size;
} argvector_t;


argvector_t *
create_argvector(int argc, char **argv)
{
  argvector_t *av;

  av = (argvector_t *) malloc(sizeof(argvector_t));
  av->argc = argc;
  av->size = argc + 1;
  av->argv = (char **) malloc(av->size * sizeof(char *));
  av->buf = NULL;
  memcpy(av->argv, argv, av->size * sizeof(char *));

  return av;
}


void free_argvector(argvector_t *av)
{
  /*
   * If av->buf is non-NULL then we must have previously allocated memory,
   * and we are responsible for freeing it.
   */
  if (av->buf != NULL) {
    free(av->buf);
  }
  free(av->argv);
  free(av);
}


int
xtod(char c)
{
  if (c >= '0' && c <= '9') {
    return c - '0';
  } else if (c >= 'A' && c <= 'F') {
    return c - 'A';
  } else if (c >= 'a' && c <= 'f') {
    return c - 'a';
  } else {
    return -1;
  }
}


argvector_t *
parse_query_string(const char *query_string)
{
  argvector_t *av;
  const char *src;
  char *dst;
  int i;

  av = (argvector_t *) malloc(sizeof(argvector_t));
  av->buf = (char *) malloc(strlen(query_string) + 1);

  if (query_string[0] == '\0') {
    av->argc = 0;
  } else {
    av->argc = 1;
    for (src = query_string; src[0] != '\0'; src++) {
      if (src[0] == '&' && src[1] != '\0') {
	av->argc++;
      }
    }
  }
  av->size = av->argc + 1;
  av->argv = (char **) malloc(av->size * sizeof(char *));

  src = query_string;
  dst = av->buf;
  for (i = 0; i < av->argc; i++) {
    av->argv[i] = (char *) dst;
    while (src[0] != '\0') {
      if (src[0] == '&') {
	*dst++ = '\0';
	src++;
	break;
      } else if (src[0] == '+') {
	*dst++ = ' ';
	src++;
      } else if (src[0] == '%' && isxdigit(src[1]) && isxdigit(src[2])) {
	*dst++ = (xtod(src[1]) * 16) + xtod(src[2]);
	src += 3;
      } else {
	*dst++ = *src++;
      }
    }
  }
  *dst = '\0';
  av->argv[av->argc] = NULL;

  return av;
}


int
envcmp(const void *p1, const void *p2)
{
  return strcmp(*((char **) p1), *((char **) p2));
}


void
sortenv(char *envp[])
{
  int envc;
  char **ptr;

  for (envc = 0, ptr = envp; *ptr != NULL; ptr++, envc++);
  qsort(envp, envc, sizeof(char *), envcmp);
}


int
printenv(char *envp[])
{
  char **ptr;
  int rc;

  sortenv(envp);

  rc = 0;
  for (ptr = envp; *ptr != NULL; ptr++) {
    rc += FCGX_FPrintF(request.out, "%s\n", *ptr);
  }

  return rc;
}


char *
read_content(void)
{
  static char *buf = NULL;
  static int buflen = 0;
  int numread;
  int toread;
  int rc;

  if (buflen == 0) {
    buflen = 128;
    buf = (char *) malloc(buflen);
  }
  numread = 0;

  while (1) {
    toread = buflen - numread;
    rc = FCGX_GetStr(buf + numread, toread, request.in);
    numread += rc;
    if (numread < buflen) {
      break;
    }
    buflen *= 2;
    buf = realloc(buf, buflen);
  }
  buf[numread] = '\0';

  return buf;
}


int
main(int argc, char *argv[], char *envp[])
{
  int sd;
  time_t now;
  char *request_method, *query_string;
  argvector_t *av;

  FCGX_Init();
  sd = FCGX_OpenSocket(argc > 1 ? argv[1] : ":8000", 5);
  FCGX_InitRequest(&request, sd, 0);

  while (FCGX_Accept_r(&request) >= 0) {

    /****************************** BEGIN CGI ******************************/

    time(&now);
    FCGX_FPrintF(request.out, "Content-type: text/plain\r\n\r\n");

    count++;
    FCGX_FPrintF(request.out, "Generating reply #%d at %s",
	count, ctime(&now));
    FCGX_FPrintF(request.out, "\n");

    FCGX_FPrintF(request.out, "environment variables:\n");
    printenv(request.envp);
    FCGX_FPrintF(request.out, "\n");

    FCGX_FPrintF(request.out, "CGI parameters:\n");
    request_method = FCGX_GetParam("REQUEST_METHOD", request.envp);
    if (!strcmp(request_method, "GET") || !strcmp(request_method, "HEAD")) {
      query_string = FCGX_GetParam("QUERY_STRING", request.envp);
    } else if (!strcmp(request_method, "POST")) {
      query_string = read_content();
    } else {
      query_string = NULL;
    }
    if (query_string != NULL) {
      av = parse_query_string(query_string);
      printenv(av->argv);
      free_argvector(av);
    }
    FCGX_FPrintF(request.out, "\n");

    /******************************* END CGI *******************************/

  }

  return 0;
}
