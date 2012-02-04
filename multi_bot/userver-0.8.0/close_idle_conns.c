/*
    userver -- (pronounced you-server or micro-server).
    This file is part of the userver, a high-performance web server designed for
    performance experiments.
          
    This file is Copyright (C) 2004-2010  Tim Brecht

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

#include "lru.h"
#include "cntl_intr.h"
#include "common.h"
#include "debug.h"
#include "sock_listener.h"

/* #define LOCAL_DEBUG */
#include "local_debug.h"

void 
close_idle_conns(struct timeval *lru_now)
{
  struct info *ip;
  struct timeval tv;
  struct timeval maxidle;
  int sd = -1;

  maxidle.tv_sec = options.idle_threshold;
  maxidle.tv_usec = 0;

  assert(options.close_idle_conns);

  notification_off();
  /* XXX: think about if this should be options.free_fd_thold */
  if (num_idle <= 0) {
    ldbg("close_idle_conns: num_idle = %d num_idle <=0\n", num_idle);
    PRINT_TIME(NOFD, &tnow, &tprev, "close_idle_conns: num_idle <=0");
    DEBG(MSG_SERVER_LOOP, "             num_idle = %d\n", num_idle);


    // assert(!new_connections_on);
    timersub(lru_now, &maxidle, &tv);

    ldbg("close_idle_conns: tv.tv_sec = %ld tv.tv_usec = %ld\n",
	  tv.tv_sec, tv.tv_usec);

    if (timercmp(&lru[lru_tail].time, &tv, <)) {
      sd = lru[lru_tail].sd;
      PRINT_TIME(NOFD, &tnow, &tprev, "close_idle_conns: reached "
	  "max idle time sd = %d", sd);
      PRINT_TIME(NOFD, &tnow, &tprev, "close_idle_conns: sec = %ld usec = %ld",
	  tv.tv_sec, tv.tv_usec);
      ldbg("close_idle_conns: reached max idle time sd = %d\n", sd);
      if (sd != -1) {
	PRINT_TIME(NOFD, &tnow, &tprev, 
	    "close_idle_conns: about to close %d", sd);
	DEBG(MSG_SERVER_LOOP, "             num_idle = %d\n", num_idle);

	/* ensure that the connection we are closing down is not one of
	 * the listener sockets
	 */
	if (sock_is_listener(sd)) {
	  printf("close_idle_conns: trying to close down a listener socket\n");
	  printf("close_idle_conns: should not be doing this\n");
	  printf("close_idle_conns: this connection must not be calling "
	      "lru_access when it should be\n");
	  exit(1);
	} else {
	  num_close_idle++;
	  notification_on();

          ldbg("close_idle_conns: closing sd = %d\n", lru[lru_tail].sd);
	  ip = info_ptr(lru[lru_tail].sd);
	  assert(ip);
	  do_close(ip, REASON_IDLE);

	  notification_off();
	}
      } else {
        PRINT_TIME(NOFD, &tnow, &tprev, "close_idle_conns: no sd");
      }
    } else {
      PRINT_TIME(NOFD, &tnow, &tprev, "close_idle_conns: not idle long enough");
    }
  }
  notification_on();
}
