/*
    userver -- (pronounced you-server or micro-server).
    This file is part of the userver, a high-performance web server designed for
    performance experiments.
          
    This file is Copyright (C) 2004-2010  Tim Brecht

    Author: Tim Brecht <brecht@cs.uwaterloo.ca>.

    This code has evolved from code for servers by: 
       David Mosberger <david.mosberger@hp.com> and
       Abhishek Chandra <abhishek@cs.umass.edu>

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
#include <unistd.h>
#include <string.h>
#include "myassert.h"
#include "common.h"
#include "lru.h"
#include "stats.h"
#include "util.h"
#include "sock_special.h"
#include "sock_listener.h"
#include "options.h"
#include "app.h"

#define LRU_DEBUG (0)

/* #define LOCAL_DEBUG */
#include "local_debug.h"

struct lru *lru = 0;
struct lru *lru_copy = 0;
struct timeval lru_now;
int lru_size = 0;
int lru_bytes = 0;
int lru_head = 0;
int lru_tail = 0;


/* ---------------------------------------------------------------------- */
/* Initialize LRU queue for MAXCONN entries: */
void
lru_init(void)
{
  int i;

  gettimeofday(&lru_now, NULL);

  /* we map the sd to the lru entry so a few slots are used up for
   * stdin, stdout, stderr, listening socket descriptors, etc. 
   */
  lru_size = maxconns + options.ip_addr_count + numprocs + MAX_APPS + 1;
  printf("lru_init: lru entries = %d\n", lru_size);
  lru_bytes = (lru_size) * sizeof (struct lru);

  lru = (struct lru *) alloc_aligned(cache_line_size, lru_bytes, ALLOC_MALLOC,
      "lru_init: (lru)");

  printf("lru_init: lru = %p\n", lru);
  printf("lru_init: lru bytes = %d\n", lru_bytes);

  memset(lru, 0, lru_bytes);

  for (i = 0; i < lru_size; ++i) {
    lru[i].next = i + 1;
    lru[i].prev = i - 1;
    lru[i].sd = -1;
  }
  lru_head = 0;
  lru_tail = lru_size - 1;
}

/* ---------------------------------------------------------------------- */
void
lru_copy_init(void)
{
  assert(lru_bytes != 0);
  printf("lru_copy_init: max_fds = %d lru_bytes = %d\n", max_fds, lru_bytes);

  lru_copy = (struct lru *) alloc_aligned(cache_line_size, lru_bytes,
      ALLOC_MALLOC, "lru_copy_init: (lru_copy)");
}
/* ---------------------------------------------------------------------- */
void
lru_closed(int sd)
{
  int self;
  self = info_index(sd);
  lru[self].sd = -1;
}

/* ---------------------------------------------------------------------- */
/* Move SELF to the front of the LRU queue: */
void
lru_access(int sd)
{
  int self;

  assert(!sock_is_listener(sd));

  if (sock_is_special(sd)) {
    return;
  }

  self = info_index(sd);
  if (self == NULL_INDEX) {
    printf("lru_access: no mapping for sd = %d\n", sd);
    exit(1);
  }

  if (LRU_DEBUG)
    printf ("%d: lru_access(%d) ->\n", (int) getpid (), self);

  if (!(self < lru_size)) {
    printf("lru_access: trying to access element %d but lru array is of size %d\n", 
            self, lru_size);
    exit(1);
  }

  lru[self].time = lru_now;
  lru[self].sd = sd;

  if (lru_head != lru_tail && self != lru_head) {
    if (self == lru_tail) {
      lru_tail = lru[self].prev;
    } else {
      lru[lru[self].prev].next = lru[self].next;
      lru[lru[self].next].prev = lru[self].prev;
    }
    lru[self].next = lru_head;
    lru[lru_head].prev = self;
    lru_head = self;
  }

  if (LRU_DEBUG) {
    int i, j, p = 0;

    for (i = 0, j = lru_head; i < lru_size; ++i) {
	printf(" %d@%d [%d,%d]", j, lru[j].sd, lru[j].prev, lru[j].next);
	if (i > 0 && lru[j].prev != p)
	  printf(" -BAD!");
	p = j;
	j = lru[j].next;
	printf("\n");
    }
    if (lru_tail != p) {
      printf (" BAD lru_tail = %d p = %d", lru_tail, p);
    }
    printf ("\n");
  }

  ldbg("lru_access: sd = %d self = %d lru_head = %d lru_tail = %d\n", 
        sd, lru_head, lru_tail, self);
}
