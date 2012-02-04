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


#ifndef TRACE_H
#define TRACE_H

#if defined (TRACE_ON)

#include <sys/types.h>
#include <errno.h>
#include "cntl_intr.h"

/* We try to size the number of events that might get generated.
 * If we generate about 10 events per connection and the server
 * can handle a peak of 5000 requests per second and we run the
 * experiments for 120 seconds this should be about
 * 10 * 5000 * 120 events in total.
 */

/* Define one of the following two to turn tracing on */
/* Trace all of the events */

extern fd_set evt_trace_bits;

/* Trace all of the events */
/* If this is commented out we only trace the specified
 * events: see the #else part of TRACE_ALL in trace.c
 */
//#define TRACE_ALL

/* Trace only a portion of the events from a start to end time 
 * This is useful for tracing the middle (or other portion) of a run.
 */
/* #define TRACE_PORTION */

#endif

#define USEC  (1000000)

#ifdef TRACE_PORTION
#define TRACE_ON
#define WAIT_START_UP_SEC      (40)
#define WARM_UP_SEC            (30)
#define RAMP_UP_SEC            (20)
#define START_UP_SEC           (WAIT_START_UP_SEC + WARM_UP_SEC + RAMP_UP_SEC)
#define DURATION_SEC           (180)
#define START_PERCENT          (20)
#define END_PERCENT            (80)
#define START_SEC              (DURATION_SEC / 100 * START_PERCENT)
#define END_SEC                (DURATION_SEC / 100 * END_PERCENT)
#define USE_START_TIME_SEC    (START_UP_SEC + START_SEC)
#define USE_END_TIME_SEC      (START_UP_SEC + END_SEC)
#define USE_START_TIME_USEC   (USE_START_TIME_SEC * USEC)
#define USE_END_TIME_USEC     (USE_END_TIME_SEC * USEC)
#endif /* TRACE_PORTION */

#define EVT_ELAPSED
#define EVT_INTERVALS
#define EVT_EXTRAS

#define RUN_LENGTH_SECS  (120)
#define PEAK_RATE        (5000)
#define EVTS_PER_CONN    (10)
/* #define EVT_COUNT        (RUN_LENGTH_SECS * EVTS_PER_CONN * PEAK_RATE) */
/* Above approach was over estimating things - this is base on empirical data
 * for 120 second runs :-)
 */
// #define TEST
#ifdef TEST
#define EVT_COUNT        (50000)
#else
#define EVT_COUNT        (5000000)
#endif /* TEST */

#define EVT_INVALID               (0)
#define EVT_READ_SOCK             (1)
#define EVT_WRITE_SOCK            (2)
#define EVT_CLOSE_SOCK            (3)
#define EVT_ACCEPT                (4)
#define EVT_SELECT                (5)
#define EVT_READ_FILE             (6)
#define EVT_CLOSE_FILE            (7)
#define EVT_FSTAT                 (8)
#define EVT_FCNTL                 (9)
#define EVT_SIGPROCMASK          (10)
#define EVT_MACCEPT              (11)
#define EVT_SENDHANDLER          (12)
#define EVT_OPEN_FILE            (13)
#define EVT_SIGIO                (14)
#define EVT_CHANGESET            (15)
#define EVT_SENDFILE             (16)
#define EVT_SENDHANDLER_CALL     (17) 
#define EVT_REPEAT_ACCEPT        (18) 
#define EVT_WAIT_EVTS            (19) 
#define EVT_BLOCK_CONNS          (20) 
#define EVT_UNBLOCK_CONNS        (21) 
#define EVT_WRITEV_SOCK          (22) 
#define EVT_AUTO_ACCEPT          (23) 
#define EVT_MMAP                 (24) 
#define EVT_MLOCK                (25) 
#define EVT_MUNMAP               (26) 
#define EVT_MUNLOCK              (27) 
#define EVT_CACHE_ADD            (28) 
#define EVT_TCP_CORK             (29) 
#define EVT_POLL                 (30) 
#define EVT_EPOLL_WAIT           (31) 
#define EVT_EPOLL_CTL            (32) 
#define EVT_AIO_WAIT             (33) 
#define EVT_AIO_SOCK_ACCEPT      (34) 
#define EVT_AIO_SOCK_READ        (35) 
#define EVT_AIO_SOCK_WRITE       (36) 
#define EVT_AIO_SOCK_WRITEV      (37) 
#define EVT_AIO_SOCK_CLOSE       (38) 
#define EVT_EPOLL_CTLV           (39) 
#define EVT_AIO_SOCK_GETEVENTS   (40) 
#define EVT_AIO_SOCK_READ_ACCEPT (41) 
#define EVT_AIO_SOCK_DESTROY     (42)
#define EVT_AIO_SOCK_MEM_REG     (43) 
#define EVT_AIO_SOCK_MEM_DEREG   (44) 
#define EVT_EVENT_PHASE          (45) 
#define EVT_ACCEPT_PHASE         (46) 
#define EVT_WORK_PHASE           (47) 
#define EVT_MAX                  (48)  /* all < EVT_MAX */
                          /* NOTE: must be <= 256 events */
/* NOTE!!!: be sure to add/modify evt_names in trace.c when changing these */

struct trace_type {
    /* these two are required */
#ifdef EVT_INTERVALS
    unsigned int interval;
#endif /* EVT_INTERVALS */
#ifdef EVT_ELAPSED
    unsigned int elapsed;
#endif /* EVT_ELAPSED */
#ifdef EVT_EXTRAS
    int extra1;
    int extra2;
    int extra3;
#endif /* EVT_INTERVALS */

#ifdef HAVE_AIO_LAYER
    int pre_posted_conns;
    int cur_conns;
#endif

    unsigned int call_time;
    unsigned char event;
}; 

#ifdef TRACE_ON

struct struct_trace_summary {
	unsigned long call_count;
	int min_time; 
	int max_time; 
	int min_interval; 
	int max_interval; 
	double total_time; 
	double total_interval; 
	double avg_time;       /* Place these here in case we want to sort */
	double avg_interval;
};

extern struct struct_trace_summary trace_summary[EVT_MAX];

extern struct trace_type *trace;
extern struct timeval last_called[EVT_MAX];
extern struct timeval trace_start;

extern struct timeval tmp_start;
extern struct timeval tmp_stop;
extern struct timeval tmp_tdiff;
extern long int tmp_idiff;
extern long int tmp_ediff;
extern long int tmp_diff;

extern struct timeval tmp_start_time;
extern struct timeval tmp_stop_time;
extern struct timeval tmp_tdiff_time;
extern long int tmp_idiff_time;
extern long int tmp_ediff_time;
extern long int tmp_diff_time;

extern int tmp_save_errno;
extern int tmp_extra1;
extern int tmp_extra2;
extern int tmp_extra3;

extern int evt_init_done;
extern int evt_trace_on;
extern int evt_trace_buf_full;

void evt_add(char event, long int time, long int interval, 
             long int elapsed,  int e1, int e2, int e3);
void evt_print_readable();
void evt_print();
void evt_init();


/*----------------------------------------------------------------------*/

/*
 * Doesn't look like we can put ifdefs and/or comments in the macro.
 * We leave tracing turned off until we get the first accept call.
 * Note that this works even if tracing accept calls is turned off.
 */

#define TRACE(evt_type, evt)                             \
  do {                                                   \
    assert(evt_type != EVT_INVALID);                     \
    assert(evt_type < EVT_MAX);                          \
                                                         \
    if (evt_trace_on == 0 &&                             \
	(evt_type == EVT_ACCEPT ||                        \
	evt_type == EVT_AIO_SOCK_ACCEPT ||               \
	evt_type == EVT_AIO_SOCK_READ_ACCEPT ||          \
	evt_type == EVT_ACCEPT ||                        \
        evt_type == EVT_AUTO_ACCEPT)) {                   \
      evt_trace_on = 1;                                  \
    }                                                    \
                                                         \
    if (!evt_init_done) {                                \
        evt_init();                                      \
    }                                                    \
                                                         \
                                                         \
    if ((FD_ISSET(evt_type, &evt_trace_bits)) && (evt_trace_on) \
				&& (!evt_trace_buf_full)) { \
                                                         \
      if (gettimeofday(&tmp_start, 0) < 0) {                 \
        perror("TRACE: gettimeofday");                   \
        exit(1);                                         \
      }                                                  \
                                                         \
      timersub(&tmp_start, &last_called[evt_type], &tmp_tdiff);  \
      tmp_idiff = (long int) (tmp_tdiff.tv_sec * 1000000) + tmp_tdiff.tv_usec;  \
                                                         \
      last_called[evt_type] = tmp_start;                     \
                                                         \
      timersub(&tmp_start, &trace_start, &tmp_tdiff);            \
      tmp_ediff = (long int) (tmp_tdiff.tv_sec * 1000000) + tmp_tdiff.tv_usec;  \
                                                         \
      errno = 0;                                         \
      evt;                                               \
      tmp_save_errno = errno;                                \
                                                         \
      if (gettimeofday(&tmp_stop, 0) < 0) {                  \
        perror("TRACE: gettimeofday");                   \
        exit(1);                                         \
      }                                                  \
                                                         \
      timersub(&tmp_stop, &tmp_start, &tmp_tdiff);                   \
      tmp_diff = (long int) (tmp_tdiff.tv_sec * 1000000) + tmp_tdiff.tv_usec;   \
                                                         \
      switch (evt_type) {                                \
        case EVT_SENDFILE:                                \
          break;                                         \
                                                         \
        default:                                         \
          tmp_extra1 = maxconns-num_idle;                 \
          break;                                         \
      }                                                  \
                                                         \
      switch (evt_type) {                                \
        case EVT_SELECT:                                 \
          tmp_extra2 = rc;                                   \
          break;                                         \
                                                         \
        default:                                         \
          tmp_extra2 = rc;                                   \
          break;                                         \
                                                         \
      }                                                  \
      tmp_extra3 = trace_fd;                                 \
                                                         \
      if ((evt_type != EVT_SELECT) || (rc != 0)) {       \
         evt_add(evt_type, tmp_diff, tmp_idiff, tmp_ediff, tmp_extra1, tmp_extra2, tmp_extra3); \
      }                                                  \
                                                         \
      errno = tmp_save_errno;                                \
    } else {                                             \
      evt;                                               \
    }                                                    \
  } while (0);

/*----------------------------------------------------------------------*/

#define TRACE_START_TIME(evt_type)                       \
  do {                                                   \
    tmp_save_errno = errno;                              \
    assert(evt_type != EVT_INVALID);                     \
    assert(evt_type < EVT_MAX);                          \
                                                         \
    if (evt_trace_on == 0 &&                             \
	(evt_type == EVT_ACCEPT ||                       \
	evt_type == EVT_AIO_SOCK_ACCEPT ||               \
	evt_type == EVT_AIO_SOCK_READ_ACCEPT ||          \
	evt_type == EVT_ACCEPT ||                        \
        evt_type == EVT_AUTO_ACCEPT)) {                  \
      evt_trace_on = 1;                                  \
    }                                                    \
                                                         \
    if (!evt_init_done) {                                \
        evt_init();                                      \
    }                                                    \
                                                         \
    if ((FD_ISSET(evt_type, &evt_trace_bits)) && (evt_trace_on) \
				&& (!evt_trace_buf_full)) { \
                                                         \
      if (gettimeofday(&tmp_start_time, 0) < 0) {        \
        perror("TRACE: gettimeofday");                   \
        exit(1);                                         \
      }                                                  \
                                                         \
      timersub(&tmp_start_time, &last_called[evt_type], &tmp_tdiff_time);  \
      tmp_idiff_time = (long int) (tmp_tdiff_time.tv_sec * 1000000) + tmp_tdiff_time.tv_usec;  \
                                                         \
      last_called[evt_type] = tmp_start_time;            \
                                                         \
      timersub(&tmp_start_time, &trace_start, &tmp_tdiff_time);            \
      tmp_ediff_time = (long int) (tmp_tdiff_time.tv_sec * 1000000) + tmp_tdiff_time.tv_usec;  \
                                                         \
      errno = tmp_save_errno;                            \
    }                                                    \
  } while (0);

/*----------------------------------------------------------------------*/
#define TRACE_END_TIME(evt_type, e2, e3)                 \
{                                                        \
  do {                                                   \
    tmp_save_errno = errno;                              \
    assert(evt_type != EVT_INVALID);                     \
    assert(evt_type < EVT_MAX);                          \
                                                         \
    if (evt_trace_on == 0 &&                             \
	(evt_type == EVT_ACCEPT ||                       \
	evt_type == EVT_AIO_SOCK_ACCEPT ||               \
	evt_type == EVT_AIO_SOCK_READ_ACCEPT ||          \
	evt_type == EVT_ACCEPT ||                        \
        evt_type == EVT_AUTO_ACCEPT)) {                  \
      evt_trace_on = 1;                                  \
    }                                                    \
                                                         \
    if (!evt_init_done) {                                \
        evt_init();                                      \
    }                                                    \
                                                         \
    if ((FD_ISSET(evt_type, &evt_trace_bits)) && (evt_trace_on) \
				&& (!evt_trace_buf_full)) { \
                                                         \
      if (gettimeofday(&tmp_stop_time, 0) < 0) {              \
        perror("TRACE: gettimeofday");                   \
        exit(1);                                         \
      }                                                  \
                                                         \
      timersub(&tmp_stop_time, &tmp_start_time, &tmp_tdiff_time);                   \
      tmp_diff_time = (long int) (tmp_tdiff_time.tv_sec * 1000000) + tmp_tdiff_time.tv_usec;   \
                                                         \
      tmp_extra1 = maxconns-num_idle;                    \
      tmp_extra2 = e2;                                   \
      tmp_extra3 = e3;                                    \
                                                         \
      if (evt_type != EVT_SELECT) {                     \
         evt_add(evt_type, tmp_diff_time, tmp_idiff_time, tmp_ediff_time, tmp_extra1, tmp_extra2, tmp_extra3); \
      }                                                  \
                                                         \
    }                                                    \
    errno = tmp_save_errno;                            \
  } while (0); \
}

#else
#define TRACE(evt_type, evt)                             \
  do {                                                   \
    evt;                                                 \
  } while (0);

#define TRACE_START_TIME(evt_type)                       \
  do {                                                   \
  } while (0);

#define TRACE_END_TIME(evt_type, e1, e2)                 \
  do {                                                   \
  } while (0);
#endif

#endif /* TRACE_H */
