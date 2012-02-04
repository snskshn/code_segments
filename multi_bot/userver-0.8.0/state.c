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

#include "stats.h"
#include "interest_set.h"
#include "state.h"
#include "options.h"
#include "debug.h"
#include "common.h"
#ifdef PROVIDE_EPOLL_HEADERS
#include "sys_epoll.h"
#else
#ifdef __linux__
#include <sys/epoll.h>
#endif
#endif /* PROVIDE_EPOLL_HEADERS */

/* #define LOCAL_DEBUG */
#include "local_debug.h"

extern void sleep_usecs(int usecs);

/* NOTE This must agree with the definitions in info.h !!! */
char *fsm_string[] = {
  /* 0 */  "FSM_UNKNOWN", 
  /* 1 */  "FSM_INITIAL_STATE",
  /* 2 */  "FSM_CONNECTING",
  /* 3 */  "FSM_READING_REQUEST",
  /* 4 */  "FSM_READ_ARRIVED_WHILE_WRITING",
  /* 5 */  "FSM_WRITING_REPLY",
  /* 6 */  "FSM_READING_NEXT_REQUEST",
  /* 7 */  "FSM_CLOSING",
  /* 8 */  "FSM_CLOSED",
  /* 9 */  "FSM_LISTENING",
  /* 10 */ "FSM_FCGI_WRITING_SET_VALUES",
  /* 11 */ "FSM_FCGI_WRITING_BEGIN_REQUEST",
  /* 12 */ "FSM_FCGI_WRITING_PARAMS_1",
  /* 13 */ "FSM_FCGI_WRITING_PARAMS_2",
  /* 14 */ "FSM_FCGI_WRITING_END_PARAMS",
  /* 15 */ "FSM_FCGI_WRITING_STDIN",
  /* 16 */ "FSM_FCGI_WRITING_END_STDIN",
  /* 17 */ "FSM_FCGI_WAITING_FOR_REPLY",
  /* 18 */ "FSM_FCGI_REQUEST_QUEUED",
  /* 19 */ "FSM_FCGI_CLEARING_BUFFER",
  /* 20 */ "FSM_FCGI_UNBLOCKING",
  /* 21 */ "FSM_CONN_ERROR",
};

/* ---------------------------------------------------------------------- */

void
set_fsm_state(struct info *ip, enum info_fsm_state state)
{
  assert(ip != NULL);
  assert(state < FSM_STATE_MAX);
  ip->fsm_state = state;
#ifdef TRACK_STUCK_CONNS
  ip->fsm_state_history |= (1 << state);
#endif /* TRACK_STUCK_CONNS */
  PRINT_TIME(ip->sd, &tnow, &tprev, "set_fsm_state: state = %d [%s] ",
      ip->fsm_state, fsm_string[ip->fsm_state]);
  if (options.state_delay != OPT_NO_STATE_DELAY) {
    if (options.state_delay == state) {
      PRINT_TIME(ip->sd, &tnow, &tprev, "set_fsm_state: start delay ");
      sleep_usecs(options.delay);
      PRINT_TIME(ip->sd, &tnow, &tprev, "set_fsm_state: end delay ");
    }
  }
}

int
get_fsm_state(int sd)
{
  struct info *ip;

  assert(sd > 0);
  assert(sd < max_fds);

  ip = info_ptr(sd);
  assert(ip);
  
  PRINT_TIME(ip->sd, &tnow, &tprev, "get_fsm_state: state = %d [%s]",
      ip->fsm_state, fsm_string[ip->fsm_state]);

  return(ip->fsm_state);
}

char *
get_fsm_state_string(int sd)
{
  struct info *ip;
  assert(sd > 0);
  assert(sd < max_fds);

  ip = info_ptr(sd);
  assert(ip);
  
  PRINT_TIME(ip->sd, &tnow, &tprev, "get_fsm_state: state = %d [%s]",
      ip->fsm_state, fsm_string[ip->fsm_state]);

  return(fsm_string[ip->fsm_state]);
}

int
fsm_state_reading(int sd)
{
  int retval = 0;
  int fsm_state = get_fsm_state(sd);

  if ((fsm_state == FSM_READING_REQUEST) || 
      (fsm_state == FSM_READING_NEXT_REQUEST) ||
      (fsm_state == FSM_LISTENING)) {
    retval = 1;
  } else {
    retval = 0;
  }
  
  PRINT_TIME(sd, &tnow, &tprev, "fsm_state_reading: retval = %d", retval);
  return(retval);
}

int
fsm_state_writing(int sd)
{
  int retval = 0;
  int fsm_state = get_fsm_state(sd);

  if (fsm_state == FSM_WRITING_REPLY) {
    retval = 1;
  } else {
    retval = 0;
  }
  
  PRINT_TIME(sd, &tnow, &tprev, "fsm_state_writing: retval = %d", retval);
  return(retval);
}

char *
fsm_state_string(int sd)
{
  return(fsm_string[get_fsm_state(sd)]);
}

char *
fsm_state_to_string(enum info_fsm_state state)
{
  return(fsm_string[state]);
}


#ifdef FIXHUP
void
convert_to_hupped(struct info *client_ip) 
{
    /* the client connection has had a problem (e.g., timeout/connection reset)
     * so we want to mark it as dead, remove it from the interest set (so we
     * don't keep getting events on that socket) and clean it up later
     */
    assert(client_ip);
    assert(is_client_conn(client_ip));
    /* we got one EPOLLHUP and now we don't want to get any more */
    assert(client_ip->got_hup == 0);
    client_ip->got_hup = 1;
    interest_set_change(client_ip->sd, ISET_NOT_READABLE | ISET_NOT_WRITABLE | ISET_DONE);
    num_convert_to_hupped++;
}
#endif /* FIXHUP */

#ifdef HAVE_EPOLL
void
sd_state_not_writable(struct info *ip)
{
  if ((ip->sd_state & EPOLLHUP) || (ip->sd_state & EPOLLERR)) {
    PRINT_TIME(ip->sd, &tnow, &tprev, "sd_state_not_writable: but have "
       "HUP/ERR sd_state now = 0x%x", ip->sd_state);
  } else {
    ip->sd_state &= ~(EPOLLOUT);
    ldbg("sd_state_not_writable: sd_state now = 0x%x\n", ip->sd_state);
    PRINT_TIME(ip->sd, &tnow, &tprev, "sd_state_not_writable: sd_state "
      "now = 0x%x", ip->sd_state);
  }
}

void
sd_state_not_readable(struct info *ip)
{
  if ((ip->sd_state & EPOLLHUP) || (ip->sd_state & EPOLLERR)) {
    PRINT_TIME(ip->sd, &tnow, &tprev, "sd_state_not_readable: but have HUP/ERR sd_state now = 0x%x", ip->sd_state);
  } else {
    ip->sd_state &= ~(EPOLLIN);
    ldbg("sd_state_not_readable: sd_state now = 0x%x\n", ip->sd_state);
    PRINT_TIME(ip->sd, &tnow, &tprev, "sd_state_not_readable: sd_state now = 0x%x", ip->sd_state);
  }
}

/* Keep track of what state each sd is in.
 * Because events are not edge triggered we make
 * all decisions of what to do next based on the
 * current state of the sd.
 * This call is used to update sd_state from events
 * obtained from epoll_wait
 */
void
sd_state_update(struct info *ip, int events)
{
  ip->sd_state |= events;
  
  if (events & EPOLLIN) {
    ip->sd_state |= EPOLLIN;
  }

  if (events & EPOLLOUT) {
    ip->sd_state |= EPOLLOUT;
  }

  if (events & EPOLLERR || events & EPOLLHUP) {
    ip->sd_state |= (EPOLLIN | EPOLLOUT);
  }

  PRINT_TIME(ip->sd, &tnow, &tprev, "sd_state_update: events = 0x%x "
    "sd_state now = 0x%x", events, ip->sd_state);
}
#endif /* HAVE_EPOLL */

#ifdef FIXHUP
/* if the client has already disconnected we don't need
 * to write the buffer out to the client and the client
 * isn't in the interest set anymore.
 */

void
unblock_hupped_client(struct info *client_ip)
{
  extern int post_socket_writable(int sd);
  int client_sd = -1;

  /* Instead of now writing the reply and then trying
   * to read another request we basically want to 
   * close down the server side of the client connection
   * without messing up the appserver connection
   */
  assert(client_ip);
  assert(is_client_conn(client_ip));
  client_sd = client_ip->sd;
  assert(client_sd > 0);
  assert(is_hungup(client_ip));
  PRINT_TIME(client_ip->sd, &tnow, &tprev, "");


  assert((get_fsm_state(client_sd) == FSM_FCGI_WAITING_FOR_REPLY) || 
         (get_fsm_state(client_sd) == FSM_FCGI_REQUEST_QUEUED));

#if 0
  assert(interest_set_not_in(client_sd));
#endif

  /* The hope is that we can just enable this socket for reading
   * and when we do the next epoll the read will fail and
   * we'll process the failure as we normally would
   */
  set_fsm_state(client_ip, FSM_READING_NEXT_REQUEST);
  /* Note that the sd was previously removed from the interest set so we need to add it */
  interest_set_change(client_sd, ISET_NOT_WRITABLE | ISET_READABLE | ISET_INIT);

  PRINT_TIME(client_sd, &tnow, &tprev, "returning");
}
#endif /* FIXHUP */
