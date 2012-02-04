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


#ifdef PROVIDE_EPOLL_HEADERS


#include <linux/unistd.h>
#include <asm/unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

#include "sys_epoll.h"

#ifndef IA64
int epoll_create(int size);
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
long epoll_wait(int epfd, struct epoll_event * events, int maxevents, int timeout);

_syscall1(int, epoll_create, int, size);
_syscall4(int, epoll_ctl, int, epfd, int, op, int, fd, struct epoll_event *, events);
_syscall4(long, epoll_wait, int, epfd, struct epoll_event *, events, int, maxevents, int, msec_timeout);

long 
sys_epoll_create(int size)
{
  return(epoll_create(size));
}

long 
sys_epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)
{
  
  return(epoll_ctl(epfd, op, fd, event));
  
}

long
sys_epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout)
{
  return(epoll_wait(epfd, events, maxevents, timeout));
}

#ifdef HAVE_EPOLL_CTLV
static inline _syscall4(int, epoll_ctlv, int, epfd, int, op,
    int, count, struct epoll_event_ctlv *, events);

long
sys_epoll_ctlv(int epfd, int op, int count, struct epoll_event_ctlv *event)
{
  return(epoll_ctlv(epfd, op, count, event));
}
#endif /* HAVE_EPOLL_CTLV */

#else
#ifndef EPOLL_ASM
long __ia64_syscall (long a0, long a1, long a2, long a3, long a4, long nr);

long
sys_epoll_create(int size)
{
  long a0 = size;
  long a1 = 0;
  long a2 = 0;
  long a3 = 0;
  long a4 = 0;
  long call_num = __NR_epoll_create;
  return(__ia64_syscall(a0, a1, a2, a3, a4, call_num));
}

long 
sys_epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)
{
  long a0 = (long) epfd;
  long a1 = (long) op;
  long a2 = (long) fd;
  long a3 = (long) event;
  long a4 = 0;
  long call_num = __NR_epoll_ctl;
  return(__ia64_syscall(a0, a1, a2, a3, a4, call_num));
}

long
sys_epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout)
{
  long a0 = (long) epfd;
  long a1 = (long) events;
  long a2 = (long) maxevents;
  long a3 = (long) timeout;
  long a4 = 0;
  long call_num = __NR_epoll_wait;
  return(__ia64_syscall(a0, a1, a2, a3, a4, call_num));
}

#ifdef HAVE_EPOLL_CTLV
long
sys_epoll_ctlv(int epfd, int op, int count, struct epoll_event_ctlv *event)
{
  long a0 = (long) epfd;
  long a1 = (long) op;
  long a2 = (long) count;
  long a3 = (long) event;
  long a4 = 0;
  long call_num = __NR_epoll_ctlv;
  return(__ia64_syscall(a0, a1, a2, a3, a4, call_num));
}
#endif /* HAVE_EPOLL_CTLV */

long __ia64_syscall (long a0, long a1, long a2, long a3, long a4, long nr)
{
        return syscall(nr, a0, a1, a2, a3, a4);
}

#endif /* EPOLL_ASM */
#endif /* IA64 */

#endif /* NEED_EPOLL_HEADERS */
