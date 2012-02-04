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

#ifndef LRU_H
#define LRU_H

#include <sys/time.h>

struct lru {
    struct timeval time;
    int prev;
    int next;
    int sd;		/* socket descriptor for this conn (or -1) */
};

extern struct lru *lru;
extern struct lru *lru_copy;
extern struct timeval lru_now;
extern int lru_head;
extern int lru_tail;
extern int lru_bytes;

extern void lru_init (void);
extern void lru_access (int self);
extern void lru_closed(int sd);
extern void lru_copy_init();

#endif /* LRU_H */
