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


#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>

//#define QUEUE_STATS


typedef struct queue_struct {
  int arraysize;
  int head;
  int tail;
  long elements;
#ifdef QUEUE_STATS
  unsigned long put_count;
  unsigned long put_max_size;
  unsigned long put_total_size;
  unsigned long push_count;
  unsigned long push_total_size;
  unsigned long get_count;
  unsigned long get_total_size;
  unsigned long peek_count;
  unsigned long peek_total_size;
#endif
  /* NOTE: this can not be moved!!! */
  int data[1];
} Queue;


Queue *alloc_queue(size_t maxsize);
void free_queue(Queue *q);
/* add to the end of the queue */
int queue_put(Queue *q, int value);
/* add to the front of the queue */
int queue_push(Queue *q, int value);
/* get from the front of the queue */
int queue_get(Queue *q);
/* look at the front of the queue */
int queue_peek(const Queue *q);
int queue_maxsize(const Queue *q);
int queue_size(const Queue *q);
int queue_is_empty(const Queue *q);
int queue_is_full(const Queue *q);


#endif /* QUEUE_H */
