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


#ifndef INTEREST_SET_H
#define INTEREST_SET_H

enum set_method_enum {
  DECLARE_INVALID    = -1,
  DECLARE_WITH_FDSET = 0,
  DECLARE_WITH_POLL,
  DECLARE_WITH_EPOLL,
  DECLARE_WITH_SEND,
  DECLARE_WITH_AIO,
}; 

enum which_set_enum {
  USE_READ_SET = 0,
  USE_WRITE_SET,
};

#define ISET_NONE           (0x00000000)
#define ISET_READABLE       (0x00000001)
#define ISET_WRITABLE       (0x00000002)
#define ISET_NOT_READABLE   (0x00000010)
#define ISET_NOT_WRITABLE   (0x00000020)
#define ISET_INIT           (0x00000100)
#define ISET_DONE           (0x00000200)

void interest_set_init(enum set_method_enum method);
void interest_set_change(int sd, unsigned int mask);
int  interest_set_is_readable(int sd);
int  interest_set_is_writable(int sd);
void interest_set_add_readable(int sd);
void interest_set_add_writable(int sd);
void interest_set_del_readable(int sd);
void interest_set_del_writable(int sd);
void interest_set_done(int sd);
void interest_set_print_info();

int  event_set_is_readable_or_writable(int sd);
int  event_set_is_readable(int sd);
int  event_set_is_writable(int sd);

void check_epollable(int sd);
#endif /* INTEREST_SET_H */
