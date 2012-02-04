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
#include <fcntl.h>
#include <sys/fcntl.h>

#include "cntl_intr.h"
#include "cntl_conns.h"
#include "debug.h"
#include "trace.h"
#include "syscalls.h"
#include "options.h"
#include "stats.h"
#include "sock_listener.h"

#ifndef __CYGWIN__
#include "sigstuff.h"
#endif /* __CYGWIN__ */

#ifdef SEND
#include "send.h"
#ifdef ARRAY_OF_BUFS
extern void send_print_info_array();
#else
#endif /* ARRAY_OF_BUFS */
#endif /* SEND */

#ifdef TBB_QLENS
static inline _syscall1(int, qlen, int, fd);
static inline _syscall1(int, qlen_young, int, fd);
static inline _syscall1(int, qlen_listenq, int, fd);
#endif /* TBB_QLENS */

/* ---------------------------------------------------------------------- */
/* Looks like changing the signal mask is slower than other options */

int entering_cs = 0;
int intr_depth = 0;
int saved_modes = 0;

void do_block();
void do_unblock();
#ifdef SEND
void block_send_events();
void unblock_send_events();
#endif /* SEND */

/* ---------------------------------------------------------------------- */
void
intr_print_info(void)
{
  printf("sigio_blocked = %d\n", sigio_blocked);
  printf("num_block_sigio = %d\n", num_block_sigio);
  printf("num_unblock_sigio = %d\n", num_unblock_sigio);
  printf("new_connections_on = %d\n", new_connections_on);
  printf("intr_depth = %d\n", intr_depth);

#ifdef ONE_LISTENER
#ifdef TBB_QLENS
  printf("qlen_listenq = %d\n", qlen_listenq(server_sd));
  printf("       qlen_young = %d        qlen = %d\n", 
      qlen_young(server_sd), qlen(server_sd));
#endif /* TBB_QLENS */
#endif /* ONE_LISTENER */

#ifdef SEND
#ifdef ARRAY_OF_BUFS
  send_print_info_array();
#else
  send_print_info();
#endif /* ARRAY_OF_BUFS */
#endif /* SEND */
  printf("-------------------------------------------------------------\n");
}

/* ---------------------------------------------------------------------- */
void 
intr_enable(void)
{
    PRINT_TIME(NOFD, &tnow, &tprev, "intr_enable new_connections_on = %d",
        new_connections_on);
    /*
     * If interrupts are also used to control incoming connections
     * then we want to enable interrupts iff:
     *   num_idle >= 1 (actually >= options.thold)
     */
    if (options.conn_on == OPT_CONN_WITH_SIGIO) {
      if (new_connections_on) {
        PRINT_TIME(NOFD, &tnow, &tprev, "intr_enable: new connections are on");
        DEBG(MSG_INTR, "intr_enable: calling unblock_sigio()\n");
        unblock_sigio();
      } else {
	PRINT_TIME(NOFD, &tnow, &tprev, "intr_enable: should enable is false");
      }
    } else if (options.conn_on == OPT_CONN_WITH_SEND_EVTS) {
#ifdef SEND
      if (new_connections_on) {
        PRINT_TIME(NOFD, &tnow, &tprev, "intr_enable: new connections are on");
        DEBG(MSG_INTR, "intr_enable: calling unblock_send_events()\n");
        unblock_send_events();
      } else {
	PRINT_TIME(NOFD, &tnow, &tprev, "intr_enable: should enable is false");
      }
#endif /* SEND */
    } else {
      PRINT_TIME(NOFD, &tnow, &tprev, 
	"intr_enable: enabling (not using SIGIO or SEND)");
    }
}

/* ---------------------------------------------------------------------- */
void 
intr_disable(void)
{
    PRINT_TIME(NOFD, &tnow, &tprev, "intr_disable new_connections_on = %d",
       	new_connections_on);
    /* Not true: could be blocked because there are no resources available */
    /* assert(!sigio_blocked); */
    if (options.conn_on == OPT_CONN_WITH_SIGIO) {
      DEBG(MSG_INTR, "intr_disable: calling block_sigio()\n");
      block_sigio();
    } else if (options.conn_on == OPT_CONN_WITH_SEND_EVTS) {
#ifdef SEND
      DEBG(MSG_INTR, "intr_disable: calling block_send_events()\n");
      block_send_events();
#endif /* SEND */
    }
}

/* ---------------------------------------------------------------------- */
/* The problem with this approach is that there that it doesn't 
 * actually prevent signals from being delivered when we don't want them
 * delivered. It tells the kernel that we now longer want to be notified
 * asynchronously when new connections are requested - but the kernel
 * will still send us signals for outstanding connection requests 
 * __even after we've told it not to notify us of new connection requests__
 */

void
block_sigio(void)
{
  PRINT_TIME(NOFD, &tnow, &tprev, "block_sigio: entered sigio_blocked = %d",
      sigio_blocked);

  if (sigio_blocked == 1) {
    PRINT_TIME(NOFD, &tnow, &tprev, 
	"block_sigio: is already blocked returning");
    return;
  }

  DEBG(MSG_INTR, "block_sigio: blocking\n");
  do_block();

  num_block_sigio++;
  DEBG(MSG_INTR, "block_sigio: unblocked = %15d blocked = %15d\n", 
      num_unblock_sigio, num_block_sigio);

  DEBG(MSG_INTR, "block_sigio: setting sigio_blocked = 1\n");
  sigio_blocked = 1;
  DEBG(MSG_INTR, "block_sigio: done setting sigio_blocked = 1\n");
  PRINT_TIME(NOFD, &tnow, &tprev, "block_sigio: returning");
}

/* ---------------------------------------------------------------------- */
void
unblock_sigio(void)
{
  PRINT_TIME(NOFD, &tnow, &tprev, "unblock_sigio: entered sigio_blocked = %d",
      sigio_blocked);

  if (sigio_blocked == 0) {
    PRINT_TIME(NOFD, &tnow, &tprev, 
	"unblock_sigio: is already unblocked returning");
    return;
  }

  num_unblock_sigio++;
  DEBG(MSG_INTR, "unblock_sigio: unblocked = %15d blocked = %15d\n", 
        num_unblock_sigio, num_block_sigio);

  /* Set this before actually unblocking because we could get */
  /* after we're unblocked we could get interrupted before this gets set. */
  DEBG(MSG_INTR, "unblock_sigio: setting sigio_blocked = 0\n");
  sigio_blocked = 0;
  DEBG(MSG_INTR, "unblock_sigio: done setting sigio_blocked = 0\n");

  DEBG(MSG_INTR, "unblock_sigio: unblocking\n");
  do_unblock();

  PRINT_TIME(NOFD, &tnow, &tprev, "unblock_sigio: returning");
}

/* ---------------------------------------------------------------------- */

/*
 * TODO: too many calls to sigprocmask here.
 * We probably know what the mask is before these calls and
 * what the mask will be after these calls so we should be
 * able to just set the appropriate mask with one call.
 */

/* ---------------------------------------------------------------------- */
void
do_block(void)
{
  int flags = 0;
  sigset_t set, old;
  int rc;
  int trace_fd = 0;
  int i;

  switch (options.sigio_cntl) {
    case OPT_SIGIO_USE_PROCMASK:
#ifdef __CYGWIN__
      printf("do_block: option not supported in CYGWIN version\n");
      exit(1);
#else
      PRINT_TIME(NOFD, &tnow, &tprev, "do_block: changing procmask");
      if (in_handler) {
	DEBG(MSG_INTR, "do_block: in_handler = 1\n");
	/* printf("do_block - in handler - sigaddset\n"); */
	assert(uc != 0);
#ifdef MACROBUG
	print_mask("do_block: uc_sigmask was \n", 
	    (sigset_t *) &uc->uc_sigmask);
	TRACE(EVT_CHANGESET,
	  trace_fd = 0;
	  rc = sigaddset((sigset_t *) &(uc->uc_sigmask), SIGIO);
	);
	print_mask("do_block: uc_sigmask is now\n", 
	    (sigset_t *) &uc->uc_sigmask);
#else
	print_mask("do_block: uc_sigmask was \n", &uc->uc_sigmask);
	TRACE(EVT_CHANGESET,
	  trace_fd = 0;
	  rc = sigaddset(&(uc->uc_sigmask), SIGIO);
	);
	print_mask("do_block: uc_sigmask is now\n", &uc->uc_sigmask);
#endif /* MACROBUG */
      } else {
	DEBG(MSG_INTR, "do_block: in_handler = 0 so doing changing mask\n");
	/* printf("do_block - in handler - sigprocmask\n"); */
	/* get current sigprocmask */
	TRACE(EVT_SIGPROCMASK,
	  trace_fd = 0;
	  rc = sigprocmask(0, NULL, &set);
	);
	if (rc < 0) {
	  perror("sigprocmask get");
	  exit(1);
	}
	print_mask("do_block: mask was\n", &set);
	sigaddset(&set, SIGIO);
	TRACE(EVT_SIGPROCMASK,
	  trace_fd = 0;
	  rc = sigprocmask(SIG_BLOCK, &set, &old);
	);
	if (rc < 0) {
	  perror("sigprocmask set");
	  exit(1);
	}
	print_cur_mask("do_block: new mask is\n");
      }
#endif /* __CYGWIN__ */
      break;

    case OPT_SIGIO_USE_FCNTL:
      PRINT_TIME(NOFD, &tnow, &tprev, "do_block: calling fcntl");
      for (i=sock_listener_min; i<=sock_listener_max; i++) {
	if (sock_is_listener(i)) {
	  /* TODO: check that these flags are done correctly */
	  TRACE(EVT_FCNTL,
	    trace_fd = i;
	    rc = fcntl(i, F_SETFL, flags | O_NONBLOCK);
	  );

	  if (rc < 0) {
	    printf("do_block: fcntl F_SETFL O_NONBLOCK failed on sd = %d\n", i);
	    perror("do_block: fcntl F_SETFL O_NONBLOCK failed\n");
	    exit(1);
	  }
	}
      }
      break;

    default:
      printf("Invalid option specified for controlling SIGIOs\n");
      exit(1);
      break;
  }
}

/* ---------------------------------------------------------------------- */
void
do_unblock(void)
{
#ifndef SUNOS
  int flags = 0;
  int i;
#endif /* SUNOS */
  sigset_t set, old;
  int rc;
  int trace_fd = 0;
  
  switch (options.sigio_cntl) {
    case OPT_SIGIO_USE_PROCMASK:
#ifdef __CYGWIN__
      printf("do_block: option not supported in CYGWIN version\n");
      exit(1);
#else
      PRINT_TIME(NOFD, &tnow, &tprev, "do_unblock: changing procmask");
      if (in_handler) {
	DEBG(MSG_INTR, "do_unblock: in_handler = 1\n");
	/* printf("do_unblock - in handler - sigdelset\n"); */
	assert(uc != 0);
#ifdef MACROBUG
	print_mask("do_unblock: uc_sigmask was \n", 
	    (sigset_t *) &uc->uc_sigmask);
	TRACE(EVT_CHANGESET,
	  trace_fd = 0;
	  rc = sigdelset((sigset_t *) &uc->uc_sigmask, SIGIO);
	);
	print_mask("do_unblock: uc_sigmask is now\n", 
	    (sigset_t *) &uc->uc_sigmask);
#else
	print_mask("do_unblock: uc_sigmask was \n", &uc->uc_sigmask);
	TRACE(EVT_CHANGESET,
	  trace_fd = 0;
	  rc = sigdelset(&uc->uc_sigmask, SIGIO);
	);
	print_mask("do_unblock: uc_sigmask is now\n", &uc->uc_sigmask);
#endif /* MACROBUG */
      } else {
	DEBG(MSG_INTR, "do_unblock: in_handler = 0 so doing changing mask\n");
	/* get current sigprocmask */
	/* printf("do_unblock - not in handler - sigprocmask\n"); */
	TRACE(EVT_SIGPROCMASK,
	  trace_fd = 0;
	  rc = sigprocmask(0, NULL, &set);
	);
	if (rc < 0) {
	  perror("sigprocmask get");
	  exit(1);
	}
	print_mask("do_unblock: mask was\n", &set);
	sigaddset(&set, SIGIO);
	TRACE(EVT_SIGPROCMASK,
	  trace_fd = 0;
	  rc = sigprocmask(SIG_UNBLOCK, &set, &old);
	);
	if (rc < 0) {
	  perror("sigprocmask set");
	  exit(1);
	}
	print_cur_mask("do_unblock: new mask is\n");
      }
#endif /* __CYGWIN__ */
      break;

    case OPT_SIGIO_USE_FCNTL:
#if defined(HPUX) || defined(SUNOS) || defined(__CYGWIN__)
      printf("SIGIO unfinished for HP-UX, SUNOS, and CYGWIN\n");
      exit(1);
#else
      PRINT_TIME(NOFD, &tnow, &tprev, "do_unblock: calling fcntl");
      for (i=sock_listener_min; i<=sock_listener_max; i++) {
	if (sock_is_listener(i)) {
	  /* TODO: check that these flags are done correctly */
	  TRACE(EVT_FCNTL,
	    trace_fd = i;
	    rc = fcntl(i, F_SETFL, flags | O_NONBLOCK | O_ASYNC);
	  );
	  if (rc < 0) {
	    printf("do_unblock: fcntl F_SETFL O_NONBLOCK | O_ASYNC "
		"on sd = %d failed\n", i);
	    exit(1);
	  }
	}
      }
#endif
      break;

    default:
      printf("Invalid option specified for controlling SIGIOs\n");
      exit(1);
      break;
  }
}

#ifdef SEND
/* ---------------------------------------------------------------------- */

/*
 * TODO: Change this so that it's actually part of one of the
 * other routines and the operations are the only part that's different
 * depending on how connections are made
 */

#define send_events_blocked       (sigio_blocked)
#define num_block_send_events     (num_block_sigio)
#define num_unblock_send_events   (num_unblock_sigio)


void
block_send_events(void)
{
  PRINT_TIME(NOFD, &tnow, &tprev, 
      "block_send_events: entered send_events_blocked = %d",
      send_events_blocked);

  if (send_events_blocked == 1) {
    PRINT_TIME(NOFD, &tnow, &tprev, 
	"block_send_events: is already blocked returning");
    return;
  }

  DEBG(MSG_INTR, "block_send_events: blocking\n");
  saved_mode = evt_notify; 
  ecb->notify = ecb->notify & ~AUTO_ACCEPT_GRP;
  ecb->notify = ecb->notify & ~ACCEPT_IO_GRP;

  num_block_send_events++;
  DEBG(MSG_INTR, "block_send_events: unblocked = %15d blocked = %15d\n", 
      num_unblock_send_events, num_block_send_events);

  DEBG(MSG_INTR, "block_send_events: setting send_events_blocked = 1\n");
  send_events_blocked = 1;
  DEBG(MSG_INTR, "block_send_events: done setting send_events_blocked = 1\n");
  PRINT_TIME(NOFD, &tnow, &tprev, "block_send_events: returning notify = 0x%x",
		  ecb->notify);
}

/* ---------------------------------------------------------------------- */
void
unblock_send_events(void)
{
  PRINT_TIME(NOFD, &tnow, &tprev, 
      "unblock_send_events: entered send_events_blocked = %d",
      send_events_blocked);

  if (send_events_blocked == 0) {
    PRINT_TIME(NOFD, &tnow, &tprev, 
	"unblock_send_events: is already unblocked returning");
    return;
  }

  num_unblock_send_events++;
  DEBG(MSG_INTR, "unblock_send_events: unblocked = %15d blocked = %15d\n", 
        num_unblock_send_events, num_block_send_events);

  /* Set this before actually unblocking because we could get */
  /* after we're unblocked we could get interrupted before this gets set. */
  DEBG(MSG_INTR, "unblock_send_events: setting send_events_blocked = 0\n");
  send_events_blocked = 0;
  DEBG(MSG_INTR, "unblock_send_events: done setting send_events_blocked = 0\n");

  DEBG(MSG_INTR, "unblock_send_events: unblocking\n");
  
  ecb->notify = saved_mode;
  saved_mode = 0;

  PRINT_TIME(NOFD, &tnow, &tprev, 
      "unblock_send_events: returning notify = 0x%x", ecb->notify);
}

/* ---------------------------------------------------------------------- */
#endif /* SEND */
