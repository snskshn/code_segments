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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/poll.h>

#include "interest_set.h"
#include "debug.h"
#include "options.h"
#include "cntl_conns.h"
#include "lru.h"
#include "q.h"
#include "sock_listener.h"
#include "sock_special.h"
#include "state.h"
#include "trace.h"

/*----------------------------------------------------------------------*/

void
process_poll_events(int n)
{
  extern struct pollfd *poll_array;
  extern int max_poll_entry;
  extern void compact_poll_array();
  int sd = -1;
  /* q_nodeptr p = NULL; */
  int i;
#ifdef TRACE_ON
  int save_n = n;
#endif

  TRACE_START_TIME(EVT_WORK_PHASE);

  PRINT_TIME(NOFD, &pnow, &pprev, "process_poll_events: entered "
      "(time process_poll_events only)");
  PRINT_TIME(NOFD, &pnow, &pprev, "process_poll_events: n = %d "
      "max_poll_entry = %d", n, max_poll_entry);


  switch (options.process_sds_order)
  {
      case OPT_PROCESS_SDS_UP:
	for (i=0; i<max_poll_entry; i++) {
	  if (event_set_is_readable_or_writable(poll_array[i].fd)) {
	    sd = poll_array[i].fd;
#define BUG_IN_POLL
#ifdef BUG_IN_POLL
            /* It seems that on 2.6.0 I'm getting write events on sd's 
	     * I've not indicated I'm interested in. 
	     * So a work around is to ensure that we should be reading/writing.
             */
	    if (sock_is_listener(sd)) {
	      do_readable(sd);
	    } else if (sock_is_special(sd)) {
	      if (event_set_is_readable(sd)) {
		do_readable(sd);
	      }
	      if (event_set_is_writable(sd)) {
		do_writable(sd);
	      }
	    } else {
	      if (event_set_is_readable(sd) && fsm_state_reading(sd)) {
		do_readable(sd);
	      }
	      if (event_set_is_writable(sd) && fsm_state_writing(sd)) {
		do_writable(sd);
	      }
            }
#else
	    if (event_set_is_readable(sd)) {
	      do_readable(sd);
	    }
	    if (event_set_is_writable(sd)) {
	      do_writable(sd);
	    }
#endif /* BUG_IN_POLL */
	    do_extra_accepts(n, options.extra_freq);
	    n--;
	  }
	} /* for */
	assert(n == 0);
	break;
    
      case OPT_PROCESS_SDS_DOWN:
	for (i=max_poll_entry-1; i>=0; i--) {
	  PRINT_TIME(NOFD, &tnow, &tprev, "process_poll_events: i = %d", i);

	  if (event_set_is_readable_or_writable(poll_array[i].fd)) {
	    sd = poll_array[i].fd;
	    PRINT_TIME(sd, &tnow, &tprev, 
		"process_poll_events: sd = %d", sd);
#ifdef BUG_IN_POLL
            /* It seems that on 2.6.0 I'm getting write events on sd's I've not indicated I'm 
             * interested in. So a work around is to ensure that we should be reading/writing.
             */
	    if (sock_is_listener(sd)) {
	      do_readable(sd);
	    } else if (sock_is_special(sd)) {
	      if (event_set_is_readable(sd)) {
		do_readable(sd);
	      }
	      if (event_set_is_writable(sd)) {
		do_writable(sd);
	      }
	    } else {
	      if (event_set_is_readable(sd) && fsm_state_reading(sd)) {
		do_readable(sd);
	      }
	      if (event_set_is_writable(sd) && fsm_state_writing(sd)) {
		do_writable(sd);
	      }
            }
#else
	    if (event_set_is_readable(sd)) {
	      do_readable(sd);
	    }
	    if (event_set_is_writable(sd)) {
	      do_writable(sd);
	    }
#endif /* BUG_IN_POLL */
	    do_extra_accepts(n, options.extra_freq);
	    n--;
	  }
	} /* for */
	assert(n == 0);
	break;

      /* Not done yet */
      case OPT_PROCESS_SDS_WRITES_FIRST_DOWN:
      case OPT_PROCESS_SDS_WRITES_FIRST_UP:
      case OPT_PROCESS_SDS_LRU:
      case OPT_PROCESS_SDS_LIFO:
      case OPT_PROCESS_SDS_FIFO:
        printf("process_poll_events: unimplemented option for "
	    "process_sds_order = %d\n",
                options.process_sds_order);
	fflush(stdout);
	break;

      default :
	fflush(stdout);
        printf("process_poll_events: unknown option for "
	    "process_sds_order = %d\n",
                options.process_sds_order);
	fflush(stdout);
	exit(1);
        break;

  } /* switch */

  compact_poll_array();

  PRINT_TIME(NOFD, &pnow, &pprev, "process_poll_events: returning "
      "(time process_sds_only)");

  TRACE_END_TIME(EVT_WORK_PHASE, save_n, 0);
} /* process_poll_events */

