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

#if defined (TRACE_ON)

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <limits.h>
#include <string.h>
#include "trace.h"
#include "debug.h"
#include "util.h"

#if !defined(FreeBSD) && !defined(DARWIN)
#include <malloc.h>
#endif


fd_set evt_trace_bits;

/* #define SELF_TEST */

/* TODO:
 *
 * DONE: - don't start tracing until first accept call
 *         and when done look through traces removing trailing select calls
 *
 * Figure out some nice way to deal with some of the select
 * calls that happen before getting the first request and
 * after getting the last request. These shouldn't get counted.
 * Possibilities 
 *  - turn tracing off until the first accept call is made?
 *  - if select is the last event - remove it.
 *  - is it true that if there are other events after select 
 *    that they are legit and need to be included. it would seem so.
 * 
 * DONE: - added some of these
 * Figure out what other info is needed to be useful and how
 * to configure it.
 *  - time at which the call was made?
 *  - time between system calls
 *  - time between successive calls to the same system call
 *    (e.g., time between accept calls and time between select calls)
 *    compare this with the time spend performing the call
 *  - relevant info to the given call
 *    e.g., select - max fd, open fds, returned n
 *  - to keep footprint down - could trace only select calls and provide
 *    this info or just select and accept
 *
 * Dynamically - turn tracing on an off using run-time parameters.
 *             - dynamically allocate the trace buffer.
 * 
 * Spend more time figure out what we are looking for and/or trying
 * to demonstrate.
 * 
 * May also want to just record the time of each event and compute
 * differences - e.g. record time relative to start of the run
 * then time between selects can be computed by subtracting
 * the times between each of the events.
 * May be more flexible but would require more backend processing
 * which isn't suitable for just doing the summaries.
 */

char *evt_names[] = {
  /*  0 */ "evt_invalid",
  /*  1 */ "evt_read_sock",
  /*  2 */ "evt_write_sock",
  /*  3 */ "evt_close_sock",
  /*  4 */ "evt_accept",
  /*  5 */ "evt_select",
  /*  6 */ "evt_read_file",
  /*  7 */ "evt_close_file",
  /*  8 */ "evt_fstat",
  /*  9 */ "evt_fcntl",
  /* 10 */ "evt_sigprocmask",
  /* 11 */ "evt_multiaccept",
  /* 12 */ "evt_sendhandler",
  /* 13 */ "evt_file_open",
  /* 14 */ "evt_sigio",
  /* 15 */ "evt_change_set",
  /* 16 */ "evt_sendfile",
  /* 17 */ "evt_sendhandler_call",
  /* 18 */ "evt_repeat_accept",
  /* 19 */ "evt_wait_evts",
  /* 20 */ "evt_block_conns",
  /* 21 */ "evt_unblock_conns",
  /* 22 */ "evt_writev_sock",
  /* 23 */ "evt_auto_accept",
  /* 24 */ "evt_mmap",
  /* 25 */ "evt_mlock",
  /* 26 */ "evt_munmap",
  /* 27 */ "evt_munlock",
  /* 28 */ "evt_cache_add",
  /* 29 */ "evt_tcp_cork",
  /* 30 */ "evt_poll",
  /* 31 */ "evt_epoll_wait",
  /* 32 */ "evt_epoll_ctl",
  /* 33 */ "evt_aio_wait",
  /* 34 */ "evt_aio_sock_accept",
  /* 35 */ "evt_aio_sock_read",
  /* 36 */ "evt_aio_sock_write",
  /* 37 */ "evt_aio_sock_writev",
  /* 38 */ "evt_aio_sock_close",
  /* 39 */ "evt_epoll_ctlv",
  /* 40 */ "evt_aio_sock_getevents",
  /* 41 */ "evt_aio_sock_read_accept",
  /* 42 */ "evt_aio_sock_destroy",
  /* 43 */ "evt_aio_sock_mem_reg",
  /* 44 */ "evt_aio_sock_mem_unreg",
  /* 45 */ "evt_event_phase",
  /* 46 */ "evt_accept_phase",
  /* 47 */ "evt_work_phase",
  /* 48 */
};

struct evtstat {
  int count;
  int max;
  int min;
  double total;
} evt_stats[EVT_MAX];

#define EVT_NUM_NAMES   ((sizeof(evt_names)) / (sizeof (char *)))

int evt_count  = -1;
int evt_init_done = 0;
int evt_trace_on = 0;  /* Start with this off */ 
                       /* it gets turned on at the first accept */
int evt_trace_buf_full = 0;
size_t trace_bytes = 0;

struct trace_type *trace;
struct timeval last_called[EVT_MAX];
struct timeval trace_start;

struct timeval tmp_start;
struct timeval tmp_stop;
struct timeval tmp_tdiff;
long int tmp_idiff = 0;
long int tmp_ediff = 0;
long int tmp_diff = 0;

struct timeval tmp_start_time;
struct timeval tmp_stop_time;
struct timeval tmp_tdiff_time;
long int tmp_idiff_time = 0;
long int tmp_ediff_time = 0;
long int tmp_diff_time = 0;

int tmp_extra1 = 0;
int tmp_extra2 = 0;
int tmp_extra3 = 0;

int tmp_save_errno = 0;

struct struct_trace_summary trace_summary[EVT_MAX];

void evt_stats_print(FILE *fp);
void evt_stats_compute();
void evt_init();
void check_evts();

#ifdef SELF_TEST
#define COUNT   (100)
main()
{
  struct timeval t;
  int pid;
  int i;
  int rc;


  for (i=0; i<COUNT; i++) {
    TRACE(1, 
      rc = gettimeofday(&t, 0);
    );

    TRACE(2,
      rc = getpid();
    );
    pid = rc;
  }

  evt_print_readable();
}
#endif /* SELF_TEST */


void
evt_add(char event, long int time, long int interval,
        long int elapsed, int e1, int e2, int e3)
{
  int index; 
  struct struct_trace_summary *s;
  assert(EVT_MAX == EVT_NUM_NAMES);
  assert(event > 0);
  assert(event < EVT_MAX);
  assert(event != EVT_INVALID);

#ifdef USE_START_TIME_USEC
  if (elapsed < USE_START_TIME_USEC) {
    return;
  }
#endif

#ifdef USE_END_TIME_USEC
  if (elapsed > USE_END_TIME_USEC) {
    return;
  }
#endif

  if (options.trace_summary) {
    index = (int) event;
    s = &trace_summary[index];
    s->total_time += (double) time;
    s->total_interval += (double) time;
    s->call_count++;
    if (time < s->min_time) {
      s->min_time = time;
    } 
    if (time > s->max_time) {
      s->max_time = time;
    } 

    if (interval < s->min_interval) {
      s->min_interval = interval;
    } 
    if (interval > s->max_interval) {
      s->max_interval = interval;
    } 

    if (time < 0) {
      printf("evt_add: time < 0 = %ld\n", time);
    }

    if (options.trace_summary_only) {
      return;
    }
  }

  if (evt_trace_buf_full || !evt_trace_on) {
    return;
  }

  index = ++evt_count;

  if (evt_count >= EVT_COUNT) {
    printf("TRACE ERROR - OUT OF SPACE\n");
    printf("TRACE ERROR - TURNING TRACING OFF\n");
    evt_trace_on = 0;
    evt_trace_buf_full = 1;
    return;
  }
  assert(evt_count < EVT_COUNT);

  DEBG(MSG_TRACE, "evt_add: entering adding event = %d, time = %ld "
      "elapsed = %ld evt_count = %d index = %d\n",
      event, time, elapsed, evt_count, index);

  trace[index].event = event;
  trace[index].call_time = time;
#ifdef EVT_EXTRAS
  trace[index].extra1 = e1;
  trace[index].extra2 = e2;
  trace[index].extra3 = e3;
#endif
#ifdef EVT_INTERVALS
  trace[index].interval = interval;
#endif
#ifdef EVT_ELAPSED
  trace[index].elapsed = elapsed;
#endif

#ifdef HAVE_AIO_LAYER
  trace[index].pre_posted_conns = maxconns - num_idle - cur_conns;
  trace[index].cur_conns = cur_conns;
#endif


  /* check_evts(); */
  DEBG(MSG_TRACE, "evt_add: returning evt_count = %d "
      "index = %d\n", evt_count, index);
}

void
evt_do_print(int readable)
{
  extern int my_proc_id;
  FILE *fp;
  int i;
  int event;
  char filename[200];
  char *myfile;

#ifndef USE_END_TIME_USEC
  /* skip the last events if they are select calls */
  /* they mean the server was just sitting and waiting for events */

  while ((evt_count >= 0) && (trace[evt_count].event == EVT_SELECT)) {
    printf("Removing trailing select call from traces : time = %d\n",
            trace[evt_count].call_time);
    evt_count--;
  }

  while ((evt_count >= 0) && (trace[evt_count].event == EVT_WAIT_EVTS)) {
    printf("Removing trailing evtctl call from traces :  time = %d\n",
            trace[evt_count].call_time);
    evt_count--;
  }
#endif /* USE_END_TIME_USEC */

  /* Print the stats out to stdout */
  fp = stdout;
  fprintf(fp, "\n");
  fprintf(fp, "Memory used for trace data = %lu bytes = %lu MB\n",
           (unsigned long) trace_bytes,
	   (unsigned long) trace_bytes / 1024 / 1024);
  evt_stats_compute();
  evt_stats_print(fp);


  if (strcmp(options.trace_filename,"stdout") == 0) {
    fp = stdout;
  } else {
    if (numprocs > 1) {
      sprintf(filename, "userver-%d.trace", my_proc_id);
      myfile = filename;
      printf("Using file = %s for trace output\n", myfile);
    } else {
      myfile = options.trace_filename;
    }

    if ((fp = fopen(myfile, "w")) == 0) {
      printf("Unable to open event output file = %s\n", myfile);
      return;
    }
  }

  fprintf(fp, "Memory used for trace data = %lu bytes = %lu MB\n",
           (unsigned long) trace_bytes,
	   (unsigned long) trace_bytes / 1024 / 1024);

  /* Also print the stats out to the event file */
  /* Leave this out so that we can more easily process this file. */
  /* evt_stats_print(fp); */

  fprintf(fp, "==================================================\n");
  fprintf(fp, "%-24s ", "Event");
  fprintf(fp, "%9s ", "Time");
#ifdef EVT_EXTRAS
  fprintf(fp, "%10s ", "Fds/size");
  fprintf(fp, "%8s ", "rc");
  fprintf(fp, "%7s ", "fd");
  fprintf(fp, "%4s ", "(fd)");
#endif /* EVT_EXTRAS */
#ifdef EVT_INTERVALS
  fprintf(fp, "%10s ", "Interval");
#endif /* EVT_INTERVALS */
#ifdef EVT_ELAPSED
  fprintf(fp, "%10s ", "Elapsed");
#endif

#ifdef HAVE_AIO_LAYER
  fprintf(fp, "%6s /", "Posted");
  fprintf(fp, "%6s ", "Conns");
#endif
  fprintf(fp, "\n");

  /* Now print out all of the events */
  for (i=0; i<=evt_count; i++) {
    if (readable) {
      event = trace[i].event;
      /* assert(event <= EVT_NUM_NAMES); */
      /* assert(event > 0); */
      /* assert(event < EVT_MAX); */
     /* assert(event != EVT_INVALID); */
      fprintf(fp, "%-24s ", evt_names[trace[i].event]);
      fprintf(fp, "%9d ", trace[i].call_time);
#ifdef EVT_EXTRAS
      fprintf(fp, "%10d ", trace[i].extra1);
      fprintf(fp, "%8d ", trace[i].extra2);
      fprintf(fp, "%7d ", trace[i].extra3);
      fprintf(fp, "%4s ", "(fd)");
#endif /* EVT_EXTRAS */
#ifdef EVT_INTERVALS
      fprintf(fp, "%10d ", trace[i].interval);
#endif /* EVT_INTERVALS */
#ifdef EVT_ELAPSED
      fprintf(fp, "%10d ", trace[i].elapsed);
#endif
#ifdef HAVE_AIO_LAYER
      fprintf(fp, "%6d /", trace[i].pre_posted_conns);
      fprintf(fp, "%6d ",  trace[i].cur_conns);
#endif
      fprintf(fp, "\n");
    } else {
      fprintf(fp, "%d ", trace[i].event); 
      fprintf(fp, "%d ", trace[i].call_time);
#ifdef EVT_EXTRAS
      fprintf(fp, "%d ", trace[i].extra1);
      fprintf(fp, "%d ", trace[i].extra2);
#endif /* EVT_EXTRAS */
#ifdef EVT_INTERVALS
      fprintf(fp, "%d ", trace[i].interval);
#endif /* EVT_INTERVALS */
#ifdef EVT_ELAPSED
      fprintf(fp, "%d ", trace[i].elapsed);
#endif
#ifdef HAVE_AIO_LAYER
      fprintf(fp, "%d ", trace[i].pre_posted_conns);
      fprintf(fp, "%d ", trace[i].cur_conns);
#endif

      fprintf(fp, "\n");
    }
  }

  fclose(fp);
}

void
evt_print_summary()
{
  struct struct_trace_summary *s;
  int i;
  double grand_total_time = 0.0;
  double pct = 0.0;

  /* Compute the totals and averages */
  for (i=0; i<EVT_MAX; i++) {
    s = &trace_summary[i];
    if (s->call_count > 0) {
      s->avg_time = s->total_time / (double) s->call_count;
      s->avg_interval = s->total_interval / (double) s->call_count;
      grand_total_time += s->total_time;
    }
  }

  /* Print the info */
  printf("-----------------------------------------------------------\n");
  printf("%24s %15s %5s %20s %10s %10s %10s %10s %10s %10s\n",
         "Evt", "Calls", "%Time", "TotTime", "AvgTime", "MinTime",
         "MaxTime", "AveInterval", "MinInterval", "MaxInterval");
  for (i=0; i<EVT_MAX; i++) {
    s = &trace_summary[i];
    if (s->call_count > 0) {
      pct = s->total_time * 100 / grand_total_time;
      printf("%24s %15ld %5.1f %20.0f %10.1f %10d %10d %10.1f %10d %10d\n",
          evt_names[i], s->call_count, pct, s->total_time, 
          s->avg_time, s->min_time,
          s->max_time, s->avg_interval, s->min_interval, s->max_interval);
      }
  }
}

void
evt_print_readable()
{
  if (options.trace_summary) {
    evt_print_summary();
  }
  evt_do_print(1);
}

void
evt_print()
{
  evt_do_print(0);
}

void
evt_stats_print(FILE *fp)
{
  int i;
  struct evtstat *s;
  double grand_total = 0.0;

  fprintf(fp, "%-24s %10s %10s %10s %10s %10s\n", 
          "event", "count", "min", "max", "avg", "total");
  for (i=0; i<EVT_MAX; i++) {
    s = &evt_stats[i];
    if (s->count != 0) {
      fprintf(fp, "%-24s %10d %10d %10d %10.2f %10.2f\n", 
              evt_names[i], s->count, s->min, s->max,  
        s->total / (double) s->count,
        s->total / (double) 1000000);
      grand_total += s->total;
    }
  }
  fprintf(fp, "%-24s %10s %10s %10s %10s %10.2f\n", 
          "", "", "", "", "", (grand_total / (double) 1000000));
}

void
evt_stats_compute()
{
  int event;
  int time;
  struct evtstat *s;
  int i;
  int skip_to_first = 1;

  /* printf("&trace[0] = %p\n", &trace[0]); */
  if (evt_count >= 0) {
    /* printf("&trace[%d] = %p\n", evt_count, &trace[evt_count]); */
  }

  for (i=0; i<EVT_MAX; i++) {
    s = &evt_stats[i];
    /* printf("s = %p\n", s); */
    s->count = 0;
    s->total = 0.0;
    s->max = 0;
    s->min = INT_MAX;
  }

  for (i=0; i<=evt_count; i++) {

    event = trace[i].event;

    /* Skip over events that are the server just waiting 
     * for the first event to happen 
     */
    if (skip_to_first && (event == EVT_SELECT || event == EVT_POLL ||
	  event == EVT_EPOLL_WAIT || event == EVT_AIO_WAIT)) {
      PRINT_TIME(NOFD, &tnow, &tprev, "evt_stats_compute: skipping "
	  "event number %d event = %s\n", i, evt_names[event]);
      continue;
    } else {
      skip_to_first = 0;
    }

    if (event <= 0) {
      printf("Invalid event = %d i = %d\n", event, i);
    }
    time = trace[i].call_time;
    s = &evt_stats[event];

    s->count++;
    s->total +=  time;

    if (time > s->max) {
      s->max = time;
    }

    if (time < s->min) {
      s->min = time;
    }
  } /* for */
}

void
evt_set_trace_evts()
{
#ifdef TRACE_ALL
   int i;
#endif
   FD_ZERO(&evt_trace_bits);

#ifdef TRACE_ALL
   for (i=0; i<EVT_MAX; i++) {
     FD_SET(i, &evt_trace_bits);
   }
#else
   /* Manually turn on the bits that should be traced */
   FD_SET(EVT_SENDFILE, &evt_trace_bits);
   /*
   FD_SET(EVT_ACCEPT, &evt_trace_bits);
   FD_SET(EVT_READ_SOCK, &evt_trace_bits);
   FD_SET(EVT_WRITE_SOCK, &evt_trace_bits);
   FD_SET(EVT_CLOSE_SOCK, &evt_trace_bits);
   */
#endif
}

void
evt_init()
{
  int i;
  struct timeval t;
  struct struct_trace_summary *s;

  evt_set_trace_evts();

  if (options.trace_summary) {
    for (i=0; i<EVT_MAX; i++) {
      s = &trace_summary[i];
      s->call_count = 0;
      s->total_time = 0.0;
      s->total_interval = 0.0;
      s->min_time = UINT_MAX;
      s->max_time = 0;
      s->min_interval = UINT_MAX;
      s->max_interval = 0;
    }
  }

  if (!options.trace_summary_only) {
    trace_bytes = sizeof(struct trace_type) * EVT_COUNT;
    printf("evt_init: allocate %lu bytes for trace buffer\n",
	(unsigned long) trace_bytes);
    trace = (struct trace_type *) alloc_aligned(pagesize, trace_bytes,
	    ALLOC_MMAP, "evt_init: (trace buffer)");

    printf("trace buf addr = %p size = %lu\n", trace,
	(unsigned long) trace_bytes);
    for (i=0; i<EVT_MAX; i++) {
      if (FD_ISSET(i, &evt_trace_bits)) {
        printf("tracing event = %d  = %s\n", i, evt_names[i]);
      }
    }
  }

  if (gettimeofday(&t, 0) < 0) {
    perror("gettimeofday");
    exit(1);
  }

  for (i=0; i<EVT_MAX; i++) {
    last_called[i] = t;
  }
  trace_start = t;

  printf("trace_start.tv_sec = %ld trace_start.tv_usec = %ld\n", 
     (long int) trace_start.tv_sec, (long int) trace_start.tv_usec);

  evt_init_done = 1;
  DEBG(MSG_TRACE, "evt_init: done\n");
}

void
check_evts()
{
  int i;

  for (i=0; i<=evt_count; i++) {
    if (trace[i].event == EVT_INVALID) {
      printf("Unknown event =  %d  addr = %p evt_count = %d\n",
	  i, &trace[i], evt_count);
      exit(1);
    }
  } /* for */
}

#endif /* TRACE_ON */

