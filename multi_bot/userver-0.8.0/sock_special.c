/*
    userver -- (pronounced you-server or micro-server).
    This file is part of the userver, a high-performance web server designed for
    performance experiments.

    Copyright (C) 2005-2010 Tim Brecht
    Based on the file originally Copyright (C) 2004  Hewlett-Packard Company

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


/* NOTE: the code assumes that any special sockets are created
 * before the listener sockets are created (i.e., the sockets
 * that listen for incoming connections from regular clients.
 */


#include <stdlib.h>
#include <limits.h>

#include "debug.h"
#include "local_debug.h"
#include "myassert.h"
#include "sock_special.h"
#include "options.h"

int sock_special_min = INT_MAX;
int sock_special_max = INT_MIN;
int sock_special_num = 0;

typedef fd_set special_set_t;
static special_set_t special_set;


/* This is only ever done once for each socket and it's
 * done before we actually start serving anything so 
 * efficiency is not an issue.
 *
 * Not clear that there is a need to do set_not_special.
 */
void
sock_set_special(int sd)
{

  if (sd > FD_SETSIZE && options.ignore_fd_setsize) {
    printf("sock_set_special: can't set special socket larger than FD_SETSIZE "
      "permits\n");
    printf("sock_set_special: sd = %d FD_SETSIZE = %d\n", sd, FD_SETSIZE);
    exit(1);
  }

  assert(!FD_ISSET(sd, &special_set));
  FD_SET(sd, &special_set);

  if (sd > sock_special_max) {
    sock_special_max = sd;
  }

  if (sd < sock_special_min) {
    sock_special_min = sd;
  }

  if (min_usable_sd == -1) {
    min_usable_sd = sd;
  }

  sock_special_num++;

  first_regular_sd = sock_special_max + 1;
  ldbg("sock_special_min %d max %d num %d\n",
      sock_special_min, sock_special_max, sock_special_num);
}


int
sock_is_special(int sd)
{
  int result = 0;

  if (sd > FD_SETSIZE && options.ignore_fd_setsize) {
    result = 0;
  } else {
    result = FD_ISSET(sd, &special_set);
  }

  PRINT_TIME(sd, &tnow, &tprev, "sock_is_special: returning %d", result);
  return(result);
}


void
sock_special_do_readable(int sd)
{
  PRINT_TIME(sd, &tnow, &tprev, "sock_special_do_readable: entered");
  PRINT_TIME(sd, &tnow, &tprev, "sock_special_do_readable: returning");
}


void
sock_special_do_writable(int sd)
{
  PRINT_TIME(sd, &tnow, &tprev, "sock_special_do_writable: entered");
  PRINT_TIME(sd, &tnow, &tprev, "sock_special_do_writable: returning");
}


void
sock_special_init(void)
{
  PRINT_TIME(NOFD, &tnow, &tprev, "sock_special_init: entered");
  PRINT_TIME(NOFD, &tnow, &tprev, "sock_special_init: returning");
}
