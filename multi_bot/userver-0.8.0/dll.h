/*
    userver -- (pronounced you-server or micro-server).
    This file is part of the userver, a high-performance web server designed for
    performance experiments.
          
    This file is Copyright (C) 2007  Mark Groves

    Original Author: Mark Groves <mjgroves@uwaterloo.ca>
    Additional Authors: 
             Tim Brecht <brecht@cs.uwaterloo.ca>
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


/* 
 * This is a doublely-linked list for
 * storing dynamic buffers.
 * In this case, buffers can be removed
 * from anywhere in the list, but only
 * added to the end. The intention is that
 * when a connection is closed, the buffer is
 * freed and removed from anywhere in the list.
 * When there are no free buffers, the intention
 * is that the userver will pop something from the
 * head of the list and check if the buffer has
 * been sent, in which case it uses the buffer.
 */

#ifndef BUFFER_DLL
#define BUFFER_DLL

#ifdef QUEUE_BUFS 
struct dll_element;

struct dll_element {
	struct dll_element *next;
	struct dll_element *prev;
	unsigned int key_sd;
	void *buf;
};

struct dll_list {
	struct dll_element *head;
	struct dll_element *tail;
	struct dll_element *free_head;
	struct dll_element *free_tail;
	unsigned int max_size;
};
	
//TODO: Memory leaks since there is no cleanup method


void * init_list(unsigned int size);
void remove_from_list(struct dll_list *list, struct dll_element *el);
struct dll_element *add_value_to_tail(struct dll_list *list, unsigned int val);
void add_el_to_tail(struct dll_list *list, struct dll_element *el);
struct dll_element * remove_from_head(struct dll_list *list, int sd);
int add_to_free(struct dll_list *list, struct dll_element *el);
struct dll_element * get_free(struct dll_list *list);
#endif /* QUEUE_BUFS */
#endif
