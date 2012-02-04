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


#include <assert.h>
#include <stdio.h>
#include <err.h>

#include "dll.h"
#include "sys_sock.h"
#include "util.h"
#include "info.h"
#include "stats.h"
#include "options.h"

#ifdef QUEUE_BUFS
struct sock_write_stats my_stats;

void *
init_list(unsigned int size) 
{
	struct dll_list *list;
	struct dll_element *ptr;
	unsigned int x;

	printf("making a dll of size %d\n", size);

#ifdef OLDWAY
	list = (struct dll_list *) malloc(sizeof(struct dll_list));
#else
	list = (struct dll_list *) alloc_aligned(ALLOC_NOALIGN, sizeof(struct dll_list), 
		ALLOC_MALLOC, "dll init_list: list");
#endif /* OLDWAY */
	if (list ==0) {
    perror("InitDLList (buffer_list)");
		exit(1);
  }


#ifdef OLDWAY
	list->free_head = (struct dll_element *) malloc(size * sizeof(struct dll_element));
#else
	list->free_head = (struct dll_element *) alloc_aligned(ALLOC_NOALIGN, 
			(size * sizeof(struct dll_element)), ALLOC_MALLOC, "dll init_list: free head");
#endif /* OLDWAY */
	if (list ==0) {
		perror("InitDLList (list_element)");
		exit(1);
	}

	//initialize the freelist
	ptr=&list->free_head[0];
	for (x=0; x<size-1; x++) {
	//	printf("attaching %p to %p\n", ptr, &list->free_head[x+1]);
		ptr->next = &list->free_head[x+1];
		//ptr->next->prev = ptr;
		ptr = ptr->next;
		//printf("attached %p to %p\n", ptr->prev, ptr);
	}
	ptr->next = NULL; 
	list->free_tail = ptr;
	list->head = NULL;
	list->tail = NULL;

	return list;

}	

int
add_to_free(struct dll_list *list, struct dll_element *el) 
{

	//printf("freeing %d\n", el->key_sd);
	
	//most likely case first (in this case, I think it's the base case)
	if (list->free_head == NULL) {
		list->free_head = el;
		el->prev = NULL;
	} else {
		el->prev = list->free_tail;
		list->free_tail->next = el;
	}

	el->next = NULL;
	el->key_sd = 0;
	list->free_tail = el;

	return 0; 	
}

struct dll_element *
get_free(struct dll_list *list) 
{
	struct dll_element *retval = list->free_head;
	
	assert(retval != NULL);
	
	list->free_head = retval->next;

	if (list->free_head  == NULL) {
		//printf("out of buffers\n");
		list->free_tail =NULL;
	}
	//else
	//	list->free_head->prev = NULL;

	return retval;
}	
	

//remove an element from anywhere in the dll and add it to the free list
void
remove_from_list(struct dll_list *list, struct dll_element *el) 
{
	//step one, remove from the dll
	struct dll_element *prev;
	struct dll_element *next;
	assert(el != NULL);
	prev = el->prev;
	next = el->next;

	el->buf = NULL;
	//printf("remove from list\n");

	//if middle
	if (prev != NULL && next != NULL) {
		prev->next = next;
  	next->prev = prev;
	} else if (prev == NULL && next == NULL) {
	  //if only element
		list->head = NULL;
		list->tail = NULL;
	} else if (next == NULL) {		//if tail
		list->tail = prev;
		list->tail->next = NULL;
	} else if (prev == NULL) {		//if head
		list->head = next;
		list->head->prev = NULL;
	}

	//next, put the removed item in the free list;
	add_to_free(list, el);
}	

/* add an element to the dll.
 * In this case, it's a new element, so have to get a free element first
 */ 
struct dll_element *
add_value_to_tail(struct dll_list *list, unsigned int val) 
{
	struct dll_element *el = get_free(list);

	el->key_sd = val;
	add_el_to_tail(list, el);

	return el;
}

//call this if you already have an element to insert to the end of the list
//like if you just popped the element, only to find that it's still in use
//Assumes that the element is free
void
add_el_to_tail(struct dll_list *list, struct dll_element *el) 
{
	if (list->head == NULL) {
    list->head =el;
  } else {
    list->tail->next = el;
	}
     
	el->next = NULL;
	el->prev = list->tail;
  list->tail = el;

	//printf("adding %d to tail\n", el->key_sd);
}

/* removes an element from the list, and checks if it has been sent.
 * if it has, stores the sd in the element and returns it
 * if not, it returns null
 * In either case, the element is put at the end of the list 
 */
struct dll_element *
remove_from_head(struct dll_list *list, int sd) 
{
	struct info *client;	
	struct dll_element *retval;
	socklen_t opt_res;
	int	res;
	int x, stop_here = 0;
	int ok_to_reuse = 1;

	//We'll check up to the first 3 elements in the queue
	for (x=0; x< 3 && !stop_here; x++) {
		retval = list->head;
		assert (retval != NULL);

		//printf("remove from head\n");

		list->head = retval->next;	
		if (list->head == NULL) {
			//if that element was the only item
			list->tail = NULL;
			stop_here=1;
		} else {
			list->head->prev = NULL;
		}

		client = info_ptr(retval->key_sd);
		if (client != NULL) {
			//ok, something is in the queue, check if the sd is done

			//since we are probably using non-blocking I/O, 
			//lets possibly save a syscall and
			//make sure everything was sent
			if ((client->done_write == 1) && (client->rep.nwritten > 0) &&
				 (client->rep.total_len - client->rep.nwritten <= 0)) {

				/* It is OK to reuse all buffers except those being used by sendfile.
				 * For sendfile there is currently no way to find out when the buffer
				 * has all been sent so we have a new call to find out some info about
				 * what has been sent
				 */

        ok_to_reuse = 1;
        if (options.use_sendfile) {
				   //client has sent everything, do a syscall
				   opt_res = sizeof(struct sock_write_stats);
				   res = getsockopt(retval->key_sd, SOL_SOCKET, 35, &my_stats, &opt_res);
				   if (res != 0) {
					   perror("get sock opts:");
						 printf("ERROR: using sendfile but need way to check if buffer can be reused\n");
						 assert(0);
				   }
				   //printf("done with value %d\n", my_stats.sws_wmem_len);
				   if (my_stats.sws_wmem_len == 0) {
             ok_to_reuse = 1;  
					 } else {
             ok_to_reuse = 0;  
					 }
				}
																																																																														 
				if (ok_to_reuse) {
					if ((client->rep.dyn_buf != NULL) || 
							(client->rep.dyn_buf != client->rep.static_buf)) {
						//printf("found a buffer that we can use\n");
						
						//printf("took from %d, gave to %d\n", retval->key_sd, sd);

						//change ownership of the buffer to the new client info ptr
						retval->buf = client->rep.dyn_buf;
						retval->key_sd = sd;

						//delete all refs to the buffer in the old client ptr
						client->rep.dyn_buf = NULL;
						client->rep.buf_dll = NULL;			//this is a ptr to the dll element (retval)

						//necessary, otherwise asserts in fcgi.c
						client->rep.buf = NULL;
						//info_reset_rep_buf(client);
						client->rep.end = client->rep.cur = client->rep.buf;
						break;
					} else {
						printf("client dyn buf is null, this shold have been removed\n");
						/* Mark has this print statement but it's not clear what action should be taken */
						assert(0);
					}
				} else {
					//still exists queued data
					// printf("damn, buffer is still in use with val %d\n", my_stats.sws_wmem_len);
					no_freeable_bufs++;
					add_el_to_tail(list, retval);		//put the element back in the list (at the end)
					//return NULL;
					retval = NULL;
				}
			
			} else {
				//client hasn't sent all the data yet (non-blocking io)
				// printf("damn, client isn't done\n");
				no_freeable_bufs++;
				add_el_to_tail(list, retval);
				//return NULL;
				retval = NULL;
			}

		} else {
			printf("ERROR: client is null, this shouldn't be!!!\n");
			assert(0);
		}
	}

	//add it back into the list
	if (retval != NULL) {
	  add_el_to_tail(list, retval);
	}

	return retval;	
	//return NULL;
}
#endif
