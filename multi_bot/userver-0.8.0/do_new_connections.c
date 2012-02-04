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

#include "common.h"
#include "sock_listener.h"
#include "cntl_conns.h"
#include "options.h"
#include "debug.h"
#include "trace.h"
#include "stats.h"
#include "do_new_connections.h"

char *called_from[] = {
/*  0 */  "do_close",
/*  1 */  "server_loop",
/*  2 */  "extras",
/*  3 */  "do_readable",
/*  4 */  "sigio_handler",
/*  5 */  "server_loop_race",
/*  6 */  "server_send_handler",
/*  7 */  "server_send_loop",
/*  8 */  "aio_sock_accept_init",
/*  9 */  "process_epoll_events",
};

int repeat_accept(int listen_sd);

void
do_new_connections(int listen_sd, int called_from)
{
  int i = 0;
  int consecutive = 0;
  int rc;
  extern int sigios_to_handle;
  int trace_fd = 0;

  num_do_new_conns_from[called_from]++;

  PRINT_TIME(NOFD, &tnow, &tprev, "do_new_connections: entering ");
  num_do_new_connections_calls++;

  if (num_idle <= options.free_fd_thold) {
    num_do_new_connections_no_idle++;
    PRINT_TIME(NOFD, &tnow, &tprev, "do_new_connections: no idle - returning");
    DEBG(MSG_CONN, 
	"do_new_connections: num_idle = %d <= options.free_fd_thold = %d\n",
	num_idle, options.free_fd_thold);
    return;
  }

  if (options.use_socket_aio && options.aio_accept_thold != 0) {
    if ((maxconns - num_idle) - cur_conns >= options.aio_accept_thold) {
      PRINT_TIME(NOFD, &tnow, &tprev, "do_new_connections: at "
	  "aio_ccept_thold - returning");
      return;
    }
  }

  if (!new_connections_on) {
    num_do_new_connections_conns_off++;
    DEBG(MSG_CNTL_CONNS, "do_new_connections: new_connections_on = %d\n",
	new_connections_on);
    return;
  }

  TRACE_START_TIME(EVT_ACCEPT_PHASE);

  switch (options.do_multiaccept) {
    case OPT_MULTIACCEPT_BY_SYSCALL:
#ifdef MACCEPT
      TRACE(EVT_MACCEPT,
	trace_fd = 0;
        rc = multiaccept(listen_sd);
      );
      consecutive = rc;
#else
      printf("do_new_connections: multiaccept system call not "
	  "installed/supported\n");
      exit(1);
#endif /* MACCEPT */
      break;

    case OPT_MULTIACCEPT_BY_REPEATING:
      DEBG(MSG_CONN, "do_new_connections: doing multiaccept\n");
      if (listen_sd == SOCK_LISTENER_USE_ALL) {
       for (i=sock_listener_min; i<=sock_listener_max; i++) {
          if (info_listener[i].sd != -1) {
	      TRACE(EVT_REPEAT_ACCEPT,
		trace_fd = 0;
		rc = repeat_accept(info_listener[i].sd);
	      );
	      consecutive = rc;
	      if (consecutive > num_max_consecutive_accepts) {
		num_max_consecutive_accepts = consecutive;
	      }

	      if (options.multiaccept_max != OPT_MULTIACCEPT_MAX_UNLIMITED) {
		if (consecutive >= options.multiaccept_max) {
		  num_accept_max_reached++;
		}
	      }
          } /* if */
        } /* for */
      } else {
	TRACE(EVT_REPEAT_ACCEPT,
	  trace_fd = 0;
	  rc = repeat_accept(listen_sd);
	);
	consecutive = rc;
      }

      break;

    case OPT_MULTIACCEPT_OFF:
      DEBG(MSG_CONN, "do_new_connections: getting one new connection\n");
      if (socket_new_conn(listen_sd) > 0) {
	consecutive++;
      }
      break;

    default:
      printf("Unrecognized do_multiaccept option  %d\n",
              options.do_multiaccept);
      exit(1);
  }


  /* If using multiple listeners these stats are tracked above */
  if (listen_sd != SOCK_LISTENER_USE_ALL) {
    if (consecutive > num_max_consecutive_accepts) {
      num_max_consecutive_accepts = consecutive;
    }

    if (options.multiaccept_max != OPT_MULTIACCEPT_MAX_UNLIMITED) {
      if (consecutive >= options.multiaccept_max) {
	num_accept_max_reached++;
      }
    }
  }

  if (sigios_to_handle > 0 && called_from == FROM_SERVER_LOOP_RACE) {
    if (consecutive > sigios_to_handle) {
      sigios_to_handle = 0;
    } else {
      sigios_to_handle -= consecutive;
    }
  }
  DEBG(MSG_CONN, "do_new_connections: sigios_to_handle = %d\n", 
      sigios_to_handle);

  DEBG(MSG_CONN, "do_new_connections: consecutive connections = %d "
      "num_idle = %d\n", consecutive, num_idle);
  fflush(stdout);
  PRINT_TIME(NOFD, &tnow, &tprev, "do_new_connections: returning "
      "consecutive = %d", consecutive);

  TRACE_END_TIME(EVT_ACCEPT_PHASE, consecutive, 0);
} /* do_new_connections */


int
repeat_accept(int listen_sd)
{
  int rc = 1;
  int connections = 0;
  extern int sock_accept_more(int listen_sd);

  PRINT_TIME(listen_sd, &tnow, &tprev, "repeat_accept: entered");
  while ((rc > 0) && (num_idle > options.free_fd_thold) && 
	 (new_connections_on)) {

    if (options.use_socket_aio) {
      if (!sock_accept_more(listen_sd)) {
          PRINT_TIME(listen_sd, &tnow, &tprev, "repeat_accept: aio_break");
	  break;
      }
    }

    rc = socket_new_conn(listen_sd);
    if (rc > 0) {
      connections++;

      if (options.multiaccept_max != OPT_MULTIACCEPT_MAX_UNLIMITED) {
	if (connections >= options.multiaccept_max) {
	  break;
	}
      }
      conns_off_if_needed();
    }
  } /* while */

  PRINT_TIME(listen_sd, &tnow, &tprev, "repeat_accept: returning "
      "rc = %d conns = %d", rc, connections);
  return(connections);
} /* repeat_accept */
