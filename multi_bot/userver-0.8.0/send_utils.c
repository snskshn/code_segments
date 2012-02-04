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

#include <arpa/inet.h>
#include <sys/poll.h>

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "send.h"
#include "debug.h"
#include "stats.h"
#include "trace.h"
#include "send_utils.h"
#ifdef ARRAY_OF_BUFS
extern void send_print_info_array();
extern void print_ecb_types_array();
#endif /* ARRAY_OF_BUFS */

#ifndef ARRAY_OF_BUFS
void 
verify_evt(const struct event *evt)
{
  if ((evt < &ecb->eventbuf[0]) || 
      (evt > &ecb->eventbuf[ecb->event_list_size-1])) {
    printf("verify_evt: Bad event pointer: evt = %p\n", evt);
    printf("verify_evt: head = %d tail = %d\n", ecb->head, ecb->tail);
    printf("verify_evt: eventbuf[0] = %p eventbuf[size] = %p\n",
       &ecb->eventbuf[0], &ecb->eventbuf[ecb->event_list_size-1]);
  }
}
#endif /* ARRAY_OF_BUFS */

/* Print a set of register, probably before dying */
void
dump_regs(struct evt_regs *r)
{
  printf("dump_regs: r = %p\n", r);

  if ((r != (struct evt_regs *) 0xffffffff) && 
      (r != (struct evt_regs *) 0)) {
    printf("Regs at %p flags:%08x\n",r,r->flags);
    printf("Regs: eax:%08x ebx:%08x ecx:%08x edx:%08x\n",
	   (u32)r->eax,(u32)r->ebx,(u32)r->ecx,(u32)r->edx);
    printf("Regs: edi:%08x esi:%08x ebp:%08x eip:%08x\n",
	   (u32)r->edi,(u32)r->esi,(u32)r->ebp,(u32)r->eip);
  } else {
    printf("dump_regs: invalid argument to dump_regs = %p\n", r);
  }
}


void 
dump_stack(struct evt_regs *r)
{
  int *ptr = (int*)(r);
  int i = 0;
  int top = ((int)r) + STACK_SIZE;
  printf("Stack: %08x\n%08x:", (u32) ptr, ((int)ptr)&~15);

  for (i=0; i < (((int)ptr)&15) ; i+=4) {
    printf("         ");
  }

  i=0;
  while(((int)ptr) != top && i < 1024) {
    if (((int)ptr) % 16 == 0) {
      printf("\n%p:",ptr);
    }
    printf(" %08x",*ptr);
    ++ptr;
    ++i;
  }
  printf("\n");
  /* exit(0); */
} /* dump_stack */

void 
print_event(const struct event *evt, struct evt_regs* r)
{
  /*  spin_lock(&print_lock); */
  printf("\n");
  printf("         # Event:  type: %i  id: %08x (%d) evt: %p\n",
	evt->type,evt->event_id, evt->event_id, evt);

  switch(evt->type) {
    case EVT_SIG:
      printf("         # Signal event\n");
      printf("         # Signal is: %s\n", strsignal(evt->event_id));
      {
	int* x = (int*) evt;
	printf("         # %08x %08x %08x %08x %08x %08x %08x %08x\n",
	       x[0],x[1],x[2],x[3],x[4],x[5],x[6],x[7]);
	x = x + 8;
	printf("         # %08x %08x %08x %08x %08x %08x %08x %08x\n",
	       x[0],x[1],x[2],x[3],x[4],x[5],x[6],x[7]);
	x = x + 8;
	printf("         # %08x %08x %08x %08x %08x %08x %08x %08x\n",
	       x[0],x[1],x[2],x[3],x[4],x[5],x[6],x[7]);
      }
      break;

    case EVT_IOREADY:
      printf("         # IO-ready event: fd: %i events: %08x tag: %p\n",
	     evt->data.io.fd,evt->data.io.events,
	     evt->data.io.tag);
      printf("         # data_read = 0x%x  data_write = 0x%x\n", 
		      evt->data.io.data_read, evt->data.io.data_write);
      print_io_events(evt->data.io.events);
      break;

    case EVT_IODONE:
      printf("         # IO-done event\n");
      break;

    case EVT_CTX:
      printf("         # Context-switch event\n");
      break;

    case EVT_MSG:
      printf("         # Message event  sender: %i data: %p len: %i\n",
      evt->event_id,
      evt->data.msg.data,
      evt->data.msg.len);
      break;

    case EVT_PAGEOUT:
      printf("         # Page-out event\n");
      break;

    case EVT_PAGEFLUSH:
      printf("         # Page-flush event\n");
      break;

    case EVT_SEMBLOCK:
      printf("         # Sem-block event\n");
      break;

    case EVT_SEMUNBLOCK:
      printf("         # Sem-unblock event\n");
      break;

    case EVT_SYNCH:
      printf("         # Synch event rv: %i nb: %i %08x\n",
	      evt->data.synch.retval,evt->data.synch.numblocks,
	      evt->data.synch.tag);
      break;

    case EVT_IPACCEPT:
      printf("         # IP Accept event: fd: %i addr: %s "
	  "port: %i events: %p\n",
	   evt->data.ipa.fd,
	   inet_ntoa(evt->data.ipa.sin.sin_addr),
	   ntohs(evt->data.ipa.sin.sin_port),
	   (void*)evt->data.ipa.events);
      print_io_events(evt->data.io.events);
      break;

    case EVT_DISPATCH:
      printf("         # Dispatch event id: %i token: %p reason %08x\n",
	     evt->data.dispatch.call_id,
	     (void*)evt->data.dispatch.token,
	     evt->data.dispatch.tag);
      break;

    default:
    {
      int* x = (int*) evt;
      printf("Unrecognized event type\n");
      printf("         # %08x %08x %08x %08x %08x %08x %08x %08x\n",
	     x[0],x[1],x[2],x[3],x[4],x[5],x[6],x[7]);
      x = x + 8;
      printf("         # %08x %08x %08x %08x %08x %08x %08x %08x\n",
	     x[0],x[1],x[2],x[3],x[4],x[5],x[6],x[7]);
      x = x + 8;
      printf("         # %08x %08x %08x %08x %08x %08x %08x %08x\n",
	     x[0],x[1],x[2],x[3],x[4],x[5],x[6],x[7]);
      printf("Current regs\n");
      if ((r != (struct evt_regs *) 0) && 
	  (r != (struct evt_regs *) 0xffffffff)) {
        dump_regs(r);
        dump_stack(r);
      }
      fflush(stdout);

#ifdef ARRAY_OF_BUFS
      print_ecb_types_array();
#else
      print_ecb_types();
#endif /* ARRAY_OF_BUFS */
      printf("Now generating segfault\n");
      fflush(stdout);
      *((int*)0)=0;
      exit(1);
    }
  }

#ifdef ARRAY_OF_BUFS
  send_print_info_array();
#else
  send_print_info();
#endif /* ARRAY_OF_BUFS */
  fflush(stdout);
} /* print_event */


void
check_fd(int fd)
{
  static int streak_good = 1;
  static int streak_bad = 1;
  static int prev_good = -1;
  static int prev_bad = -1;
  static int prev_fd = 0;

  if (dup2(fd, fd) < 0) {
    if (errno == EBADF) {
      if (streak_good) {
        if (prev_good != -1) {
          printf("      check_fd: ...\n");
          printf("      check_fd: fd = %d is good\n", prev_good);
        }
        printf("  *** check_fd: fd = %d is bad\n", fd);
      } else if (fd == FD_SETSIZE) {
        printf("  *** check_fd: ...\n");
        printf("  *** check_fd: fd = %d is bad\n", fd);
      }
    } else {
      printf("      check_fd: dup2 failed on %d with errno = %d\n", 
	  fd, errno);
    }
    streak_bad = 1;
    streak_good = 0;
    prev_bad = fd;

  } else {
    if (streak_bad) {
      if (prev_bad != -1) {
        printf("  *** check_fd: ...\n");
        printf("  *** check_fd: fd = %d is bad\n", prev_bad);
      }
      printf("      check_fd: fd = %d is good\n", fd);
    } else if (fd == FD_SETSIZE) {
      printf("      check_fd: ...\n");
      printf("      check_fd: fd = %d is good\n", fd);
    }
    streak_good = 1;
    streak_bad = 0;
    prev_good = fd;
  }

  prev_fd = fd;
  fflush(stdout);
}

#ifndef ARRAY_OF_BUFS
void
print_ecb_types(void)
{
  register int i;

  for (i=0; i<ecb->event_list_size; ++i) {
    if (ecb->eventbuf[i].type!=-1) {
      if (ecb->eventbuf[i].type >= 0 &&
	  ecb->eventbuf[i].type <= 1000) {

	printf("[%i].type == %i\n", i,ecb->eventbuf[i].type);
      } else {
	printf("[%d].type == 0x%x\n", i,ecb->eventbuf[i].type);

      }
    }
  }
}
#endif /* ARRAY_OF_BUFS */

void
print_io_events(int events)
{
  if (events == 0) {
    printf("         # NO IO EVENTS\n");	
  }
  if (events & POLLIN) {
    printf("         # POLLIN\n");	
  }
  if (events & POLLPRI) {
    printf("         # POLLPRI\n");	
  }
  if (events & POLLOUT) {
    printf("         # POLLOUT\n");	
  }
  if (events & POLLRDNORM) {
    printf("         # POLLRDNORM\n");	
  }
  if (events & POLLRDBAND) {
    printf("         # POLLRDBAND\n");	
  }
  if (events & POLLWRNORM) {
    printf("         # POLLWRNORM\n");	
  }
  if (events & POLLWRBAND) {
    printf("         # POLLWRBAND\n");	
  }
  if (events & POLLMSG) {
    printf("         # POLLMSG\n");	
  }
  if (events & POLLERR) {
    printf("         # POLLERR\n");	
  }
  if (events & POLLHUP) {
    printf("         # POLLHUP\n");	
  }
  if (events & POLLNVAL) {
    printf("         # POLLNVAL\n");	
  }
  if (events & POLLHINT) {
    printf("         # POLLHINT\n");	
  }
  if (events & POLLREMOVE) {
    printf("         # POLLREMOVE\n");	
  }
  if (events & POLLFIN) {
    printf("         # POLLFIN\n");	
  }
} /* print_io_events */

#ifndef ARRAY_OF_BUFS
void 
print_all_evts(void)
{
  int i;
  struct event *evt = 0;

  if (ecb == 0) {
    return;
  }

  /* pause for a bit in case new events are still arriving */
  sleep(1);
  printf("Remaining events: head = %d tail = %d\n", 
      ecb->head, ecb->tail);

  for (i=(ecb->head)+1; i<ecb->tail; i++) {
    evt = (struct event *) &ecb->eventbuf[i % ecb->event_list_size];
    printf("index = %d (%d)\n", i, (i % ecb->event_list_size));
    print_event(evt, 0);
  } /* for */
}


void
send_print_info(void)
{
  if (options.send_events && ecb) {
    printf("ecb->queue = 0x%08x ecb->notify = 0x%08x\n",
       	ecb->queue, ecb->notify);
    printf("ecb->head = %d ecb->tail = %d\n",
       	ecb->head, ecb->tail);
    printf("ecb->threshold = %d ecb->num_queued = %d\n",
       	ecb->threshold, ecb->num_queued);
#ifdef TBB_KINFO
    if (kinfo) {
      printf("kinfo->qlen_young = %d kinfo->qlen = %d\n",
	  kinfo->qlen_young, kinfo->qlen);
    }
#endif /* TBB_KINFO */
  }
}
#endif /* ARRAY_OF_BUFS */

#endif /* SEND */
