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



#include <sys/fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "cntl_conns.h"
#include "cntl_intr.h"
#include "debug.h"
#include "options.h"
#include "stats.h"
#include "trace.h"
#include "interest_set.h"
#include "sock_listener.h"


#ifdef SEND
#include "send.h"
#endif /* SEND */

/*
 * Change History:
 *
 * socket_new_conn_off: now uses FD_CLR instead of FD_SET.
 *
 */


int new_connections_on = 1;
int should_try_accept = 0;
#ifdef SEND
extern int send_readfds;
#ifdef ARRAY_OF_BUFS
extern int free_ecb_slots_in_buf(int buf_index);
#endif /* ARRAY_OF_BUFS */
#endif /* SEND */



/* ---------------------------------------------------------------------- */
int
should_enable_new_conns(void)
{
  int retval = 0;
#ifdef SEND
  int free_slots;
#endif /* SEND */

  if (new_connections_on) {
    return(0);
  }

  DEBG(MSG_CNTL_CONNS,
      "should_enable_new_conns: entered in_hander = %d in_cs = %d\n",
      in_handler, in_cs);

#ifdef SEND
  if (options.auto_accept) {
#ifdef ARRAY_OF_BUFS
    free_slots = free_ecb_slots_in_buf(EVT_ACCEPT_BUF);
#else
    free_slots = free_ecb_slots();
#endif /* ARRAY_OF_BUFS */
    PRINT_TIME(NOFD, &tnow, &tprev, 
        "should_enable_new_conns: free_ecb_slots = %d", free_slots);
    PRINT_TIME(NOFD, &tnow, &tprev, "should_enable_new_conns: thold high = %d", 
        options.ecb_threshold_high);
    PRINT_TIME(NOFD, &tnow, &tprev, "should_enable_new_conns: num_idle = %d "
        "free_fd_thold = %d\n", num_idle, options.free_fd_thold);
    
    /* NOTE: we can't use num_idle here because with auto accept there */
    /*       can be a bunch of accepted connections in the ecb buffer */
    /*       who already have fd's assigned. */
    
    if (num_idle > options.free_fd_thold) {
      PRINT_TIME(NOFD, &tnow, &tprev, 
          "should_enable_new_conns: setting retval = 1\n");
      retval = 1;
    } else {
      PRINT_TIME(NOFD, &tnow, &tprev, 
          "should_enable_new_conns: setting retval = 0\n");
      retval = 0;
    }
  } else {
#endif /* SEND */
    if (num_idle > options.free_fd_thold) {
      retval = 1;
    } else {
      retval = 0;
    }

    if (options.use_socket_aio) {
      if ((maxconns - num_idle) - cur_conns < options.aio_accept_thold) {
        retval = 1;
      }
    }

#ifdef SEND
  }
#endif /* SEND */


  PRINT_TIME(NOFD, &tnow, &tprev, 
      "should_enable_new_conns: returning %d\n", retval);
  return(retval);
}


/* ---------------------------------------------------------------------- */

/* only needs to happen with SIGIO enabled because otherwise the 
 * number of connections can't change while processing sds.
 */
void
conns_off_if_needed(void)
{
#ifdef SEND
  int free_slots;
#endif /* SEND */

  PRINT_TIME(NOFD, &tnow, &tprev,
      "conns_off_if_needed: sigio_blocked = %d in_handler = %d",
       sigio_blocked, in_handler);
  PRINT_TIME(NOFD, &tnow, &tprev,
      "conns_off_if_needed: get_connections = %d num_idle = %d", 
       options.get_connections, num_idle);
  PRINT_TIME(NOFD, &tnow, &tprev,
      "conns_off_if_needed: new_connections_on = %d", 
       new_connections_on);

  switch (options.get_connections) {
    case OPT_CONN_WITH_SIGIO:
      if ((!sigio_blocked) && 
          (new_connections_on && num_idle <= options.free_fd_thold)) {
	PRINT_TIME(NOFD, &tnow, &tprev,
	    "conns_off_if_needed: calling socket_new_conn_off");
	socket_new_conn_off();
      }
      break;

#ifdef SEND
    case OPT_CONN_WITH_SEND_EVTS:
#ifdef ARRAY_OF_BUFS
      free_slots = free_ecb_slots_in_buf(EVT_ACCEPT_BUF);
#else
      free_slots = free_ecb_slots();
#endif /* ARRAY_OF_BUFS */
      PRINT_TIME(NOFD, &tnow, &tprev,
	  "conns_off_if_needed: queue = 0x%x notify = 0x%x",
	  ecb->queue, ecb->notify);
      PRINT_TIME(NOFD, &tnow, &tprev,
	  "conns_off_if_needed: free_ecb_slots = %d", free_slots);
      if (options.auto_accept) {
	/* NOTE: we can't use num_idle here because with auto accept there */
	/*       can be a bunch of accepted connections in the ecb buffer */
	/*       who already have fd's assigned. */
	if ((new_connections_on) && 
	    (free_slots <= options.ecb_threshold_low)) {
	  PRINT_TIME(NOFD, &tnow, &tprev,
	      "conns_off_if_needed: auto_accept -> socket_new_conn_off");
	  socket_new_conn_off();
	}
      } else if ((!sigio_blocked) && 
	  (new_connections_on && num_idle <= options.free_fd_thold)) {
	PRINT_TIME(NOFD, &tnow, &tprev,
	    "conns_off_if_needed: calling socket_new_conn_off");
	socket_new_conn_off();
      }
      break;
#endif /* SEND */

    case OPT_CONN_WITH_AIO_ACCEPT:
      if ((new_connections_on && num_idle <= options.free_fd_thold) ||
         (options.aio_accept_thold != 0 && new_connections_on && 
	  (maxconns - num_idle) - cur_conns >= options.aio_accept_thold)) {
	PRINT_TIME(NOFD, &tnow, &tprev,
	    "conns_off_if_needed: calling socket_new_conn_off");
	socket_new_conn_off();
      }
      break;

    default :
      /* Don't turn connections off this way for other types of connections. */
      /* So there isn't anything to do */
      break;

  } /* switch */
  
} /* conns_off_if_needed */


/* ---------------------------------------------------------------------- */
void
do_socket_new_conn_on(void)
{
  int i;
  int rc = 0;
  int trace_fd = 0;

  PRINT_TIME(NOFD, &tnow, &tprev, "do_socket_new_conn_on: entering");
  assert(new_connections_on == 0);
  new_connections_on = 1;
  num_new_conns_on++;

  DEBG(MSG_CNTL_CONNS, "%15d new_conns_on :\n", num_new_conns_on);
  DEBG(MSG_CNTL_CONNS, "%s\n",
      (num_new_conns_off != num_new_conns_on) ? "UNEQUAL" : "EQUAL");

  switch (options.conn_on) {
      case OPT_CONN_WITH_SELECT_POLL_EPOLL:
	 for (i=sock_listener_min; i<=sock_listener_max; i++) {
	   if (sock_is_listener(i)) {
	     TRACE(EVT_UNBLOCK_CONNS,
	       trace_fd = i;
	       interest_set_change(i, ISET_READABLE);
	       rc = i;
	     );
	   }
	 }

	 if (options.epoll_edge_triggered) {
            should_try_accept = 1;
	 }
	 break;

#ifdef SEND
      case OPT_CONN_WITH_SEND_SELECT:
	 for (i=sock_listener_min; i<=sock_listener_max; i++) {
	   if (sock_is_listener(i)) {
	     TRACE(EVT_UNBLOCK_CONNS,
	       trace_fd = i;
	       FD_SET(i, (fd_set *) &send_readfds);
	       rc = i;
	     );
	   }
	 }
	 break;
#endif /* SEND */

      case OPT_CONN_WITH_SIGIO:
	 /* trace_fd and rc are just placeholders here */
	 TRACE(EVT_UNBLOCK_CONNS,
	   trace_fd = sock_listener_min;
	   unblock_sigio();
	   rc = sock_listener_max;
	 );
	 break;

#ifdef SEND
      case OPT_CONN_WITH_SEND_EVTS:
         if (options.auto_accept) {
	   /* trace_fd and rc are just placeholders here */
	   TRACE(EVT_UNBLOCK_CONNS,
	     trace_fd = sock_listener_min;
	     unblock_auto_accept_delivery();
	     rc = sock_listener_max;
	   );
         } else {
	   TRACE(EVT_UNBLOCK_CONNS,
	     /* trace_fd and rc are just placeholders here */
	     trace_fd = sock_listener_min;
	     unblock_send_events();
	     rc = sock_listener_max;
	   );
         }
	 break;
#endif /* SEND */

      case OPT_CONN_WITH_AIO_ACCEPT:
	 break;

      default :
          printf("do_socket_new_conn_on: unhandled option conn_on = %d\n",
	          options.conn_on); 
	  assert(0);
	  exit(1);
          break;
  } /* switch */

  PRINT_TIME(NOFD, &tnow, &tprev, "do_socket_new_conn_on: returning");
} /* do_socket_new_conn_on */


/* ---------------------------------------------------------------------- */
void
do_socket_new_conn_off(void)
{
  int rc;
  int trace_fd = 0;
  int i;

  assert(new_connections_on == 1);
  new_connections_on = 0;
  num_new_conns_off++;

  DEBG(MSG_CNTL_CONNS, "%15d new_conns_off\n", num_new_conns_off);

  PRINT_TIME(NOFD, &tnow, &tprev, "do_socket_new_conn_off: entering");
  switch (options.conn_off) {
      case OPT_CONN_WITH_SELECT_POLL_EPOLL:
	 for (i=sock_listener_min; i<=sock_listener_max; i++) {
	   if (sock_is_listener(i)) {
	     TRACE(EVT_BLOCK_CONNS,
	       trace_fd = i;
	       interest_set_change(i, ISET_NOT_READABLE);
	       rc = i;
	     );
	   }
	 }
	 break;

#ifdef SEND
      case OPT_CONN_WITH_SEND_SELECT:
	 for (i=sock_listener_min; i<=sock_listener_max; i++) {
	   if (sock_is_listener(i)) {
	     TRACE(EVT_BLOCK_CONNS,
	       trace_fd = i;
	       FD_CLR(i, (fd_set *) &send_readfds);
	       rc = i;
	     );
	   }
	 }
	 break;
#endif /* SEND */

      case OPT_CONN_WITH_SIGIO:
	 /* trace_fd and rc are just placeholders here */
	 TRACE(EVT_BLOCK_CONNS,
	   trace_fd = sock_listener_min;
	   block_sigio();
	   rc = sock_listener_max;
	 );
	 break;

#ifdef SEND
      case OPT_CONN_WITH_SEND_EVTS:
         if (options.auto_accept) {
  	   PRINT_TIME(NOFD, &tnow, &tprev,
	       "do_socket_new_conn_off: calling block_auto_accept_delivery");
	   /* trace_fd and rc are just placeholders here */
	   TRACE(EVT_BLOCK_CONNS,
	     trace_fd = sock_listener_min;
	     block_auto_accept_delivery();
	     rc = sock_listener_max;
	   );
         } else {
  	   PRINT_TIME(NOFD, &tnow, &tprev,
	       "do_socket_new_conn_off: calling block_send_events");
	   /* trace_fd and rc are just placeholders here */
	   TRACE(EVT_BLOCK_CONNS,
	     trace_fd = sock_listener_min;
	     block_send_events();
	     rc = sock_listener_max;
	   );
         }
	 break;
#endif /* SEND */

      case OPT_CONN_WITH_AIO_ACCEPT:
	if ((new_connections_on && num_idle <= options.free_fd_thold) ||
	   (new_connections_on && 
	    (maxconns - num_idle) - cur_conns >= options.aio_accept_thold)) {
	  PRINT_TIME(NOFD, &tnow, &tprev,
	      "do_socket_new_conn_off: calling socket_new_conn_off");
	  socket_new_conn_off();
	}
	break;

      default :
          printf("do_socket_new_conn_off: unhandled option conn_off = %d\n",
	          options.conn_off); 
	  assert(0);
	  exit(1);
          break;
  } /* switch */

  PRINT_TIME(NOFD, &tnow, &tprev, "do_socket_new_conn_off: returning");
} /* do_socket_new_conn_off */
/* ---------------------------------------------------------------------- */

