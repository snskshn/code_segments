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


#ifndef SOCK_LISTENER_H
#define SOCK_LISTENER_H

#include <limits.h>

#include "util.h"

#define LISTENER_IS_NOT_SSL   (0)
#define LISTENER_IS_SSL       (1)

#define  SOCK_LISTENER_UNKNOWN   (-2)

extern int sock_listener_min;
extern int sock_listener_max;
extern int sock_listener_num;

#ifdef ONE_LISTENER
extern int server_sd;

#define SOCK_LISTENER_USE_ANY      (server_sd)
#define SOCK_LISTENER_USE_ALL      (server_sd)
#define SOCK_LISTENER_USE_UNKNOWN  (server_sd)

void sock_set_listener(int sd);
#define sock_is_listener(sd) ((sd == server_sd) ? 1 : 0)

#else

#define SOCK_LISTENER_USE_ANY     (-5)
#define SOCK_LISTENER_USE_ALL     (-50)
#define SOCK_LISTENER_USE_UNKNOWN (-100)

#define MAX_LISTENERS             (200)

struct info_listen_struct {
  int sd;
  sockaddr_t addr;
  int sd_state;
  int accept_count;
  int cur_conns;
  int max_cur_conns;
  int aio_accept_outstanding;
  int maxconns;
  int max_thold;
};

extern struct info_listen_struct info_listener[MAX_LISTENERS];

void info_listener_init();
void info_listener_print_status();

void sd_state_listener_not_readable(int sd);
void sd_state_listener_readable(int sd);
void stats_print_listeners();
void info_listener_compute_limits();

/* track which sockets are listening for new connections */
void sock_set_listener(int sd);

/* is this socket listening for new connections */
int sock_is_listener(int sd);


#endif /* ONE_LISTENER */

#endif /* SOCK_LISTENER_H */
