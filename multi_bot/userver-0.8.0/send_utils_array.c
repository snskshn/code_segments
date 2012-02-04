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


#ifdef SEND
#ifdef ARRAY_OF_BUFS

#include <arpa/inet.h>
#include <sys/poll.h>

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "send.h"
#include "debug.h"
#include "stats.h"
#include "trace.h"
#include "send_utils.h"
#include "sock_listener.h"

void print_all_evts_array();
void print_ecb_types_array();
void print_evt_type_info(int buf_index);

void 
verify_evt_array(const struct event *evt, int buf_index)
{
  if (evt < &ecb->eventbuf[buf_index][0] || 
      evt > &ecb->eventbuf[buf_index][ecb->event_list_size[buf_index]-1]) {
    printf("verify_evt: Bad event pointer: evt = %p\n", evt);
    printf("verify_evt: head = %d tail = %d\n", 
	    ecb->head[buf_index], ecb->tail[buf_index]);
    printf("verify_evt: eventbuf[size] = %d\n",
       ecb->event_list_size[buf_index]);
  }
}

void
print_ecb_types_array(void)
{
  register int i;

  for (i=0; i<EVT_BUFS; i++) {
    printf("Event type info for buffer %d\n", i);
    print_evt_type_info(i);
    printf("\n");
  }
}

void
print_evt_type_info(int buf_index)
{
  int i;
  for (i=0; i<ecb->event_list_size[buf_index]; i++) {
    if (ecb->eventbuf[buf_index][i].type!=-1) {
      if (ecb->eventbuf[buf_index][i].type >= 0 &&
	  ecb->eventbuf[buf_index][i].type <= 1000) {

	printf("[%i].type == %i\n", i,ecb->eventbuf[buf_index][i].type);
      } else {
	printf("[%d].type == 0x%x\n", i,ecb->eventbuf[buf_index][i].type);
      }
    }
  }
}

void 
print_all_evts_array(void)
{
  int i;
  struct event *evt = 0;
  int buf_index = 0;

  if (ecb == 0) {
    printf("Event control block is null - events not used\n"); 
    return;
  }

  /* pause for a bit in case new events are still arriving */
  sleep(1);

  for (buf_index=0; buf_index<EVT_BUFS; buf_index++) {
    printf("Remaining events in buffer %d: head = %d tail = %d\n", 
	    buf_index, ecb->head[buf_index], ecb->tail[buf_index]);

    for (i=(ecb->head[buf_index])+1; i<ecb->tail[buf_index]; i++) {
      evt = (struct event *) 
	&ecb->eventbuf[buf_index][i % ecb->event_list_size[buf_index]];
      printf("event number = %d (%d)\n", 
	  i, (i % ecb->event_list_size[buf_index]));
      print_event(evt, 0);
    } /* for */
  }
}


void
send_print_info_array(void)
{
  int i;
  if (options.send_events && ecb) {
    printf("ecb->queue = 0x%08x ecb->notify = 0x%08x\n",
      ecb->queue, ecb->notify);
    printf("ecb->num_queued = %d\n",
	ecb->num_queued);
    for (i=0; i<EVT_BUFS; i++) {
      printf("ecb->head[%d] = %d ecb->tail[%d] = %d\n",
	  i, ecb->head[i], i, ecb->tail[i]);
    }
#ifdef TBB_KINFO
    if (kinfo) {
      printf("kinfo->qlen_young = %d kinfo->qlen = %d\n",
	  kinfo->qlen_young, kinfo->qlen);
    }
#endif /* TBB_KINFO */
  }
}

void
send_init_event_mappings(void)
{
  int i;

  /* ensure that there are no sds set for the default buffer */
  FD_ZERO(&ecb->fdset[EVT_DEFAULT_BUF]);
  PRINT_TIME(NOFD, &tnow, &tprev, "send_init_event_mappings: "
      "zeroing fdset %d", EVT_DEFAULT_BUF);

  if (options.auto_accept) {
    ecb->evt_type_mask[EVT_ACCEPT_BUF] = (1<<EVT_IPACCEPT);
    PRINT_TIME(NOFD, &tnow, &tprev, 
      "send_init_event_mappings: set evt_type_mask = 0x%x for index = %d\n",
      ecb->evt_type_mask[EVT_ACCEPT_BUF], EVT_ACCEPT_BUF);
  } else {
    /* if we want to have events associated with the listener sockets
     * sent to a buffer that is separate from where other 
     * events are sent we need to set that up.
     */
    FD_ZERO(&ecb->fdset[EVT_ACCEPT_BUF]);
    PRINT_TIME(NOFD, &tnow, &tprev, "send_init_event_mappings: "
	"zeroing fdset %d", EVT_ACCEPT_BUF);

    for (i=sock_listener_min; i<=sock_listener_max; i++) {
      if (sock_is_listener(i)) {
	FD_SET(i, &ecb->fdset[EVT_ACCEPT_BUF]);
	PRINT_TIME(i, &tnow, &tprev, "send_init_event_mappings: "
	    "setting %d in fdset %d", i, EVT_ACCEPT_BUF);
      }
    }
  }

  /* NOTE: SPECIAL FD/SD delivery setup could/should go here */


  /* now set things up so that all events on other 
   * sds go to another buffer
   */

  FD_ZERO(&ecb->fdset[EVT_IO_BUF]);
  PRINT_TIME(NOFD, &tnow, &tprev, "send_init_event_mappings: "
      "zeroing fdset %d", EVT_IO_BUF);
  PRINT_TIME(NOFD, &tnow, &tprev, 
      "send_init_event_mappings: setting bits from %d to %d in fdset %d",
      first_regular_sd, FD_SETSIZE-1, EVT_IO_BUF);
  for (i=first_regular_sd; i<FD_SETSIZE; i++) {
    FD_SET(i, &ecb->fdset[EVT_IO_BUF]);
  }
}

#endif /* ARRAY_OF_BUFS */
#endif /* SEND */
