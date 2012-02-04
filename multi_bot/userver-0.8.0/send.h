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


#ifndef SEND_H
#define SEND_H

/* Needed for linux/event.h */
#include <sys/types.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <netinet/in.h>
typedef unsigned int u32;
typedef unsigned long long u64;

#include <linux/event.h>
/* To get the definitions for the new system calls */
#include <asm-i386/unistd.h>
#include <errno.h>
#include "common.h"

#define PAGE_SIZE   (4096)
/* #define STACK_SIZE  (PAGE_SIZE * 20) */
#define STACK_SIZE  (PAGE_SIZE * 1000)

#define EVT_ACCEPT_BUF   (EVT_DEFAULT_BUF+1)
#define EVT_IO_BUF       (EVT_DEFAULT_BUF+2)
#define EVT_BUFS         (EVT_DEFAULT_BUF+3)

/* #define SIG_GRP_NUM         (10) */
/* #define AUTO_ACCEPT_GRP_NUM (10) */
/* #define IO_GRP_NUM          (20) */
/* #define ACCEPT_IO_GRP_NUM   (29) */
#define SIG_GRP_NUM         (0)
#define AUTO_ACCEPT_GRP_NUM (4)
#define IO_GRP_NUM          (8)
#define ACCEPT_IO_GRP_NUM   (12)
#define MSG_GRP_NUM         (16)
#define SIG_GRP             (0x1 << SIG_GRP_NUM)
#define AUTO_ACCEPT_GRP     (0x1 << AUTO_ACCEPT_GRP_NUM)
#define ACCEPT_IO_GRP       (0x1 << ACCEPT_IO_GRP_NUM)
#define IO_GRP              (0x1 << IO_GRP_NUM)
#define MSG_GRP             (0x1 << MSG_GRP_NUM)

#ifndef ARRAY_OF_BUFS
static inline _syscall2(int,init_event,struct event_control*,buf,int,len);
#endif /* ARRAY_OF_BUFS */
static inline _syscall4(int,evtctl,int,event,int,command,void*,data,int,len);

typedef void (*sighandlerfn_t)(int cpu, const struct event* evt);

extern int num_evts();
extern int free_ecb_slots();
extern void block_auto_accept_delivery();
extern void unblock_auto_accept_delivery();

#ifdef TBB_KINFO
extern void do_kinfo_init();
extern int pre_kinfo_init();
extern struct kinfo_struct *kinfo;
#endif /*  TBB_KINFO */


extern int init_ecb();
extern int send_prime_io(struct info *ip);
extern void send_print_info();
extern void send_release_queued_evts();
extern void print_ecb_types();
extern void start_critical();
extern void end_critical();
extern void send_async_setup(int sd);
extern void print_all_evts();
extern sighandlerfn_t send_sig_kill_handler();
extern sighandlerfn_t send_sig_usr1_handler();
extern sighandlerfn_t send_sig_usr2_handler();
extern sighandlerfn_t send_sig_crash_handler();
extern sighandlerfn_t send_sig_trap_handler();
extern sighandlerfn_t send_sig_pipe_handler();
extern void send_handler();
extern int send_intrs_to_handle;
extern int send_handler_evts_done;
extern int send_need_to_check_events;

extern void set_notify(int mode);
extern void set_queue(int mode);

extern struct event_control *ecb;
extern int evt_notify;
extern int evt_queue;
extern sighandlerfn_t sigs[];

/* From <asm/poll.h>
 * but including it conflicts with <sys/poll.h>
 */
#ifndef POLLREMOVE
#define POLLREMOVE      (0x1000)
#endif
#ifndef POLLHINT
#define POLLHINT        (0x2000)
#endif
#ifndef POLLFIN
#define POLLFIN         (0x4000)
#endif

/* NOTE: this has been picked so that it doesn't interfere with poll.h */
#define POLLREADRETRY       (0x8000)

#endif /* SEND_H */
