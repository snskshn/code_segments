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
#include "info.h"
#include "state.h"
#include "trace.h"
#ifdef HAVE_SSL
#include "u_ssl_utils.h"
#endif

/*----------------------------------------------------------------------*/
/* Prototypes */
int is_fdset(int sd, fd_set *set);
int is_readable_or_writable(int sd, fd_set *readable, fd_set *writable);
void do_sd(int sd, fd_set *readable, fd_set *writable);
int do_readable(int sd);
void do_writable(int sd);
void do_lru(int n, fd_set *readable, fd_set *writable);
void check_and_do_read_write(int *n, int sd, fd_set *readable, fd_set *writable);

	  
/*----------------------------------------------------------------------*/
void
process_sds(int n, fd_set *readable, fd_set *writable, int max_usable_sd)
{
  int sd;
  q_nodeptr p = NULL;
#ifdef TRACE_ON
  int save_n = n;
#endif

  TRACE_START_TIME(EVT_WORK_PHASE);

  PRINT_TIME(NOFD, &pnow, &pprev, "process_sds: entered "
      "(time process_sds_only)");
  PRINT_TIME(NOFD, &pnow, &pprev, "process_sds: "
      "min_usable_sd = %d max_usable_sd = %d", min_usable_sd, max_usable_sd);

  switch (options.process_sds_order)
  {
      case OPT_PROCESS_SDS_UP:
	for (sd = min_usable_sd; n > 0 && sd <= max_usable_sd; sd++) {
	  /* DEBG(MSG_PROCESS_SDS, "sd = %d\n", sd); */
	  check_and_do_read_write(&n, sd, readable, writable);
	  do_extra_accepts(n, options.extra_freq);
	} /* for */
	assert(n == 0);
	break;
    
      case OPT_PROCESS_SDS_DOWN:
	for (sd = max_usable_sd; n > 0 && sd >= min_usable_sd; --sd) {
	  /* DEBG(MSG_PROCESS_SDS, "sd = %d\n", sd); */
	  check_and_do_read_write(&n, sd, readable, writable);
	  do_extra_accepts(n, options.extra_freq);
	} /* for */
	assert(n == 0);
	break;

      case OPT_PROCESS_SDS_WRITES_FIRST_DOWN:
	PRINT_TIME(NOFD, &tnow, &tprev, "process_sds: starting "
	    "writes-first-down writable ");
	for (sd = max_usable_sd; n > 0 && sd >= min_usable_sd; --sd) {
	  if (event_set_is_writable(sd)) {
	    --n;
	    PRINT_TIME(sd, &tnow, &tprev, "process_sds: do_writable ");
	    do_writable(sd);
	    conns_off_if_needed();
	    do_extra_accepts(n, options.extra_freq);
	  }
	} /* for */

	PRINT_TIME(NOFD, &tnow, &tprev, "process_sds: starting "
	    "writes-first-down readable ");
	for (sd = max_usable_sd; n > 0 && sd >= min_usable_sd; --sd) {
	  /* DEBG(MSG_PROCESS_SDS, "sd = %d\n", sd); */
	  if (event_set_is_readable(sd)) {
	    --n;
	    PRINT_TIME(sd, &tnow, &tprev, "process_sds: do_readable ");
	    do_readable(sd);
	    conns_off_if_needed();
	    do_extra_accepts(n, options.extra_freq);
	  }
	} /* for */
	assert(n == 0);
	break;

      case OPT_PROCESS_SDS_WRITES_FIRST_UP:
	for (sd = min_usable_sd; n > 0 && sd <= max_usable_sd; sd++) {
	  /* DEBG(MSG_PROCESS_SDS, "sd = %d\n", sd); */
	  if (event_set_is_writable(sd)) {
	    --n;
	    PRINT_TIME(sd, &tnow, &tprev, "process_sds: do_writable ");
	    do_writable(sd);
	    conns_off_if_needed();
	    do_extra_accepts(n, options.extra_freq);
	  }
	} /* for */

	for (sd = min_usable_sd; n > 0 && sd <= max_usable_sd; sd++) {
	  /* DEBG(MSG_PROCESS_SDS, "sd = %d\n", sd); */
	  if (event_set_is_readable(sd)) {
	    --n;
	    PRINT_TIME(sd, &tnow, &tprev, "process_sds: do_readable ");
	    do_readable(sd);
	    conns_off_if_needed();
	    do_extra_accepts(n, options.extra_freq);
	  }
	} /* for */
	assert(n == 0);
	break;

      case OPT_PROCESS_SDS_LRU:
        do_lru(n, readable, writable);
        break;

      /* Depending on which option is used things are added to the
       * queue in a different order so we always start at the
       * front and move to the rear
       */
      case OPT_PROCESS_SDS_LIFO:
      case OPT_PROCESS_SDS_FIFO:
	p = q_front;
        while (p != NULL) {
	  sd = p->fd;
	  DEBG(MSG_PROCESS_SDS, "process_sds: lifo or fifo n = %d\n", n);
	  DEBG(MSG_PROCESS_SDS, "process_sds: lifo or fifo sd = %d\n", sd);
	  check_and_do_read_write(&n, sd, readable, writable);
	  do_extra_accepts(n, options.extra_freq);
	  p = p->next;
	} /* while */
	DEBG(MSG_PROCESS_SDS, "process_sds: loop done - lifo or "
	    "fifo n = %d\n", n);
	assert(n == 0);
	if (options.process_sds_order == OPT_PROCESS_SDS_LIFO) {
	  q_sync(Q_ADD_TO_FRONT);
	} else if (options.process_sds_order == OPT_PROCESS_SDS_FIFO) {
	  q_sync(Q_ADD_TO_REAR);
	} else {
          printf("process_sds: problem in options:process_sds_order\n");
	  exit(1);
	}
	break;
        

      default :
	fflush(stdout);
        printf("process_sds: unknown option for process_sds_order = %d\n",
                options.process_sds_order);
	fflush(stdout);
	exit(1);
        break;

  } /* switch */

  PRINT_TIME(NOFD, &pnow, &pprev, "process_sds: returning "
      "(time process_sds_only)");

  TRACE_END_TIME(EVT_WORK_PHASE, save_n, 0);
} /* process_sds */


/*----------------------------------------------------------------------*/
void
do_sd(int sd, fd_set *readable, fd_set *writable)
{
  if (event_set_is_readable(sd)) {
    do_readable(sd);
  } else if (event_set_is_writable(sd)) {
    do_writable(sd);
  }
} /* do_sd */


/*----------------------------------------------------------------------*/
int
do_readable(int sd)
{
  int retval = 1;
  struct info *ip;

  assert(sd > 2);

  if (sock_is_listener(sd) 
#ifdef HAVE_SSL
      || is_ssl_listener(sd)
#endif /* HAVE_SSL */
    ) {
    assert(!sock_is_special(sd));
    PRINT_TIME(sd, &tnow, &tprev, "do_readable: calling "
	"do_new_connection");
    do_new_connections(sd, FROM_DO_READABLE);
  } else {
    assert(!sock_is_listener(sd));

    ip = info_ptr(sd);
    assert(ip);

    PRINT_TIME(sd, &tnow, &tprev, "do_readable: calling socket_readable ");
    DEBG(MSG_READABLE, "socket_readable: sd = %d\n", sd);
    DEBG(MSG_READABLE, "socket_readable: info = %p\n", info);
    DEBG(MSG_READABLE, "socket_readable: ip = %p\n", ip);
    if (options.loop_on_readable >= 1) {
      /* THIS WAS THE WAY t-event2 did things */
      while (socket_readable(ip) > 0);
    } else {
      retval = socket_readable(ip);
    }
  }

  PRINT_TIME(sd, &tnow, &tprev, "do_readable: returning %d", retval);
  return retval;
} /* do_readable */


/*----------------------------------------------------------------------*/
void
do_writable(int sd)
{
  struct info *ip;

  ip = info_ptr(sd);
  assert(ip);

  if (ip->skip > 0) {
    if (ip->skip_info < ip->skip) {
      ip->skip_info++;
      PRINT_TIME(sd, &tnow, &tprev, "do_writable: skipping write ip->skip = %d ip->skip_info %d", 
                 ip->skip, ip->skip_info);
      num_skips++;
      return;
    } else {
      assert(ip->skip_info == ip->skip); 
      assert(ip->skip == options.victim_skip);
      ip->skip_info = 0;
      PRINT_TIME(sd, &tnow, &tprev, "do_writable: resetting skip_info ip->skip_info = %d", ip->skip_info);
    }
  } 

  PRINT_TIME(sd, &tnow, &tprev, "do_writable: calling socket_writable ");
#ifdef DEBUG_ON
  if (ip->type == INFO_CLIENT && ip->fsm_state != FSM_WRITING_REPLY) {
    printf("do_writable: fsm_state = %s fd write is set to = %d\n",
      fsm_state_string(sd), interest_set_is_writable(sd));
   //return ; 
   assert(ip->type != INFO_CLIENT || ip->fsm_state == FSM_WRITING_REPLY);
  }
#endif /* DEBUG_ON */

  socket_writable(ip);
} /* do_writable */


/*----------------------------------------------------------------------*/
void
do_lru(int n, fd_set *readable, fd_set *writable)
{
  int done;
  int t = lru_tail;
  int h = lru_head;
  int cur;
  int sd = -1;

  DEBG(MSG_PROCESS_SDS, "lru_head = %d lru_tail = %d\n", lru_head, lru_tail);

  /* Copy the lru array into the lru_copy because as we process sds the lru
   * will change and would cause this to break. We use the lru_copy as a 
   * snapshot of an lru ordering 
   */

  if (!memcpy(lru_copy, lru, lru_bytes)) {
    perror("memcpy");
    printf("do_lru: memcpy failed\n");
    exit(1);
  }

  cur = h;
  done = 0;

  while(!done && n > 0) {
    sd = lru_copy[cur].sd;
    if (sd != -1) {
      conns_off_if_needed();
      DEBG(MSG_PROCESS_SDS, "do_lru: n = %d sd = %d cur = %d "
	  "h = %d t = %d\n", n, sd, cur, h, t);
      if (event_set_is_readable_or_writable(sd)) {
	--n;
	do_sd(sd, readable, writable);
	do_extra_accepts(n, options.extra_freq);
      }
    } else {
      DEBG(MSG_PROCESS_SDS, "do_lru: sd = -1 n = %d cur = %d "
	  "h = %d t = %d\n", n, cur, h, t);
    }

    if (cur == t) {
      DEBG(MSG_PROCESS_SDS, "do_lru: t reached n = %d cur = %d "
	  "h = %d t = %d\n", n, cur, h, t);
      done = 1;
    } else {
      cur = lru_copy[cur].next;
    }
  } /* while */

  DEBG(MSG_PROCESS_SDS, "do_lru: done while loop\n");
  DEBG(MSG_PROCESS_SDS, "             n = %d cur = %d "
      "h = %d t = %d\n", n, cur, h, t);

  assert(n == 0);
}


/*----------------------------------------------------------------------*/
int
is_readable_or_writable(int sd, fd_set *readable, fd_set *writable)
{
  if (is_fdset(sd, readable)) {
    return 1;
  }

  if (is_fdset(sd, writable)) {
    return 1;
  }
  return 0;
}

/*----------------------------------------------------------------------*/
int
is_fdset(int sd, fd_set *set)
{
  if (set) {
    if (FD_ISSET(sd, set)) {
      return 1;
    }
  }
  return 0;
}
/*----------------------------------------------------------------------*/

void
check_and_do_read_write(int *n, int sd, fd_set *readable, fd_set *writable)
{
  int doread = 0;
  int dowrite = 0;

  doread = FD_ISSET(sd, readable);
  dowrite = FD_ISSET(sd, writable);

  if (doread) {
    --*n;
    FD_CLR(sd, readable);
    do_readable(sd);
  }

  if (dowrite) {
    --*n;
    FD_CLR(sd, writable);
    do_writable(sd);
  }

  if (doread || dowrite) {
    conns_off_if_needed();
  }
}
