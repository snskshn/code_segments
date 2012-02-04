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


#ifndef Q_H
#define Q_H

#define Q_ADD_TO_REAR    (0)
#define Q_ADD_TO_FRONT   (1)

typedef struct q_node *q_nodeptr;

struct q_node {
	int fd;
	q_nodeptr next;
	q_nodeptr prev;
};

extern q_nodeptr q_front;
extern q_nodeptr q_rear;

void q_init(int max_nodes);
void q_add_to_front(int fd);
void q_add_to_rear(int fd);
void q_print();
void q_delete(int fd);
void q_sync(int add_to);
void q_to_add(int fd);
void q_to_delete(int fd);

#endif /* Q_H */
