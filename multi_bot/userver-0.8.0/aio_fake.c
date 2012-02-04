/*
    userver -- (pronounced you-server or micro-server).
    This file is part of the userver, a high-performance web server designed for
    performance experiments.

    This file Copyright (C) 2004  Hewlett-Packard Company

    Authors: Tim Brecht <brecht@cs.uwaterloo.ca>
             Done while working part-time for HP
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

#ifdef AIO_FAKE

#ifdef HAVE_AIO_LAYER

#include "stdlib.h"
#include "string.h"
#include "aio_layer.h"
#include "debug.h"
#include "stats.h"
#include "options.h"

/* #define LOCAL_DEBUG */
// #include "local_debug.h"

#define FAKE_LISTENER    (6)
#define FAKE_NEW_CONN    (7)

static int request_num = 0;

char *fake_requests[] = {
"GET docs/1B.txt HTTP/1.1\r\nUser-Agent: aio_faker\r\nHost: localhost\r\n\r\n",
"GET docs/2B.txt HTTP/1.1\r\nUser-Agent: aio_faker\r\nHost: localhost\r\n\r\n",
"GET docs/10B.txt HTTP/1.1\r\nUser-Agent: aio_faker\r\nHost: localhost\r\n\r\n",
"GET docs/1K.txt HTTP/1.1\r\nUser-Agent: aio_faker\r\nHost: localhost\r\n\r\n",
};

#define FAKE_REQUESTS ((int) (sizeof(fake_requests) / sizeof(char *)))

int write_len = 0;
int read_len = 0;
int request_count = 0;
int total_request_count = 0;
int do_over = 0;

#define REQUESTS_PER_CONN  (1)
#define MAX_REQUEST_COUNT  (10)
#define MAX_REPLIES        (MAX_REQUEST_COUNT * REQUESTS_PER_CONN)
#define MAX_REGIONS        (3000)

/* So that we have something we can link against and run */

struct reg_entry {
  char *start;
  char *end;
  char *handle;
};

struct reg_entry reg_table[MAX_REGIONS];

/* this simple test implementation assume that one doesn't register
 * the same address more than once
 */

void
reg_table_add(void *addr, size_t size, void **reg_handle)
{
  static char *cur_handle = (char *) 1;
  struct reg_entry *rptr = 0;
  int i;

  PRINT_TIME(NOFD, &tnow, &tprev, "reg_table_add: addr = %p size = %d "
      "handle addr = %p", addr, size, reg_handle);

  for (i=0; i<MAX_REGIONS; i++) {
    if (reg_table[i].start == 0) {
      PRINT_TIME(NOFD, &tnow, &tprev, "reg_table_add: using entry = %d "
	  "start = %p ", i, reg_table[i].start);
      break;
    }
  }

  if (i == MAX_REGIONS) {
    printf("reg_table_add: no available regions MAX_REGIONS = %d\n",
	MAX_REGIONS);
    exit(1);
  } else {
    rptr = &(reg_table[i]);
    rptr->start = (char *) addr;
    rptr->end = (char *) addr + size;
    rptr->handle = cur_handle;
    *reg_handle = (void *) cur_handle;
    PRINT_TIME(NOFD, &tnow, &tprev, "reg_table_add: entry = %d start = %p "
	"end = %p handle = %p", i, rptr->start, rptr->end, rptr->handle);
    cur_handle++;
  }
}

void
reg_table_print(void)
{
  struct reg_entry *rptr;
  int i;
  ldbg("Region Table Contents\n");
  ldbg("%6s %20s %20s %20s\n", "index", "start", "end", "handle");
  for (i=0; i<MAX_REGIONS; i++) {
    rptr = &(reg_table[i]);
    ldbg("%6d %20p %20p %20p\n", i, rptr->start, rptr->end, rptr->handle);
  }
  ldbg("------------------------------\n");
}

int
reg_table_find(void *addr, size_t size, void *reg_handle)
{
  struct reg_entry *rptr = 0;
  char *tmp_addr = (char *) addr;
  int i;

  PRINT_TIME(NOFD, &tnow, &tprev, "reg_table_find: addr = %p size = %p "
      "handle = %p", addr, size, reg_handle);
  for (i=0; i<MAX_REGIONS; i++) {
    rptr = &(reg_table[i]);
    if (rptr->handle == reg_handle) {
      break;
    }
  }

  if (i == MAX_REGIONS) {
    printf("reg_table_find: can't find region = %p\n", reg_handle);
    exit(1);
  }

  if (tmp_addr != rptr->start) {
    printf("reg_table_find: reg_handle matches but start address does not\n");
    printf("reg_table_find: start = %p addr = %p\n", rptr->start, addr);
    exit(1);
  }

  if (tmp_addr + size != rptr->end) {
    printf("reg_table_find: reg_handle matches but end address does not\n");
    printf("reg_table_find: start = %p addr = %p\n", rptr->end, tmp_addr+size);
    exit(1);
  }

  return(i);
}

void
reg_table_del(void *addr, size_t size, void *reg_handle)
{
  struct reg_entry *rptr = 0;
  int entry = reg_table_find(addr, size, reg_handle);

  rptr = &(reg_table[entry]);

  PRINT_TIME(NOFD, &tnow, &tprev, "reg_table_del: start = %p end = %p "
      "handle = %p", rptr->start, rptr->end, rptr->handle);

  rptr->start = 0;
  rptr->end = 0;
  rptr->handle = 0;
}


void
reg_verify(void *buf, size_t count, void *reg_handle)
{
  struct reg_entry *rptr = 0;
  char *bufend = (char *) buf + count;
  int i = 0;
  int verified = 0;

  PRINT_TIME(NOFD, &tnow, &tprev, "reg_verify: buf = %p count = %d "
      "reg_handle = %p", buf, count, reg_handle);

  for (i=0; i<MAX_REGIONS; i++) {
    rptr = &(reg_table[i]);
    PRINT_TIME(NOFD, &tnow, &tprev, "reg_verify: i = %d start = %p "
	"end = %p handle = %p", i, rptr->start, rptr->end, rptr->handle);
    if (((char *) buf >= rptr->start) && 
	((char *) bufend <= rptr->end) && 
	((char *) reg_handle == rptr->handle)) {
      PRINT_TIME(NOFD, &tnow, &tprev, "reg_verify: found match\n");
      verified = 1;
      break;
    }
  }

  if (verified != 1) {
    ldbg("reg_table_verify : unable to verify specified region\n");
    ldbg("reg_table_verify : buf = %p count = %d bufend = %p "
	"reg_handle = %p\n", buf, count, bufend, reg_handle);
    reg_table_print();
    exit(1);
  }  
}

void
reg_table_init(void)
{
  int i;
  for (i=0; i<MAX_REGIONS; i++) {
    reg_table[i].start = 0;
    reg_table[i].end = 0;
    reg_table[i].handle = 0;
  }
}

int
aio_sock_init(int lowest_sd, int highest_sd)
{
  int i;
  PRINT_TIME(NOFD, &tnow, &tprev, "aio_sock_init: entered");
  printf("aio_sock_init: lowest_sd = %d highest_sd = %d\n", 
    lowest_sd, highest_sd);
  reg_table_init();

  for (i=0; i<FAKE_REQUESTS; i++) {
    printf("aio_sock_init: fake request[%d] = [%s]\n", i, fake_requests[i]);
  }

  return 1;
}

void
aio_sock_uninit(void)
{
  PRINT_TIME(NOFD, &tnow, &tprev, "aio_sock_uninit: entered");
}

int
aio_sock_create_compl_q(size_t len, int event_type)
{
  PRINT_TIME(NOFD, &tnow, &tprev, "aio_sock_create_compl_q: "
      "len = %d event_type = %d", len, event_type);
  return 1;
}

int
aio_get_stats(FILE *outf, double *stats, char **what, int size)
{
  return 0;
}

void
aio_reset_stats(int full)
{
}

int
aio_sock_mem_reg(void *addr, size_t size, void **reg_handle)
{
  PRINT_TIME(NOFD, &tnow, &tprev, "aio_sock_mem_reg: addr = %p size = %d "
      "reg_handle = %p", addr, size, reg_handle);
  reg_table_add(addr, size, reg_handle);
  // reg_verify(addr, size, *reg_handle);
  // reg_table_print();
  return 1;
}

int
aio_sock_mem_dereg(void *addr, size_t size, void *reg_handle)
{
  PRINT_TIME(NOFD, &tnow, &tprev, "aio_sock_mem_dereg: addr = %p "
      "size = %d handle = %p", addr, size, reg_handle);
  reg_table_del(addr, size, reg_handle);
  return 1;
}

conn_t
aio_sock_socket(int domain, int type, int protocol)
{
  return FAKE_LISTENER;
}

int
aio_sock_bind(conn_t s, struct sockaddr *my_addr, socklen_t addrlen)
{
  PRINT_TIME(NOFD, &tnow, &tprev, "aio_sock_bind: entered");

  return(0);
}

int
aio_sock_listen(conn_t s, int backlog)
{

  PRINT_TIME(NOFD, &tnow, &tprev, "aio_sock_listen: "
      "s = %d backlog = %d", s, backlog);

  return 1;
}

int
aio_sock_accept(conn_t sd)
{
  return 1;
}

int
aio_sock_read_accept(conn_t listen_sd, void *buf, size_t count, 
    void *reg_handle)
{
  char *tmp = (char *) buf;
  PRINT_TIME(listen_sd, &tnow, &tprev, "aio_sock_read_accept: addr = %p "
      " count = %d handle %p", buf, count, reg_handle);
  /* load up the buffer so we can later trigger an event */

  if (request_count < REQUESTS_PER_CONN) {

    ldbg("aio_sock_read_accept: request_num = %d request = [%s]\n", 
	 request_num, fake_requests[request_num]);

    read_len = strlen(fake_requests[request_num]);
    strncpy((char *) buf, fake_requests[request_num], count);
    tmp[count] = '\0';

    reg_verify(buf, count, reg_handle);

    request_num++;
    if (request_num >= FAKE_REQUESTS) {
      request_num = 0;
    }
  } else {
    ldbg("aio_sock_read_accept: will return zero bytes (EOF)\n");
    reg_verify(buf, count, reg_handle);
    read_len = 0;
  }

  return FAKE_NEW_CONN;
}

int
aio_sock_read(conn_t sd, void *buf, size_t count, void *reg_handle)
{
  char *tmp = (char *) buf;
  PRINT_TIME(sd, &tnow, &tprev, "aio_sock_read: addr = %p count = %d handle %p",
      buf, count, reg_handle);
  /* load up the buffer so we can later trigger an event */

  if (request_count < REQUESTS_PER_CONN) {

    ldbg("aio_sock_read: request_num = %d request = [%s]\n", 
	 request_num, fake_requests[request_num]);

    read_len = strlen(fake_requests[request_num]);
    strncpy((char *) buf, fake_requests[request_num], count);
    tmp[count] = '\0';

    reg_verify(buf, count, reg_handle);

    request_num++;
    if (request_num >= FAKE_REQUESTS) {
      request_num = 0;
    }
  } else {
    ldbg("aio_sock_read: will return zero bytes (EOF)\n");
    reg_verify(buf, count, reg_handle);
    read_len = 0;
  }

  return 1;
}

int
aio_sock_write(conn_t sd, void *buf, size_t count, void *reg_handle)
{
  char *tmp = (char *) buf;
  write_len = count;
  tmp[count] = '\0';
  PRINT_TIME(sd, &tnow, &tprev, "aio_sock_write: addr = %p count = %d "
      "handle = %p", buf, count, reg_handle);
  PRINT_TIME(sd, &tnow, &tprev, "aio_sock_write: writing [%s]", buf);

  reg_verify(buf, count, reg_handle);

  return 1;
}

int
aio_sock_writev(conn_t sd, struct aio_vec *vec, size_t count)
{
  char *tmp = 0;
  void *reg_handle = 0;
  void *base = 0;
  unsigned int len = 0;
  unsigned int i;

  for (i=0; i<count; i++) {
    len = vec[i].len;
    base = vec[i].base;
    reg_handle = vec[i].reg_handle;
    PRINT_TIME(sd, &tnow, &tprev, "aio_sock_writev: entry = %d base = %p "
	"len = %d handle = %p", i, base, len, reg_handle);
    if (len && base) {
      write_len += len;
      tmp = (char *) base;
      // tmp[len] = '\0';
      PRINT_TIME(sd, &tnow, &tprev, "aio_sock_writev: base = %p len = %d "
	  "handle = %p", base, len, reg_handle);
      PRINT_TIME(sd, &tnow, &tprev, "aio_sock_writev: writing [%s]", base);

      reg_verify(base, len, reg_handle);
    } else {
      printf("aio_sock_writev: invalid value for base or len\n");
      printf("aio_sock_writev: vec[%d].base = %p vec[%d].len = %d\n",
              i, base, i, len);
      exit(1);
    }
  }

  PRINT_TIME(sd, &tnow, &tprev, "aio_sock_writev: returning 1");

  return 1;
}

int
aio_sock_close(conn_t sd)
{
  PRINT_TIME(sd, &tnow, &tprev, "aio_sock_close: sd = %d", sd);

  return 1;
}

int
aio_sock_destroy(conn_t sd)
{
  PRINT_TIME(sd, &tnow, &tprev, "aio_sock_destroy: sd = %d", sd);

  return 1;
}

int
aio_sock_num_events(int compl_q)
{

  return 1;
}

int
aio_sock_getevents(int compl_q, int max, compl_evt_t *events)
{

  return 1;
}

int
aio_sock_batch_size(int size)
{

  return 1;
}

int
aio_sock_batch_flush(void)
{

  return 1;
}

int
aio_sock_wait(int event_bitmap, compl_evt_t *evts, int max,
    struct timeval *tv)
{

  return(0);
}


int
aio_wait(int event_bitmap, compl_evt_t *evts, int max,
    struct timeval *tv)
{
  int evt_count = 0;
  static int state = 0;

  switch(state) {

    case 0:
      PRINT_TIME(NOFD, &tnow, &tprev, "aio_wait: trigger accept completion");
      /* trigger a fake incoming connection accepted */
      evts[evt_count].type   = SOCK_ACCEPT_COMPL;
      evts[evt_count].handle = FAKE_LISTENER;
      evts[evt_count].result = FAKE_NEW_CONN;
      evts[evt_count].result_errno = 0;
      evt_count++;
      state = 1;
      if (total_request_count > MAX_REQUEST_COUNT) {
	PRINT_TIME(NOFD, &tnow, &tprev, "aio_wait: num_replies = %d",
	    num_replies);
	PRINT_TIME(NOFD, &tnow, &tprev, "aio_wait: stats_print and exit");
	print_all();
	exit(0);
      }

      break;

    case 1:
      /* trigger a fake read completion */
      evts[evt_count].type   = SOCK_READ_COMPL;
      evts[evt_count].handle = FAKE_NEW_CONN;
      evts[evt_count].result = read_len;
      evts[evt_count].result_errno = 0;
      evt_count++;

      if (read_len != 0) {
        PRINT_TIME(NOFD, &tnow, &tprev, "aio_wait: trigger read completion");
        request_count++;
	total_request_count++;
        state = 2;
      } else {
        PRINT_TIME(NOFD, &tnow, &tprev, "aio_wait: trigger read EOF "
	    "completion");
        state = 3;
	request_count = 0;
      }

      break;

    case 2:
      /* trigger a fake write completion */
      if (!options.use_sendfile && options.caching_on) {
        PRINT_TIME(NOFD, &tnow, &tprev, "aio_wait: trigger writev completion");
        /* there may not be a writev completion event type
        evts[evt_count].type   = SOCK_WRITEV_COMPL;
	*/
        evts[evt_count].type   = SOCK_WRITE_COMPL;
      } else {
        PRINT_TIME(NOFD, &tnow, &tprev, "aio_wait: trigger write completion");
        evts[evt_count].type   = SOCK_WRITE_COMPL;
      }
      evts[evt_count].handle = FAKE_NEW_CONN;
      evts[evt_count].result = write_len;
      evts[evt_count].result_errno = 0;
      evt_count++;
      num_replies++;
      state = 1;
      break;

    case 3:
      /* trigger a fake close completion */
      PRINT_TIME(NOFD, &tnow, &tprev, "aio_wait: trigger close completion");
      evts[evt_count].type   = SOCK_CLOSE_COMPL;
      evts[evt_count].handle = FAKE_NEW_CONN;
      evts[evt_count].result = 1;
      evts[evt_count].result_errno = 0;
      state = 0;
      evt_count++;
      if (num_replies >= MAX_REPLIES) {
        printf("aio_wait: num_replies = %d MAX_REPLIES = %d",
	    num_replies, MAX_REPLIES);
	printf("aio_wait: did max replies\n");
	exit(1);
      }
      break;

    default:
      printf("aio_wait: Unknown state\n");
      exit(-1);
      break;
  }

  PRINT_TIME(NOFD, &tnow, &tprev, "aio_wait: num_replies = %d MAX_REPLIES = %d",
      num_replies, MAX_REPLIES);
  return(evt_count);
} /* aio_wait */

#endif /* HAVE_AIO_LAYER */


#endif /* AIO_FAKE */
