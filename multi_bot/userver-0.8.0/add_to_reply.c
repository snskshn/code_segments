/*
    userver -- (pronounced you-server or micro-server).
    This file is part of the userver, a high-performance web server designed for
    performance experiments.
          
    This file is Copyright (C) 2004-2010  Tim Brecht

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


/*----------------------------------------------------------------------*/

#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "add_to_reply.h"
#include "options.h"
#include "trace.h"
#include "syscalls.h"
#include "fcgi.h"
#include "stats.h"

static int add_to_reply_buffer_httpd(struct info *ip);

/*----------------------------------------------------------------------*/
/* This gets called when more data is needed to be added to the
 * reply buffer.
 * It's up to the underlying routine to track how much data 
 * has been written, how much more needs to be written and,
 * when it has written everything it needs to.
 *
 * Note that the response header may not been written yet.
 */
 
int
add_to_reply_buffer(struct info *ip)
{
  int rc = 0;
  int max_bytes = 0;
  struct rep *rep = &ip->rep;

  switch(options.acting_as)  {
    case OPT_ACTING_AS_HTTPD:
      rc = add_to_reply_buffer_httpd(ip);
      break;

    case OPT_ACTING_AS_BITTORRENT:
      max_bytes = (rep->buf + rep->buf_size) - rep->end;
#ifdef TODO
      rc = add_to_reply_buffer_bittorrent(rep->end, max_bytes);
#endif
      break;

    default:
      printf("add_to_reply_buffer: unrecognized acting_as option = %d\n",
	  options.acting_as);
      exit(1);
      break;
  }

  return rc;
}

/*----------------------------------------------------------------------*/
/* This is the httpd implementation for adding to the
 * reply buffer from an uncached file.
 * This reads data from the open file and places it into the reply buffer
 * where it will later be written to the proper socket.
 *
 * The response header has already been added to the buffer using
 * the setup_http call.
 */

static int
add_to_reply_buffer_httpd(struct info *ip)
{
  struct rep *rep = &ip->rep;
  int rc = -1;
  int trace_fd = -1;
  int nread = 0;
  int max_bytes;

  if (rep->fd <= 0) {
    return (0);
  }

  switch (ip->type) {

    case INFO_CLIENT:
      switch (ip->req.type) {
	case REQ_STATIC:
          num_file_read_calls++;
	  max_bytes = (rep->buf + rep->buf_size) - rep->end;
	  PRINT_TIME(ip->sd, &tnow, &tprev, "send_reply: non cached - "
		     "calling read %d bytes",
		     max_bytes);
	  TRACE(EVT_READ_FILE,
		trace_fd = rep->fd;
		rc = read(rep->fd, rep->end, max_bytes);
		);
	  nread = rc;
	  PRINT_TIME(ip->sd, &tnow, &tprev, "send_reply: %d bytes read "
		     "from file", nread);
	  if (nread <= 0) {
	    PRINT_TIME(ip->sd, &tnow, &tprev, "send_reply: file "
		       "read got 0 bytes ");
	    info_close_rep_fd(ip);

	    if (nread < 0) {
	      perror ("read");
	      PRINT_TIME(ip->sd, &tnow, &tprev, "send_reply: "
			 "file read failed -  returning  ");
	      return -1;
	    }
	  }
          num_file_read_bytes += nread; 
          num_file_read_successful++;
	  rep->end += nread;
	  break;
	case REQ_FASTCGI:
	  /* nothing to do */
	  break;
	default:
	  assert(0);
      }
      break;

    case INFO_FASTCGI:
      nread = fcgi_add_to_reply_buffer_httpd(ip);  // increments rep->end
      //TODO handle error case(s) ?
      break;

    default:
      assert(0);
  }

  return nread;
} /* add_to_reply_buffer_httpd */
/*----------------------------------------------------------------------*/
