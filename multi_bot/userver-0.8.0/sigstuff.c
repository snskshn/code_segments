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


#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "common.h"
#ifndef __CYGWIN__
#include "sigstuff.h"
#endif /* __CYGWIN__ */
#include "debug.h"
#include "options.h"
#include "stats.h"
#include "syscalls.h"
#include "cntl_intr.h"
#include "cntl_conns.h"
#include "trace.h"
#include "netstat.h"
#include "sock_listener.h"
#include "aio_layer.h"
#include "info.h"
#include "interest_set.h"
#include "cache_mapped_new.h"
#include "logging.h"
#include "sock_special.h"

#ifdef CALL_STATS
#include "call_stats.h"
#endif /* CALL_STATS */

#ifdef SEND
#include "send.h"
#ifdef ARRAY_OF_BUFS
extern void print_all_evts_array();
#endif /* ARRAY_OF_BUFS */
#endif

#ifndef __CYGWIN__
ucontext_t *uc = 0;
#endif /* __CYGWIN__ */

int sigios_to_handle = 0;
int exit_due_to_signal = 0;
void cleanup();
void exit_notification();

/* -------------------------------------------------------------------- */
#ifdef __CYGWIN__
/* cygwin only provides one parameter */
void
sigio_handler(int signum)
#else
void
sigio_handler(int signum, siginfo_t *info, void *ucontext)
#endif
{
  int rc;
  /* used for trace data */
  int trace_fd = 0;

  /* state we were in before getting into signal handler */
  in_handler = 1;
#ifndef __CYGWIN__
  uc = (ucontext_t *) ucontext;
#endif /* __CYGWIN__ */

  /* printf("--->  sigio_handler: entered num_idle = %d\n", num_idle); */
  PRINT_TIME(NOFD, &tnow, &tprev, "--->  sigio_handler: "
      "entered num_idle = %d", num_idle);
  DEBG(MSG_INTR, "sigio_handler: entered num_idle = %d "
      "in_cs = %d sigio_blocked = %d\n", num_idle, in_cs, sigio_blocked);

  /* either entering a critical section or in one so just return */
  if (entering_cs || in_cs) {
    PRINT_TIME(NOFD, &tnow, &tprev, "<--- sigio_handler: "
	"returning - sigio_race detected\n");
    num_sigio_races++;
    sigios_to_handle++;
    in_handler = 0;
    return;
  }

  if (!new_connections_on || sigio_blocked) {
    PRINT_TIME(NOFD, &tnow, &tprev, "<--- sigio_handler: "
	"returning - new connections not on\n");
    num_sigio_false++;
    sigios_to_handle++;
    in_handler = 0;
    return;
  }

  /* Current approach disables signals them when calling handler. */
#ifdef FIX
  if (options.sigio_cntl != OPT_SIGIO_USE_PROCMASK) {
    intr_disable();
  }
#else
  /* printf("Calling intr_disable\n"); */
  intr_disable();
#endif

  /* should not be able to get an interrupt while in a critical section */
  assert(!in_cs);

#ifndef __CYGWIN__
  /* print_mask("sigio_handler mask\n", &uc->uc_sigmask); */
#endif /* __CYGWIN__ */

  num_sigios++;
  TRACE(EVT_SIGIO,
    trace_fd = 0;
    rc = num_sigios;
  );

  /* no need to turn off and on notification here because 
   * we're in the handler
   */

  /* TODO: things get messy if we have multiple listeners here
   *   really would like to have a way of figuring out which socket
   *   generated the SIGIO so we could accept only on that socket
   *   For now we'll just ensure that if we use SIGIO that ONE_LISTENER
   *   is defined.
   */
#ifdef ONE_LISTENER
  do_new_connections(server_sd, FROM_SIGIO_HANDLER);
#else
  printf("sigio_handler: Support for multiple listeners and "
         "SIGIO is not implemented yet\n");
  exit(1);
#endif /* ONE_LISTENER */

  DEBG(MSG_INTR, "sigio_handler: done handling new connection\n");

  assert(!in_cs);


  /* TODO: Note that if we now consumed all of the available resources */
  /* (e.g., fds) then we don't want interrupts enabled upon return. */
  /* Not sure about how to do this given the existing signal interface. */
  if (new_connections_on) {
    DEBG(MSG_INTR, "sigio_handler: calling intr_enable\n");
#ifdef FIX
    if (options.sigio_cntl != OPT_SIGIO_USE_PROCMASK) {
      intr_enable();
    }
#else
    /* printf("Calling intr_enable\n"); */
    intr_enable();
#endif
  } else {
    DEBG(MSG_INTR, "sigio_handler: calling intr_disable\n");
    /* printf("Calling intr_disable\n"); */
    intr_disable();
  }

#ifndef __CYGWIN__
  uc = 0;
#endif /* __CYGWIN__ */

  /* TODO: not sure if this needs to be done before or after 
   * changing interrupts.
   * This must be set before we check if we should enable new connections 
   */
  in_handler = 0;

  DEBG(MSG_INTR, "sigio_handler: returning num_idle = %d "
      "in_cs = %d sigio_blocked = %d\n", num_idle, in_cs, sigio_blocked);
  PRINT_TIME(NOFD, &tnow, &tprev, "<--- sigio_handler: returning");
  /* printf("<--- sigio_handler: returning\n"); */
}


/* -------------------------------------------------------------------- */
void
sig_kill_handler(void)
{
  static int caught_count = 0;

  if (caught_count) {
    fprintf(stderr, "sig_kill_handler: caught count = %d\n", caught_count);
    exit(-1);
  }
  caught_count++;

  printf("Caught signal SIGINT SIGHUP SIGCHLD or SIGTERM\n");
  PRINT_TIME(NOFD, &tnow, &tprev, "Caught signal");

  exit_due_to_signal = 1;

  exit(0);
}

void
cleanup(void)
{
  extern int new_connections_on;
  extern void stats_print_aio_sock_mem_reg();
  extern void stats_print_aio_sock_getevents();
#ifdef HAVE_AIO_LAYER
  extern void *read_reg_addr;
  extern void *read_reg_handle;
  extern int   read_reg_size;
  extern void *write_reg_addr;
  extern void *write_reg_handle;
  extern int   write_reg_size;
#endif /* HAVE_AIO_LAYER */
#ifdef HAVE_PROC_FILE_INFO
  int rc;
#endif
  int i;
	struct info *ip = 0;

	exit_notification();

	printf("STATS START\n");

  if (options.use_logging) {
    logging_cleanup();
  }

  info_listener_print_status();
  info_print_status();

  print_all();

  /* close all of the listening sockets */
  for (i=sock_listener_min; i<=sock_listener_max; i++) {
    if (sock_is_listener(i)) {

#ifdef HAVE_AIO_LAYER
#else
      PRINT_TIME(NOFD, &tnow, &tprev, "cleanup: closing listener %d:", i);
      close(i);
#endif /* HAVE_AIO_LAYER */
    }
  }
  
  /* close all of the app server sockets */
  for (i=sock_special_min; i<=sock_special_max; i++) {
		ip = info_ptr(i);
		assert(ip);
    if (ip->appserver) {
			assert(ip->appserver->sd == i);
      // printf("cleanup: closing app connection sd = %d\n", i);
      PRINT_TIME(NOFD, &tnow, &tprev, "cleanup: closing app connection %d:", i);
      close(i);
    }
  }
  
  if (!new_connections_on) {
    fprintf(stdout, "WARNING: server stopped and new "
	"connections are disabled\n");
    fprintf(stderr, "WARNING: server stopped and new "
	"connections are disabled\n");
  }

  cache_check_ref_counts();

  if (options.caching_on) {
    PRINT_TIME(NOFD, &tnow, &tprev, "cleanup: calling cache_cleanup");
    cache_cleanup();
  }

#ifdef HAVE_AIO_LAYER
  if (options.use_socket_aio) {

    PRINT_TIME(NOFD, &tnow, &tprev, "cleanup: aio_sock_mem_dereg: "
	"addr = %p size = %d handle = %p", 
	read_reg_addr, read_reg_size, read_reg_handle);
    rc = aio_sock_mem_dereg(read_reg_addr, read_reg_size, read_reg_handle);
    if (rc < 0) {
      printf("cleanup: aio_sock_mem_dereg failed for read buffers "
	  "rc = %d\n", rc);
      printf("cleanup: addr = %p size = %d handle = %p\n",
              read_reg_addr, read_reg_size, read_reg_handle);
    } else {
      stats_dereg_bytes(read_reg_size);
    }

    PRINT_TIME(NOFD, &tnow, &tprev, "cleanup: aio_sock_mem_dereg: "
	"addr = %p size = %d handle = %p", 
	write_reg_addr, write_reg_size, write_reg_handle);
    rc = aio_sock_mem_dereg(write_reg_addr, write_reg_size, write_reg_handle);
    if (rc < 0) {
      printf("cleanup: aio_sock_mem_dereg failed for write buffers "
	  "rc = %d\n", rc);
      printf("cleanup: addr = %p size = %d handle = %p\n",
              write_reg_addr, write_reg_size, write_reg_handle);
    } else {
      stats_dereg_bytes(write_reg_size);
    }

    stats_print_aio_sock_mem_reg();
    stats_print_aio_sock_getevents();


    PRINT_TIME(NOFD, &tnow, &tprev, "cleanup: aio_sock_uninit");
    aio_sock_uninit();
  }
#endif /* HAVE_AIO_LAYER */

#ifdef HAVE_PROC_FILE_INFO
  printf("From /proc/interrupts (after):\n");
  rc = system("grep 'eth' /proc/interrupts");
#endif /* HAVE_PROC_FILE_INFO */

#ifdef CALL_STATS
    if (options.call_stats) {
      call_stats_print();
    }
#endif /* CALL_STATS */
	printf("STATS DONE\n");

  delete_pidfile();

  free_aligned_all();  // should be last

	/* Just let someone know if there was a failure or not */
	exit_notification();
} /* cleanup */

/* -------------------------------------------------------------------- */
void
sig_crash_handler(void)
{
  struct sigaction act, old;

  printf("%s: Caught signal SIGSEGV or SIGBUS\n", prog_name);
  PRINT_TIME(NOFD, &tnow, &tprev, "Caught SIGSEGV or SIGBUS");
  fprintf(stderr, "%s: Caught SIGSEGV or SIGBUS signal\n", prog_name);
  fflush(stdout);
  fflush(stderr);
  printf("EXITING by restarting\n");


  printf("Setting handlers to default\n");
  act.sa_handler = SIG_DFL;
  sigemptyset(&act.sa_mask);

  /*  allow system calls to be restarted */
  act.sa_flags = SA_RESTART;

  if (sigaction(SIGSEGV, &act, &old) < 0) {
    perror("sigaction SIGSEGV call failed");
    exit(1);
  }

  if (sigaction(SIGBUS, &act, &old) < 0) {
    perror("sigaction SIGBUS call failed");
    exit(1);
  }
  printf("Returning from handler\n");

}

/* -------------------------------------------------------------------- */
void
sig_usr1_handler(void)
{
  MSG_MASK = 0xffffffff;
  printf("Caught signal SIGUSR1\n");
  PRINT_TIME(NOFD, &tnow, &tprev, "Caught SIGUSR1");
  // print_all();
  
}


/* -------------------------------------------------------------------- */
void
sig_usr2_handler(void)
{
  printf("Caught signal SIGUSR2\n");
  PRINT_TIME(NOFD, &tnow, &tprev, "Caught SIGUSR2");
  options.interactive = 1;
  printf("Interactive mode now set - wait for command prompt\n");
}

/* -------------------------------------------------------------------- */
void
sig_other_handler(int signum)
{
  PRINT_TIME(NOFD, &tnow, &tprev, "Caught signal");
  printf("Caught signal = %d\n", signum);
  fflush(stdout);
  fflush(stderr);
}

/* -------------------------------------------------------------------- */
void
sig_pipe_handler(void)
{
  PRINT_TIME(NOFD, &tnow, &tprev, "Caught SIGPIPE");
  printf("Caught signal SIGPIPE\n");
  num_sigpipes++;
  fflush(stdout);
  fflush(stderr);
}

/* -------------------------------------------------------------------- */
void
sig_urg_handler(void)
{
  PRINT_TIME(NOFD, &tnow, &tprev, "Caught SIGURG");
  printf("Caught signal SIGURG\n");
  num_sigurg++;
  fflush(stdout);
  fflush(stderr);
}


/* -------------------------------------------------------------------- */
void
install_sig_handlers(void)
{
  int i;
  struct sigaction act, old;

  act.sa_handler = (void (*)(int)) sig_urg_handler;
  sigemptyset(&act.sa_mask);

  /*  allow system calls to be restarted */
  act.sa_flags = SA_RESTART;

  if (sigaction(SIGURG, &act, &old) < 0) {
    perror("sigaction SIGURG call failed");
    exit(1);
  }
  printf("Installed sigurg handler\n");

  if (options.count_sigpipes) {
    act.sa_handler = (void (*)(int)) sig_pipe_handler;
    sigemptyset(&act.sa_mask);

    /*  allow system calls to be restarted */
    act.sa_flags = SA_RESTART;

    if (sigaction(SIGPIPE, &act, &old) < 0) {
      perror("sigaction SIGPIPE call failed");
      exit(1);
    }
    printf("Installed sigpipe handler\n");
  } else {
    /* Don't disturb just because a TCP connection closed on us... */
    signal (SIGPIPE, SIG_IGN);
    printf("Ignoring sigpipes\n");
  }

  DEBG(MSG_SIGNALS, "install_sig_handlers: entered\n");

  act.sa_handler = (void (*)(int)) sig_crash_handler;
  sigemptyset(&act.sa_mask);

  /*  allow system calls to be restarted */
  act.sa_flags = SA_RESTART;

  if (sigaction(SIGSEGV, &act, &old) < 0) {
    perror("sigaction SIGSEGV call failed");
    exit(1);
  }

  if (sigaction(SIGBUS, &act, &old) < 0) {
    perror("sigaction SIGBUS call failed");
    exit(1);
  }

  if (options.get_connections == OPT_CONN_WITH_SIGIO) {
    DEBG(MSG_SIGNALS, "install_sig_handlers: installing sigio_handler\n");
    /* set up the handler mask and flags */
    act.sa_handler = (void (*)(int)) sigio_handler;

    /* block SIGIO's while in signal handler */
    /* handle race condition later */
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGIO);
    print_mask("act.sa_mask should contain only SIGIO:\n", &act.sa_mask);

    /*  allow system calls to be restarted */
#ifdef __CYGWIN__
    /* cygwin doesn't do siginfo that I know of yet */
    act.sa_flags = SA_RESTART;
#else
    act.sa_flags = SA_RESTART | SA_SIGINFO;
#endif

    if (sigaction(SIGIO, &act, &old) < 0) {
      perror("sigaction SIGIO call failed");
      exit(1);
    }
  }


  act.sa_handler = (void (*)(int)) sig_usr2_handler;
  sigemptyset(&act.sa_mask);
  sigaddset(&act.sa_mask, SIGUSR2);
  print_mask("act.sa_mask should contain only SIGUSR2:\n", &act.sa_mask);

  /*  allow system calls to be restarted */
  act.sa_flags = SA_RESTART;

  if (sigaction(SIGUSR2, &act, &old) < 0) {
    perror("sigaction SIGUSR2 call failed");
    exit(1);
  }


  act.sa_handler = (void (*)(int)) sig_usr1_handler;
  sigemptyset(&act.sa_mask);
  sigaddset(&act.sa_mask, SIGUSR1);
  print_mask("act.sa_mask should contain only SIGUSR1:\n", &act.sa_mask);

  /*  allow system calls to be restarted */
  act.sa_flags = SA_RESTART;

  if (sigaction(SIGUSR1, &act, &old) < 0) {
    perror("sigaction SIGUSR1 call failed");
    exit(1);
  }

  act.sa_handler = (void (*)(int)) sig_kill_handler;
  sigemptyset(&act.sa_mask);
  sigaddset(&act.sa_mask, SIGINT);
  print_mask("act.sa_mask should contain only SIGINT:\n", &act.sa_mask);

  /*  allow system calls to be restarted */
  act.sa_flags = SA_RESTART;

  if (sigaction(SIGINT, &act, &old) < 0) {
    perror("sigaction SIGINT call failed");
    exit(1);
  }

  if (sigaction(SIGHUP, &act, &old) < 0) {
    perror("sigaction SIGHUP call failed");
    exit(1);
  }

  if (sigaction(SIGTERM, &act, &old) < 0) {
    perror("sigaction SIGTERM call failed");
    exit(1);
  }

  print_cur_mask("mask after installing signal handlers:\n");

  for (i=1; i<NSIG; i++) {
    switch (i) {

      case SIGQUIT:
      case SIGABRT:
      case SIGALRM:
      case SIGPROF:
      case SIGKILL:
      case SIGSTOP:
      case SIGTSTP:
      case SIGCONT:
#if !defined(SUNOS) && !defined(HPUX) && \
    !defined(__CYGWIN__) && !defined(FreeBSD) && !defined(DARWIN)
      case SIGSTKFLT:
#endif
	/* do nothing */
	DEBG(MSG_SIGNALS, "%2d let this signal through or handle "
	    "it elsewhere\n", i);
	break;

      case SIGCHLD: /* For now something weird has happened if we get this */
      case SIGINT:
      case SIGHUP:
      case SIGTERM:
	DEBG(MSG_SIGNALS, "%2d already installed sig_kill_handler\n", i);
	break;

      case SIGUSR2:
	DEBG(MSG_SIGNALS, "%2d already installed sig_usr2_handler\n", i);
	break;

      case SIGUSR1:
	DEBG(MSG_SIGNALS, "%2d already installed sig_usr1_handler\n", i);
	break;

      case SIGIO:
	DEBG(MSG_SIGNALS, "%2d already installed sigio_handler\n", i);
	break;

      case SIGFPE:
      case SIGSEGV:
      case SIGBUS:
	DEBG(MSG_SIGNALS, "%2d already installed sig_crash_handler\n", i);
	break;

      case SIGPIPE:
	DEBG(MSG_SIGNALS, "%2d already installed sig_pipe_handler\n", i);
	break;

      case SIGURG:
	DEBG(MSG_SIGNALS, "%2d already installed sig_urg_handler\n", i);
	break;

      default :
	DEBG(MSG_SIGNALS, "Installing handler for %d\n", i);
	act.sa_handler = (void (*)(int)) sig_other_handler;
	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask, i);

	/*  allow system calls to be restarted */
        act.sa_flags = SA_RESTART;

	if (sigaction(i, &act, &old) < 0) {
	  DEBG(MSG_SIGNALS, "install_others: sigaction call "
	      "failed on signal = %d\n", i);
	}
	break;

    } /* switch */
      
  } /* for */

  DEBG(MSG_SIGNALS, "install_sig_handler: done\n");
}


/* -------------------------------------------------------------------- */
void
print_all(void)
{
  extern void printCache();
  extern int my_proc_id;
  
  printf("print_all: my proc_id = %d\n",my_proc_id);

#ifdef FIXHUP
  print_app_info();
#endif

  interest_set_print_info();

#ifdef SEND
  if (options.send_events) {
#ifdef ARRAY_OF_BUFS
    print_all_evts_array();
#else
    print_all_evts();
#endif /* ARRAY_OF_BUFS */
  }
#endif /* SEND */
  stats_print();
  print_syscall_info();
  intr_print_info();
#ifdef HAVE_AIO_LAYER
  aio_get_stats(stdout, NULL, NULL, 0);
#endif

#ifdef TRACE_ON
  evt_print_readable();
#endif /* TRACE_ON */
  if (options.caching_on && options.cache_table_print) {
    printCache();
  }
  fflush(stdout);
  fflush(stderr);

}

/* -------------------------------------------------------------------- */
void
print_pending(void)
{
  sigset_t s;
  sigfillset(&s);
  /* sigemptyset(&s); */
  /* sigaddset(&s, SIGIO); */
  if (sigpending(&s) < 0) {
    perror("sigpending ");
    exit(1);
  }
  print_mask("pending interrupts\n", &s);

}

/* -------------------------------------------------------------------- */
int
sigio_pending(void)
{
  sigset_t s;
  sigemptyset(&s);
  sigaddset(&s, SIGIO);
  if (sigpending(&s) < 0) {
    perror("sigpending ");
    exit(1);
  }
  if (sigismember(&s, SIGIO)) {
    return(1);
  } else {
    return(0);
  }
}

/* -------------------------------------------------------------------- */
void
exit_notification()
{
	if (!exit_due_to_signal) {
		fprintf(stdout, "UNEXPECTED EXIT CALL: SHUTTING SERVER DOWN\n");
		fprintf(stderr, "UNEXPECTED EXIT CALL: SHUTTING SERVER DOWN\n");
		fflush(stdout);
		fflush(stderr);
	}
}
