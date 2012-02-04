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
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#ifdef HAVE_READLINE
#include <readline/readline.h>
#include <readline/history.h>
#endif /* HAVE_READLINE */

#include "cntl_intr.h"
#include "cntl_conns.h"
#include "options.h"
#include "stats.h"
#include "syscalls.h"
#include "debug.h"
#include "cntl_intr.h"
#include "sock_listener.h"

#ifdef SEND
#include "send.h"
#endif /* SEND */

#ifdef ARRAY_OF_BUFS
void print_all_evts_array();
void send_print_info_array();
#endif /* ARRAY_OF_BUFS */

int interactive_skip = 0;

void
interactive_mode(void)
{
  extern void print_pending();
#ifdef HAVE_READLINE
  char *cmd;
#else
  int x;
  char cmd[80];
#endif /* HAVE_READLINE */
  int done = 0;

  interactive_skip = 0;

  do {
#ifdef HAVE_READLINE
    cmd = readline("Enter command: ");
#else
    printf("Enter command: ");
    x = scanf("%s", cmd);
#endif /* HAVE_READLINE */
    printf("Got cmd = [%s]\n", cmd);

    if ((strcmp(cmd, "h") == 0) || 
        (strcmp(cmd, "?") == 0) ||
        (strcmp(cmd, "help") == 0)) {
      printf("connoff     - to turn off new connections\n");
      printf("connon      - to turn on  new connections\n");
      printf("connoff!    - force new connections off\n");
      printf("connon!     - force new connections on\n");
      printf("debugoff    - to turn off debugging\n");
      printf("debugon     - to turn on  debugging\n");
      printf("cont        - turn off interactive mode and continue "
	     "with execution\n");
      printf("curmask     - print current signal mask\n");
      printf("quit/exit   - halt program immediately\n");
      printf("info/stats  - print out all stats/info\n");
      printf("status/intr - print out interrupt status information\n");
      printf("step        - do one step/loop through the select loop "
	     "(may need timeout)\n");
      printf("pending     - check for / print pending interrupts\n");
#ifdef SEND
      printf("events      - print events\n");
      printf("sendinfo    - print info from send ecb\n");
#endif /* SEND */
      printf("options     - print options\n");
      printf("skip        - skip one loop of doing work on the send_loop\n");
#ifdef DEBUG_ON
    } else if (strcmp(cmd, "debugoff") == 0) {
      MSG_MASK = 0;
    } else if (strcmp(cmd, "debugon") == 0) {
      MSG_MASK = 0xffffffff;
#endif /* DEBUG_ON */
#ifdef SEND
    } else if (strcmp(cmd, "events") == 0) {
#ifdef ARRAY_OF_BUFS
      print_all_evts_array();
    } else if (strcmp(cmd, "sendinfo") == 0) {
      send_print_info_array();
#else
      print_all_evts();
    } else if (strcmp(cmd, "sendinfo") == 0) {
      send_print_info();
#endif /* ARRAY_OF_BUFS */
#endif /* SEND */
    } else if (strcmp(cmd, "connon!") == 0) {
      do_unblock();
    } else if (strcmp(cmd, "connoff!") == 0) {
      do_block();
    } else if (strcmp(cmd, "connoff") == 0) {
      if (new_connections_on == 0) {
        printf("New connections already off\n");
      } else {
	socket_new_conn_off();
      }
    } else if (strcmp(cmd, "connon") == 0) {
      if (new_connections_on == 1) {
        printf("New connections already on\n");
      } else {
	socket_new_conn_on();
      }
    } else if (strcmp(cmd, "off") == 0) {
      options.interactive = 0;
    } else if (strcmp(cmd, "cont") == 0) {
      options.interactive = 0;
      done = 1;
    } else if ((strcmp(cmd, "info") == 0) ||
	       (strcmp(cmd, "stats") == 0)) {
      print_all();
    } else if ((strcmp(cmd, "status") == 0) ||
	       (strcmp(cmd, "intr") == 0)) {
      intr_print_info();
    } else if ((strcmp(cmd, "quit") == 0) ||
	       (strcmp(cmd, "exit") == 0)) {
      printf("Exiting\n");
      exit(0);
    } else if (strcmp(cmd, "pending") == 0) {
      print_pending();
    } else if (strcmp(cmd, "skip") == 0) {
      interactive_skip = 1;
    } else if (strcmp(cmd, "options") == 0) {
      options_print();
    } else if (strcmp(cmd, "curmask") == 0) {
      print_cur_mask("Current signal mask is:\n");
    } else if (strcmp(cmd, "step") == 0) {
      done = 1;
    } else {
      printf("Command not recognized\n");
      printf("Use {h|?|help} for list of commands\n");
    }

#ifdef HAVE_READLINE
    if (cmd) {
      free(cmd);
    }
#endif /* HAVE_READLINE */

  } while (!done);

  printf("leaving interactive mode\n");

}

