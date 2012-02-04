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


#ifdef HAVE_EPOLL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#ifdef HAVE_EPOLL

#ifdef PROVIDE_EPOLL_HEADERS
#include "sys_epoll.h"
#else
#include <sys/epoll.h>
#endif

#ifdef IA64
/* Uncomment this if you can point to the real deal */
/* #include <asm-ia64/types.h> */
#endif /* IA64 */
#endif /* HAVE_EPOLL */

#include "interest_set.h"
#include "debug.h"
#include "options.h"
#include "cntl_conns.h"
#include "sock_listener.h"
#include "lru.h"
#include "q.h"
#include "state.h"
#include "stats.h"
#include "trace.h"

#define   READABLE  (0x1)
#define   WRITABLE  (0x2)
#define   BOTH      (0x3)

#ifdef FIXHUP
#define MAXADDRLEN   (100)
#endif /* FIXHUP */

/*----------------------------------------------------------------------*/
int epoll_is_readable_or_writable(uint32_t evt);
int epoll_is_writable(uint32_t evt);
int epoll_is_readable(uint32_t evt);
int epoll_is_hungup(uint32_t evt);

int do_epoll_event(int n, struct epoll_event *evt, int *did_something);
int do_epoll_event_et(int n, struct epoll_event *evt, int *did_something, int *did_accept);


int
process_epoll_events(int n)
{
  extern struct epoll_event *epoll_results;
#ifdef DEBUG_ON
  extern int max_epoll_entry;
#endif /* DEBUG_ON */

#ifdef NEED_TO_ADD_DID_SOMETHING
  int sd = -1;
#endif /* NEED_TO_ADD_DID_SOMETHING */

  TRACE_START_TIME(EVT_WORK_PHASE);

  /* q_nodeptr p = NULL; */
  int i;
  int num_fds = n;
  int did_something = 0;
  int did_accept = 0;
  int handled_event = 0;

  PRINT_TIME(NOFD, &pnow, &pprev, "process_epoll_events: entered "
      "(time process_epoll_events only)");
  PRINT_TIME(NOFD, &pnow, &pprev, "process_epoll_events: n = %d "
      "max_epoll_entry = %d", n, max_epoll_entry);


  switch (options.process_sds_order)
  {
      case OPT_PROCESS_SDS_DOWN:
	for (i=num_fds-1; i>=0; i--) {

	  handled_event = 0;

	  if (options.epoll_edge_triggered) {
	    n = do_epoll_event_et(n, &epoll_results[i], &handled_event, &did_accept);
	  } else {
	    n = do_epoll_event(n, &epoll_results[i], &handled_event);
	  }

          if (handled_event) {
	    did_something = 1;
	  } else {
	     PRINT_TIME(NOFD, &tnow, &tprev, "process_epoll_events: " 
	        "did not handle sd = %d evts = 0x%x\n",
	         epoll_results[i].data.fd, epoll_results[i].events);
	  }
	} /* for */

	assert(n == 0);
	break;

      case OPT_PROCESS_SDS_UP:
	for (i=0; i<num_fds; i++) {
	  n = do_epoll_event(n, &epoll_results[i], &did_something);
	} /* for */
	assert(n == 0);
	break;

#ifdef NEED_TO_ADD_DID_SOMETHING
      case OPT_PROCESS_SDS_WRITES_FIRST_DOWN:
	PRINT_TIME(NOFD, &tnow, &tprev, "process_epoll_events: "
	    "starting writes-first-down writable ");
	for (i=num_fds-1; n>0 && i>=0; i--) {
	  if (epoll_is_writable(epoll_results[i].events)) {
	    --n;
	    sd = epoll_results[i].data.fd;
	    PRINT_TIME(sd, &tnow, &tprev, 
		"process_epoll_events: do_writable ");
	    do_writable(sd);
	    do_extra_accepts(n, options.extra_freq);
	  }
	} /* for */

	PRINT_TIME(NOFD, &tnow, &tprev, "process_epoll_events: "
	    "starting writes-first-down readable ");
	for (i=num_fds-1; n>0 && i>=0; i--) {
	  conns_off_if_needed();
	  if (epoll_is_readable(epoll_results[i].events)) {
	    --n;
	    sd = epoll_results[i].data.fd;
	    PRINT_TIME(sd, &tnow, &tprev, 
		"process_epoll_events: do_readable ");
	    do_readable(sd);
	    do_extra_accepts(n, options.extra_freq);
	  }
	} /* for */
	assert(n == 0);
	break;

      case OPT_PROCESS_SDS_WRITES_FIRST_UP:
	for (i=0; n>0 && i<num_fds; i--) {
	  /* DEBG(MSG_PROCESS_SDS, "sd = %d\n", sd); */
	  conns_off_if_needed();
	  if (epoll_is_writable(epoll_results[i].events)) {
	    --n;
	    sd = epoll_results[i].data.fd;
	    PRINT_TIME(sd, &tnow, &tprev, 
		"process_epoll_events: do_writable ");
	    do_writable(sd);
	    do_extra_accepts(n, options.extra_freq);
	  }
	} /* for */

	for (i=0; n>0 && i<num_fds; i--) {
	  /* DEBG(MSG_PROCESS_SDS, "sd = %d\n", sd); */
	  conns_off_if_needed();
	  if (epoll_is_readable(epoll_results[i].events)) {
	    --n;
	    sd = epoll_results[i].data.fd;
	    PRINT_TIME(sd, &tnow, &tprev, 
		"process_epoll_events: do_readable ");
	    do_readable(sd);
	    do_extra_accepts(n, options.extra_freq);
	  }
	} /* for */
	assert(n == 0);
	break;
#endif /* NEED_TO_ADD_DID_SOMETHING */

      default :
	fflush(stdout);
        printf("process_epoll_events: unknown or unhandled option for "
	    "process_sds_order = %d\n",
                options.process_sds_order);
	fflush(stdout);
	exit(1);
        break;

  } /* switch */


  PRINT_TIME(NOFD, &pnow, &pprev, "process_epoll_events: returning "
      "did_something = %d (time process_sds_only)", did_something);

  TRACE_END_TIME(EVT_WORK_PHASE, num_fds, 0);
  return(did_something);
} /* process_epoll_events */

int
do_epoll_event(int n, struct epoll_event *evt, int *did_something)
{
  int sd;
  int retval = 1;
  struct info *ip;

  sd = evt->data.fd;
  PRINT_TIME(sd, &tnow, &tprev, "do_epoll_event: n = %d "
    "events = 0x%x fd = %d", n, evt->events, evt->data.fd);
 
  ip = info_ptr(sd);
  if (epoll_is_hungup(evt->events) ) {
    /* epoll reported an error, so close the connection down */
    PRINT_TIME(sd, &tnow, &tprev, "do_epoll_event: epoll got EPOLLHUP or EPOLLERR");

    /*
     * When epoll hangs up we need to close the client connection properly. We
     * can't just call do_close(), because then any appserver that is servicing
     * the client won't get released. Instead we let either do_readable or
     * do_writable detect the error and close things down properly.
     */

    /* do_close(ip); */

#ifdef FIXHUP
    // get_peer_addr_str(sd, clientaddr, MAXADDRLEN-1);
    // printf("got hup: sd = %d fsm_state = %d client addr = %s\n", sd, ip->fsm_state, clientaddr);
    if (fsm_state_reading(sd)) {
      do_readable(sd);
      *did_something = 1;
    } else if (fsm_state_writing(sd)) {
      do_writable(sd);
      *did_something = 1;
    } else {
      PRINT_TIME(sd, &tnow, &tprev, "do_epoll_event: epoll got EPOLLHUP or EPOLLERR and did not read or write");
      switch(get_fsm_state(sd)) {
        /* Not quite sure which of these can actually happen,
         * as long as the sd is still in the epoll_fd set it will
         * get EPOLLHUPs (even if we aren't interested in them).
         * Need to look more carefully at the code to see when
         * the sd is removed from the epoll_fd set.
         *
         * It may be better to avoid getting them altogether
         * by removing the sd from the interest set while
         * we are not communicating with the client but 
         * are instead communicating with the app server.
         */
        case FSM_FCGI_WRITING_SET_VALUES:
        case FSM_FCGI_WAITING_FOR_REPLY:
        case FSM_FCGI_WRITING_BEGIN_REQUEST:
        case FSM_FCGI_WRITING_PARAMS_1:
        case FSM_FCGI_WRITING_PARAMS_2:
        case FSM_FCGI_WRITING_END_PARAMS:
        case FSM_FCGI_WRITING_STDIN:
        case FSM_FCGI_WRITING_END_STDIN:
        case FSM_FCGI_REQUEST_QUEUED:
	  PRINT_TIME(sd, &tnow, &tprev, "do_epoll_event: converting to hupped");
	  convert_to_hupped(ip);
          break;

        default:
          printf("got HUP and fsm state not as expected state = %s\n", get_fsm_state_string(ip->sd));
	  printf("NOT SURE HOW OR WHY WE ARE HERE!\n");
	  assert(0);
          break;
      }
    }
#else
    if (fsm_state_reading(sd)) {
      do_readable(sd);
      *did_something = 1;
    } else if (fsm_state_writing(sd)) {
      do_writable(sd);
      *did_something = 1;
    }
#endif

#if 0
    /* we really do want to set this as done above now if
     * something was done
     */
    *did_something  = 0;
#endif
    if (evt->events & EPOLLERR) num_epoll_epollerr++;
    if (evt->events & EPOLLHUP) num_epoll_epollhup++;

  } else {

    /* epoll gave an event(s), so process that event(s) */
    if (options.use_epoll) {
      if (epoll_is_readable(evt->events)) {
	   do_readable(sd);
           *did_something = 1;
      }
      if (epoll_is_writable(evt->events)) {
       if(ip->type != INFO_CLIENT || fsm_state_writing(sd) ) { 
         do_writable(sd);
         *did_something = 1;
       } 
      }
    } else if (options.use_epoll2) {
      if ((sock_is_listener(sd)) && (epoll_is_readable(evt->events))) {
        do_readable(sd);
        *did_something = 1;
      } else {
        if (info_map_valid(sd)) {
	  if ((fsm_state_reading(sd)) && (epoll_is_readable(evt->events))) {
	    retval = do_readable(sd);
            *did_something = 1;
	  }
        }
        if (info_map_valid(sd)) {
	  if ((fsm_state_writing(sd)) && (epoll_is_writable(evt->events))) {
	    do_writable(sd);
            *did_something = 1;
	  }
        }
      }
    } else {
      printf("do_epoll_event: doing epoll but undefined as to which type\n");
      exit(1);
    }
  }

  do_extra_accepts(n, options.extra_freq);
  return(n-1);
}


int
do_epoll_event_et(int n, struct epoll_event *evt, 
   int *did_something, int *did_accept)
{
  int sd;
  int retval = 1;
  struct info *ip = 0;

  sd = evt->data.fd;
  PRINT_TIME(sd, &tnow, &tprev, "do_epoll_event_et: n = %d "
    "events = 0x%x fd = %d", n, evt->events, evt->data.fd);

  assert(options.epoll_edge_triggered);
  assert(sd >= 0);

  ip = info_ptr(sd);

  if (ip) {
    sd_state_update(ip, evt->events);
  } else {
    if (!sock_is_listener(sd)) {
      printf("do_epoll_event_et: sd = %d evt = 0x%x\n", sd, evt->events);
      exit(1);
    }
  }


  if (sock_is_listener(sd)) {
    sd_state_listener_readable(sd);
    do_readable(sd);
    *did_something = 1;
    *did_accept = 1;
  } else {

    /* NOTE info_map_valid can change as a result of these calls that's why it
     * gets checked in both places
     */
    if (info_map_valid(sd)) {
      PRINT_TIME(sd, &tnow, &tprev, "do_epoll_event_et: fsm_state = %s ip->sd_state = 0x%x", 
           get_fsm_state_string(sd), ip->sd_state);
      if (fsm_state_reading(sd) && epoll_is_readable(ip->sd_state)) {
#ifdef dpariag_loop_read
	do { 
          retval = do_readable(sd); 
        } while ( retval == 1 );
#else 
	retval = do_readable(sd);
#endif
	*did_something = 1;
      }
    } else {
      PRINT_TIME(sd, &tnow, &tprev, "do_epoll_event_et: info_map not valid 1\n");
    }

    /* NOTE info_map_valid can change as a result of these calls that's why it
     * gets checked in both places
     */

    /* After reading above we may have transitioned to writable, if so
     * and the socket is writable, we should write
     */
    if (info_map_valid(sd)) {
      if (fsm_state_writing(sd) && epoll_is_writable(ip->sd_state)) {
	do_writable(sd);
	*did_something = 1;

	/* After writing the last bit of the file we transition to reading
	 * and it's possible that the next request is there and ready to go
	 * I don't believe that it's possible to go beyond this many events
	 * because we won't have a write notification yet.
	 */

        if (info_map_valid(sd)) {
  	  if (fsm_state_reading(sd) && epoll_is_readable(ip->sd_state)) {
	    retval = do_readable(sd);
	    *did_something = 1;
	  }
	}
      }

    } else {
      PRINT_TIME(sd, &tnow, &tprev, "do_epoll_event_et: info_map not valid 2\n");
    }

  }

  PRINT_TIME(sd, &tnow, &tprev, "do_epoll_event_et: did_something = %d\n",
    *did_something);
  do_extra_accepts(n, options.extra_freq);
  return(n-1);
}




int
epoll_is_readable_or_writable(uint32_t evt)
{
  int retval = 0;
  if (epoll_is_readable(evt)) {
    retval = READABLE;
  }
  if (epoll_is_writable(evt)) {
    retval |= WRITABLE;
  }

  return(retval);
}

int 
epoll_is_writable(uint32_t evt)
{
  int retval = 0;
  if (evt & EPOLLOUT) {
    retval = 1;
  }
  return(retval);
}

int 
epoll_is_readable(uint32_t evt)
{
  int retval = 0;
  if (evt & EPOLLIN) {
    retval = 1;
  }
  return(retval);
}

int 
epoll_is_hungup(uint32_t evt)
{
  int retval = 0;
  if (evt & EPOLLHUP || evt & EPOLLERR) {
    retval = 1;
  }
  return(retval);
}

#endif /* HAVE_EPOLL */
