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


#include <sys/types.h>
#include <stdlib.h>
#include <limits.h>
#ifdef SUNOS
/* for memset */
#include <string.h>
#endif

#ifdef DARWIN
/* for bzero */
#include <strings.h>
#endif

#include "debug.h"
#include "util.h"
#include "myassert.h"
#include "sock_listener.h"
#include "interest_set.h"
#include "options.h"
#ifdef HAVE_EPOLL
#ifdef PROVIDE_EPOLL_HEADERS
#include "sys/epoll.h"
#else
#include "sys/epoll.h"
#endif /* PROVIDE_EPOLL_HEADERS */
#endif /* HAVE_EPOLL */
#include <sys/poll.h>

// #define LOCAL_DEBUG
#ifdef LOCAL_DEBUG
#include <sys/mman.h>
#endif /* LOCAL_DEBUG */


int sock_listener_min = INT_MAX;
int sock_listener_max = INT_MIN;
int sock_listener_num = 0;

#ifdef ONE_LISTENER
int server_sd = -1;

/*----------------------------------------------------------------------*/
void
sock_set_listener(int sd)
{
  assert(server_sd == -1);
  server_sd = sd;
  sock_listener_min = server_sd;
  sock_listener_max = server_sd;
  sock_listener_num = 1;
  if (min_usable_sd == -1) {
    min_usable_sd = sd;
  }

  first_regular_sd = server_sd + 1;

  printf("socket_set_listener: server_sd = %d\n", server_sd);
  info_listener[sd].sd = sd;
}
#else

typedef fd_set listener_set_t;
static listener_set_t listener_set;
struct info_listen_struct info_listener[MAX_LISTENERS];

/*----------------------------------------------------------------------*/
/* this is only ever done once for each socket and it's
 * done before we actually start serving anything so 
 * efficiency is not an issue.
 *
 * Not clear that there is a need to do set_not_listener
 * as this is done elsewhere in different ways.
 */
void
sock_set_listener(int sd)
{
  if (sd > FD_SETSIZE && options.ignore_fd_setsize) {
    printf("sock_set_listener: can't set listener larger than FD_SETSIZE "
	"permits\n");
    printf("sock_set_listener: sd = %d FD_SETSIZE = %d\n", sd, FD_SETSIZE);
    exit(1);
  }

  assert(!FD_ISSET(sd, &listener_set));
  FD_SET(sd, &listener_set);

  if (sd > sock_listener_max) {
    sock_listener_max = sd;
  }

  if (sd < sock_listener_min) {
    sock_listener_min = sd;
  }

  if (min_usable_sd == -1) {
    min_usable_sd = sd;
  }

  sock_listener_num++;

  first_regular_sd = sock_listener_max + 1;
  PRINT_TIME(sd, &tnow, &tprev, "sock_set_listener: sock_listener_min = %d "
      "sock_listener_max = %d", sock_listener_min, sock_listener_max);
  PRINT_TIME(sd, &tnow, &tprev, "sock_set_listener: first_regular_sd = %d",
      first_regular_sd);
  if (sd >= MAX_LISTENERS) {
    printf("socket_set_listener: sd = %d info_listener has max of %d\n",
	    sd, MAX_LISTENERS);
    exit(1);
  }
  info_listener[sd].sd = sd;

}

/*----------------------------------------------------------------------*/
int
sock_is_listener(int sd)
{
  int result = 0;

  // assert(sd >= sock_listener_min); 
  // assert(sd <= sock_listener_max); 
  
  
  if (sd > FD_SETSIZE && options.ignore_fd_setsize) {
    result = 0;
  } else {
    result = FD_ISSET(sd, &listener_set);
  }

  // PRINT_TIME(sd, &tnow, &tprev, "sock_is_listener: returning %d", result);
  return(result);
}

#endif /* ONE_LISTENER */


/*----------------------------------------------------------------------*/
/* 
 * Could put this in with the general info data structures
 * but for now we don't want to mess things up that are currently
 * working and it might be better to keep these separate anyway
 * since they really don't require that much info
 */
void
info_listener_init(void)
{
  int i;

  for (i=0; i<MAX_LISTENERS; i++) {
    info_listener[i].sd = -1;
    info_listener[i].sd_state = 0;
    info_listener[i].accept_count = 0;
    info_listener[i].cur_conns = 0;
    info_listener[i].max_cur_conns = 0;
    info_listener[i].aio_accept_outstanding = 0;
    info_listener[i].maxconns = 0;
    info_listener[i].max_thold = 0;
  }

#ifndef ONE_LISTENER
  FD_ZERO(&listener_set);
#endif
}


/*----------------------------------------------------------------------*/
void
info_listener_print_status(void)
{
  int i;

  printf("---------------------------------------------------------------------\n");
  for (i=0; i<MAX_LISTENERS; i++) {
    if (info_listener[i].sd > 0) {
       printf("listener info sd = %d sd_state = 0x%x accept = %d "
          "cur = %d max_cur = %d maxconns = %d thold = %d sock_state = %c%c interest = %c%c\n",
          info_listener[i].sd, 
          info_listener[i].sd_state,
          info_listener[i].accept_count,
          info_listener[i].cur_conns,
          info_listener[i].max_cur_conns,
          info_listener[i].maxconns,
          info_listener[i].max_thold,
	  (is_sock_readable(info_listener[i].sd, 0) ? 'R' : '-'),
	  (is_sock_writable(info_listener[i].sd, 0) ? 'W' : '-'),
	  (interest_set_is_readable(info_listener[i].sd) ? 'R' : '-'),
	  (interest_set_is_writable(info_listener[i].sd) ? 'W' : '-'));
    }
  }
}


/*----------------------------------------------------------------------*/
void
info_listener_compute_limits(void)
{
  int i = 0;
  int count = 0;
  int maxconns_per_listener = -1;
  int maxconns_remaining = -1;
  int max_thold_per_listener = -1;
  int max_thold_remaining = -1;
  struct info_listen_struct *ilp = 0;
 
  maxconns_per_listener = maxconns / sock_listener_num;
  maxconns_remaining = maxconns % sock_listener_num;
  max_thold_per_listener = options.aio_accept_thold / sock_listener_num;
  max_thold_remaining = options.aio_accept_thold % sock_listener_num;
  PRINT_TIME(NOFD, &tnow, &tprev, 
    "info_listener_compute_limits: maxconns_per_listener = %d",
    maxconns_per_listener);
  PRINT_TIME(NOFD, &tnow, &tprev, 
    "info_listener_compute_limits: maxconns_remaining = %d",
    maxconns_remaining);
  PRINT_TIME(NOFD, &tnow, &tprev, 
    "info_listener_compute_limits: max_thold_per_listener = %d",
    max_thold_per_listener);
  PRINT_TIME(NOFD, &tnow, &tprev, 
    "info_listener_compute_limits: max_thold_remaining = %d",
    max_thold_remaining);

  /* set some parameters now */
  count = 0;
  for (i=sock_listener_min; i<=sock_listener_max; i++) {
    if (sock_is_listener(i)) {
      ilp = &info_listener[i];
      ilp->max_thold = max_thold_per_listener;
      ilp->maxconns = maxconns_per_listener;
      if (count < max_thold_remaining) {
        ilp->max_thold++;
      }
      if (count < maxconns_remaining) {
        ilp->maxconns++;
      }

      if (options.aio_accept_thold &&  ilp->max_thold <= 0) {
        printf("info_listener_compute_limits: max_thold <= 0 for "
	    "listener %d\n", i);
        printf("info_listener_compute_limits: adjust number of listening"
	    "sockets or --aio-accept_thold\n");
	assert(0);
	exit(1);
      }
      if (ilp->maxconns <= 0) {
        printf("info_listener_compute_limits: maxconns <= 0 for "
	    "listener %d\n", i);
        printf("info_listener_compute_limits: adjust number of listening"
	    "sockets or --maxconns\n");
        printf("The server limits the maximum number of connections\n");
        printf("We try to spread those connections evenly accross all listening sockets\n");
        printf("This includes sockets listening for SSL connections\n");
        printf("The combination of the number of listening sockets you have created and"
                "the maximum number of connections requested can not be satisfied\n");

	assert(0);
	exit(1);

      }
      count++;
    }
  }
}

/*----------------------------------------------------------------------*/
void
sd_state_listener_not_readable(int sd)
{
  info_listener[sd].sd_state = 0;
  PRINT_TIME(sd, &tnow, &tprev, "sd_state_listener_nont_readable: "
    "is not readable");
}

void
sd_state_listener_readable(int sd)
{
#ifdef HAVE_EPOLL
  info_listener[sd].sd_state = EPOLLIN;
#else
  info_listener[sd].sd_state = POLLIN;
#endif
  PRINT_TIME(sd, &tnow, &tprev, "sd_state_listener_readable: is readable");
}


/*----------------------------------------------------------------------*/
void
stats_print_listeners(void)
{
  struct info_listen_struct *ilp;
  int i;

  printf("Listener stats and info\n");

  printf("%2s %10s %10s %10s %10s %10s %10s %10s\n", 
      "sd", "accepted", "cur_conns", "max_cur", "outstanding", "maxconns", 
      "thold", "sd_state");
  for (i=sock_listener_min; i<=sock_listener_max; i++) {
    if (info_listener[i].sd != -1) {
      ilp = &info_listener[i];
      printf("%2d %10d %10d %10d %10d %10d %10d %10d\n", 
	  ilp->sd, ilp->accept_count, ilp->cur_conns,
	  ilp->max_cur_conns, ilp->aio_accept_outstanding, 
	  ilp->maxconns, ilp->max_thold, ilp->sd_state);
    }
  }
  printf("\n");
}
/*----------------------------------------------------------------------*/

int
sock_accept_more(int listen_sd)
{
   int retval = 1;
   struct info_listen_struct *ilp = 0;

   assert(sock_is_listener(listen_sd));

   ilp = &info_listener[listen_sd];

   PRINT_TIME(listen_sd, &tnow, &tprev, 
	"sock_accept_more: maxconns = %d",
	ilp->maxconns);
   PRINT_TIME(listen_sd, &tnow, &tprev, 
	"sock_accept_more: max_thold = %d",
	ilp->max_thold);

   PRINT_TIME(listen_sd, &tnow, &tprev, "sock_accept_more: outstanding = %d", 
       ilp->aio_accept_outstanding);
   PRINT_TIME(listen_sd, &tnow, &tprev, "sock_accept_more: cur_conns = %d", 
       ilp->cur_conns);

   if ((ilp->aio_accept_outstanding + ilp->cur_conns) < 
        ilp->maxconns) {
      PRINT_TIME(listen_sd, &tnow, &tprev, 
	  "sock_accept_more: not reached connection limit");
   } else {
      PRINT_TIME(listen_sd, &tnow, &tprev, 
	  "sock_accept_more: reached connection limit");
      retval = 0;
   }

   if (options.aio_accept_thold != 0) {
     if (ilp->aio_accept_outstanding < ilp->max_thold) {
        PRINT_TIME(listen_sd, &tnow, &tprev, 
	    "sock_accept_more: not reached thold limit");
     } else {
      PRINT_TIME(listen_sd, &tnow, &tprev, 
	  "sock_accept_more: reached thold limit");
      retval = 0;
     }
   }

   PRINT_TIME(listen_sd, &tnow, &tprev, 
       "sock_accept_more: retval = %d", retval);
   return(retval);
}

/*----------------------------------------------------------------------*/
