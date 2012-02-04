/*
    userver -- (pronounced you-server or micro-server).
    This file is part of the userver, a high-performance web server designed for
    performance experiments.

    Copyright (C) 2005-2010 Tim Brecht
    Based on the file originally Copyright (C) 2004  Hewlett-Packard Company

    Authors: Tim Brecht <brecht@cs.uwaterloo.ca>
             David Pariag <db2paria@cs.uwaterloo.ca>
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
#include <string.h>
#include <sys/stat.h>

#include "info.h"
#include "debug.h"
#include "options.h"
#include "lru.h"
#include "q.h"
#include "netstat.h"
#include "rusage.h"
#include "interest_set.h"
#include "sock_listener.h"
#include "sock_special.h"
#include "aio_layer.h"
#include "sigstuff.h"
#include "util.h"

#ifdef SEND
#include "send.h"
#endif /* SEND */

#define PROG_NAME_LEN  (100)

char prog_name[PROG_NAME_LEN + 1];

void print_extra_info();
void do_proc_info(char *proc_file);
void select_loop_init(void);
void poll_loop_init(void);
void send_loop_init(void);
#ifdef HAVE_EPOLL
void epoll_loop_init(void);
#endif /* HAVE_EPOLL */
void aio_loop_init(void);

#ifdef SEND
extern unsigned int send_readfds;
#endif /* SEND */

#ifdef CACHE_MAPPED_NEW
extern void cache_warm(const char *files, const char *docroot, int printcache);
#endif

int
main(int argc, char **argv)
{
  const char *ptr;
  int server_mode = -1;
  int i;
  int rc = 0;
  FILE *xmlf;

#if !defined(HPUX)
  setlinebuf(stdout);
#endif

  ptr = strrchr(argv[0], '/');
  if (ptr == NULL) {
    strncpy(prog_name, argv[0], PROG_NAME_LEN);
  } else {
    strncpy(prog_name, ptr + 1, PROG_NAME_LEN);
  }

  pagesize = getpagesize();
  printf("pagesize = %d\n", pagesize);
  cache_line_size = OPT_CACHE_LINE_EST;

#ifdef TBB_KINFO
  int kinfo_fd = 0;
#endif /* TBB_KINFO */
#ifdef TBB_KINFO
  if (options.kinfo) {
    kinfo_fd = pre_kinfo_init();
  }
#endif /* TBB_KINFO */

  init_print_time(&tnow, &tprev);

  /* must happen before processing arguments */
  options_set();

  /* Process command line arguments */
  process_args(argc, argv);

  /* Check for conflicts in the specified cmd line options */
  check_options();

	printf("INIT START\n");

  num_idle = maxconns;
  printf("main: num_idle = %d\n", num_idle);
  printf("main: maxconns = %d\n", maxconns);
  printf("main: max_fds = %d\n", max_fds);

#ifdef DEBUG_ON
  printf("Debugging is on debugging messages mask = 0x%x\n", MSG_MASK);
#else
  printf("Compiled without debugging\n");
#endif /* DEBUG_ON */

  if (options.use_socket_aio) {
    // for now leave some room for a few open files and
    // a few listening sockets (unfortunately we need to call this before
    // we know how many listening sockets there will be
    // max_sock_sd = maxconns + 20;

    // For now we just pick a value that _SHOULD_ avoid
    // collisions with future tests.
    // Note that if there were sd/fd collisions things
    // _should_ likely work just fine but this may
    // make finding problems easier - since there is
    // potential to control which range of values the
    // underlying aio layer is returning.

#ifdef HAVE_AIO_LAYER
    /* These are just estimates !!! */
    int aio_lowest_sock_sd = OPT_AIO_LOWEST_SD;
    int aio_highest_sock_sd = (maxconns + aio_lowest_sock_sd - 1) + 
                               OPT_AIO_MAX_LISTENERS;
    max_sock_sd = aio_highest_sock_sd;

    if (max_sock_sd > max_fds) {
      printf("main: max_sock_sd = %d > max_fds = %d\n", max_sock_sd, max_fds);
      exit(1);
    }

    printf("main: aio_lowest_sock_sd = %d\n", aio_lowest_sock_sd);
    printf("main: aio_highest_sock_sd = %d\n", aio_highest_sock_sd);
    PRINT_TIME(NOFD, &tnow, &tprev, "main: calling aio_sock_init with "
	"max_sock_sd = %d", max_sock_sd);
    rc = aio_sock_init(aio_lowest_sock_sd, aio_highest_sock_sd);
    if (rc < 0) {
      printf("main: aio_sock_init failed rc = %d\n", rc);
      exit(1);
    }
#endif /* HAVE_AIO_LAYER */

  } else {
    max_sock_sd = max_fds;
  }

  printf("main: max_sock_sd = %d\n", max_sock_sd);
  printf("main: FD_SETSIZE = %d\n", FD_SETSIZE);
  printf("main: sizeof(fd_set) = %d\n", (int) sizeof(fd_set));


#ifdef TBB_KINFO
  if (options.kinfo) {
    kinfo_fd = pre_kinfo_init();
  }
#endif /* TBB_KINFO */

  rc = info_init();


  info_listener_init();

  /* must happen after processing arguments */
  init_app_queues();
  create_pidfile();
  options_valid();
  options_print();

  xmlf = fopen("options.xml", "w");
  if (!xmlf) {
    printf("main: failed to create options.xml\n");
    exit(1);
  }
  options_print_xml(xmlf);
  fclose(xmlf);

#ifndef AIO_WORKAROUND_BUGS
  print_extra_info();
#endif /* AIO_WORKAROUND_BUGS */

#ifdef HAVE_NETSTAT
  netstat_init();
#endif /* HAVE_NETSTAT */

  sock_special_init();

  /* initialize the event dispatch mechanism */
  if (options.use_poll) {
    PRINT_TIME(NOFD, &tnow, &tprev, "userver: calling poll_loop_init");
    poll_loop_init();
#ifdef HAVE_EPOLL
  } else if (options.use_epoll || options.use_epoll2) {
    PRINT_TIME(NOFD, &tnow, &tprev, "userver: calling epoll_loop_init");
    epoll_loop_init();
#endif /* HAVE_EPOLL */
#ifdef HAVE_AIO_LAYER
  } else if (options.use_socket_aio) {
    PRINT_TIME(NOFD, &tnow, &tprev, "userver: calling aio_loop_init");
    aio_loop_init();
#endif /* HAVE_AIO_LAYER */
  } else if (!options.send_loop) {
    assert(options.send_loop == 0);
    PRINT_TIME(NOFD, &tnow, &tprev, "userver: calling select_loop_init");
    select_loop_init();
  } else {
#ifdef SEND
    assert(options.send_loop == 1);
    PRINT_TIME(NOFD, &tnow, &tprev, "userver: calling send_loop_init");
    send_loop_init();
#endif
  }

  switch (options.get_connections)
  {
    case OPT_CONN_WITH_SELECT_POLL_EPOLL:
      server_mode = LISTENER_NOT_ASYNC_INIT;
      break;

#ifdef SEND
    case OPT_CONN_WITH_SEND_SELECT:
      server_mode = LISTENER_NOT_ASYNC_INIT | LISTENER_SEND_INIT;
      break;
#endif /* SEND */
    
    case OPT_CONN_WITH_SIGIO:
      server_mode = LISTENER_DO_ASYNC_INIT;
      break;

    case OPT_CONN_WITH_SEND_EVTS:
      server_mode = LISTENER_SEND_INIT;
      break;

    case OPT_CONN_WITH_AIO_ACCEPT:
      server_mode = LISTENER_AIO_INIT;
      break;

    default :
      printf("%s: options.get_connections = %d not handled\n",
              argv[0], options.get_connections);
      exit(1);
      break;
  } /* switch */

  printf("Calling server_init with mode = 0x%x = %d\n", 
          server_mode, server_mode);
  server_init(server_mode);

  for (i=sock_listener_min; i<=sock_listener_max; i++) {
    if (sock_is_listener(i)) {
      printf("listen_sd = %d\n", i);
      if (i > max_sd) {
        max_sd = i;
      }
    }
  }

  switch (options.process_sds_order)
  {
      case OPT_PROCESS_SDS_LRU:
	lru_copy_init();
	break;
    
      case OPT_PROCESS_SDS_LIFO:
        q_init(max_fds+1);
	if (options.get_connections == OPT_CONN_WITH_SELECT_POLL_EPOLL) {
	  for (i=sock_listener_min; i<=sock_listener_max; i++) {
	    if (sock_is_listener(i)) {
	      q_add_to_front(i);
	    }
	  }
	}
	break;

      case OPT_PROCESS_SDS_FIFO:
        q_init(max_fds+1);
	if (options.get_connections == OPT_CONN_WITH_SELECT_POLL_EPOLL) {
	  for (i=sock_listener_min; i<=sock_listener_max; i++) {
	    if (sock_is_listener(i)) {
	      q_add_to_rear(i);
	    }
	  }
	}
	break;

      default :
	/* do nothing */
	break;
  }

  if (options.caching_on) {
    initCache(options.cache_table_size, options.cache_max_bytes, 
              options.cache_max_file_size, options.cache_max_load_factor, 
              options.cache_lock_pages);
#ifdef CACHE_MAPPED_NEW
      if (options.cache_warm) {
        cache_warm(options.cache_warm_file,
                   (options.doc_root[0] != '\0') ? options.doc_root : NULL,
                   options.cache_table_print);
       }
#endif
  }

  rusage_init();

  fork_servers(numprocs);
  return 0;
}

/* ---------------------------------------------------------------------- */
void 
print_extra_info()
{
  int rc = 0;
#ifdef HAVE_IFCONFIG
  printf("ifconfig output:\n");
  fflush(stdout);
  fflush(stderr);
  rc = system("/sbin/ifconfig");
  fflush(stdout);
  fflush(stderr);
  printf("-----------------------------------------------\n");
#endif /* HAVE_IFCONFIG */

#ifdef HAVE_PROC_FILE_INFO
  do_proc_info("/proc/version");
  do_proc_info("/proc/sys/kernel/rtsig-max");
  do_proc_info("/proc/sys/fs/file-max");
  do_proc_info("/proc/sys/net/ipv4/tcp_max_syn_backlog");
  do_proc_info("/proc/sys/net/ipv4/ip_local_port_range");
  do_proc_info("/proc/sys/net/ipv4/tcp_tw_recycle");
  do_proc_info("/proc/sys/net/ipv4/tcp_max_tw_buckets");
  do_proc_info("/proc/sys/net/ipv4/tcp_fin_timeout");
  do_proc_info("/proc/sys/net/ipv4/tcp_abort_on_overflow");
  do_proc_info("/proc/sys/net/core/somaxconn");

  fflush(stdout);
  fflush(stderr);
  printf("From /proc/cpuinfo:\n");
  fflush(stdout);
  fflush(stderr);
  rc = system("egrep 'model name|processor|MHz|cache|_id' /proc/cpuinfo");
  fflush(stdout);
  fflush(stderr);

  printf("From /proc/interrupts (before):\n");
  rc = system("grep 'eth' /proc/interrupts");

#endif /* HAVE_PROC_FILE_INFO */

#ifdef HAVE_LIMITS
  printf("-----------------------------------------------\n");
  printf("Limits:\n");
  fflush(stdout);
  fflush(stderr);
  rc = system("csh -c limit");
  fflush(stdout);
  fflush(stderr);
  printf("-----------------------------------------------\n");
  fflush(stdout);
  fflush(stderr);
#endif /* HAVE_LIMITS */
}
/* ---------------------------------------------------------------------- */
void
do_proc_info(char *proc_file)
{
  char cmd[200];
  struct stat st;
  int rc;

  rc = stat(proc_file, &st);
  if (rc == 0) {
    printf("%s   ", proc_file);
    /* Interestingly these are required in order to get proper output */
    fflush(stdout);
    fflush(stderr);
    sprintf(cmd, "cat %s", proc_file);
    rc = system(cmd);
    fflush(stdout);
    fflush(stderr);
  }
}

/* ---------------------------------------------------------------------- */

void
create_pidfile()
{
  FILE *fp;

  printf("%s pid = %d\n", prog_name, (int) getpid());

  if (options.pid_filename[0] != '\0') {
    fp = fopen(options.pid_filename, "w");
    if (fp == NULL) {
      perror("create_pidfile: fopen failed");
      exit(1);
    }
    fprintf(fp, "%d\n", (int) getpid());
    fclose(fp);
  }
}

/* ---------------------------------------------------------------------- */

void
delete_pidfile()
{
  if (options.pid_filename[0] != '\0') {
    unlink(options.pid_filename);
  }
}
