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


#ifndef DO_NEW_CONNECTIONS_H
#define DO_NEW_CONNECTIONS_H

#define FROM_DO_CLOSE               (0)
#define FROM_SERVER_LOOP            (1)
#define FROM_EXTRAS                 (2)
#define FROM_DO_READABLE            (3)
#define FROM_SIGIO_HANDLER          (4)
#define FROM_SERVER_LOOP_RACE       (5)
#define FROM_SEND_HANDLER           (6)
#define FROM_SEND_LOOP              (7)
#define FROM_AIO_SOCK_ACCEPT_INIT   (8)
#define FROM_PROCESS_EPOLL_EVENTS   (9)
#define FROM_MAX                    (10)  /* number of elements */
                 /* !!!! if adding here, also update do_new_connections.c */


extern char *called_from[];
extern void do_new_connections(int listen_sd, int from);

#endif /* DO_NEW_CONNECTIONS_H */
