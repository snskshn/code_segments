/*
    userver -- (pronounced you-server or micro-server).
    This file is part of the userver, a high-performance web server designed for
    performance experiments.
          
    This file is Copyright (C) 2004  Craig Barkhouse

    Authors: Craig Barkhouse <cabarkho@uwaterloo.ca>
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


#ifndef BITMAP_H
#define BITMAP_H

#include <limits.h>

#define ROUND_UP(value, divisor) \
  (((value) % (divisor)) ? (value - ((value) % (divisor)) + divisor) : value)

#ifndef CHAR_BIT
#define CHAR_BIT      (8)
#endif


#define WORD_BYTES	(sizeof(int))
#define WORD_BITS	(WORD_BYTES * CHAR_BIT)


typedef struct bitmap {
  size_t size;
  int num_free;
  int map_bytes;
  unsigned map[1];
} bitmap_t;


bitmap_t *alloc_bitmap(size_t size);
void free_bitmap(bitmap_t *bitmap);
void bitmap_init(bitmap_t *bitmap);
int bitmap_find_and_set(bitmap_t *bitmap);
int bitmap_is_set(const bitmap_t *bitmap, int index);
void bitmap_clear(bitmap_t *bitmap, int index);

#define bitmap_size(bitmap) (bitmap)->size
#define bitmap_num_free(bitmap) (bitmap)->num_free


#endif /* BITMAP_H */
