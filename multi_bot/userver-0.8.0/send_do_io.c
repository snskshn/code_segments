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



#include <sys/poll.h>
#include <stdlib.h>

#include "debug.h"
#include "stats.h"
#include "trace.h"
#include "cntl_conns.h"
#include "state.h"
#include "errno.h"

#ifdef SEND
#include "send.h"
#endif

/*----------------------------------------------------------------------*/
/*
 * NOTE: it may look like sd and ip->sd are redundant here but ip->sd
 *       will change during this call (side effects) sd will not.
 *       And sd is used/need for debugging output.
 */
int
send_do_io(int sd, struct info *ip, int events) 
{
  int event_processed = 0;
  int rc;
#ifdef SEND
  evt_msgdata_t msg;
#endif /* SEND */

  assert(ip);
  PRINT_TIME(sd, &tnow, &tprev, 
      "send_do_io: fsm_state = %s events = 0x%x", 
      fsm_state_string(sd), events);

#ifdef SEND
  PRINT_TIME(sd, &tnow, &tprev, "send_do_io: fsm_state = %d", 
      ip->fsm_state);
  /* The connection has been previously closed by the client and we  */
  /* later get a FIN */
  if ((!event_processed) && (!info_map_valid(sd)) && (events & POLLFIN)) {
    /* This can actually happen from another state. */
    /* assert(ip->fsm_state == FSM_CLOSED); */
    num_pollfin_on_closed_consumed++;
    PRINT_TIME(sd, &tnow, &tprev, 
	"send_do_io: consuming POLLFIN on closed socket");
    event_processed = 1;
  }
#endif /* SEND */

  PRINT_TIME(sd, &tnow, &tprev, "send_do_io: fsm_state = %d", 
      ip->fsm_state);
  /* The connection has been closed and we later get notification that */
  /* we are clear to write more */
  if ((!event_processed) && (!info_map_valid(sd)) && (events & POLLOUT)) {
    /* This can actually happen from another state. */
    /* assert(ip->fsm_state == FSM_CLOSED); */
    num_pollout_on_closed_consumed++;
    PRINT_TIME(ip->sd, &tnow, &tprev, 
	"send_do_io: consuming POLLOUT on closed socket");
    event_processed = 1;
  }

  PRINT_TIME(sd, &tnow, &tprev,
      "send_do_io: fsm_state = %d", ip->fsm_state);
  if ((!event_processed) && (info_map_valid(sd)) && 
      (events & POLLERR)) {
    PRINT_TIME(sd, &tnow, &tprev,
       	"send_do_io: POLLERR calling do_close");
    num_pollerr_on_open_socket++;
    do_close(ip, REASON_SEND_POLL_ERROR);
    event_processed = 1;
  }

#ifdef SEND
  PRINT_TIME(sd, &tnow, &tprev, "send_do_io: fsm_state = %d", 
      ip->fsm_state);
  /* The connection has been closed */
  if ((!event_processed) && (info_map_valid(sd)) && (events & POLLFIN)) {
    if (ip->fsm_state == FSM_READING_NEXT_REQUEST) {
      PRINT_TIME(sd, &tnow, &tprev, 
	  "send_do_io: got POLLFIN close normal");
      num_close_send_normal++;
    } else {
      PRINT_TIME(sd, &tnow, &tprev, 
	  "send_do_io: got POLLFIN close early");
      num_close_send_early_fin++;
    }

    PRINT_TIME(sd, &tnow, &tprev, "send_do_io: got calling do_close");
    rc = do_close(ip, REASON_SEND_POLL_FIN);
    event_processed = 1;
  }
#endif /* SEND */

  PRINT_TIME(sd, &tnow, &tprev,
      "send_do_io: fsm_state = %d", ip->fsm_state);
  if ((!event_processed) && (info_map_valid(sd)) && 
      ((ip->fsm_state == FSM_READING_REQUEST) || 
       (ip->fsm_state == FSM_READING_NEXT_REQUEST)) && 
      (events & POLLIN)) {
    PRINT_TIME(sd, &tnow, &tprev, 
	"send_do_io: calling socket_readable");
    rc = socket_readable(ip);

    if (rc == -EAGAIN) {
#ifdef SEND
      if (events & POLLREADRETRY) {
        num_socket_read_retry_failed++;
      } else {
        num_socket_read_eagain2++;

	msg.data = (void *) ip->sd;
	msg.len = sizeof(int);
	msg.dest = getpid();
	rc = evtctl(EVT_ANY, EVT_SENDMSG, &msg, sizeof(msg));
	if (rc < 0) {
	   printf("evtctl failed rc = %d errno = %d\n", rc, errno);
	   fflush(stdout);
	}
      }
#else
      /* Should not get indication for read if can't actually read */
      assert(1);
#endif /* SEND */
    }
    event_processed = 1;
  } 

  PRINT_TIME(sd, &tnow, &tprev, "send_do_io: fsm_state = %d", 
      ip->fsm_state);
  if ((!event_processed) && (info_map_valid(sd)) && 
      (ip->fsm_state == FSM_WRITING_REPLY) && (events & POLLOUT)) {
    PRINT_TIME(sd, &tnow, &tprev, 
	"send_do_io: calling socket_writable");
    rc = socket_writable(ip);
    event_processed = 1;
  }

  /* System tells us that we can write again when socket buffer is empty */
  /* Don't call socket_writable unless we have something to write */
  if ((!event_processed) && (info_map_valid(sd)) && 
      (ip->fsm_state != FSM_WRITING_REPLY) && 
      ((events & POLLOUT) && !(events & POLLERR))) {
    PRINT_TIME(sd, &tnow, &tprev,
       	"send_do_io: consuming POLLOUT event = 0x%x", events);
    num_pollout_consumed++;
    event_processed = 1;
  }

#ifdef POSSIBLE_CHANGE_FOR_ABOVE
  /* System tells us that we can write again when socket buffer is empty */
  /* Don't call socket_writable unless we have something to write */
  if ((!event_processed) && (ip->sd != -1) && 
      ((ip->fsm_state != FSM_WRITING_REPLY) ||
       (ip->fsm_state != FSM_READING_NEXT_REQUEST)) && 
      ((events & POLLOUT) && !(events & POLLERR))) {
    PRINT_TIME(sd, &tnow, &tprev,
       	"send_do_io: consuming POLLOUT event = 0x%x", events);
    num_pollout_consumed++;
    event_processed = 1;
  }
#endif

  PRINT_TIME(sd, &tnow, &tprev,
      "send_do_io: fsm_state = %d", ip->fsm_state);
  if ((!event_processed) && (info_map_valid(sd)) && (events & POLLERR)) {
    PRINT_TIME(sd, &tnow, &tprev,
       	"send_do_io: got POLLERR on closed socket - consuming event");
    /* rc = do_close(ip); */
    num_pollerr_on_closed_consumed++;
    event_processed = 1;
  }

#ifdef SEND
  PRINT_TIME(sd, &tnow, &tprev,
      "send_do_io: fsm_state = %d", ip->fsm_state);
  if ((!event_processed) && (info_map_valid(sd)) && (events & POLLHINT)) {
    PRINT_TIME(sd, &tnow, &tprev,
       	"send_do_io: got POLLHINT on closed socket - consuming event");
    /* rc = do_close(ip); */
    num_pollhint_on_closed_consumed++;
    event_processed = 1;
  }

  PRINT_TIME(sd, &tnow, &tprev,
      "send_do_io: fsm_state = %d", ip->fsm_state);
  if ((!event_processed) && (info_map_valid(sd)) && 
      (events == POLLHINT)) {
    PRINT_TIME(sd, &tnow, &tprev,
       	"send_do_io: consuming POLLHINT event = 0x%x", events);
    num_pollhint_only_consumed++;
    event_processed = 1;
  }
#endif /* SEND */

  PRINT_TIME(sd, &tnow, &tprev,
      "send_do_io: fsm_state = %d", ip->fsm_state);
  if ((!event_processed) && (info_map_valid(sd)) && (events == 0)) {
    PRINT_TIME(sd, &tnow, &tprev,
       	"send_do_io: consuming empty event = 0x%x", events);
    num_empty_events_consumed++;
    event_processed = 1;
  }

  PRINT_TIME(sd, &tnow, &tprev,
      "send_do_io: fsm_state = %d", ip->fsm_state);


  if (!event_processed) {
    printf("send_do_io: WARNING - event not processed\n");
    printf("send_do_io: sd = %d events = 0x%x\n", sd, events);
    printf("send_do_io: ip->fsm_state = %d ip->sd = %d\n", 
      ip->fsm_state, ip->sd);
  }

  conns_off_if_needed();
  return(event_processed);
} /* send_do_io */
