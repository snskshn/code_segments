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
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <errno.h>


#include "cntl_conns.h"
#include "common.h"
#include "debug.h"
#include "sock_listener.h"
#include "util.h"

/* This is or isn't defined in debug.h depending on desired behaviour */
unsigned int MSG_MASK = MSG_MASK_CUR;

struct timeval tnow;
struct timeval tprev;
struct timeval pnow;
struct timeval pprev;

#ifdef DEBUG_SIGMASK
/* Used for debugging signal masks */
static char *signames[] = {
  "NONE", "SIGHUP", "SIGINT", "SIGQUIT", "SIGILL", "SIGTRAP", "SIGABRT",
  "SIGBUS", "SIGFPE", "SIGKILL", "SIGUSR1", "SIGSEGV", "SIGUSR2",
  "SIGPIPE", "SIGALRM", "SIGTERM", "SIGSTKFLT", "SIGCHLD", "SIGCONT", 
  "SIGSTOP", "SIGTSTP", "SIGTTIN", "SIGTTOU", "SIGURG", "SIGXCPU", 
  "SIGXFSZ", "SIGVTALRM", "SIGPROF", "SIGWINCH", "SIGIO", "SIGPWR",
  "SIGUNUSED"
};
#endif /* DEBUG_SIGMASK */

void
init_print_time(struct timeval *tnow, struct timeval *tprev)
{
  if (gettimeofday(tnow, 0) < 0) {
    perror("gettimeofday");
    exit(-1);
  }

  if (gettimeofday(&pnow, 0) < 0) {
    perror("gettimeofday");
    exit(-1);
  }
}
void
print_info(char *s, struct info *ip)
{
  /* 
  printf("%10ld usec %5s %7s %6d %6d  %s\n", diff, "", "", 
      max_sd, num_idle, s);
  */
  printf("%10s      %5s %7s %6s %6d  INFO: %s\n", "", "", "", "", 
      ip->conn_num, s);
}

void
print_time(int sd, struct timeval *tnow, struct timeval *tprev, 
  const char *func, int line, char *fmt, ...)
{
  unsigned long diff;
  struct timeval tdiff;
  va_list argp;
  struct info *ip = 0;
  struct info *peer_ip = 0;
  int peer_sd = 0;
  int got_hup = -1;
  int peer_got_hup = -1;
  int fsm_state = FSM_UNKNOWN;
  int peer_state = FSM_UNKNOWN;
  int conn = 0;
  int req = 0;

  if (sd > 0) {
    if (info_map_valid(sd)) {
      ip = info_ptr(sd);

      if (ip) {
	req = ip->req.num;
	conn = ip->conn_num;
	fsm_state = ip->fsm_state;
        got_hup = ip->got_hup;
      } else {
	req = -1;
	conn = -1;
	fsm_state = FSM_UNKNOWN;
      }
    } else {
      if (sock_is_listener(sd)) {
	req = -1;
	conn = -1;
	fsm_state = FSM_LISTENING;
      } else {
	req = -1;
	conn = -1;
	fsm_state = FSM_CLOSED; 
      }
    }
  } else {
    fsm_state = FSM_UNKNOWN;
  }

  *tprev = *tnow;
  if (gettimeofday(tnow, 0) < 0) {
    perror("gettimeofday");
    exit(-1);
  }

  timersub(tnow, tprev, &tdiff);

  diff = (tdiff.tv_sec * 1000000) + tdiff.tv_usec;
  if (sd == NOFD) {
    va_start(argp, fmt);
#ifdef MAXOPENFD
    printf("%8ld usec %5s %6s  %8s %4s  %6s %4s %6d %6d %6d  %6s %6s  ", 
           diff, "", "", "", "", "", "", max_sd, num_idle, max_opensd, "", "");
#else
    printf("%8ld usec %5s %6s  %8s %4s  %6s %4s %6d %6d  %6s %6s  ", 
           diff, "", "", "", "", "", "", max_sd, num_idle, "", "");
#endif /* MAXOPENFD */

    printf(" %s : %d -- ", func, line);
    vprintf(fmt, argp);
    va_end(argp);
    printf("\n");

  } else {
    /* check if there is a peer we can provide info about */
    peer_sd = 0;
    peer_got_hup = -1;
    if (ip) {
      if (ip->rep.fd != -1) {
        peer_ip = info_ptr(ip->rep.fd);
        if (peer_ip) {
          peer_sd = peer_ip->sd;
          peer_state = peer_ip->fsm_state;
          peer_got_hup = peer_ip->got_hup;
        }
      }
    }
 
    va_start(argp, fmt);
#ifdef MAXOPENFD
    printf("%8ld usec (sd = %6d /%2d / %2d : %6d /%2d / %2d) %6d %6d %6d [%6d|%6d] ", 
           diff, sd, fsm_state, got_hup, peer_sd, peer_state, peer_got_hup, max_sd, num_idle, max_opensd, conn, req);
#else
    printf("%8ld usec (sd = %6d /%2d / %2d : %6d /%2d / %2d) %6d %6d [%6d|%6d] ", 
           diff, sd, fsm_state, got_hup, peer_sd, peer_state, peer_got_hup, max_sd, num_idle, conn, req);
#endif /* MAXOPENFD */
    printf(" %s : %d -- ", func, line);
    vprintf(fmt, argp);
    va_end(argp);
    printf("\n");
  }
  fflush(stdout);
  fflush(stderr);
}


void
print_mask(char *msg, sigset_t *s)
{
#ifdef DEBUG_SIGMASK
  int i;
  int none = 1;

  if (MSG_MASK & MSG_SIGMASKS) {
    printf("%s", msg);

    for (i=1; i<32; i++) {
      if (sigismember(s, i)) {
	  printf("        signal %2d is on = %12s\n", i, signames[i]);
	  none = 0;
      }
    }
    if (none) {
      printf("  nothing set\n");
    }
    fflush(stdout);
    fflush(stderr);
  }
#endif /* DEBUG_SIGMASK */
}

void
print_cur_mask(char *msg)
{
#ifdef DEBUG_SIGMASK
  sigset_t tmp;
  if (MSG_MASK & MSG_SIGMASKS) {
    if (sigprocmask(0, NULL, &tmp)) {
      perror("sigprocmask");
      exit(1);
    }
    print_mask(msg, &tmp);
  }
#endif /* DEBUG_SIGMASK */
}


void
print_fdset(char *str, fd_set *fdset, int max) 
{
  int fd;
  int count = 0;
  int total = 0;
  printf("print_fdset: fd_set = %p\n", fdset);
  printf("print_fdset: sizeof(fd_set) = %lu\n", (unsigned long) sizeof(fd_set));

  printf("%s max_sd = %d\n", str, max);
  for (fd = 0; fd < FD_SETSIZE; fd++) {
    total++;
    if (FD_ISSET(fd, fdset)) {
      printf("%c%d%s%s",
	  count > 0 ? ',' : ' ',
	  fd,
	  is_sock_readable(fd, 0) ? "r" : "",
	  is_sock_writable(fd, 0) ? "w" : "");
      count++;
    }
  }
  printf(" (count = %d of total = %d)\n", count, total);
}

void
debug_fdset(fd_set *fdset, int max) 
{
  int tmp;
  struct timeval tv;
  fd_set newrd, newwr, newex;
  extern fd_set *rdfds;
  extern fd_set *wrfds;
  int i, j, n;
  int sleep_time = 2;

  sleep_time = 64;
  printf("Sleeping for %d seconds\n", sleep_time);
  fflush(stdout);
  fflush(stderr);
  sleep(sleep_time);
  sleep_time = 2;

  for (i=0; i<6 && num_idle != maxconns; i++) {
    printf("=======================================================\n");

    for (j=0;j<4 && num_idle != maxconns; j++) {
      
      printf("------------------------------------------------------\n");
      printf("Check on rdfds with select\n");

      tv.tv_sec = 1;
      tv.tv_usec = 0;

      newrd = *rdfds;
      newex = *rdfds;
      newwr = *wrfds;

      print_fdset("newrd before select:\n", &newrd, max);
      print_fdset("newwr before select:\n", &newwr, max);
      print_fdset("newex before select:\n", &newex, max);

      printf("debug_fdset: calling select\n");
      n = 0;
      errno = 0;
      n = select(max, &newrd, &newwr, &newex, &tv);
      printf("debug_fdset: select returns n = %d errno = %d\n", n, errno);
      if (n > 0) {
	print_fdset("newrd after select:\n", &newrd, max);
	print_fdset("newwr after select:\n", &newwr, max);
	print_fdset("newex after select:\n", &newex, max);
	for (tmp=0; tmp<max; tmp++) {
	   if (FD_ISSET(tmp, &newrd) && !sock_is_listener(tmp)) {
	     printf("tmp = %6d is readable\n", tmp);
	     socket_readable(info + tmp);
	   }
	   if (FD_ISSET(tmp, &newwr) && !sock_is_listener(tmp)) {
	     printf("tmp = %6d is writable\n", tmp);
	     socket_writable(info + tmp);
	   }
	}
      }
    } /* for */

    printf("Sleeping for %d seconds\n", sleep_time);
    fflush(stdout);
    fflush(stderr);
    sleep(sleep_time);
    sleep_time *= 2;

  } /* for */
}
