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


#include "common.h"

#include <sys/fcntl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include "debug.h"
#include "myassert.h"
#include "syscalls.h"
#include "stats.h"
#include "trace.h"

#ifdef MAXOPENFD
#define MAX_CALLS      (500000)

static int num_sys_close = 0;

struct closestruct {
  int fd;
  int num_idle;
  int max_sd;
  int max_open;
  int open;
  long time;
} close_info[MAX_CALLS];


void
SYS_CLOSE_TIME(int fd)
{
  // int i;
  int rc;
  struct timeval tv;
  long start, stop;
  int open_count = 0;

  // num_closed++;

  gettimeofday (&tv, 0);
  start = tv.tv_sec * 1000000 + tv.tv_usec;
  rc = close(fd);
  gettimeofday (&tv, 0);
  stop = tv.tv_sec * 1000000 + tv.tv_usec;

  open_count = maxconns - num_idle;

  close_info[num_sys_close].fd = fd;
  close_info[num_sys_close].num_idle = num_idle;
  close_info[num_sys_close].max_sd = max_sd;
  close_info[num_sys_close].time = stop - start;
  close_info[num_sys_close].max_open = max_opensd;
  close_info[num_sys_close].open = open_count;
  num_sys_close++;
  assert(num_sys_close < MAX_CALLS);
}

void
print_close_info()
{
  int i;

  printf("%8s %8s %8s %8s %8s %8s %20s\n",
         "call", "fd", "idle", "max_sd", "max_open", "open", "time");
  for (i=0; i<num_sys_close; i++) {
    printf("%8d %8d %8d %8d %8d %8d %20ld\n",
            i,
            close_info[i].fd,
            close_info[i].num_idle,
            close_info[i].max_sd,
            close_info[i].max_open,
            close_info[i].open,
            close_info[i].time);
  }
}
#endif /* MAXOPENFD */

void
print_syscall_info()
{
#ifdef MAXOPENFD
  print_close_info();
#endif /* MAXOPENFD */
}


void
CLOSE_STATS(int fd)
{
   num_close_calls++;
   if (close(fd) < 0) {
     num_close_failed++;
   }
}

int
close_file(int fd)
{
  int rc;
  int trace_fd = fd;
  PRINT_TIME(NOFD, &tnow, &tprev, "close_file: fd = %d", fd);

  TRACE(EVT_CLOSE_FILE,
    trace_fd = fd;
    rc = close(fd);
  );
  return rc;
}

int
close_sock(int sd)
{
  int rc;
  int save_errno = 0;
  /* used for trace data */
  int trace_fd = sd;
  assert(sd > 2);
  num_close_calls++;
  num_sock_close_calls++;

  TRACE(EVT_CLOSE_SOCK,
    trace_fd = sd;
    rc = close(sd);
    save_errno = errno;
  );
  PRINT_TIME(sd, &tnow, &tprev, "close_sock: rc = %d", rc);
  errno = save_errno;
  return rc;
}
