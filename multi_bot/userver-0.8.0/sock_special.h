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


#ifndef SOCK_SPECIAL_H
#define SOCK_SPECIAL_H


extern int sock_special_min;
extern int sock_special_max;
extern int sock_special_num;

/* Track which sockets are special sockets that interact 
 * with external entities.
 * The idea here is to be able to say that this is a special
 * socket of a specific type. 
 */
void sock_set_special(int sd);

/* is this socket a special socket (i.e., one that communicates
 * with an external entity like another server, a GCI or PHP process,
 * etc.
 */
int sock_is_special(int sd);

/* Function to call when a read event occurs on a special socket */
void sock_special_do_readable(int sd);

/* Function to call when a write event occurs on a special socket */
void sock_special_do_writable(int sd);

/* Function to call to initialize the special sockets */
void sock_special_init(void);


#endif /* SOCK_SPECIAL_H */
