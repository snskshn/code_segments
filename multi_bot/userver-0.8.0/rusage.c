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
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdlib.h>

/* #define LOCAL_DEBUG */
#include "common.h"
#include "local_debug.h"

void rusage_init();
void rusage_print();
void print(struct rusage *r);

struct rusage start;

void
rusage_init(void)
{
  if (getrusage(RUSAGE_SELF, &start) < 0) {
    printf("rusage_init: Unable to getrusage\n");
    exit(1);
  }
}

void
rusage_print(void)
{
  struct rusage end;
  struct timeval diff;

  printf("-------------------------------------------------------------\n");

  if (getrusage(RUSAGE_SELF, &end) < 0) {
    printf("Rusage : unable to get usage info\n");
  } else {
#ifdef LOCAL_DEBUG
    printf("Rusage at start of run\n");
    print(&start);
    printf("\n");
    printf("Rusage at start of run\n");
    print(&end);
    printf("\n");
#endif
    printf("Rusage : usage info\n");

    /* Total amount of user time used.  */
    timersub(&end.ru_utime, &start.ru_utime, &diff);
    end.ru_utime = diff;
    /* Total amount of system time used.  */
    timersub(&end.ru_stime, &start.ru_stime, &diff);
    end.ru_stime = diff;
    /* Maximum resident set size (in kilobytes).  */
    end.ru_maxrss -= start.ru_maxrss;
    /* Amount of sharing of text segment memory
       with other processes (kilobyte-seconds).  */
    end.ru_ixrss -= start.ru_ixrss;
    /* Amount of data segment memory used (kilobyte-seconds).  */
    end.ru_idrss -= start.ru_idrss;
    /* Amount of stack memory used (kilobyte-seconds).  */
    end.ru_isrss -= start.ru_isrss;
    /* Number of soft page faults (i.e. those serviced by reclaiming
       a page from the list of pages awaiting reallocation.  */
    end.ru_minflt -= start.ru_minflt;
    /* Number of hard page faults (i.e. those that required I/O).  */
    end.ru_majflt -= start.ru_majflt;
    /* Number of times a process was swapped out of physical memory.  */
    end.ru_nswap -= start.ru_nswap;
    /* Number of input operations via the file system.  Note: This
       and `ru_oublock' do not include operations with the cache.  */
    end.ru_inblock -= start.ru_inblock;
    /* Number of output operations via the file system.  */
    end.ru_oublock -= start.ru_oublock;
    /* Number of IPC messages sent.  */
    end.ru_msgsnd -= start.ru_msgsnd;
    /* Number of IPC messages received.  */
    end.ru_msgrcv -= start.ru_msgrcv;
    /* Number of signals delivered.  */
    end.ru_nsignals -= start.ru_nsignals;
    /* Number of voluntary context switches, i.e. because the process
       gave up the process before it had to (usually to wait for some
       resource to be available).  */
    end.ru_nvcsw -= start.ru_nvcsw;
    /* Number of involuntary context switches, i.e. a higher priority process
       became runnable or the current process used up its time slice.  */
    end.ru_nivcsw -= start.ru_nivcsw;

    print(&end);
  }
}

void 
print(struct rusage *r)
{
  double usec = 0.0;
  double sec = 0.0;

  usec = ((double) r->ru_utime.tv_sec * 1000000.0) + 
           (double) r->ru_utime.tv_usec;
  sec = usec / 1000000.0;
  printf("ru_utime user time                = %10.2lf\n", sec);

  usec = ((double) r->ru_stime.tv_sec * 1000000.0) + 
           (double) r->ru_stime.tv_usec;
  sec = usec / 1000000.0;
  printf("ru_stime system time              = %10.2lf\n", sec);

  printf("ru_maxrss resident set size (KB)  = %10ld\n", r->ru_maxrss);
  printf("ru_ixrss shared text seg (KB sec) = %10ld\n", r->ru_ixrss);
  printf("ru_idrss data seg (KB sec)        = %10ld\n", r->ru_idrss);
  printf("ru_isrss stack (KB sec)           = %10ld\n", r->ru_isrss);
  printf("ru_minflt soft/reclaimed faults   = %10ld\n", r->ru_minflt);
  printf("ru_majflt hard/IO faults          = %10ld\n", r->ru_majflt);
  printf("ru_nswap process swapped out      = %10ld\n", r->ru_nswap);
  printf("ru_inblock fs input (non cached)  = %10ld\n", r->ru_inblock);
  printf("ru_oublock fs output (non cached) = %10ld\n", r->ru_oublock);
  printf("ru_msgsnd IPC msg sent            = %10ld\n", r->ru_msgsnd);
  printf("ru_msgrcv IPC msg received        = %10ld\n", r->ru_msgrcv);
  printf("ru_nsignals signals               = %10ld\n", r->ru_nsignals);
  printf("ru_nvcsw voluntary cs             = %10ld\n", r->ru_nvcsw);
  printf("ru_nivcsw involuntary cs          = %10ld\n", r->ru_nivcsw);
}
/* ------------------------------------------------------------------- */
#ifdef TESTMAIN
int
main(int argc, char *argv[])
{
  int i;
  char *x = 0;
  int n = 100;
  int p = getpagesize();

  if (argc > 1) {
    n = atoi(argv[1]);
  }

  rusage_init();

  ldbg("Calling malloc for %d bytes\n", n * p);
  x = (char *) malloc(n * p);
  if (x == 0) {
    perror("main: Unable to malloc");
    exit(1);
  }

  ldbg("x = %p\n", x);

  for (i=0; i<n*p; i+=p) {
    x[i] = 'A';
  }

  rusage_print();

  exit(0);
}
#endif /* TESTMAIN */
