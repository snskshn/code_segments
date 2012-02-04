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


/* This is a standin 
 * Eventually the stuff here this will appear in <sys/poll.h>
 */
#ifndef SYS_EPOLL_H
#define SYS_EPOLL_H

#ifdef PROVIDE_EPOLL_HEADERS

#ifndef _SYS_EPOLL_H 
#define _SYS_EPOLL_H 1 

#include <sys/types.h> 

#ifndef __NR_epoll_create
#ifdef IA64
#define __NR_epoll_create               (1243)
#define __NR_epoll_ctl                  (1244)
#define __NR_epoll_wait                 (1245)
#else
#define __NR_epoll_create   (254)
#define __NR_epoll_ctl      (255)
#define __NR_epoll_wait     (256)
#endif /* IA64 */
#endif /* ifndef __NR_epoll_create */

#ifdef HAVE_EPOLL_CTLV
#ifndef __NR_epoll_ctlv
#ifdef IA64
#define __NR_epoll_ctlv                 (1262)
#else
#define __NR_epoll_ctlv     (274)
#endif /* IA64 */
#endif /* __NR_epoll_ctlv */
#endif /* HAVE_EPOLL_CTLV */

enum EPOLL_EVENTS { 
        EPOLLIN = 0x001, 
#define EPOLLIN EPOLLIN 

        EPOLLPRI = 0x002, 
#define EPOLLPRI EPOLLPRI 

        EPOLLOUT = 0x004, 
#define EPOLLOUT EPOLLOUT 

#ifdef __USE_XOPEN 

        EPOLLRDNORM = 0x040, 
#define EPOLLRDNORM EPOLLRDNORM 

        EPOLLRDBAND = 0x080, 
#define EPOLLRDBAND EPOLLRDBAND 

        EPOLLWRNORM = 0x100, 
#define EPOLLWRNORM EPOLLWRNORM 

        EPOLLWRBAND = 0x200, 
#define EPOLLWRBAND EPOLLWRBAND 

#endif /* #ifdef __USE_XOPEN */ 

#ifdef __USE_GNU 
        EPOLLMSG = 0x400, 
#define EPOLLMSG EPOLLMSG 
#endif /* #ifdef __USE_GNU */ 

        EPOLLERR = 0x008, 
#define EPOLLERR EPOLLERR 

        EPOLLHUP = 0x010 
#define EPOLLHUP EPOLLHUP 

}; 

/* Set the One Shot behaviour for the target file descriptor */
#define EPOLLONESHOT (1 << 30)
                                                                                
/* Set the Edge Triggered behaviour for the target file descriptor */
#define EPOLLET (1 << 31)

/* Valid opcodes ( "op" parameter ) to issue to epoll_ctl() */ 
#define EPOLL_CTL_ADD 1 /* Add a file descriptor to the interface */ 
#define EPOLL_CTL_DEL 2 /* Remove a file descriptor from the interface */ 
#define EPOLL_CTL_MOD 3 /* Change file descriptor epoll_event structure */ 
#ifdef HAVE_EPOLL_CTLV
#define EPOLL_CTLV    4 /* Treat this as an epoll_ctlv call */ 
                        /* In this case the data.u32 field contains the op */
#endif /* HAVE_EPOLL_CTLV */

typedef union epoll_data { 
        void *ptr; 
        int fd; 
        __uint32_t u32; 
        __uint64_t u64; 
} epoll_data_t; 

struct epoll_event { 
        __uint32_t events; /* Epoll events */ 
        epoll_data_t data; /* User data variable */ 
}; 


#ifdef HAVE_EPOLL_CTLV
struct epoll_event_ctlv {
	struct epoll_event event;
	int op;
	long res;
} EPOLL_PACKED;

extern long sys_epoll_ctlv(int epfd, int op, int count,
    struct epoll_event_ctlv *event);
#endif /* HAVE_EPOLL_CTLV */
extern long sys_epoll_create(int size);
extern long sys_epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
extern long sys_epoll_wait(int epfd, struct epoll_event *events, 
    int maxevents, int timeout);



#endif /* #ifndef _SYS_EPOLL_H */ 

#endif /* HAVE_EPOLL */

#endif /* PROVIDE_EPOLL_HEADERS */
