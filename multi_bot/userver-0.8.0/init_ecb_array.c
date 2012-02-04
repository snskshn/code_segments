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
#ifdef ARRAY_OF_BUFS

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

#define FAIL_ON_MISMATCH        (1)
#define DONT_FAIL_ON_MISMATCH   (0)

#define EVT_BUF_MIN_SIZE        (50)

static char *saved_stack_addr = 0;
static char *stack = NULL;
static int  evt_buf_elements[EVT_BUFS_MAX];

char *
allocate_stack(int stack_size, int zero_fd)
{
  char *stack;

  stack = (char *) mmap(0, STACK_SIZE,
	  PROT_READ|PROT_WRITE|PROT_EXEC,
	  MAP_PRIVATE,
	  zero_fd, 0);

  if (stack == MAP_FAILED) {
    printf("allocate_stack: - no memory\n");
    exit(1);
  }

  printf("allocate_stack: stack size = %d\n", STACK_SIZE);

  saved_stack_addr = stack;
  *saved_stack_addr = 0xbe;

  DEBG(MSG_SEND, "Got stack: %p\n", stack);
  DEBG(MSG_SEND, "Saved stack addr = %p\n", saved_stack_addr);
  DEBG(MSG_SEND, "      value at stack addr = 0x%x\n", *saved_stack_addr);
   
  return(stack);
}

int
round_to_page_size(int size)
{
  int ret_val = size;
  if (size % PAGE_SIZE != 0) {
     ret_val = (((size / PAGE_SIZE) * PAGE_SIZE) + PAGE_SIZE);
  }

  return(ret_val);
}


void
init_evt_control(void)
{
  int ret;
  int group;

  if (options.auto_accept) {
    group = AUTO_ACCEPT_GRP_NUM;
    ret = evtctl(EVT_IPACCEPT, SEVTGROUP, &group, sizeof(int));
    if (ret < 0) {
      printf("init_evt_control: bad SEVTGROUP %i %s\n", 
	  errno, strerror(errno));
      printf("init_evt_control: ret = %d\n", ret);
      exit(-1);
    }
  } 

  group = SIG_GRP_NUM;
  ret = evtctl(EVT_SIG, SEVTGROUP, &group, sizeof(int));
  if (ret < 0) {
    printf("init_evt_control: bad SEVTGROUP: %i %s\n", 
	errno, strerror(errno));
    printf("init_evt_control: ret = %d\n", ret);
    exit(-1);
  }

  group = MSG_GRP_NUM;
  ret = evtctl(EVT_MSG, SEVTGROUP, &group, sizeof(int));
  if (ret < 0) {
    printf("init_evt_control: bad SEVTGROUP: %i %s\n", 
	errno, strerror(errno));
    printf("init_evt_control: ret = %d\n", ret);
    exit(-1);
  }

  /* Define critical section */
  ret = evtctl(EVT_ANY, SEVTCRITCODE, start_critical, 
	     (int) end_critical- (int) start_critical);
  if (ret < 0) {
    printf("init_evt_control: bad SEVTCRITCODE: %i %s\n", 
	errno, strerror(errno));
    printf("init_evt_control: ret = %d\n", ret);
    exit(-1);
  }


#ifdef KDEBUG
  if (evtctl(EVT_ANY, SEVTDEBUG, (void *) debug_level, 
      sizeof(debug_level)) < 0) {
    printf("init_evt_control: evtctl call failed\n");
    exit(1);
  }
#endif /* KDEBUG */
}


void
init_send_sig(void)
{
  int i;
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
}

static inline _syscall3(int, send_init, struct event_control*, buf,
    int, sizeof_fd_set, int, fail_on_mismatch);
static inline _syscall3(int, send_init_buf, struct event_control*, 
    ecbptr, struct event*, evtbuf, int, elements);

int 
init_ecb_array(void)
{

#ifdef KDEBUG
  int debug_level = options.send_kdebug;
#endif /* KDEBUG */

  int ret;
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
  DEBG(MSG_SEND, "init_ecb_array: zero_fd = %d\n", zero_fd);

  size = sizeof(struct event_control);
  alt_size = round_to_page_size(size);

  ecb = (struct event_control *) mmap(0, alt_size,
	PROT_READ|PROT_WRITE|PROT_EXEC, MAP_PRIVATE, zero_fd, 0);

  if (ecb == MAP_FAILED) {
    printf("init_ecb_array: - no memory\n");
    exit(1);
  }

  printf("init_ecb_array: ecb size = %d\n", alt_size);
  printf("init_ecb_array: ecb = %p\n", ecb);
  DEBG(MSG_SEND, "ecb = %p\n", ecb);

  stack = allocate_stack(STACK_SIZE, zero_fd);

  printf("init_ecb_array: stack top = %p\n", stack);
  stack+= STACK_SIZE - 4;
  printf("init_ecb_array: stack bottom = %p\n", stack);
  printf("init_ecb_array: sizeof(fd_set) = %d\n", sizeof(fd_set));

  ret = send_init(ecb, sizeof(fd_set), FAIL_ON_MISMATCH);
  if (ret < 0) {
    printf("init_ecb_array: send_init failed %i %s\n", errno, strerror(errno));
    printf("init_ecb_array: ret = %d\n", ret);
    printf("init_ecb_array: errno = %d\n", errno);
    if (errno == ENOSYS) {
	    printf("send_init: system call not supported in this kernel\n");
    }
    exit(-1);
  }

  if (options.ecb_size != OPT_DEFAULT_ECB_SIZE) {
    num_evts = options.ecb_size;
  } else {
    num_evts = maxconns + 1;
    /* num_evts = max_fds + 1; */
  }
  DEBG(MSG_SEND, "init_ecb_array: num_evts = %d\n", num_evts);
  DEBG(MSG_SEND, "init_ecb_array: maxconns = %d\n", maxconns);
  DEBG(MSG_SEND, "init_ecb_array: sizeof(struct event) = %d\n",
      sizeof(struct event));

  /* 0'th element */
  evt_buf_elements[EVT_DEFAULT_BUF] = 100;
  /* 1'st element */
  /* evt_buf_elements[EVT_ACCEPT_BUF] = num_evts; */
  evt_buf_elements[EVT_ACCEPT_BUF] = num_evts;
  /* 2'nd element */
  evt_buf_elements[EVT_IO_BUF] = num_evts;

  for (i=0; i<EVT_BUFS; i++) {

    /* for now all event buffers are the same size
     * in general we will not want this to be the case
     * as for some it will be sufficient if they are small
     */

    if (evt_buf_elements[i] < EVT_BUF_MIN_SIZE) {
      printf("init_ecb_array: setting buffer %d to minimum size = %d\n", 
	      i, EVT_BUF_MIN_SIZE);
      evt_buf_elements[i] = EVT_BUF_MIN_SIZE;
    }

    size = evt_buf_elements[i] * sizeof(struct event);
    DEBG(MSG_SEND, "init_ecb_array: sizeof(struct event) = %d "
       "elements = %d\n", sizeof(struct event), evt_buf_elements[i]);
    DEBG(MSG_SEND, "init_ecb_array: ecb array size = %d\n", size);

    /* round size to next highest page size */
    alt_size = round_to_page_size(size);
    DEBG(MSG_SEND, "rounded up ecb alt_size = %d\n", alt_size);
    assert(alt_size % PAGE_SIZE == 0);

    ecb->eventbuf[i] = (struct event *) mmap(0, alt_size,
	   PROT_READ|PROT_WRITE|PROT_EXEC, MAP_PRIVATE, zero_fd, 0);

    if (ecb->eventbuf[i] == MAP_FAILED) {
      printf("init_ecb_array: - no memory for eventbuf %d\n", i);
      exit(1);
    }

#define PRETOUCH
#ifdef PRETOUCH
    { int k; int dummy;
    for (k=0; k<ecb->event_list_size[i]; k++) {
      dummy = ecb->eventbuf[i][k].type;
    }
    }
#endif /* PRETOUCH */

    ret = send_init_buf(ecb, (struct event *) ecb->eventbuf[i], 
	evt_buf_elements[i]);
    if (ret < 0) {
      printf("send_init_buf failed ret = %d errno = %d\n", ret, errno);
      exit(1);
    }
    assert(ret == i);

    printf("init_ecb_array: ecb->eventbuf[%d] = %p\n", i, ecb->eventbuf[i]);
    printf("init_ecb_array: ecb->event_list_size[%d] = %d\n",
	    i, ecb->event_list_size[i]);
    printf("init_ecb_array: Array %d bytes = %d\n",
	    i, (ecb->event_list_size[i] * sizeof(struct event)));
    printf("init_ecb_array: &ecb->eventbuf[%d][%d] = %p\n", 
	    i, (ecb->event_list_size[i] - 1), 
	    &ecb->eventbuf[i][(ecb->event_list_size[i]-1)]);
    printf("init_ecb_array: ecb->head[%d] = %d\n", i, ecb->head[i]);
    printf("init_ecb_array: ecb->tail[%d] = %d\n", i, ecb->tail[i]);

    ecb->threshold[i] = 0;

    /* TODO: still need to set fdset info */
  } /* for */

  init_evt_control();

  /* Set stack for event handler */
  ecb->stack = stack;
  printf("init_ecb_array: ECB: notify:%08x pid:%i\n",
      (u32) ecb->notify, getpid());
  printf("init_ecb_array: ECB: ecb: %08x stack: %08x\n",
	    (unsigned) ecb, (u32) ecb->stack);

  if (ecb->stack == NULL) {
    printf("init_ecb_array: Bad ECB stack %p\n", 
	__builtin_return_address(0));
    exit(-1);
  }

  ecb->queue = 0;
  ecb->notify = 0;
  ecb->handler = &send_handler;
  ecb->no_block = 0;
  ecb->threshold = 0;

#ifdef SEND_AUTO_ACCEPT_CONTROL
  /*
  ecb->aperture = maxconns;
  */
  ecb->aperture = options.auto_accept_aperature;
  ecb->completed = 0;
#endif /* SEND_AUTO_ACCEPT_CONTROL */

  init_send_sig();

  return 0;
}

#endif /* ARRAY_OF_BUFS */
#endif /* SEND */
