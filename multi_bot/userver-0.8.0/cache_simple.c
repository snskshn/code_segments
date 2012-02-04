/*
    userver -- (pronounced you-server or micro-server).
    This file is part of the userver, a high-performance web server designed for
    performance experiments.
          
    This file is Copyright (C) 2004-2010  Tim Brecht

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


#ifdef CACHE_SIMPLE
/*
 * Title   : cache_simple.c
 * Author  : Tim Brecht
 * Date    : Fri Apr 20 14:38:47 PDT 2001
 *
 * Very simple cache implementation.
 * Adds files and doesn't delete them. 
 * Very small number of entries.
 * Not designed to be general. Only for testing with something like
 *      httperf using the same files over and over again.
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "cache_simple.h"
#include "debug.h"
#include "stats.h"
#include "syscalls.h"
#include "cntl_conns.h"

#define CACHE_MAX       (5)
#define MAX_HEADER_LEN  (128)

struct cacheinfo cache[CACHE_MAX];

static int cur_entry = 0;

struct cacheinfo *
cache_lookup(char *uri) 
{
  int i;

  for (i=0; i<CACHE_MAX; i++) {
    if (strcmp(cache[i].name, uri) == 0) {
      return(&cache[i]);
    }
  }
  return(0);
}

struct cacheinfo *
cache_add(char *uri) 
{
  int fd, bytes;
  int len;
  struct cacheinfo *entry;
  struct stat stat;
  char *cur;
  char header_buf[MAX_HEADER_LEN];
  int header_len;
  int rc;
  char *rc_ptr;
  int trace_fd = 0;

  PRINT_TIME(NOFD, &tnow, &tprev, "cache_add: entered ");

  num_cache_add_calls++;

  if ((entry = cache_lookup(uri)) != 0) {
    num_cache_hits++;
    PRINT_TIME(NOFD, &tnow, &tprev, "cache_add: entry found - returning");
    return(entry);
  } else {
    PRINT_TIME(NOFD, &tnow, &tprev, "cache_add: entry not found ");
  }

  if (cur_entry < CACHE_MAX) {
    TRACE(EVT_OPEN_FILE,
          trace_fd = 0;
          rc = open(uri, O_RDONLY);
          );
    fd = rc;
    if (fd < 0) {
      perror (uri);
      PRINT_TIME(NOFD, &tnow, &tprev, "cache_add: open failed - returning failure ");
      return(0);
    }


    TRACE(EVT_FSTAT,
          trace_fd = fd;
          rc = fstat (fd, &stat);
          );

    if (rc < 0) {
      perror (uri);
      close_file(fd);
      PRINT_TIME(NOFD, &tnow, &tprev, "cache_add: fstat failed - returning failure ");
      return(0);
    }

    /* fill in some of the cache info */
    strcpy(cache[cur_entry].name, uri);


    /* fill in the header buffer */
    header_len = sprintf(header_buf,
	"HTTP/1.1 200 OK\r\nServer: %s\r\nContent-Length: %lu\r\n\r\n",
	server_ident, stat.st_size);
    len = header_len + stat.st_size;
    cache[cur_entry].buf_len = len;

    /* allocate the buffer */
    cache[cur_entry].buf = malloc(len);
	  num_bytes_malloced += len;
    if (cache[cur_entry].buf == 0) {
      printf("cache_add: malloc failed\n");
      exit(1);
    }

    /* load the buffer */
    cur = cache[cur_entry].buf;

    /* copy in the header */
    rc_ptr = memcpy(cur, header_buf, header_len);
    if (rc_ptr == 0) {
      perror("cache_add: memcpy failed\n");
      exit(1);
    }

    cur = cur + header_len;

    bytes = read(fd, cur, stat.st_size);  
    if (bytes != stat.st_size) {
      perror("cache_add: read failed\n");
      exit(1);
    }
    cur_entry++;

    PRINT_TIME(NOFD, &tnow, &tprev, "cache_add: returning success ");
    return(&cache[cur_entry-1]);
  } else {
    PRINT_TIME(NOFD, &tnow, &tprev, "cache_add: returning failure ");
    return(0);
  }
}


void
cache_cleanup(void)
{
  int i;

  for (i=0; i<CACHE_MAX; i++) {
    if (cache[i].buf != 0) {
      free(cache[i].buf);
    }
  }
}
#endif /* CACHE_SIMPLE */
