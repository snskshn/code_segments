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


#include <limits.h>
#include <stdlib.h>
#include <stddef.h>

#include "myassert.h"
#include "util.h"
#include "bitmap.h"
#include "stats.h"

bitmap_t *
alloc_bitmap(size_t size)
{
  bitmap_t *bitmap;
  size_t numbytes;

  assert((ROUND_UP(size, WORD_BITS) / 8) > 0);

  numbytes = (size_t) offsetof(bitmap_t, map) + (ROUND_UP(size, WORD_BITS) / 8);
  if (numbytes < (size_t) offsetof(bitmap_t, map)) {
    goto overflow;
  }

  printf("alloc_bitmap: size = %lu WORD_BITS = %lu ROUND_UP = %lu roundup/8 = %lu, numbytes = %lu\n",
          (unsigned long) size, (unsigned long) WORD_BITS, (unsigned long) ROUND_UP(size, WORD_BITS), 
          (unsigned long) (ROUND_UP(size, WORD_BITS) / 8), (unsigned long) numbytes);

  bitmap = (bitmap_t *) alloc_aligned(ALLOC_NOALIGN, numbytes,
      ALLOC_MALLOC, "alloc_bitmap");
  if (bitmap != NULL) {
    bitmap->size = size;
    bitmap->map_bytes = (ROUND_UP(size, WORD_BITS) / 8);
    bitmap_init(bitmap);
    return bitmap;
  }
  goto fail;

overflow:
  printf("alloc_bitmap: overflow calculating size for bitmap\n");
fail:
  return NULL;
}


void
free_bitmap(bitmap_t *bitmap)
{
  assert(bitmap != NULL);
  free(bitmap);
}


void
bitmap_init(bitmap_t *bitmap)
{
  size_t size;
  unsigned *ptr;

  assert(bitmap != NULL);
  size = bitmap->size;
  ptr = bitmap->map;

  while (size > 0) {
    if (size < WORD_BITS) {
      *ptr = (UINT_MAX - 1) << (size - 1);
      printf("bitmap_init: last entry in map = 0x%x\n", *ptr);
      break;
    }
    *ptr++ = 0;
    size -= WORD_BITS;
  }

  bitmap->num_free = bitmap->size;
}


int
bitmap_find_and_set(bitmap_t *bitmap)
{
  size_t size;
  unsigned *ptr, mask;
  int index;

  assert(bitmap != NULL);
  size = bitmap->size;
  ptr = bitmap->map;
  for (index = 0; (size_t) index < size; ptr++, index += WORD_BITS) {
    if (*ptr != UINT_MAX) {
      for (mask = 1; (size_t) index < size; mask <<= 1, index++) {
	if (!(*ptr & mask)) {
	  *ptr |= mask;
	  assert(bitmap->num_free > 0);
	  bitmap->num_free--;
	  dyn_buffers_used += (bitmap->size - bitmap->num_free);
	  dyn_buffers_used_sample_count++;
		occ_bufs = (bitmap->size - bitmap->num_free);
	  if (max_dyn_buffers_used < ((int) bitmap->size - bitmap->num_free)) {
		max_dyn_buffers_used = (int) bitmap->size - bitmap->num_free;
	  }
	  assert(bitmap->num_free >= 0);
	  return index;
	}
      }
    }
  }

  assert(bitmap->num_free == 0);
  return -1;
}


int
bitmap_is_set(const bitmap_t *bitmap, int index)
{
  assert(bitmap != NULL);
  assert(index >= 0);
  assert((size_t) index < bitmap->size);

  return (bitmap->map[index / WORD_BITS] >> (index % WORD_BITS)) & 1;
}


void
bitmap_clear(bitmap_t *bitmap, int index)
{
  assert(bitmap != NULL);
  assert(index >= 0);
  assert((size_t) index < bitmap->size);

  if (bitmap->map[index / WORD_BITS] & (1 << (index % WORD_BITS))) {
    bitmap->map[index / WORD_BITS] &= ~(1 << (index % WORD_BITS));
    bitmap->num_free++;
		occ_bufs = (bitmap->size - bitmap->num_free);
    assert(bitmap->num_free > 0);
    assert(bitmap->num_free <= (int) bitmap->size);
  }
}
