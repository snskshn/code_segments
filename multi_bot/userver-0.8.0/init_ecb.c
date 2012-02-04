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


#ifdef SEND
#ifndef ARRAY_OF_BUFS

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <asm/unistd.h>

#include "debug.h"
#include "options.h"
#include "send.h"


/* #define KDEBUG */

static char *saved_stack_addr = 0;
static char *stack = NULL;

#ifdef TBB_KINFO
struct kinfo_struct *kinfo = 0;
static inline _syscall3(int, kinfo_init, struct kinfo_struct*,
    buf, int, len, int, fd);
#endif /*  TBB_KINFO */

int 
init_ecb(void)
{

#ifdef KDEBUG
  int debug_level = options.send_kdebug;
#endif /* KDEBUG */

  int ret;
  int group = 1;
  int size = 0;
  int alt_size = 0;
  int i = 0;
  int zero_fd;
  int num_evts = 0;
  
  zero_fd = open("/dev/zero", O_RDONLY);
  if (zero_fd < 0) {
    perror("open /dev/zero");
    exit(-1);
  }
  DEBG(MSG_SEND, "init_ecb: zero_fd = %d\n", zero_fd);

  if (options.ecb_size != OPT_DEFAULT_ECB_SIZE) {
    num_evts = options.ecb_size;
  } else {
    num_evts = maxconns + 1;
    /* num_evts = max_fds + 1; */
  }
  size = ((num_evts) * sizeof(struct event)) + sizeof(struct event_control);
  DEBG(MSG_SEND, "init_ecb: num_evts = %d\n", num_evts);
  DEBG(MSG_SEND, "init_ecb: maxconns = %d\n", maxconns);
  DEBG(MSG_SEND, "init_ecb: sizeof(struct event) = %d\n", 
      sizeof(struct event));
  DEBG(MSG_SEND, "init_ecb: sizeof(struct event_control) = %d\n", 
                  sizeof(struct event_control));
  DEBG(MSG_SEND, "init_ecb: ecb size = %d\n", size);

  /* round size to next highest page size */
  if (size % PAGE_SIZE != 0) {
    alt_size = ((size / PAGE_SIZE) * PAGE_SIZE) + PAGE_SIZE;
  }
  DEBG(MSG_SEND, "rounded up ecb alt_size = %d\n", alt_size);
  assert(alt_size % PAGE_SIZE == 0);

  ecb = (struct event_control *) mmap(0, alt_size,
	PROT_READ|PROT_WRITE|PROT_EXEC, MAP_PRIVATE, zero_fd, 0);

  if (ecb == MAP_FAILED) {
    printf("init_ecb: - no memory\n");
    exit(1);
  }

  printf("init_ecb: ecb size = %d\n", alt_size);

  printf("init_ecb: ecb = %p\n", ecb);
  DEBG(MSG_SEND, "ecb = %p\n", ecb);

  stack = (char *) mmap(0, STACK_SIZE,
	  PROT_READ|PROT_WRITE|PROT_EXEC,
	  MAP_PRIVATE,
	  zero_fd, 0);

  if (stack == MAP_FAILED) {
    printf("init_ecb - no memory\n");
    exit(1);
  }

  printf("init_ecb: stack size = %d\n", STACK_SIZE);

  saved_stack_addr = stack;
  *saved_stack_addr = 0xbe;

  DEBG(MSG_SEND, "Got stack: %p\n", stack);
  DEBG(MSG_SEND, "Saved stack addr = %p\n", saved_stack_addr);
  DEBG(MSG_SEND, "      value at stack addr = 0x%x\n", *saved_stack_addr);
  printf("init_ecb: stack top = %p\n", stack);
  stack+= STACK_SIZE - 4; /* 4092; */
  printf("init_ecb: stack bottom = %p\n", stack);

  ret = init_event(ecb, size);
  if (ret < 0) {
    printf("init_ecb: bad init_event %i %s\n", errno, strerror(errno));
    printf("init_ecb: ret = %d\n", ret);
    if (errno == ENOSYS) {
      printf("init_ecb: system call not supported in this kernel\n");
    }
    exit(-1);
  }
  printf("init_ecb: ecb->eventbuf[0] = %p\n", &ecb->eventbuf[0]);
  printf("init_ecb: ecb->eventbuf[ecb->event_list_size-1] = %p\n",
      &ecb->eventbuf[ecb->event_list_size-1]);

  /*NEW */
  for(i=0; i<ecb->event_list_size; ++i) {
    /* ecb->eventbuf[i].type = (u32)-1; */
  }

  /* DEBG(MSG_SEND, "init_ecb: calling print_ecb_types\n"); */
  /* print_ecb_types(); */


  if (options.auto_accept) {
    group = AUTO_ACCEPT_GRP_NUM;
    ret = evtctl(EVT_IPACCEPT, SEVTGROUP, &group, sizeof(int));
    if (ret < 0) {
      printf("init_ecb: bad SEVTGROUP %i %s\n", errno, strerror(errno));
      printf("init_ecb: ret = %d\n", ret);
      exit(-1);
    }
  } 


  group = SIG_GRP_NUM;
  ret = evtctl(EVT_SIG, SEVTGROUP, &group, sizeof(int));
  if (ret < 0) {
    printf("init_ecb: bad SEVTGROUP: %i %s\n", errno, strerror(errno));
    printf("init_ecb: ret = %d\n", ret);
    exit(-1);
  }

  group = MSG_GRP_NUM;
  ret = evtctl(EVT_MSG, SEVTGROUP, &group, sizeof(int));
  if (ret < 0) {
    printf("init_ecb: bad SEVTGROUP: %i %s\n", errno, strerror(errno));
    printf("init_ecb: ret = %d\n", ret);
    exit(-1);
  }

  /* Define critical section */
  ret = evtctl(EVT_ANY, SEVTCRITCODE, start_critical, 
	     (int) end_critical- (int) start_critical);
  if (ret < 0) {
    printf("init_ecb: bad SEVTCRITCODE: %i %s\n", errno, strerror(errno));
    printf("init_ecb: ret = %d\n", ret);
    exit(-1);
  }

  /* Set stack for event handler */
  ecb->stack = stack;
  printf("init_ecb: ECB: list_size:%i notify:%08x pid:%i\n",
	 ecb->event_list_size, (u32) ecb->notify, getpid());
  printf("init_ecb: ECB: ecb: %08x stack: %08x buf:%08x end_buf: %08x\n",
	    (unsigned) ecb, (u32) ecb->stack,
	    (unsigned)&ecb->eventbuf[0],
	    (unsigned)&ecb->eventbuf[ecb->event_list_size-1]);

  if (ecb->stack == NULL) {
    printf("init_ecb: Bad ECB stack %p\n", __builtin_return_address(0));
    exit(-1);
  }

#ifdef KDEBUG
  if (evtctl(EVT_ANY, SEVTDEBUG, (void *) debug_level, 
      sizeof(debug_level)) < 0) {
    printf("init_ecb: evtctl call failed\n");
    exit(1);
  }
#endif

  ecb->queue = 0;
  ecb->notify = 0;
  ecb->handler = &send_handler;
  ecb->no_block = 0;
  ecb->threshold = 0;

#ifdef SEND_AUTO_ACCEPT_CONTROL
  /*
  ecb->aperture = maxconns;
  */
  ecb->aperture = options.auto_accept_aperture;
  ecb->completed = 0;
#endif /* SEND_AUTO_ACCEPT_CONTROL */

  for (i=0; i<NSIG;  i++) {
    sigs[i] = NULL;
  } /* for */

  sigs[SIGUSR1]  = (sighandlerfn_t) send_sig_usr1_handler;
  /* For a while these we not generating core dumps so this was an attempt */
  /* sigs[SIGSEGV]  = (sighandlerfn_t) send_sig_crash_handler; */
  /* sigs[SIGBUS]   = (sighandlerfn_t) send_sig_crash_handler; */
  sigs[SIGUSR2]  = (sighandlerfn_t) send_sig_usr2_handler;
  /* sigs[20]    = handle_sigstop; */
  sigs[SIGIO]    = (sighandlerfn_t)(-1);
  /* Don't know where the SIGCHLD comes from but for now die if we get this */
  sigs[SIGCHLD]   = (sighandlerfn_t) send_sig_kill_handler;
  sigs[SIGINT]   = (sighandlerfn_t) send_sig_kill_handler;
  sigs[SIGHUP]   = (sighandlerfn_t) send_sig_kill_handler;
  sigs[SIGTERM]  = (sighandlerfn_t) send_sig_kill_handler;
  sigs[SIGPIPE]  = (sighandlerfn_t) send_sig_pipe_handler;
  /* sigs[SIGTRAP]    = (sighandlerfn_t)(-1); */
  /* sigs[SIGTRAP]  = (sighandlerfn_t) send_sig_crash_handler; */

  for (i=0; i<NSIG;  i++) {
    DEBG(MSG_SEND, "sigs[%d] = %p\n", i, sigs[i]);
  } /* for */
  return 0;
}

#ifdef ONE_LISTENER
#ifdef TBB_KINFO
void 
do_kinfo_init(int zero_fd)
{
  int ret = 0;
  int alt_size;
  int size = (sizeof(struct kinfo_struct));
  assert(zero_fd > 2);

  DEBG(MSG_SEND, "do_kinfo_init: size = %d\n", size);

  DEBG(MSG_SEND, "do_kinfo_init: zero_fd = %d\n", zero_fd);

  /* round size to next highest page size */
  if (size % PAGE_SIZE != 0) {
    alt_size = ((size / PAGE_SIZE) * PAGE_SIZE) + PAGE_SIZE;
  }
  DEBG(MSG_SEND, "rounded up kinfo alt_size = %d\n", alt_size);
  assert(alt_size % PAGE_SIZE == 0);

  kinfo = (struct kinfo_struct *) mmap(0, alt_size,
	PROT_READ|PROT_WRITE|PROT_EXEC,
	MAP_PRIVATE,
	zero_fd, 0);

  if (kinfo == MAP_FAILED) {
    printf("do_kinfo_init: - no memory\n");
    close(zero_fd);
    exit(1);
  }

  printf("do_kinfo_init: kinfo = %p size = %d\n", kinfo, alt_size);

  ret = kinfo_init(kinfo, size, server_sd);
  PRINT_TIME(server_sd, &tnow, &tprev, "do_kinfo_init: kinfo_init "
      "returns %d", ret);
  if (ret < 0) {
    printf("do_kinfo_init: bad kinfo_init %i %s\n", errno, strerror(errno));
    printf("do_kinfo_init: ret = %d\n", ret);
    close(zero_fd);
    exit(-1);
  }
}

int
pre_kinfo_init(void)
{
  int zero_fd;

  DEBG(MSG_SEND, "pre_kinfo_init: entered\n");
  zero_fd = open("/dev/zero", O_RDONLY);
  if (zero_fd < 0) {
    perror("open /dev/zero");
    exit(-1);
  }
  DEBG(MSG_SEND, "pre_kinfo_init: zero_fd = %d\n", zero_fd);
  return(zero_fd);
}
#endif /*  TBB_KINFO */
#endif /* ONE_LISTENER */

#endif /* ARRAY_OF_BUFS */
#endif /* SEND */
