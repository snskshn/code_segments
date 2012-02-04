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
#include "myassert.h"
#include "q.h"
#include "debug.h"
#include "stats.h"
#include "util.h"

#ifdef DEBUG_Q
static int debug = 1;
#define DBG_Q printf
#else
static int debug = 0;
#define DBG_Q  if (debug) printf
#endif

q_nodeptr q_front = NULL;
q_nodeptr q_rear = NULL;
static int max_size = 0;
static int cur_size = 0;
static q_nodeptr *fd_array;
static q_nodeptr node_array;

static q_nodeptr first_free = NULL;

static q_nodeptr q_get_node();
static void q_free_node(q_nodeptr p);
static void q_print_node_array();

typedef enum {TODO_ADD, TODO_DELETE} todo_type;
static int todo_count = 0;

static struct todo_info {
    int fd;
    todo_type todo;
} *todo;


void
q_init(int max_nodes)
{
	int i;
	int size;

	assert(max_nodes > 0);
	max_size = max_nodes;
	size = sizeof(q_nodeptr) * max_size;
	fd_array = (q_nodeptr *) alloc_aligned(ALLOC_NOALIGN, size,
	    ALLOC_MALLOC, "q_init: (fd_array)");
	printf("fd_array = %p size = %d\n", fd_array, size);

	size = sizeof(struct q_node) * max_size;
	node_array = (q_nodeptr) alloc_aligned(ALLOC_NOALIGN, size,
	    ALLOC_MALLOC, "q_init: (node_array)");
	printf("node_array = %p size = %d\n", node_array, size);

	size = sizeof(int) * max_size * 2;
	todo = (struct todo_info *) alloc_aligned(ALLOC_NOALIGN, size,
	    ALLOC_MALLOC, "q_init: (todo)");
	printf("todo = %p size = %d\n", todo, size);

	DBG_Q("q_init: finished init\n");

	for (i=0; i<max_size; i++) {
		fd_array[i] = NULL;
	}

	for (i=0; i<max_size-1; i++) {
		node_array[i].fd = -1;
		node_array[i].next = &node_array[i+1];
	}
	node_array[i].next = NULL;
	first_free = &node_array[0];

	if (debug) {
	  q_print_node_array();
	}
	cur_size = 0;
	todo_count = 0;
}

void
q_add_to_front(int fd)
{
	q_nodeptr q_new = q_get_node();

	assert(fd >= 0 && fd <= max_size);
	assert(q_new);

	DBG_Q("q_add_to_front: adding fd %d\n", fd);
	q_new->fd = fd;
	q_new->next = q_front;
	q_new->prev = NULL;
	assert(fd_array[fd] == (q_nodeptr) 0);
        fd_array[fd] = q_new;

	if (q_front) {
		DBG_Q("q_add_to_front: old q_front %p  = %d\n", 
		    q_front, q_front->fd);
		q_front->prev = q_new;
	}

	if (q_rear == NULL) {
		q_rear = q_new;
	}
	q_front = q_new;
	cur_size++;
	DBG_Q("q_add_to_front: new q_front %p  = %d cur_size = %d\n", 
			q_front, q_front->fd, cur_size);
}

void
q_add_to_rear(int fd)
{
	q_nodeptr q_new = q_get_node();

	assert(fd >= 0 && fd <= max_size);
	assert(q_new);

	DBG_Q("q_add_to_rear: adding fd %d\n", fd);
	q_new->fd = fd;
	q_new->prev = q_rear;
	q_new->next = NULL;
	assert(fd_array[fd] == NULL);
        fd_array[fd] = q_new;

	if (q_rear) {
		DBG_Q("q_add_to_rear old q_rear %p  = %d\n", 
		    q_rear, q_rear->fd);
		q_rear->next = q_new;
	}

	if (q_front == NULL) {
		q_front = q_new;
	}
	q_rear = q_new;
	cur_size++;
	DBG_Q("q_add_to_rear: new q_rear %p  = %d cur_size = %d\n", 
			q_rear, q_rear->fd, cur_size);
}

void
q_delete(int fd)
{
	q_nodeptr tmp;
	q_nodeptr p;

        DBG_Q("q_delete: deleting fd = %d\n", fd);
	assert(fd >= 0 && fd < max_size);
	p = fd_array[fd];
	assert(p != NULL);

	/* only one node in the q */
	if (p == q_front && p == q_rear) {
	    DBG_Q("q_delete: deleting only entry %p = %6d\n", p, fd);
	    q_front = NULL;
	    q_rear = NULL;
	} else if (p == q_front) {
	    /* more than one node and it's the q_front being deleted */
	    DBG_Q("q_delete: deleting from q_front %p = %6d\n", p, fd);
	    q_front = q_front->next;
	    q_front->prev = NULL;
	} else if (p == q_rear) {
	   /* more than one node and it's the q_rear being deleted */
	    DBG_Q("q_delete: deleting from q_rear %p = %6d\n", p, fd);
	    q_rear = q_rear->prev;
	    q_rear->next = NULL;
	} else {
	    /* handle the case where it's in the middle 
	    * at this point there must be at least 3 nodes because
	    * if there are only two nodes then the one being deleted 
	    * must be either the q_front or the q_rear 
	    */
	    DBG_Q("q_delete: deleting from middle %p = %6d\n", p, fd);
	    DBG_Q("q_delete: p->prev = %p  p->next = %p\n", 
		p->prev, p->next);
	    
	    assert(cur_size >= 3);
	    tmp = p->prev;
	    tmp->next = p->next;
	    p->next->prev = tmp;
		
	}

	fd_array[fd] = NULL;
	q_free_node(p);
	cur_size--;
	assert(cur_size >= 0);
}

static void
q_print_node_array(void)
{
	int i;
	for (i=0; i<max_size; i++) {
		printf("i = %6d  &i = %p  i->next = %p\n", 
				i, &node_array[i], node_array[i].next);
	}
}

void
q_print(void)
{
	q_nodeptr p;
	printf("q_print: cur_size = %6d\n", cur_size); p = q_front;
	while (p!= NULL) {
		printf("p = %p  p->fd = %6d  p->next = %p  p->prev = %p\n", 
				p, p->fd, p->next, p->prev);
		p = p->next;
	}

}

static q_nodeptr
q_get_node(void)
{
	q_nodeptr p;

	if (first_free == NULL) {
		p = NULL;
		printf("q_get_node: can't find free node\n");
		exit(1);
	} else {
		p = first_free;
		first_free = p->next;
		DBG_Q("q_get_node: found free node at p = %p\n", p);
	}

	DBG_Q("q_get_node: returning p = %p\n", p);
	return(p);
}


static void
q_free_node(q_nodeptr p)
{
	q_nodeptr old_first;
	assert(p);

	p->fd = -1;
       	old_first = first_free;
	first_free = p;
	first_free->next = old_first;
	DBG_Q("q_free_node: first_free = %p  first_free->next = %p\n", 
			first_free, first_free->next);
}


void
q_to_add(int fd)
{
    assert(fd >= 0 && fd <= max_size);
    todo[todo_count].fd = fd;
    todo[todo_count].todo = TODO_ADD;
    todo_count++;
    DBG_Q("q_to_add: added fd = %6d count now = %6d\n", fd, todo_count);
}

void
q_to_delete(int fd)
{
    assert(fd >= 0 && fd <= max_size);
    todo[todo_count].fd = fd;
    todo[todo_count].todo = TODO_DELETE;
    todo_count++;
    DBG_Q("q_to_delete: added fd = %6d count now = %6d\n", fd, todo_count);
}

void 
q_sync(int add_to)
{
    int i;

    DBG_Q("q_sync: add_to = %d\n", add_to);
    DBG_Q("q_sync: todo_count = %d\n", todo_count);
    for (i=0; i<todo_count; i++) {
      assert(todo[i].fd >=0 && todo[i].fd <= max_size);
      if (todo[i].todo == TODO_ADD) {
	if (add_to == Q_ADD_TO_FRONT) {
	  q_add_to_front(todo[i].fd);
	} else if (add_to == Q_ADD_TO_REAR) {
	  q_add_to_rear(todo[i].fd);
	}
      } else if (todo[i].todo == TODO_DELETE) {
        q_delete(todo[i].fd);
      }
    }
    todo_count = 0;
}
