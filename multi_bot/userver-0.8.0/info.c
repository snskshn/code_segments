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
#include <string.h>
#include <unistd.h>
#include <errno.h>
#ifdef SUNOS
#include <sys/types.h>
#endif
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "common.h"
#include "options.h"
#include "info.h"
#include "myassert.h"
#include "state.h"
#include "debug.h"
#include "aio_layer.h"
#include "stats.h"
#include "syscalls.h"
#include "util.h"
#include "interest_set.h"
#include "fcgi.h"
#include "sock_listener.h"
#include "dll.h"

// #define LOCAL_DEBUG
#include "local_debug.h"

int sd_map_first_free = NULL_INDEX;
int info_first_free = NULL_INDEX;
int *sd_map;
int num_info_structs = 0;
struct info *info;


static void info_free_entry(int index);
static int info_get_entry();
void sd_map_print();
void sd_map_free_print();
void info_free_print();
void info_print();
void check_if_alive(int sd);

#ifdef HAVE_AIO_LAYER
void *read_reg_addr = 0;
void *read_reg_handle = 0;
int   read_reg_size = 0;
void *write_reg_addr = 0;
void *write_reg_handle = 0;
int   write_reg_size = 0;
#endif /* HAVE_AIO_LAYER */

char *reason_str[] = {
/*   0 */  "",
/*  -1 */  "Client closed connection",
/*  -2 */  "Connection was idle",
/*  -3 */  "Not enough space for response",
/*  -4 */  "AIO write completion error",
/*  -5 */  "SEND poll error",
/*  -6 */  "SEND poll fin",
/*  -7 */  "Close after accept option specified",
/*  -8 */  "Close after socket init option specified",
/*  -9 */  "Read failed with connection error",
/* -10 */  "Close after read option specified",
/* -11 */  "Close after parse option specified",
/* -12 */  "HTTP 1.0 close after response",
/* -13 */  "Special escape sequence specified in URI",
/* -14 */  "Fstat failed",
/* -15 */  "Send reply failed",
};



/* ---------------------------------------------------------------------- */
int
info_init(void)
{
#ifdef HAVE_AIO_LAYER
  int rc = 0;
#endif /* HAVE_AIO_LAYER */
  int i;
  struct info *ip;
  int size;
  char *tmp_read = 0;
  char *tmp_reply = 0;
  app_t *app;
  appserver_t *appserver;

  /*
   * Compute the maximum number of info structs we could possibly use.
   * First, we'll need an info struct for each possible client connection.
   */
  num_info_structs = maxconns;

  /*
   * Finally, we'll need some extra info structs to accommodate any special
   * sockets which might use them.  Currently, this means one for each
   * FastCGI application server.
   */
  for (i = 0; i < options.app_count; i++) {
    app = &options.app[i];
    switch (app->req_type) {
      case REQ_FASTCGI:
	appserver = app->server;
	assert(appserver != NULL);
	do {
	  num_info_structs++;
	  appserver = appserver->next;
	} while (appserver != NULL);
	break;
      default:
	/* nothing to do for this type of application */
	break;
    }
  }

  printf("info_init: sizeof(struct req)  = %d mod cache_line_size = %d "
          "mod 4 = %d\n", 
          (int) sizeof(struct req),
          (int) sizeof(struct req) % cache_line_size,
          (int) sizeof(struct req) % 4);
  printf("info_init: sizeof(struct rep)  = %d mod cache_line_size = %d "
          "mod 4 = %d\n", 
          (int) sizeof(struct rep),
          (int) sizeof(struct rep) % cache_line_size,
          (int) sizeof(struct rep) % 4);

  printf("info_init: sizeof(struct info) = %d mod cache_line_size = %d "
          "mod 4 = %d\n", 
          (int) sizeof(struct info),
          (int) sizeof(struct info) % cache_line_size,
          (int) sizeof(struct info) % 4);

  size = num_info_structs * sizeof(struct info);
  if (size / (int) sizeof(struct info) != num_info_structs) {
    printf("info_init: overflow calculating size for info\n");
    exit(1);
  }

  info = (struct info *) alloc_aligned(cache_line_size, size, 
         ALLOC_MALLOC, "info_init (info array):");

  printf("info_init: info entries = %d size = %lu\n", 
      num_info_structs, (unsigned long) size);
  // memset(info, 0, size);

  printf("info_init: read buffer size = %d\n", options.read_buffer_size);
  size = num_info_structs * options.read_buffer_size;
  if (size / options.read_buffer_size != num_info_structs) {
    printf("info_init: overflow calculating size for tmp_read\n");
    exit(1);
  }

  /* NOTE: this is mmap'ed because it's an easy way to ensure
     that the data is page aligned, which IIRC is required for 
     aio_sock_mem_reg below
   */
  tmp_read = (char *) alloc_aligned(pagesize, size, ALLOC_MMAP, "info_init:");

  printf("info_init: read buffer entries = %d total size = %lu\n", 
      num_info_structs, (unsigned long) size);

  /* Depending on the underlying layer this may have to be done
   * for each chunk below - instead of all at once like this 
   */
  if (options.use_socket_aio) {
#ifdef HAVE_AIO_LAYER
    rc = aio_sock_mem_reg(tmp_read, size, &read_reg_handle);
    stats_reg_bytes(size);
    if (rc < 0) {
      printf("info_init: aio_sock_mem_reg failed (read buffer) "
	  "addr = %p size = %d handle = %p\n", tmp_read, size, read_reg_handle);
      exit(1);
    }

    PRINT_TIME(NOFD, &tnow, &tprev, "info_init: aio_sock_mem_reg start = %p "
      "size = %d handle = %p", tmp_read, size, read_reg_handle);
    read_reg_addr = tmp_read;
    read_reg_size = size;
#else
    printf("info_init: aio not handled\n");
    exit(1);
#endif /* HAVE_AIO_LAYER */
  }

  printf("info_init: reply buffer size = %d\n", options.reply_buffer_size);
  size = num_info_structs * options.reply_buffer_size;
  if (size / options.reply_buffer_size != num_info_structs) {
    printf("info_init: overflow calculating size for tmp_reply\n");
    exit(1);
  }

  tmp_reply = (char *) alloc_aligned(pagesize, size, ALLOC_MMAP,
      "info_init: (reply buffer)");
  printf("info_init: reply buffer entries = %d total size = %lu\n", 
      num_info_structs, (unsigned long) size);

  /* Depending on the underlying layer this may have to be done
   * for each chunk below - instead of all at once like this 
   */
  if (options.use_socket_aio) {
#ifdef HAVE_AIO_LAYER
    rc = aio_sock_mem_reg(tmp_reply, size, &write_reg_handle);
    stats_reg_bytes(size);
    if (rc < 0) {
      printf("info_init: aio_sock_mem_reg failed (reply buffer) "
        "addr = %p size = %d handle = %p\n", 
        tmp_reply, size, write_reg_handle);
      exit(1);
    }
    PRINT_TIME(NOFD, &tnow, &tprev, "info_init: aio_sock_mem_reg start = %p "
      "size = %d handle = %p", tmp_reply, size, write_reg_handle);

    write_reg_addr = tmp_reply;
    write_reg_size = size;
#else
    printf("info_init: aio not handled\n");
    exit(1);
#endif /* HAVE_AIO_LAYER */
  }

  for (i = 0; i < num_info_structs; i++) {
    ip = &info[i];
#ifdef FIXHUP
    ip->got_hup = 0;
#endif /* FIXHUP */

#ifdef TRACK_STUCK_CONNS
    ip->fsm_state_history = 0;
    ip->prev_fsm_state_history = 0;
    ip->read_count = 0;
    ip->read_nread = -99;
    ip->read_errno = -99;
    ip->session_len = 0;
#endif /* TRACK_STUCK_CONNS */
    ip->sd = -1;

#ifdef HAVE_SSL
    // ip->is_ssl = 0;
    ip->ssl = 0;
    // ip->ssl_ctx = 0;
#endif /* HAVE_SSL */

    set_fsm_state(ip, FSM_INITIAL_STATE);
    ip->skip = 0;
    ip->skip_info = 0;
#ifdef HAVE_EPOLL_CTLV
    ip->sd_state = 0;
#endif /* HAVE_EPOLL_CTLV */
    ip->req.num = -1;
    ip->conn_num = -1;
    ip->req.static_buf = tmp_read;
    ip->req.dyn_buf = NULL;
    ip->req.buf = ip->req.static_buf;
    ip->req.buf_size = options.read_buffer_size;
    ip->req.end = ip->req.cur = ip->req.buf;
    PRINT_TIME(NOFD, &tnow, &tprev, "info_init: (read) req.buf[%d] = %p", 
       i, ip->req.buf);
    tmp_read += ip->req.buf_size;
    ip->rep.fd = -1;
    ip->rep.static_buf = tmp_reply;
    ip->rep.dyn_buf = NULL;
    ip->rep.buf = ip->rep.static_buf;
    ip->rep.buf_size = options.reply_buffer_size;
    ip->rep.end = ip->rep.cur = ip->rep.buf;
    tmp_reply += ip->rep.buf_size;
#ifdef QUEUE_BUFS
		ip->done_write = 0;
#endif
#ifdef HAVE_AIO_LAYER
    ip->req.read_reg_handle = read_reg_handle;
    ip->rep.write_reg_handle = write_reg_handle;
#endif /* HAVE_AIO_LAYER */

#ifdef CALL_STATS
    ip->req.client_id = 0;
    ip->req.call_id = 0;
#endif /* CALL_STATS */

  } /* for */


  size = (max_fds + 1) * sizeof(int);
  if (size / (int) sizeof(int) != (max_fds + 1)) {
    printf("info_init: overflow calculating size for sd_map\n");
    exit(1);
  }

  sd_map = (int *) alloc_aligned(cache_line_size, size, ALLOC_MALLOC,
      "info_init: (sd_map)");
  printf("info_init: sd_map buffer entries = %d total size = %lu\n", 
      max_fds, (unsigned long) size);

  for (i = 0; i <= max_fds; i++) {
    sd_map[i] = NULL_INDEX;
  }

  /* NOTE: I think that they must be done in this order */
  for (i = num_info_structs - 1; i >= 0; i--) {
    info_free_entry(i);
  }

#ifdef LOCAL_DEBUG
  info_free_print();
  info_print();
  sd_map_free_print();
  sd_map_print();
#endif /* LOCAL_DEBUG */

  return 1;
} /* info_init */

/* ---------------------------------------------------------------------- */
void
info_free_print(void)
{
  int i = info_first_free;

  printf("info_free_print: printing free info entries\n");
  while (i != NULL_INDEX) {
    printf("info[%d] is free\n", i);
    i = info[i].next_free;
  }
}

/* ---------------------------------------------------------------------- */
void
info_print(void)
{
  int i;

  printf("info_print: printing info entries\n");
  printf("info_print: info_first_free = %d\n", info_first_free);
  for (i = 0; i < num_info_structs; i++) {
    printf("info[%d].next_free = %d\n", i, info[i].next_free);
  }
}

/* ---------------------------------------------------------------------- */
void
info_print_status(void)
{
  int i;
  int sd_count = 0;
  int iffy_sd_count = 0;

  printf("------------------------------------------------------------\n");
  printf("info_print_status: printing info status entries\n");
  printf("info_print_status: info_first_free = %d\n", info_first_free);
  printf("info_print_status: num_info_structs = %d\n", num_info_structs);
  for (i = 0; i < num_info_structs; i++) {
    if (info[i].sd > 0) {
      sd_count++;
      /* only print out info about listners, clients and potentially 
       * interesting/worrisome FCGI connections 
       */
      if ((info[i].type == INFO_LISTENER) || 
          (info[i].type == INFO_CLIENT) ||
          ((info[i].type == INFO_FASTCGI) && 
           ((info[i].fsm_state != FSM_WRITING_REPLY)) && (info[i].fsm_state != FSM_INITIAL_STATE)) ||
          ((info[i].type == INFO_FASTCGI) && (info[i].rep.fd >= 0)) ) {

          /* if an sd is in one of these states we probably want to understand why,
           * sometimes it might be because the server has been killed before handling
           * all connections, BUT it might be because some things are not getting 
           * cleaned up properly
           */
          iffy_sd_count++;

#ifdef TRACK_STUCK_CONNS
	  printf("%6d type = %d sd = %6d sd_state = 0x%4x fsm_state = %2d history = 0x%08x prev history = 0x%08x "
		 "reads = %d nread = %d read_errno = %d session_len = %6d sock state = %c%c%c interest = %c%c conn_num = %6d cache = %12p HTTP 1.%d ", 
#else
	  printf("%6d type = %d sd = %6d sd_state = 0x%4x fsm_state = %2d "
		 "sock state = %c%c interest = %c%c%c conn_num = %6d cache = %12p HTTP 1.%d ", 
#endif /* TRACK_STUCK_CONNS */
		  i, 
		  info[i].type,
		  info[i].sd, 
		  info[i].sd_state,
		  info[i].fsm_state,
#ifdef TRACK_STUCK_CONNS
		  info[i].fsm_state_history,
		  info[i].prev_fsm_state_history,
		  info[i].read_count,
		  info[i].read_nread,
		  info[i].read_errno,
		  info[i].session_len,
#endif /* TRACK_STUCK_CONNS */
		  (is_sock_readable(info[i].sd, 0) ? 'R' : '-'),
		  (is_sock_writable(info[i].sd, 0) ? 'W' : '-'),
		  (is_sock_hupped(info[i].sd, 0) ? 'H' : '-'),
		  (interest_set_is_readable(info[i].sd) ? 'R' : '-'),
		  (interest_set_is_writable(info[i].sd) ? 'W' : '-'),
		  info[i].conn_num,
		  info[i].rep.cache, 
		  info[i].req.close ? 0 : 1);

	  printf("rep.fd = %6d ", info[i].rep.fd);

	  if (info[i].rep.cache) {
	    printf("fd = %6d", info[i].rep.cache->fd);
	  }

#ifdef FIXHUP
	  printf(" got_hup = %d\n", info[i].got_hup);
#endif /* FIXHUP */

	  printf("\n");

#ifdef TRACK_STUCK_CONNS
          if (info[i].type == INFO_CLIENT) {
#ifdef HAVE_EPOLL
             if (options.use_epoll) {
                check_epollable(info[i].sd);
             }
             /* above test is probably sufficient */
             check_if_alive(info[i].sd);
#endif /* HAVE_EPOLL */
          }
#endif /* TRACK_STUCK_CONNS */
      }

    }
  }
  printf("sd_count      = %10d\n", sd_count);
  printf("iffy_sd_count = %10d\n", iffy_sd_count);
#if 0
  printf("Sleeping for 60 seconds to check on clients\n");
  sleep(60);
#endif
}

/* ---------------------------------------------------------------------- */
struct info *
info_add(int sd)
{
  int index = NULL_INDEX;
  ldbg("info_add: entered sd = %d\n", sd);

  assert(sd >= 0);
  assert(sd <= max_fds);
  assert(sd_map[sd] == NULL_INDEX);

  index = info_get_entry();

  sd_map[sd] = index;
  info[index].sd = sd;

  ldbg("info_add: returning sd = %d index = %d\n", sd, index);

  return(&info[index]);
}

/* ---------------------------------------------------------------------- */
struct info *
info_reserve(void)
{
  int index;
  struct info *ip;

  ldbg("info_reserve: entered\n");
  index = info_get_entry();
  assert(index != NULL_INDEX);
  ip = &info[index];

  ldbg("info_reserve: returning index = %d ip = %p\n", index, ip);
  return ip;
}

/* ---------------------------------------------------------------------- */
void
info_unreserve(struct info *ip)
{
  int index;

  ldbg("info_register: entered ip = %p\n", ip);
  assert(ip != NULL);
  index = ip - info;
  assert(index >= 0);
  assert(index <= max_sock_sd);
  assert(ip == &info[index]);

  info_free_entry(index);
}

/* ---------------------------------------------------------------------- */
void
info_register(struct info *ip, int sd)
{
  int index;

  ldbg("info_register: entered ip = %p sd = %d\n", ip, sd);
  assert(ip != NULL);
  assert(sd >= 0);
  assert(sd <= max_fds);
  index = ip - info;
  assert(index >= 0);
  assert(index <= max_sock_sd);
  assert(ip == &info[index]);

  sd_map[sd] = index;
  info[index].sd = sd;

  ldbg("info_register: returning index = %d\n", index);
}

/* ---------------------------------------------------------------------- */
void
info_delete(int sd)
{
  int index = NULL_INDEX;
  assert(sd >= 0);
  assert(sd <= max_fds);
  ldbg("info_delete: entered sd = %d sd_map_first_free = %d\n",
      sd, info_first_free);

  info[sd_map[sd]].sd = -1;
  info[sd_map[sd]].fsm_state = FSM_INITIAL_STATE;
  /* TODO Look into how to do this  properly */
  /* info_set_fsm_state(info[sd_map[sd]],  FSM_INITIAL_STATE); */


  index = sd_map[sd];
  assert(index != NULL_INDEX);
  info[index].sd = -1;
  info_free_entry(index);
  sd_map[sd] = NULL_INDEX;

  ldbg("info_delete: returning sd = %d sd_map_first_free = %d\n",
      sd, sd_map_first_free);

}

/* ---------------------------------------------------------------------- */
static void
info_free_entry(int index)
{
  int old_first = info_first_free;
  int old_next = NULL_INDEX;

  assert((index >= 0) && (index <= max_sock_sd));

  if (info_first_free != NULL_INDEX) {
    old_next = info[info_first_free].next_free;
  }

  ldbg("info_free_entry: entered index = %d old_first = %d old_next = %d\n",
	index, old_first, old_next);

  info_first_free = index;
  info[info_first_free].next_free = old_first;
  ldbg("info_free_entry: freed index = %d first_free = %d\n",
	index, info_first_free);
  if (info_first_free != NULL_INDEX) {
     ldbg("info_free_entry: next = %d\n", info[info_first_free].next_free);
  }
}

/* ---------------------------------------------------------------------- */
static int
info_get_entry(void)
{
  int index = info_first_free;
#ifdef LOCAL_DEBUG
  ldbg("info_get_entry: first_free = %d next = %d cur_conns = %d\n",
	info_first_free, info[info_first_free].next_free, cur_conns);
#endif /* LOCAL_DEBUG */

#ifdef LOCAL_DEBUG
  info_free_print();
  info_print();
  sd_map_free_print();
  sd_map_print();
#endif /* LOCAL_DEBUG */

  assert(info_first_free != NULL_INDEX);
  assert(info_first_free >= 0); 
  assert(info_first_free <= max_fds);

  info_first_free = info[index].next_free;
  info[index].next_free = NULL_INDEX;

  ldbg("info_get_entry: returning index = %d first_free = %d\n",
	index, info_first_free);
  
  return(index);
}

/* ---------------------------------------------------------------------- */
void
sd_map_print(void)
{
  int i;
  printf("sd_map_print:\n");

  for (i = 0; i <= max_fds; i++) {
    printf("sd_map[%d] = %d\n", i, sd_map[i]);
  }
}

/* ---------------------------------------------------------------------- */
void
sd_map_free_print(void)
{
  int tmp = sd_map_first_free;
  printf("sd_map_free_print\n");

  while (tmp != NULL_INDEX) {
    printf("free = %d\n", tmp);
    tmp = sd_map[tmp];
  }
}

/* ---------------------------------------------------------------------- */
struct info *
info_ptr(int sd)
{
  int index;
  struct info *ip;

  index = info_index(sd);
  if (index == NULL_INDEX) {
    ip = NULL;
  } else {
    ip = &info[index];
    assert(ip->sd == sd);
  }
  ldbg("info_ptr: sd = %d index = %d ip = %p\n", sd, index, ip);
  return(ip);
}

/* ---------------------------------------------------------------------- */
struct req *
info_req_ptr(int sd)
{
  struct info *ip = info_ptr(sd);
  assert(ip);
  return(&(ip->req));
}

/* ---------------------------------------------------------------------- */
struct rep *
info_rep_ptr(int sd)
{
  struct info *ip = info_ptr(sd);
  assert(ip);
  return(&(ip->rep));
}

/* ---------------------------------------------------------------------- */
int
info_get_fsm_state(int sd)
{
  struct info *ip = info_ptr(sd);
  assert(ip);
  return(ip->fsm_state);
}

/* ---------------------------------------------------------------------- */
void
info_set_fsm_state(int sd, enum info_fsm_state state)
{
  struct info *ip = info_ptr(sd);
  assert(ip);
  set_fsm_state(ip, state);
}

/* ---------------------------------------------------------------------- */
int
info_index(int sd)
{
  int index = NULL_INDEX;
  ldbg("info_index: looking for sd = %d\n", sd);
  assert(sd >= 0);
  assert(sd <= max_fds);

  index = sd_map[sd];
  assert(index <= max_sock_sd);

  ldbg("info_index: sd = %d returning %d\n", sd, index);
  return(index);
}

/* ---------------------------------------------------------------------- */
int
info_map_valid(int sd)
{
  int valid = 0;
  ldbg("info_map_done: looking for sd = %d\n", sd);
  assert(sd >= 0);
  assert(sd <= max_fds);

  if (sd_map[sd] == NULL_INDEX) {
    valid = 0;
  } else {
    valid = 1;
  }

  ldbg("info_map_valid: sd = %d returning %d\n", sd, valid);
  return(valid);
}

/* ---------------------------------------------------------------------- */
/* Close the fd used for replies.  */
/* For static requests: 
 *   if the file doesn't come from the cache we close the fd for the file
 * For dynamic requests we actually trigger the servicing of the next request
 *   TBB - I really don't like that this servicing is done in a routine
 *   that sounds like it is closing things down.
 *
 * I think that this should only ever be called on a client socket
 * (i.e., it probably shouldn't get called on an appserver socket)
 * At some point we should figure this out and add an assertion
 * if it is true.
 */

int
info_close_rep_fd(struct info *ip)
{
  int peer_sd;
  struct info *peer_ip;
  int rc = 0;

  PRINT_TIME(ip->sd, &tnow, &tprev, "info_close_rep_fd: ip->type = %d fd = %d",
      ip->type, ip->rep.fd);
  PRINT_TIME(ip->sd, &tnow, &tprev, "info_close_rep_fd: ip->req.type = %d ip->rep.use_cache = %d",
      ip->req.type, ip->rep.use_cache);

  if (ip->rep.fd < 0) {
    return 0;
  }

  /* we shouldn't be closing stdin, stdout, or stderr */
  assert(ip->rep.fd >= 3);

  switch (ip->type) {
    case INFO_CLIENT:
      switch (ip->req.type) {
	case REQ_STATIC:
	  if (!ip->rep.use_cache) {
	    /* only done if not using the cached copy */
	    rc = close_file(ip->rep.fd);
	  }
	  break;
	case REQ_FASTCGI:
	  peer_sd = ip->rep.fd;
	  peer_ip = info_ptr(peer_sd);
	  assert(peer_ip != NULL);
	  assert(peer_ip->rep.fd == ip->sd);
	  /* the FastCGI app is free to service another request */
          PRINT_TIME(ip->sd, &tnow, &tprev, "info_close_rep_fd: service next request with peer_sd = %d", peer_sd);
	  fcgi_service_next_request(peer_ip);
	  break;
#ifdef MOD_SPECWEB99
	case REQ_SPECWEB99:
	  /* nothing to do */
	  break;
#endif
	default:
	  printf("info_close_rep_fd(): invalid ip->req.type %d\n",
	      ip->req.type);
	  assert(0);
      }
      break;

    default:
      printf("info_close_rep_fd(): invalid ip->type %d\n", ip->type);
      assert(0);
  }

  /* invalidate the fd */
  ip->rep.fd = -1;

  return rc;
}

/* ---------------------------------------------------------------------- */
/*
 * Use this function to reset rep.buf to point to rep.static_buf, and to
 * reset rep.cur and rep.end so that there's no data in the buf.
 * IMPORTANT: Do not use this function as part of initialization of an
 * info struct; fields in the info struct are assumed to be meaningful.
 */
void
info_reset_rep_buf(struct info *ip)
{
  app_t *app;
  int sd = -1;
#ifdef AVOID_BUF_REUSE
  int ret=0;
  struct info *fcgi_ip, *first_ip=0;
#endif


  PRINT_TIME(ip->sd, &tnow, &tprev, " entered");
  /*
   * If we had allocated a dynamic buffer for this reply, we should free
   * it now.
   */
  if (ip->rep.dyn_buf != NULL) {
    PRINT_TIME(ip->sd, &tnow, &tprev, " freeing dynamic buffer");
#ifdef QUEUE_BUFS
		remove_from_list(dyn_bufpool->used_buf_q, ip->rep.buf_dll);
    ip->rep.buf_dll = NULL;
#endif

	bufpool_free(dyn_bufpool, ip->rep.dyn_buf);
    ip->rep.dyn_buf = NULL;

    /*
     * If this is now the only free dynamic buffer, check to see if there
     * are any applications that are blocked, waiting for a free buffer.
     */
      app = app_unblock_next();
      PRINT_TIME(ip->sd, &tnow, &tprev, "app_unblock_next returns app = %p", app);
	//printf("unblocking a blocked app-server\n");
      if (app != NULL) {
        PRINT_TIME(ip->sd, &tnow, &tprev, "unblocking a blocked app-server");
	switch (app->req_type) {
	  case REQ_FASTCGI:
#ifdef FIXHUP
	    /*
	     * Recover the request that caused the application to block
	     * in the first place (there *must* be one), and process it
	     * just as if it were arriving now.
	     *
	     * Not sure that there still has to be one.
	     * They may have timed out.
	     * I'm also concerned that there can be starvation if there
	     * are already other things in the queue.
	     * We may need a special condition to handle this case.
	     */
	    sd = fcgi_find_next_request(ip, app);
            if (sd < 0) {
              PRINT_TIME(ip->sd, &tnow, &tprev, " no new clients requests to work on doing nothing");
              // fcgi_go_idle(ip);
              /* The app server is unblocked now, is it considered idle or not? */
            } else {
              PRINT_TIME(ip->sd, &tnow, &tprev, " next sd to work on = %d", sd);
	      fcgi_process_request(sd);
            }
#else
	    /*
	     * Recover the request that caused the application to block
	     * in the first place (there *must* be one), and process it
	     * just as if it were arriving now.
	     */
	    sd = queue_get(app->req_q);
	    assert(sd >= 0);
            PRINT_TIME(ip->sd, &tnow, &tprev, " processing fcgi request on sd = %d", sd);
	    fcgi_process_request(sd);
#endif /* FIXHUP */
	    break;
	  default:
	    printf("info_reset_rep_buf(): invalid app->req_type %d\n",
		app->req_type);
	    assert(0);
	}
      }
#ifdef AVOID_BUF_REUSE
      else {
        PRINT_TIME(ip->sd, &tnow, &tprev, "no blocked app servers");
	//printf("no blocked app servers\n");
#ifdef FIXHUP
        assert(ip);
        fcgi_ip = fcgi_find_idle(ip);
#else
        fcgi_ip = fcgi_find_idle(0);
#endif
	while(fcgi_ip != first_ip) {
		if (fcgi_ip != NULL) {
			if (first_ip != 0) {
				if (first_ip != fcgi_ip) {
                                        PRINT_TIME(NOFD, &tnow, &tprev, "trying to service next request on %d", fcgi_ip->sd);
					ret = fcgi_service_next_request(fcgi_ip);
				} else {
					break;
                                }
			} else {
				first_ip = fcgi_ip;
                                PRINT_TIME(NOFD, &tnow, &tprev, "trying to service next request on %d", fcgi_ip->sd);
				ret = fcgi_service_next_request(fcgi_ip);
			}
		} else {
			break;
                }
	}
       } /* else */
#endif
      } else {
	PRINT_TIME(ip->sd, &tnow, &tprev, " no dynamic buffer to free");
      }

#ifdef QUEUE_BUFS
	ip->done_write = 0;
#endif

  /*
   * If we had switched the active reply buffer to something other than our
   * static buffer, switch it back now.
   */
  if (ip->rep.buf != ip->rep.static_buf) {
    assert(ip->type == INFO_CLIENT);
    ip->rep.buf = ip->rep.static_buf;
    ip->rep.buf_size = options.reply_buffer_size;
  }

  /*
   * Finally, reset our 'cur' and 'end' pointers to flush any stale data.
   */
  ip->rep.end = ip->rep.cur = ip->rep.buf;
  PRINT_TIME(ip->sd, &tnow, &tprev, " returning");
}

/* ---------------------------------------------------------------------- */
const char *
info_req_state_str(enum info_req_state state)
{
  switch (state)
  {
    case PARSE_METHOD:
      return "PARSE_METHOD";
    case PARSE_URI:
      return "PARSE_URI";
    case PARSE_VERSION:
      return "PARSE_VERSION";
    case PARSE_SKIP:
      return "PARSE_SKIP";
    case PARSE_CR_1:
      return "PARSE_CR_1";
    case PARSE_LF_1:
      return "PARSE_LF_1";
    case PARSE_HEADER_NAME:
      return "PARSE_HEADER_NAME";
    case PARSE_COLON:
      return "PARSE_COLON";
    case PARSE_COOKIE:
      return "PARSE_COOKIE";
    case PARSE_IF_MODIFIED_SINCE:
      return "PARSE_IF_MODIFIED_SINCE";
    case PARSE_CONTENT_LENGTH:
      return "PARSE_CONTENT_LENGTH";
    case PARSE_CR_2:
      return "PARSE_CR_2";
    case PARSE_LF_2:
      return "PARSE_LF_2";
    case PARSE_DONE:
      return "PARSE_DONE";
    /* FASTCGI states */
    case FCGI_READING_HEADER:
      return "FCGI_READING_HEADER";
    case FCGI_READING_END_REQUEST:
      return "FCGI_READING_END_REQUEST";
    case FCGI_READING_STDOUT:
      return "FCGI_READING_STDOUT";
    case FCGI_READING_STDERR:
      return "FCGI_READING_STDERR";
    case FCGI_READING_GET_VALUES_RESULT:
      return "FCGI_READING_GET_VALUES_RESULT";
    case FCGI_READING_UNKNOWN_TYPE:
      return "FCGI_READING_UNKNOWN_TYPE";
#ifdef MOD_SPECWEB99
    /* SPECweb99 states */
    case SPECWEB99_REQUEST_DONE:
      return "SPECWEB99_REQUEST_DONE";
    case SPECWEB99_RESPONSE_DONE:
      return "SPECWEB99_RESPONSE_DONE";
    case SPECWEB99_SOCKET_CLOSED:
      return "SPECWEB99_SOCKET_CLOSED";
#endif

#ifdef CALL_STATS
    case PARSE_CLIENT_ID:
      return "PARSE_CLIENT_ID";
#endif /* CALL_STATS */
  }

  return "unknown";
}

/* ---------------------------------------------------------------------- */
const char *
info_req_method_str(enum info_req_method method)
{
  switch (method)
  {
    case HTTP_NONE:
      return "NONE";
    case HTTP_SIMPLE:
      return "SIMPLE";
    case HTTP_GET:
      return "GET";
    case HTTP_HEAD:
      return "HEAD";
    case HTTP_POST:
      return "POST";
    case HTTP_NOT_IMPL:
      return "NOT_IMPL";
    case HTTP_RETURN_ERROR:
      return "RETURN_ERROR";
  }

  return "unknown";
}

/* ---------------------------------------------------------------------- */
void
store_sock_addr(int sd, const sockaddr_t *addr)
{
  struct info *ip;
  struct info_listen_struct *ilp;

  if (sock_is_listener(sd)) {
    ilp = &info_listener[sd];
    memcpy(&ilp->addr, addr, sizeof(sockaddr_t));
  } else {
    ip = info_ptr(sd);
    assert(ip != NULL);
    memcpy(&ip->addr, addr, sizeof(sockaddr_t));
  }
}

/* ---------------------------------------------------------------------- */
void
get_sock_addr(int sd)
{
  struct info *ip;

  ip = info_ptr(sd);
  assert(ip != NULL);

  ip->addr.len = sizeof(ip->addr.u);
  if (getsockname(sd, &ip->addr.u.sa, &ip->addr.len) < 0) {
    printf("get_sock_addr(): failed\n");
    memset(&ip->addr, '\0', sizeof(ip->addr));
  } else {
    switch (ip->addr.u.sa.sa_family) {
      case AF_INET:
	ldbg("get_sock_addr(): AF_INET addr %s port %d\n",
	    inet_ntoa(ip->addr.u.sa_in.sin_addr), ip->addr.u.sa_in.sin_port);
	break;
      case AF_UNIX:
	ldbg("get_sock_addr(): AF_UNIX path %s\n",
	    ip->addr.u.sa_un.sun_path);
	break;
      default:
	printf("get_sock_addr(): unsupported family [%d]\n",
	    ip->addr.u.sa.sa_family);
	memset(&ip->addr, '\0', sizeof(ip->addr));
	break;
    }
  }
}

#ifdef TRACK_STUCK_CONNS
void
check_if_alive(int sd)
{
  char *buf = "abcd";
  int rc = 0;
  socklen_t len = 0;
  int async_errno = 0;
  struct sockaddr_in name;
  socklen_t namelen;

  printf("Trying to read 0 bytes from sd = %d  ", sd);
  errno = 0;
  rc = 0;
  rc = read(sd, buf, 0);
  printf("return = %d errno = %d\n", rc, errno);
  
  printf("Trying to read 1 bytes from sd = %d  ", sd);
  errno = 0;
  rc = 0;
  rc = read(sd, buf, 1);
  printf("return = %d errno = %d\n", rc, errno);

  printf("Trying to write 0 bytes to sd = %d  ", sd);
  errno = 0;
  rc = 0;
  rc = write(sd, buf, 0);
  printf("return = %d errno = %d\n", rc, errno);
  
  printf("Trying to write 1 bytes to sd = %d  ", sd);
  errno = 0;
  rc = 0;
  rc = write(sd, buf, 1);
  printf("return = %d errno = %d\n", rc, errno);

  printf("getsockopt on sd = %d ", sd);
  len = sizeof (async_errno);
  async_errno = 0;
  errno = 0;
  rc = 0;
  rc = getsockopt(sd, SOL_SOCKET, SO_ERROR, &async_errno, &len);
  printf("return = %d errno = %d async_errno = %d\n", rc, errno, async_errno);

  printf("getpeername on sd = %d ", sd);
  namelen = sizeof(name);
  errno = 0;
  rc = 0;
#ifdef SUNOS
  rc = getpeername(sd, (struct sockaddr *) &name, &namelen);
#else
  rc = getpeername(sd, &name, &namelen);
#endif
  printf("return = %d errno = %d\n", rc, errno);
  printf("Peer's address is: %s:%d\n", inet_ntoa(name.sin_addr), ntohs(name.sin_port));
}


void
get_peer_addr_str(int sd, char *str, int maxlen)
{
  int rc = 0;
  struct sockaddr_in name;
  socklen_t namelen;

  if (sd < 0) {
    printf("get_peer_addr_str: sd invalid = %d\n",rc);
    return;
  }


  namelen = sizeof(name);
#ifdef SUNOS
  rc = getpeername(sd, (struct sockaddr *) &name, &namelen);
#else
  rc = getpeername(sd, &name, &namelen);
#endif
  if (rc < 0) {
    // perror("get_peer_addr_str: couldn't get peer\n");
    snprintf(str, maxlen, "couldn't get peer for sd = %d errno = %d", sd, errno);
  } else {
    snprintf(str, maxlen, "%s:%d", inet_ntoa(name.sin_addr), ntohs(name.sin_port));
  }
}

#endif /* TRACK_STUCK_CONNS */

int
is_client_conn(struct info *ip)
{
  assert(ip);
  return(ip->type == INFO_CLIENT);
}

int
is_app_conn(struct info *ip)
{
  assert(ip);
  return(ip->type == INFO_FASTCGI);
}

int
is_listening_conn(struct info *ip)
{
  assert(ip);
  return(ip->type == INFO_LISTENER);
}

int
is_hungup(struct info *ip)
{
  assert(ip);
  return(ip->got_hup == INFO_LISTENER);
}

