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

#include "debug.h"
#include "options.h"
#include "common.h"
#include "cntl_intr.h"
#include "q.h"
#include "interactive_mode.h"
#ifndef __CYGWIN__
#include "sigstuff.h"
#endif /* __CYGWIN__ */
#include "sock_listener.h"

int
server_loop_top(void)
{
  extern int sigios_to_handle;
  int call_continue = 0;
  
  if (options.interactive) {
    interactive_mode();
  }

  if (sigios_to_handle > 0) {
    notification_off();
      PRINT_TIME(NOFD, &tnow, &tprev, "select_loop: new conns - race "
	  "handling");
#ifdef ONE_LISTENER
      do_new_connections(server_sd, FROM_SERVER_LOOP);
#else
      printf("sigio_handler: Support for multiple listeners and "
	     "SIGIO is not implemented yet\n");
      exit(1);
#endif /* ONE_LISTENER */
      if (options.accepts_only) {
	  DEBG(MSG_SERVER_LOOP, "select_loop: back to new connections "
	      "(sigios)\n");
	  notification_on();
	  call_continue = 1;
	  return call_continue;
      }
    notification_on();

    notification_off();
      if (options.process_sds_order == OPT_PROCESS_SDS_LIFO) {
	q_sync(Q_ADD_TO_FRONT);
      } else if (options.process_sds_order == OPT_PROCESS_SDS_FIFO) {
	q_sync(Q_ADD_TO_REAR);
      }
    notification_on();

  } else if ((options.get_connections == OPT_CONN_WITH_SIGIO) && 
	      (sigio_pending())) {
      notification_off();
      PRINT_TIME(NOFD, &tnow, &tprev, "select_loop: new conns - "
	  "(pending)");
#ifdef ONE_LISTENER
      do_new_connections(server_sd, FROM_SERVER_LOOP);
#else
      printf("sigio_handler: Support for multiple listeners and "
	     "SIGIO is not implemented yet\n");
      exit(1);
#endif /* ONE_LISTENER */
      if (options.accepts_only) {
	  DEBG(MSG_SERVER_LOOP, "select_loop: back to new connections "
	      "(pending)\n");
	  notification_on();
	  call_continue = 1;
	  return call_continue;
      }
    notification_on();

    notification_off();
      if (options.process_sds_order == OPT_PROCESS_SDS_LIFO) {
	q_sync(Q_ADD_TO_FRONT);
      } else if (options.process_sds_order == OPT_PROCESS_SDS_FIFO) {
	q_sync(Q_ADD_TO_REAR);
      }
    notification_on();
  }

  return call_continue;
}
