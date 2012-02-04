/*
    userver -- (pronounced you-server or micro-server).
    This file is part of the userver, a high-performance web server designed for
    performance experiments.
          
    Copyright (C) 2005-2010  Tim Brecht
    Original Copyright (C) 2004  Craig Barkhouse

    Original Author: Craig Barkhouse <cabarkho@uwaterloo.ca>
    Additional Authors: Mark Groves <mjgroves@uwaterloo.ca>
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


#include <stddef.h>
#include "myassert.h"
#include "util.h"
#include "queue.h"


Queue *
alloc_queue(size_t maxsize)
{
  Queue *q;
  size_t arraysize, numbytes;

  /*
   * We actually allocate an array of maxsize + 1, so that we can distinguish
   * between the queue being empty and the queue being full.
   */
  arraysize = maxsize + 1;
  assert(arraysize > 0);

  /* TBB: not sure why things are done this way but
   *      this is allocating enough memory for the structure plus
   *      an array at the end of the structure. Then it's used
   *      as though the array was a static part of the structure.
   */
  numbytes = (size_t) offsetof(Queue, data[0]) + (arraysize * sizeof(q->data[0]));

  printf("alloc_queue: maxsize = %lu arraysize = %lu numbytes = %lu\n", 
          (unsigned long) maxsize, (unsigned long) arraysize, (unsigned long) numbytes);
  q = (Queue *) alloc_aligned(ALLOC_NOALIGN, numbytes,
      ALLOC_MALLOC, "alloc_queue");
  if (q != NULL) {
    q->arraysize = arraysize;
    q->head = 0;
    q->tail = 0;
    q->elements = 0;
#ifdef QUEUE_STATS
    q->put_count = 0;
    q->put_max_size = 0;
    q->put_total_size = 0;
    q->push_count = 0;
    q->push_total_size = 0;
    q->get_count = 0;
    q->get_total_size = 0;
    q->peek_count = 0;
    q->peek_total_size = 0;
#endif
    return q;
  }

  return NULL;
}


void
free_queue(Queue *q)
{
  assert(q != NULL);
  free(q);
}


int
queue_put(Queue *q, int value)
{
  int newtail;
#ifdef QUEUE_STATS
  int size;
#endif

  assert(q != NULL);
#ifdef QUEUE_STATS
  q->put_count++;
  size = queue_size(q);
  if (size > q->put_max_size) {
    assert(size == q->put_max_size + 1);
    q->put_max_size = size;
  }
  q->put_total_size += size;
#endif
  newtail = (q->tail + 1) % q->arraysize;
  if (newtail == q->head) {
    return -1;  // queue is full
  }
  q->data[q->tail] = value;
  q->tail = newtail;
  q->elements++;

  assert(q->elements == queue_size(q));

  return 0;
}


int
queue_push(Queue *q, int value)
{
  int newhead;

  assert(q != NULL);
#ifdef QUEUE_STATS
  q->push_count++;
  q->push_total_size += queue_size(q);
#endif
  if (q->head == 0) {
    newhead = q->arraysize - 1;
  } else {
    newhead = q->head - 1;
  }
  if (newhead == q->tail) {
    return -1;  // queue is full
  }
  q->head = newhead;
  q->data[newhead] = value;
  q->elements++;

  assert(q->elements == queue_size(q));

  return 0;
}


int
queue_get(Queue *q)
{
  int value;

  assert(q != NULL);
#ifdef QUEUE_STATS
  q->get_count++;
  q->get_total_size += queue_size(q);
#endif
  if (q->head == q->tail) {
    return -1;  // queue is empty
  }
  value = q->data[q->head];
  q->head = (q->head + 1) % q->arraysize;
  q->elements--;

  assert(q->elements == queue_size(q));

  return value;
}


int
queue_peek(const Queue *q)
{
  assert(q != NULL);
#ifdef QUEUE_STATS
  ((Queue *) q)->peek_count++;
  ((Queue *) q)->peek_total_size += queue_size(q);
#endif
  assert(q->elements == queue_size(q));

  return q->data[q->head];
}


int
queue_maxsize(const Queue *q)
{
  assert(q != NULL);
  return q->arraysize - 1;
}


int
queue_size(const Queue *q)
{
  int tmp;
  int size;

  assert(q != NULL);
  tmp = q->tail - q->head;
  if (tmp >= 0) {
    size = tmp;
  } else {
    size = tmp + q->arraysize;
  }

  assert(q->elements == size);
  return(size);
}


int
queue_is_empty(const Queue *q)
{
  int is_empty;
  assert(q != NULL);
  is_empty = q->head == q->tail;

  if (is_empty) {
    assert(q->elements == 0);
  }
  return(is_empty);
}


int
queue_is_full(const Queue *q)
{
  int is_full;
  assert(q != NULL);
  is_full = ((q->tail + 1) % q->arraysize) == q->head;

  if (is_full) {
    assert(q->elements == q->arraysize - 1);
  }
  return(is_full);
}
