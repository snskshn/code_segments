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


#include <stdlib.h>
#include <sys/poll.h>
#ifdef HAVE_EPOLL

#ifdef PROVIDE_EPOLL_HEADERS
#include "sys_epoll.h"
#else
#include <sys/epoll.h>
#endif


#include <stdint.h>
#include <errno.h>
#include <asm/unistd.h>
/* #include <asm-ia64/types.h> */
#ifdef IA64
/* Uncomment this if you can point to the real deal */
/* #include <asm-ia64/unistd.h> */
#endif /* IA64 */
#ifdef PROVIDE_EPOLL_HEADERS
#include "sys_epoll.h"
#else
#include <sys/epoll.h>
#endif
#endif /* HAVE_EPOLL */

#include "debug.h"
#include "trace.h"
#include "interest_set.h"
#include "sys_epoll.h"
#include "sock_listener.h"
#include "options.h"
#include "stats.h"

static enum set_method_enum set_method = DECLARE_INVALID;

/* 
 * interest_set is used to let the system know what events are of interest.
 * event_set is used by the system to let you know what events occurred.
 */

static int interest_set_is_set(int sd, enum which_set_enum which_set);
static void interest_set_add(int sd, enum which_set_enum which_set);
static void interest_set_del(int sd, enum which_set_enum which_set);

static int event_set_is_set(int sd, enum which_set_enum which_set);

extern fd_set *rdfds, *wrfds;

static int poll_is_set(int sd, enum which_set_enum which_set, short evt);
static int poll_event_is_set(int sd, enum which_set_enum which_set);
static int poll_revent_is_set(int sd, enum which_set_enum which_set);
static void poll_event_del(int sd, enum which_set_enum which_set);
static void poll_event_add(int sd, enum which_set_enum which_set);

static int find_poll_array_entry(int sd);
static void free_poll_array_entry(int sd);
static int new_poll_array_entry(int sd);

extern struct pollfd *poll_array;
extern int max_poll_fd;
extern int max_poll_entry;

#ifdef HAVE_EPOLL
static int epoll_is_set(int sd, enum which_set_enum which_set, uint32_t evt);
static int epoll_event_is_set(int sd, enum which_set_enum which_set);
static void epoll_event_del(int sd, enum which_set_enum which_set);
static void epoll_event_add(int sd, enum which_set_enum which_set);

static int find_epoll_array_entry(int sd);
static void free_epoll_array_entry(int sd);

extern struct epoll_event *epoll_array;
extern struct epoll_event *epoll_results;
extern int max_epoll_fd;
extern int max_epoll_entry;
extern int epoll_fd;
#endif /* HAVE_EPOLL */

void
interest_set_init(enum set_method_enum method)
{
  set_method = method;
  assert(set_method != DECLARE_INVALID);
  PRINT_TIME(NOFD, &tnow, &tprev, "interest_set_init: done\n");
}

void
select_event_change(int sd, unsigned int mask)
{
  assert(sd < FD_SETSIZE);

  PRINT_TIME(sd, &tnow, &tprev, "select_event_change: sd = %d mask = 0x%x ",
       sd, mask);

  if (mask & ISET_DONE) {
    FD_CLR(sd, rdfds);
    FD_CLR(sd, wrfds);
  } else {
    if (mask & ISET_READABLE) {
      FD_SET(sd, rdfds);
    }

    if (mask & ISET_WRITABLE) {
      FD_SET(sd, wrfds);
    }

    if (mask & ISET_NOT_READABLE) {
      FD_CLR(sd, rdfds);
    }

    if (mask & ISET_NOT_WRITABLE) {
      FD_CLR(sd, wrfds);
    }
  }

  PRINT_TIME(sd, &tnow, &tprev, "select_event_change: %c%c", 
             (interest_set_is_readable(sd) ? 'R' : '-'),
             (interest_set_is_writable(sd) ? 'W' : '-'));

}

void
poll_event_change(int sd, unsigned int mask)
{
  int index = -1;
  assert(sd < max_fds);

  PRINT_TIME(sd, &tnow, &tprev, "poll_event_change: sd = %d mask = 0x%x "
      "max_poll_fd = %d",
       sd, mask, max_poll_fd);

  index = find_poll_array_entry(sd);
  if (index == -1) {
    index = new_poll_array_entry(sd);
    assert(index != -1);
  }

  poll_array[index].fd = sd;

  if (mask & (ISET_READABLE | ISET_WRITABLE)) {
    if (index > max_poll_fd) {
      max_poll_fd = index;
    }
  }

  if (mask & ISET_DONE) {
      poll_array[index].events &= (short) (~POLLIN);
      poll_array[index].events &= (short) (~POLLOUT);
      free_poll_array_entry(sd);
  } else {
    if (mask & ISET_READABLE) {
      poll_array[index].events |= POLLIN;
    }

    if (mask & ISET_WRITABLE) {
      poll_array[index].events |= POLLOUT;
    }

    if (mask & ISET_NOT_READABLE) {
      poll_array[index].events &= (short) (~POLLIN);
    }

    if (mask & ISET_NOT_WRITABLE) {
      poll_array[index].events &= (short) (~POLLOUT);
    }
  }

  PRINT_TIME(sd, &tnow, &tprev, "poll_event_change: sd = %d mask = 0x%x "
      "max_poll_entry = %d",
       sd, mask, max_poll_entry);
  PRINT_TIME(sd, &tnow, &tprev, "poll_event_change: fd = %d events = 0x%x "
      "revents = 0x%x", poll_array[index].fd, poll_array[index].events, 
       poll_array[index].revents);
}

#ifdef HAVE_EPOLL
void
epoll_event_change(int sd, unsigned int mask)
{
#ifdef HAVE_EPOLL_CTLV
  extern struct epoll_event_ctlv *new_epoll_ctlv_evts;
  extern int new_epoll_ctlv_count;
#endif /* HAVE_EPOLL_CTLV */

  assert(epoll_fd > 0);

  int rc = 0;
  int index = -1;
  int save_errno = 0;
  int trace_fd = 0;
  struct epoll_event ep_evt;
#ifdef HAVE_EPOLL_CTLV
  struct epoll_event_ctlv ep_evt_ctlv;
#endif /* HAVE_EPOLL_CTLV */

  ep_evt.events = 0;

  if (options.epoll_edge_triggered) {
    if (!sock_is_listener(sd)) {
      ep_evt.events |= EPOLLET;
    }
  }

  int epoll_op = EPOLL_CTL_ADD;
  assert(sd < max_fds);

  assert(sd >=0);
  index = sd;

  PRINT_TIME(sd, &tnow, &tprev, "epoll_event_change: sd = %d mask = 0x%x "
      "max_epoll_entry = %d epoll_fd = %d",
       sd, mask, max_epoll_entry, epoll_fd);

  if (mask & ISET_DONE) {
    ep_evt.data.fd = sd;

    epoll_op = EPOLL_CTL_DEL;

    PRINT_TIME(sd, &tnow, &tprev, "epoll_event_change: sd = %d op = 0x%x "
         "event = 0x%x deleting", sd, epoll_op, ep_evt.events);
    num_epoll_ctl_calls++;

/* Done this way because nesting these macros doesn't work */
#ifdef PROVIDE_EPOLL_HEADERS
    TRACE(EVT_EPOLL_CTL,
      trace_fd = sd;
      rc = sys_epoll_ctl(epoll_fd, epoll_op, sd, &ep_evt);
      save_errno = errno;
    );
#else
    TRACE(EVT_EPOLL_CTL,
      trace_fd = sd;
      rc = epoll_ctl(epoll_fd, epoll_op, sd, &ep_evt);
      save_errno = errno;
    );
#endif /* PROVIDE_EPOLL_HEADERS */

    if (rc < 0) {
      perror("epoll_array_change: epoll_ctl failed: ");
      fprintf(stderr,"\nepoll fd = %d, epoll_op= 0x%x, sd= %d, ep_evt= 0x%x, rc= %d, errno= %d\n",
                epoll_fd,epoll_op,sd,ep_evt.events,rc,errno);
      fprintf(stdout,"\nepoll fd = %d, epoll_op= 0x%x, sd= %d, ep_evt= 0x%x, rc= %d, errno= %d\n",
                epoll_fd,epoll_op,sd,ep_evt.events,rc,errno);
      exit(1);
    }

    epoll_array[sd].data.fd = -1;
    epoll_array[sd].events = 0;
    if (options.epoll_edge_triggered) {
      if (!sock_is_listener(sd)) {
	epoll_array[sd].events |= EPOLLET;
      }
    }
  } else {

    /* either initializing or modifying */
    if (mask & ISET_INIT) {
      epoll_op = EPOLL_CTL_ADD;
      PRINT_TIME(sd, &tnow, &tprev, "epoll_event_change: adding");
    } else {
      epoll_op = EPOLL_CTL_MOD;

      /* with epoll2 the idea is to try to reduce the number of epoll_ctl
       * calls by setting the initial state to read and write and leaving
       * them that way until we are done with them
       */
      if (options.use_epoll2) {
        if (sock_is_listener(sd)) {
	   if (options.epoll_modify_listeners) {
	     PRINT_TIME(sd, &tnow, &tprev, "epoll_event_change: will change listener");
	   } else {
	     PRINT_TIME(sd, &tnow, &tprev, "epoll_event_change: will not change listener");
	     return;
	   }
	} else {
	  PRINT_TIME(sd, &tnow, &tprev, "epoll_event_change: epoll2 no change on non listener");
	  return;
	}
      }

    }
    if (options.epoll_edge_triggered) {
      if (!sock_is_listener(sd)) {
        epoll_array[index].events |= EPOLLET;
      }
    }

    if (mask & ISET_READABLE) {
      epoll_array[index].events |= EPOLLIN | EPOLLHUP | EPOLLERR;
    }

    if (mask & ISET_WRITABLE) {
      // epoll_array[index].events |= EPOLLOUT | EPOLLHUP | EPOLLERR;
      epoll_array[index].events |= EPOLLOUT | EPOLLHUP | EPOLLERR;
    }

    if (mask & ISET_NOT_READABLE) {
      // epoll_array[index].events &= (short) ~(EPOLLIN | EPOLLHUP | EPOLLERR);
      epoll_array[index].events &= (short) ~(EPOLLIN);
    }

    if (mask & ISET_NOT_WRITABLE) {
      // epoll_array[index].events &= (short) ((~EPOLLOUT) | (~EPOLLHUP) | (~EPOLLERR));
      epoll_array[index].events &= (short) ~(EPOLLOUT);
    }

    epoll_array[index].data.fd = sd;

    ep_evt.data.fd = epoll_array[index].data.fd;
    ep_evt.events = epoll_array[index].events;

    if (options.use_epoll_ctlv) {
#ifdef HAVE_EPOLL_CTLV
      /* save the changes away and change things with only one syscall */
      /* for now we hijack this field to store the operation/op */
      ep_evt_ctlv.op = epoll_op;
      ep_evt_ctlv.event.data.fd = sd;
      ep_evt_ctlv.event.events = ep_evt.events;

      PRINT_TIME(sd, &tnow, &tprev, "epoll_event_change: adding event to "
	"ctlv array at count = %d", new_epoll_ctlv_count);

      new_epoll_ctlv_evts[new_epoll_ctlv_count] = ep_evt_ctlv;
      new_epoll_ctlv_count++;
#else
      printf("epoll_event_change: EPOLL_CLTV not defined\n");
      exit(1);
#endif /* HAVE_EPOLL_CTLV */
    } else {

      if (epoll_op == EPOLL_CTL_ADD) {
        PRINT_TIME(sd, &tnow, &tprev, "epoll_event_change: sd = %d op = 0x%x event = 0x%x "
	   "adding", sd, epoll_op, ep_evt.events);
      } else if (epoll_op == EPOLL_CTL_MOD)  {
        PRINT_TIME(sd, &tnow, &tprev, "epoll_event_change: sd = %d op = 0x%x event = 0x%x "
	   "modifying", sd, epoll_op, ep_evt.events);
      } else {
        PRINT_TIME(sd, &tnow, &tprev, "epoll_event_change: sd = %d op = 0x%x event = 0x%x "
	   "STRANGE OP", sd, epoll_op, ep_evt.events);
      }

      num_epoll_ctl_calls++;

/* Done this way because nesting these macros doesn't work */
#ifdef PROVIDE_EPOLL_HEADERS
      TRACE(EVT_EPOLL_CTL,
	trace_fd = sd;
	rc = sys_epoll_ctl(epoll_fd, epoll_op, sd, &ep_evt);
	save_errno = errno;
      );
#else
      TRACE(EVT_EPOLL_CTL,
	trace_fd = sd;
	rc = epoll_ctl(epoll_fd, epoll_op, sd, &ep_evt);
	save_errno = errno;
      );
#endif /* PROVIDE_EPOLL_HEADERS */

      if (rc < 0) {
        errno = save_errno;
	perror("epoll_event_change: epoll_ctl failed ");
	fprintf(stderr,"\nepoll fd = %d, epoll_op= 0x%x, sd= %d, ep_evt= 0x%x, rc= %d, errno= %d\n",
                epoll_fd,epoll_op,sd,ep_evt.events,rc,errno);
        fprintf(stdout,"\nepoll fd = %d, epoll_op= 0x%x, sd= %d, ep_evt= 0x%x, rc= %d, errno= %d\n",
                epoll_fd,epoll_op,sd,ep_evt.events,rc,errno);
         /*printf("epoll_fd = %d sd = %d errno = %d\n", epoll_fd, sd, save_errno);*/
	exit(1);
      }
    }
  }

  PRINT_TIME(sd, &tnow, &tprev, "epoll_event_change: data.fd = %d "
      "events = 0x%x ", ep_evt.data.fd, ep_evt.events);
} /* epoll_event_change */

#endif /* HAVE_EPOLL */


void
interest_set_change(int sd, unsigned int mask)
{
  PRINT_TIME(sd, &tnow, &tprev, "interest_set_change: sd = %d "
      "mask = 0x%x", sd, mask);

  assert(sd > 2);
  assert(sd < max_fds);

  /* ISET_INIT and ISET_DONE are mutually exclusive flags */
  assert(!((mask & ISET_INIT) && (mask & ISET_DONE)));

  switch (set_method) {
    case DECLARE_WITH_FDSET:
      select_event_change(sd, mask);
      break;

    case DECLARE_WITH_POLL:
      poll_event_change(sd, mask);
      break;

#ifdef HAVE_EPOLL
    case DECLARE_WITH_EPOLL:
       PRINT_TIME(sd, &tnow, &tprev, "interest_set_change: epoll_fd = %d sd = %d " 
                  "mask = 0x%x", epoll_fd, sd, mask);
      epoll_event_change(sd, mask);
      break;
#endif /* HAVE_EPOLL */

    case DECLARE_WITH_SEND:
    case DECLARE_WITH_AIO:
      /* nothing to do */
      break;

    default:
      printf("interest_set_change: invalid method for declaring "
	     "interest = %d\n", set_method);
      exit(1);
      break;
  }
} /* interest_set_change */

/*----------------------------------------------------------------------*/
int
event_set_is_readable_or_writable(int sd)
{
  return(event_set_is_readable(sd) ||
         event_set_is_writable(sd));
}

int 
event_set_is_readable(int sd)
{
  return(event_set_is_set(sd, USE_READ_SET));
}

int 
event_set_is_writable(int sd)
{
  return(event_set_is_set(sd, USE_WRITE_SET));
}

static int
event_set_is_set(int sd, enum which_set_enum which_set)
{
  extern fd_set *readable, *writable;
  int retval = -1;
  assert(sd < max_fds);
  switch (set_method) {
    case DECLARE_WITH_FDSET:
      assert(sd < FD_SETSIZE);
      if (which_set == USE_READ_SET) {
        retval = FD_ISSET(sd, readable);
      } else {
        retval = FD_ISSET(sd, writable);
      }
      break;

    case DECLARE_WITH_POLL:
      retval = poll_revent_is_set(sd, which_set);
      break;

    case DECLARE_WITH_EPOLL:
      printf("event_set_is_set: epoll version not implemented\n");
      printf("  it shouldn't be needed/used\n");
      exit(1);
      break;


    case DECLARE_WITH_SEND:
      printf("event_set_is_set: should not happen\n");
      exit(1);
      break;

    default:
      printf("event_set_is_set: invalid method for declaring "
	     "event = %d\n", set_method);
      exit(1);
      break;
  }

  PRINT_TIME(sd, &tnow, &tprev, "event_set_is_set: sd = %d "
      "which_set = %d retval = %d", sd, which_set, retval);

  return(retval);
}


/*----------------------------------------------------------------------*/
int 
interest_set_is_readable(int sd)
{
  return(interest_set_is_set(sd, USE_READ_SET));
}

int 
interest_set_is_writable(int sd)
{
  return(interest_set_is_set(sd, USE_WRITE_SET));
}

static int
interest_set_is_set(int sd, enum which_set_enum which_set)
{
  int retval = -1;
  assert(sd < max_fds);
  switch (set_method) {
    case DECLARE_WITH_FDSET:
      assert(sd < FD_SETSIZE);
      if (which_set == USE_READ_SET) {
        retval = FD_ISSET(sd, rdfds);
      } else {
        retval = FD_ISSET(sd, wrfds);
      }
      break;

    case DECLARE_WITH_POLL:
      retval = poll_event_is_set(sd, which_set);
      break;

#ifdef HAVE_EPOLL
    case DECLARE_WITH_EPOLL:
      retval = epoll_event_is_set(sd, which_set);
      break;
#endif /* HAVE_EPOLL */

    case DECLARE_WITH_SEND:
      printf("interest_set_is_set: should not happen\n");
      exit(1);
      break;

    case DECLARE_WITH_AIO:
      /* nothing to do */
      break;

    default:
      printf("interest_set_is_set: invalid method for declaring "
	     "interest = %d\n", set_method);
      exit(1);
      break;
  }

  PRINT_TIME(sd, &tnow, &tprev, "interest_set_is_set: sd = %d "
      "which_set = %d retval = %d", sd, which_set, retval);

  return(retval);
}

/*----------------------------------------------------------------------*/

void
interest_set_add_readable(int sd)
{
  interest_set_add(sd, USE_READ_SET);
}

void
interest_set_add_writable(int sd)
{
  interest_set_add(sd, USE_WRITE_SET);
}

static void
interest_set_add(int sd, enum which_set_enum which_set)
{
  PRINT_TIME(sd, &tnow, &tprev, "interest_set_add: sd = %d "
      "which_set = %d", sd, which_set);
  assert(sd > 2);
  assert(sd < max_fds);

  switch (set_method) {
    case DECLARE_WITH_FDSET:
      assert(sd < FD_SETSIZE);
      if (which_set == USE_READ_SET) {
        FD_SET(sd, rdfds);
      } else {
        FD_SET(sd, wrfds);
      }
      break;

    case DECLARE_WITH_POLL:
      poll_event_add(sd, which_set);
      break;

#ifdef HAVE_EPOLL
    case DECLARE_WITH_EPOLL:
      PRINT_TIME(sd, &tnow, &tprev, "interest_set_add: "
        "calling epoll_event_add");
      epoll_event_add(sd, which_set);
      break;
#endif /* HAVE_EPOLL */

    case DECLARE_WITH_SEND:
    case DECLARE_WITH_AIO:
      /* nothing to do */
      break;

    default:
      printf("interest_set_add: invalid method for declaring "
	     "interest = %d\n", set_method);
      exit(1);
      break;
  }
}

/*----------------------------------------------------------------------*/
void
interest_set_del_readable(int sd)
{
  interest_set_del(sd, USE_READ_SET);

}
void
interest_set_del_writable(int sd)
{
  interest_set_del(sd, USE_WRITE_SET);
}

static void
interest_set_del(int sd, enum which_set_enum which_set)
{
  PRINT_TIME(sd, &tnow, &tprev, "interest_set_del: sd = %d "
      "which_set = %d", sd, which_set);
  assert(sd > 2);
  assert(sd < max_fds);

  switch (set_method) {
    case DECLARE_WITH_FDSET:
      assert(sd < FD_SETSIZE);
      if (which_set == USE_READ_SET) {
        FD_CLR(sd, rdfds);
      } else {
        FD_CLR(sd, wrfds);
      }
      break;

    case DECLARE_WITH_POLL:
      poll_event_del(sd, which_set);
      break;

#ifdef HAVE_EPOLL
    case DECLARE_WITH_EPOLL:
      epoll_event_del(sd, which_set);
      break;
#endif /* HAVE_EPOLL */

    case DECLARE_WITH_SEND:
    case DECLARE_WITH_AIO:
      /* nothing to do */
      break;

    default:
      printf("interest_set_del: invalid method for declaring "
	     "interest = %d\n", set_method);
      exit(1);
      break;
  }
}

/*----------------------------------------------------------------------*/
void 
interest_set_done(int sd)
{
  switch (set_method) {
    case DECLARE_WITH_FDSET:
      /* Do nothing */
      break;

    case DECLARE_WITH_POLL:
      free_poll_array_entry(sd);
      break;

#ifdef HAVE_EPOLL
    case DECLARE_WITH_EPOLL:
      free_epoll_array_entry(sd);
      break;
#endif /* HAVE_EPOLL */

    case DECLARE_WITH_SEND:
    case DECLARE_WITH_AIO:
      /* nothing to do */
      break;

    default:
      printf("interest_set_done: invalid method for declaring "
	     "interest = %d\n", set_method);
      exit(1);
      break;
  }
}

/*----------------------------------------------------------------------*/

static int
poll_is_set(int sd, enum which_set_enum which_set, short evt)
{
  int retval = -1;
  int index = -1;
  assert(sd < max_fds);

  index = find_poll_array_entry(sd);
  
  if (index == -1) {
    retval = 0;
  } else {
    switch (which_set) {
      case USE_READ_SET:
	retval = evt & (POLLIN | POLLERR | POLLHUP);
	break;

      case USE_WRITE_SET:
	retval = evt & (POLLOUT | POLLERR | POLLHUP);
	break;
    }

    PRINT_TIME(sd, &tnow, &tprev, "poll_is_set: fd = %d events = 0x%x "
      "revents = 0x%x", poll_array[index].fd, poll_array[index].events, 
      poll_array[index].revents);
  }

  PRINT_TIME(sd, &tnow, &tprev, "poll_is_set: sd = %d index = %d "
      "which_set = %d evt = 0x%x retval = %d", sd, 
      index, which_set, evt, retval);
  return(retval);
}

static int
poll_event_is_set(int sd, enum which_set_enum which_set)
{
  int retval = -1;
  int index = -1;
  assert(sd < max_fds);

  index = find_poll_array_entry(sd);

  if (index == -1) {
    retval = 0;
  } else if (poll_array[index].fd == -1) {
    retval = 0;
  } else {
    assert(poll_array[index].fd == sd);
    retval = poll_is_set(sd, which_set, poll_array[index].events);
  }

  PRINT_TIME(sd, &tnow, &tprev, "poll_event_is_set: sd = %d "
      "which_set = %d retval = %d", sd, which_set, retval);

  return(retval);
}

static int
poll_revent_is_set(int sd, enum which_set_enum which_set)
{
  int retval = -1;
  int index = find_poll_array_entry(sd);

  if (index == -1) {
    retval = 0;
  } else if (poll_array[index].fd == -1) {
      retval = 0;
  } else {
    assert(poll_array[index].fd == sd);
    retval = poll_is_set(sd, which_set, poll_array[index].revents);
  }

  PRINT_TIME(sd, &tnow, &tprev, "poll_revent_is_set: sd = %d "
      "which_set = %d retval = %d", sd, which_set, retval);

  return(retval);
}

static void
poll_event_del(int sd, enum which_set_enum which_set)
{
  int index = -1;
  assert(sd < max_fds);

  index = find_poll_array_entry(sd);
  assert(index != -1);

  switch (which_set) {
    case USE_READ_SET:
      poll_array[index].events &= (short) (~POLLIN);
      break;

    case USE_WRITE_SET:
      poll_array[index].events &= (short) (~POLLOUT);
      break;
  }


  PRINT_TIME(sd, &tnow, &tprev, "poll_event_del: sd = %d index = %d "
      "which_set = %d max_poll_entry = %d",
       sd, index, which_set, max_poll_entry);
  PRINT_TIME(sd, &tnow, &tprev, "poll_event_del: fd = %d events = 0x%x "
      "revents = 0x%x", poll_array[index].fd, poll_array[index].events,
       poll_array[index].revents);
}

static void
poll_event_add(int sd, enum which_set_enum which_set)
{
  int index = -1;
  assert(sd < max_fds);

  index = find_poll_array_entry(sd);
  if (index == -1) {
    index = new_poll_array_entry(sd);
    assert(index != -1);
  }

  poll_array[index].fd = sd;


  if (index > max_poll_fd) {
    max_poll_fd = index;
  }

  switch (which_set) {
    case USE_READ_SET:
      poll_array[index].events |= POLLIN;
      break;

    case USE_WRITE_SET:
      poll_array[index].events |= POLLOUT;
      break;

  }

  PRINT_TIME(sd, &tnow, &tprev, "poll_event_add: sd = %d index = %d "
      "which_set = %d max_poll_entry = %d",
       sd, index, which_set, max_poll_entry);
  PRINT_TIME(sd, &tnow, &tprev, "poll_event_add: fd = %d events = 0x%x "
      "revents = 0x%x", poll_array[index].fd, poll_array[index].events, 
       poll_array[index].revents);
}
/*----------------------------------------------------------------------*/

static int 
new_poll_array_entry(int sd)
{
  extern int *poll_map;
  int retval = -1;

  retval = max_poll_entry;
  poll_map[sd] = max_poll_entry;
  max_poll_entry++;
  if (max_poll_entry > max_fds) {
    printf("new_poll_array_entry: poll entries exceeded max_fds = %d\n",
	max_fds);
    exit(1);
  }

  PRINT_TIME(sd, &tnow, &tprev, "new_poll_array_entry: retval = %d", retval);
  return(retval);
}

static int 
find_poll_array_entry(int sd)
{
  extern int *poll_map;
  int retval = -1;

  if (sd == -1) {
    retval = -1;
  } else {
    assert(sd < max_fds);
    assert(sd >= 0);
    retval = poll_map[sd];
  }

  PRINT_TIME(sd, &tnow, &tprev, "find_poll_array_entry: retval = %d", retval);
  return(retval);
}

static void
free_poll_array_entry(int sd)
{
  extern int *poll_map;
  int index = -1;

  assert(sd < max_fds);

  index = find_poll_array_entry(sd);
  poll_array[index].fd = -1;
  poll_array[index].events = 0;
  poll_array[index].revents = 0;
  poll_map[sd] = -1;
  PRINT_TIME(sd, &tnow, &tprev, "free_poll_array_entry: index = %d", index);
}

void
compact_poll_array(void)
{
  extern int *poll_map;
  int i;
  PRINT_TIME(NOFD, &tnow, &tprev, "compact_poll_array: starting "
      "max_poll_entry now = %d", max_poll_entry);

  for (i=0; i<max_poll_entry; i++) {
    if (poll_array[i].fd == -1) {
      PRINT_TIME(NOFD, &tnow, &tprev, "compact_poll_array: copying %d to %d",
	  max_poll_entry-1, i);

      /* Copy the last entry in the array into the current position */
      poll_array[i] = poll_array[max_poll_entry-1];
      /* No set the mapping for the fd */
      poll_map[poll_array[i].fd] = i;

      /* Essentially remove the old entry */
      poll_array[max_poll_entry-1].fd = -1;
      poll_array[max_poll_entry-1].events = 0;
      poll_array[max_poll_entry-1].revents = 0;

      max_poll_entry--;
    }
  }

  PRINT_TIME(NOFD, &tnow, &tprev, "compact_poll_array: done max_poll_entry "
      "now = %d", max_poll_entry);
}


/*----------------------------------------------------------------------*/
#ifdef HAVE_EPOLL

static int
epoll_is_set(int sd, enum which_set_enum which_set, uint32_t evt)
{
  int retval = -1;
  int index = -1;
  assert(sd < max_fds);

  index = find_epoll_array_entry(sd);
  
  if (index == -1) {
    retval = 0;
  } else {
    switch (which_set) {
      case USE_READ_SET:
	retval = evt & (EPOLLIN | EPOLLERR | EPOLLHUP);
	break;

      case USE_WRITE_SET:
	retval = evt & (EPOLLOUT | EPOLLERR | EPOLLHUP);
	break;
    }

    PRINT_TIME(sd, &tnow, &tprev, "epoll_is_set: fd = %d events = 0x%x ",
      epoll_array[index].data.fd, epoll_array[index].events);
  }

  PRINT_TIME(sd, &tnow, &tprev, "epoll_is_set: sd = %d index = %d "
      "which_set = %d evt = 0x%x retval = %d", sd, 
      index, which_set, evt, retval);
  return(retval);
}

/*----------------------------------------------------------------------*/
static int
epoll_event_is_set(int sd, enum which_set_enum which_set)
{
  int retval = -1;
  int index = -1;
  assert(sd < max_fds);

  index = find_epoll_array_entry(sd);

  if (index == -1) {
    retval = 0;
  } else if (epoll_array[index].data.fd == -1) {
    retval = 0;
  } else {
    assert(epoll_array[index].data.fd == sd);
    retval = epoll_is_set(sd, which_set, epoll_array[index].events);
  }

  PRINT_TIME(sd, &tnow, &tprev, "epoll_event_is_set: sd = %d "
      "which_set = %d retval = %d", sd, which_set, retval);

  return(retval);
}


static void
epoll_event_del(int sd, enum which_set_enum which_set)
{
  int trace_fd = 0;
  int rc = 0;
  int save_errno = 0;
  struct epoll_event ep_evt;
  int epoll_op = EPOLL_CTL_MOD;
  int index = -1;
  assert(sd < max_fds);

  assert(sd >= 0);
  index = sd;
  assert(sd == epoll_array[index].data.fd);
  switch (which_set) {
    case USE_READ_SET:
      epoll_array[index].events &= (short) (~POLLIN);
      break;

    case USE_WRITE_SET:
      epoll_array[index].events &= (short) (~POLLOUT);
      break;
  }

  ep_evt.data.fd = sd;
  ep_evt.events = epoll_array[index].events;

  PRINT_TIME(sd, &tnow, &tprev, "epoll_event_del: sd = %d op = 0x%x event = 0x%x",
       sd, epoll_op, ep_evt.events);

  num_epoll_ctl_calls++;

/* Done this way because nesting these macros doesn't work */
#ifdef PROVIDE_EPOLL_HEADERS
  TRACE(EVT_EPOLL_CTL,
    trace_fd = sd;
    rc = sys_epoll_ctl(epoll_fd, epoll_op, sd, &ep_evt);
    save_errno = errno;
  );
#else
  TRACE(EVT_EPOLL_CTL,
    trace_fd = sd;
    rc = epoll_ctl(epoll_fd, epoll_op, sd, &ep_evt);
    save_errno = errno;
  );
#endif /* PROVIDE_EPOLL_HEADERS */

  if (rc < 0) {
    perror("epoll_event_del: epoll_ctl ");
    exit(1);
  }

  PRINT_TIME(sd, &tnow, &tprev, "epoll_event_del: sd = %d index = %d "
      "which_set = %d max_epoll_entry = %d",
       sd, index, which_set, max_epoll_entry);
  PRINT_TIME(sd, &tnow, &tprev, "epoll_event_del: data.fd = %d "
      "events = 0x%x ", ep_evt.data.fd, ep_evt.events);

}

static void
epoll_event_add(int sd, enum which_set_enum which_set)
{
  int rc = 0;
  int index = -1;
  int save_errno = 0;
  int trace_fd = 0;
  struct epoll_event ep_evt;
  int epoll_op = EPOLL_CTL_ADD;
  assert(sd < max_fds);

  assert(sd >=0);
  index = sd;
  if (epoll_array[index].data.fd == -1) {
    epoll_op = EPOLL_CTL_ADD;
  } else {
    epoll_op = EPOLL_CTL_MOD;
  }

  epoll_array[index].data.fd = sd;
  switch (which_set) {
    case USE_READ_SET:
      epoll_array[index].events |= EPOLLIN | EPOLLHUP | EPOLLERR;
      break;

    case USE_WRITE_SET:
      epoll_array[index].events |= EPOLLOUT | EPOLLHUP | EPOLLERR;
      break;

  }

  ep_evt.data.fd = epoll_array[index].data.fd;
  ep_evt.events = epoll_array[index].events;

  PRINT_TIME(sd, &tnow, &tprev, "epoll_event_add: sd = %d op = 0x%x event = 0x%x",
       sd, epoll_op, ep_evt.events);

  num_epoll_ctl_calls++;

/* Done this way because nesting these macros doesn't work */
#ifdef PROVIDE_EPOLL_HEADERS
  TRACE(EVT_EPOLL_CTL,
    trace_fd = sd;
    rc = sys_epoll_ctl(epoll_fd, epoll_op, sd, &ep_evt);
    save_errno = errno;
  );
#else
  TRACE(EVT_EPOLL_CTL,
    trace_fd = sd;
    rc = epoll_ctl(epoll_fd, epoll_op, sd, &ep_evt);
    save_errno = errno;
  );
#endif /* PROVIDE_EPOLL_HEADERS */

  if (rc < 0) {
    perror("epoll_event_add: epoll_ctl ");
    exit(1);
  }

  PRINT_TIME(sd, &tnow, &tprev, "epoll_event_add: sd = %d index = %d "
      "which_set = %d max_epoll_entry = %d",
       sd, index, which_set, max_epoll_entry);
  PRINT_TIME(sd, &tnow, &tprev, "epoll_event_add: data.fd = %d "
      "events = 0x%x ", ep_evt.data.fd, ep_evt.events);
}
/*----------------------------------------------------------------------*/


static int 
find_epoll_array_entry(int sd)
{
  extern int *epoll_map;
  int retval = -1;

  /* fake out compiler warning */
  retval = epoll_map[sd];
  assert(sd < max_fds);
  assert(sd >= 0);
  retval = sd;
  assert(retval != -1);

  PRINT_TIME(sd, &tnow, &tprev, "find_epoll_array_entry: retval = %d", retval);
  return(retval);
}

static void
free_epoll_array_entry(int sd)
{
  int trace_fd = 0;
  int save_errno = 0;
  int rc = 0;
  struct epoll_event ep_evt;
  ep_evt.data.fd = sd;
  ep_evt.events = 0;

  PRINT_TIME(sd, &tnow, &tprev, "free_epoll_array_entry: sd = %d op = 0x%x event = 0x%x",
       sd, EPOLL_CTL_DEL, ep_evt.events);

  num_epoll_ctl_calls++;

/* Done this way because nesting these macros doesn't work */
#ifdef PROVIDE_EPOLL_HEADERS
  TRACE(EVT_EPOLL_CTL,
    trace_fd = sd;
    rc = sys_epoll_ctl(epoll_fd, EPOLL_CTL_DEL, sd, &ep_evt);
    save_errno = errno;
  );
#else
  TRACE(EVT_EPOLL_CTL,
    trace_fd = sd;
    rc = epoll_ctl(epoll_fd, EPOLL_CTL_DEL, sd, &ep_evt);
    save_errno = errno;
  );
#endif /* PROVIDE_EPOLL_HEADERS */

  if (rc < 0) {
    perror("free_epoll_array_entry: epoll_ctl ");
    exit(1);
  }

  epoll_array[sd].data.fd = -1;
  epoll_array[sd].events = 0;

  PRINT_TIME(sd, &tnow, &tprev, 
    "free_epoll_array_entry: sd = %d max_epoll_entry = %d", 
    sd, max_epoll_entry);
}

/*----------------------------------------------------------------------*/

void
check_epollable(int sd)
{
  int rc = 0;
  struct epoll_event ep_evt;
  int epoll_op;
  int save_errno = 0;

  assert(sd >= 0);
  /* we are checking to ensure that the socket we are checking
   * is already in epoll_fd's interest set
   */
  ep_evt.data.fd = sd;
  ep_evt.events = EPOLLIN | EPOLLOUT | EPOLLHUP | EPOLLERR;
  epoll_op = EPOLL_CTL_ADD;
  rc = epoll_ctl(epoll_fd, epoll_op, sd, &ep_evt);
  save_errno = errno;

  /* this should fail because supposedly we are already interested in this sd */
  printf("check_epollable epoll_ctl on sd = %d returns rc = %d and errno = %d\n", sd, rc, save_errno);
  if (rc < 0) {
     if (errno == EEXIST) {
       printf("check_epollable: checked as expected on sd = %d\n", sd);
       /* check about events for this 1 fd with no blocking/waiting */
       rc = epoll_wait(epoll_fd, &ep_evt, 1, 0);

       if (rc == 0) {
         printf("check_epollable: epoll_wait reports sd %d has no events\n", sd);
       } else {
         printf("check_epollable: epoll_wait reports sd %d has events = 0x%x\n", sd, ep_evt.events);
       }
     } else {
       printf("check_epollable: epoll_ctl succeeded on sd = %d but got unexpected errno = %d\n", sd, save_errno);
     }
  } else {
    printf("check_epollable FAILED CHECK on sd = %d -- not currently in interest set\n", sd);
  }
}

void
epoll_print_info()
{
  int sd = 0;
  int index = -1;
  printf("\nepoll_print_info\n");
  for (sd=0; sd<max_sd; sd++) {
    index = find_epoll_array_entry(sd);
    if (index != -1 && epoll_array[index].data.fd != -1) {
      printf("sd = %d index = %d data.fd = %d events = 0x%x\n",
              sd, index, epoll_array[index].data.fd, epoll_array[index].events);
    } 
  }
}

#endif /* HAVE_EPOLL */

/*----------------------------------------------------------------------*/
void
interest_set_print_info()
{

  PRINT_TIME(NOFD, &tnow, &tprev, "");

  switch (set_method) {
#ifdef HAVE_EPOLL
    case DECLARE_WITH_EPOLL:
      epoll_print_info();
      break;
#endif /* HAVE_EPOLL */


    case DECLARE_WITH_FDSET:
    case DECLARE_WITH_POLL:
    case DECLARE_WITH_SEND:
    case DECLARE_WITH_AIO:
    default:
      printf("interest_set_print_info: printing info for current method = %d not implemented yet\n",
              set_method);
      // exit(1);
      break;
  }
} /* interest_set_print_info */

