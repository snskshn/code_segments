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


#define CONFIG_SOCK_WRITE_STATS 

#include <stdlib.h>

#include "myassert.h"
#include "util.h"
#include "bufpool.h"
#include "common.h"
#include "dll.h"

bufpool_t *
alloc_bufpool(unsigned numblocks, size_t blocksize, int flags)
{
  bufpool_t *bufpool;
  size_t numbytes;

  assert(numblocks > 0);
  blocksize = ROUND_UP(blocksize, WORD_BYTES);
  assert(blocksize > 0);

  numbytes = numblocks * blocksize;
  if (numbytes / blocksize != numblocks) {
    goto overflow;
  }

  bufpool = (bufpool_t *) alloc_aligned(ALLOC_NOALIGN, sizeof(bufpool_t),
      ALLOC_MALLOC, "alloc_bufpool");
  if (bufpool != NULL) {
    bufpool->map = alloc_bitmap(numblocks);
    if (bufpool->map != NULL) {
      printf("alloc_bufpool: allocating numblocks = %u blocksize = %Zu numbytes = %Zu\n",
              numblocks, blocksize, numbytes);
      bufpool->buf = (char *) alloc_aligned_track(pagesize, numbytes,
	  flags, "alloc_bufpool", &bufpool->track);
      if (bufpool->buf != NULL) {
				bufpool->flags = flags;
				bufpool->blocksize = blocksize;
#ifdef QUEUE_BUFS
				bufpool->used_buf_q = (struct dll_list *) init_list(numblocks);
				//printf("Buffer q is at %p\n", bufpool->used_buf_q);
#endif
				return bufpool;
      }
      free_bitmap(bufpool->map);
    }
    free(bufpool);
  }
  goto fail;

overflow:
  printf("alloc_bufpool: overflow calculating size for bufpool\n");
fail:
  return NULL;
} /* alloc_bufpool */


void
free_bufpool(bufpool_t *bufpool)
{
  assert(bufpool != NULL);
  assert(bufpool->map != NULL);
  free(bufpool->map);
  bufpool->map = NULL;
  free(bufpool);
}


void *
bufpool_alloc(bufpool_t *bufpool, int sd)
{
  int index;
#ifdef QUEUE_BUFS 
	struct dll_element *el;
	struct info *client;
	void *ptr;
#endif 
  assert(bufpool != NULL);
  if ((index = bitmap_find_and_set(bufpool->map)) < 0) {
#ifdef  QUEUE_BUFS
		//check the used_buf_q to see if a buffer has finished being serviced
		ptr = check_finished_buf(bufpool->used_buf_q,sd);
		return ptr;	
			
  }
	else {
		//data was found in the bitmap, but
		//store the info_client sd for use when finding if the buffer is all sent
		el = add_value_to_tail(bufpool->used_buf_q, sd);
		client = info_ptr(sd);
		client->rep.buf_dll = el; 
	}

#else
		return NULL;
	}
#endif
  return bufpool->buf + index * bufpool->blocksize;
}

void bufpool_free(bufpool_t *bufpool, void *buf)
{
  int index;
  
	assert(bufpool != NULL);
  index = ((char *) buf - bufpool->buf) / bufpool->blocksize;
  assert((bufpool->buf + index * bufpool->blocksize) == buf);
  assert(bitmap_is_set(bufpool->map, index));

  bitmap_clear(bufpool->map, index);
}


unsigned
bufpool_num_free(bufpool_t *bufpool)
{
  assert(bufpool != NULL);
  return bitmap_num_free(bufpool->map);
}

#ifdef QUEUE_BUFS 
void * check_finished_buf(struct dll_list *q, int sd) {
	struct info *client;
	struct dll_element *el;

	if ((el= remove_from_head(q, sd)) != NULL)	{
		//found a buffer that was no longer in use
		//store the dll element for the client that requested it
		client = info_ptr(sd);
		client->rep.buf_dll = el;
		return el->buf;		//bufpool_alloc expects the address of the dynamic buffer
	}
	return NULL;
}
#endif
