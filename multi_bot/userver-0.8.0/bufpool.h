/*
    userver -- (pronounced you-server or micro-server).
    This file is part of the userver, a high-performance web server designed for
    performance experiments.
          
    This file is Copyright (C) 2004  Craig Barkhouse

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


#ifndef BUFPOOL_H
#define BUFPOOL_H

#include "util.h"
#include "bitmap.h"
#include "dll.h"

typedef struct bufpool {
  int flags;
  size_t blocksize;
  bitmap_t *map;
  char *buf;
  alloc_track_t *track;
	struct dll_list *used_buf_q;
} bufpool_t;


bufpool_t *alloc_bufpool(unsigned numblocks, size_t blocksize, int flags);
void free_bufpool(bufpool_t *bufpool);
void *bufpool_alloc(bufpool_t *bufpool, int sd);
void bufpool_free(bufpool_t *bufpool, void *buf);

unsigned bufpool_num_free(bufpool_t *bufpool);
void *check_finished_buf(struct dll_list *list, int sd);
#define bufpool_blocksize(bufpool) (bufpool)->blocksize


#endif /* BUFPOOL_H */
