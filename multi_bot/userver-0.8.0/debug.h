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


#ifndef DEBUG_H
#define DEBUG_H

#include <sys/types.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include "common.h"
#include "myassert.h"

/* --- DEFINES -------------------------------------------------------- */

/* SEE BELOW FOR EXTRA DEBUGGING THAT CAN BE TURNED ON 
 * E.g., DEBUG_OUTPUT_BUFS, DEBUG_READ_BUFS, etc.
 */

#define MSG_TIME                   (1<<0)
#define MSG_GENERAL                (1<<1)
#define MSG_SOCKBUF                (1<<2)
#define MSG_DELAY                  (1<<3)
#define MSG_SIGNALS                (1<<4)
#define MSG_MULTIACCEPT            (1<<5)
#define MSG_READABLE               (1<<6)
#define MSG_WRITABLE               (1<<7)
#define MSG_CNTL_CONNS             (1<<8)
#define MSG_CONN                   (1<<9)
#define MSG_NOTE                   (1<<10)
#define MSG_ASYNC                  (1<<11)
#define MSG_INTR                   (1<<12)
/* AS - MSG_SEND is already defined in sys/socket.h in MAC OS X
 * For now, don't define it because we don't have SEND support in OS X
 */
#ifdef SEND
#define MSG_SEND                   (1<<13)
#endif /* SEND */
#define MSG_SERVER_LOOP            (1<<14)
#define MSG_PROCESS_SDS            (1<<15)
#define MSG_SIGMASKS               (1<<16)
#define MSG_TRACE                  (1<<17)
#define MSG_WRITEV                 (1<<18)
#define MSG_SEND_REPLY             (1<<19)

#define MSG_MASK_ALL     (0xffffffff)
/* 
#define MSG_MASK_CUR     (MSG_WRITEV | MSG_SIGNALS | MSG_TIME | \
                          MSG_INTR | MSG_CNTL_CONNS | MSG_ASYNC | MSG_SEND)
*/
/* #define MSG_MASK_CUR     (MSG_WRITEV | MSG_SEND_REPLY | MSG_WRITABLE) */
#define MSG_MASK_CUR        (0)

extern unsigned int MSG_MASK;

#ifdef DEBUG_ON
/* #define DEBUG_NOT_CLOSED */
/* #define DEBUG_OUTPUT_BUFS */
/* #define DEBUG_READ_BUFS */
/* #define MSG_NOT_CLOSED */
/* #define DEBUG_SIGMASK */
/* #define DEBUG_SOCKBUF */
/* #define DEBUG_Q */
/* #define DEBUG_SEND */
/* #define DEBUG_FDSET */
#endif /* DEBUG_ON */


/*
 * NOTE: only using ... and __VA_ARGS__ doesn't work properly on the
 * version of gcc we are using.  So we use the gcc extension which
 * does seem to work correctly.
 * See below if you are using a more up to date compiler.
 */
#ifdef OLD_VARARGS

#ifdef DEBUG_ON
#define PRINT_TIME(sd, now, prev, __VA_ARGS__...) \
   do {\
     if (MSG_MASK & MSG_TIME) { \
       print_time(sd, now, prev, __func__, __LINE__, __VA_ARGS__); \
     } \
   } while (0);

#define PRINT_DELAY(sd, now, prev, __VA_ARGS__...) \
   do {\
     if (MSG_MASK & MSG_DELAY) { \
       print_time(sd, now, prev, __func__, __LINE__, __VA_ARGS__); \
     } \
   } while (0);

#define DEBG(x, __VA_ARGS__...) \
   do {\
     if (MSG_MASK & (x)) { \
       printf(__VA_ARGS__); \
       fflush(stdout); \
       fflush(stderr); \
     } \
   } while (0);

#else /* !DEBUG_ON */

#define PRINT_TIME(sd, now, prev, __VA_ARGS__...)
#define PRINT_DELAY(sd, now, prev, __VA_ARGS__...)
#define DEBG(x, __VA_ARGS__...)
#endif /* DEBUG_ON */

#else /* !OLD_VARARGS */

#ifdef DEBUG_ON
#define PRINT_TIME(sd, now, prev, ...) \
   do {\
     if (MSG_MASK & MSG_TIME) { \
       print_time(sd, now, prev, __func__, __LINE__, __VA_ARGS__); \
     } \
   } while (0);

#define PRINT_DELAY(sd, now, prev, ...) \
   do {\
     if (MSG_MASK & MSG_DELAY) { \
       print_time(sd, now, prev, __func__, __LINE__, __VA_ARGS__); \
     } \
   } while (0);

#define DEBG(x, ...) \
   do {\
     if (MSG_MASK & (x)) { \
       printf(__VA_ARGS__); \
       fflush(stdout); \
       fflush(stderr); \
     } \
   } while (0);

#else /* !DEBUG_ON */
#define PRINT_TIME(sd, now, prev, ...)     do { } while (0);
#define PRINT_DELAY(sd, now, prev, ...)    do { } while (0);
#define DEBG(x, ...)                       do { } while (0);

#endif /* DEBUG_ON */
#endif /* OLD_VARARGS */

void init_print_time(struct timeval *tnow, struct timeval *tprev);
void print_time(int fd, struct timeval *tnow, struct timeval *tprev,
     const char *file, int line, char *fmt, ...);
void print_info(char *s, struct info *ip);
void print_mask(char *msg, sigset_t *s);
void print_cur_mask(char *msg);
void print_fdset(char *str, fd_set *fdset, int max);
void debug_fdset(fd_set *fdset, int max);

extern struct timeval tnow;
extern struct timeval tprev;
extern struct timeval pnow;
extern struct timeval pprev;

#define NOFD     (-9)

#endif /* DEBUG_H */
