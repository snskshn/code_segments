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


#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/fcntl.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#include "common.h"
#include "debug.h"
#include "local_debug.h"
#include "stats.h"
#include "options.h"
#include "cntl_intr.h"
#include "cntl_conns.h"
#include "lru.h"
#include "q.h"
#include "syscalls.h"
#include "trace.h"
#include "interest_set.h"
#include "sock_listener.h"
#include "post_sock.h"
#include "aio_layer.h"
#include "util.h"


#ifdef HAVE_SSL
#include "u_ssl_utils.h"
#endif /* HAVE_SSL */

#ifdef SEND
#include "send.h"
#endif /* SEND */

#define LOCAL_DEBUG
#include "local_debug.h"

/*----------------------------------------------------------------------*/
/* Prototypes */
int socket_setup(int listen_sd, int sd);
void check_and_record_aio_wait_errors(int aio_wait_errno);
static int set_client_sockopts(int listen_sd, int sd);

/*----------------------------------------------------------------------*/
/* Multiaccept specific stuff */
#ifdef MACCEPT
#define SYS_MACCEPT   (18)
static int maccept_init_done = 0;
void multiaccept_init();

struct maccept {
  int rc;
  int state;
  int addrlen;
  struct sockaddr_in saddr;
};

struct syscallargs {
  int fd;
  struct maccept *arr;
  int buflen;
} syscall_args;

/* System call with two parameters in this case it's a socket 
 * system call and the parameters are the socket system 
 * call number and a pointer to a structure containing the 
 * arguments to the actual system call 
 */

static inline _syscall2(int, socketcall, int, num, void*, ptr);
#endif /* MACCEPT */
/*----------------------------------------------------------------------*/
/* New syscall attempt for use with the SEND kernel */

#ifdef SEND
#ifdef HAVE_ACCEPT_SEND
static inline _syscall4(int, accept_send, int, fd, 
    struct sockaddr*, addr, socklen_t*, addrlen, int*, events);
#endif /* HAVE_ACCEPT_SEND */
#endif /* SEND */

/*----------------------------------------------------------------------*/
/* returns 0 on failure */
int
socket_new_conn(int listen_sd)
{
  int sd = -1;
  int x = 1;
  int rc = 0;
  int err = 0;
  int trace_fd = 0;
  struct info *ip = NULL;
#ifdef SEND
#ifdef HAVE_ACCEPT_SEND
  unsigned int events = 0;
#endif /* HAVE_ACCEPT_SEND */
#endif /* SEND */

#ifdef HAVE_SSL
  SSL *client_ssl;
#endif

  PRINT_TIME(listen_sd, &tnow, &tprev, "socket_new_conn: listen_sd = %d", 
      listen_sd);

  num_accept_calls++;

  if (options.use_aio_accept) {

#ifdef HAVE_AIO_LAYER
    if (options.aio_read_before_accept) {
      /* reserve an element in the info/connection array for use
       * for this connection (we don't know the sd yet)
       */
      ip = info_reserve();
      PRINT_TIME(listen_sd, &tnow, &tprev, "socket_new_conn: "
	         "aio_sock_read_accept ip = %p",
		 ip);
      TRACE(EVT_AIO_SOCK_READ_ACCEPT,
	trace_fd = listen_sd;
	rc = aio_sock_read_accept(listen_sd, ip->req.buf,
	       ip->req.buf_size - 1, ip->req.read_reg_handle);
	err = errno;
      );
       
      sd = rc;

      PRINT_TIME(listen_sd, &tnow, &tprev, "socket_new_conn: "
	         "aio_sock_read_accept sd = %d", sd);

      if (sd < 0) {
	printf("socket_new_conn: aio_sock_read_accept failed\n");
	info_unreserve(ip);
      } else {
	/* now we associate the reserved info element with the sd */
        info_register(ip, sd);
	lru_access(sd);
	set_fsm_state(ip, FSM_CONNECTING);
	ip->parent_sd = listen_sd;
      }
    } else {
      PRINT_TIME(listen_sd, &tnow, &tprev, "socket_new_conn: aio_sock_accept");
      TRACE(EVT_AIO_SOCK_ACCEPT,
	trace_fd = listen_sd;
	rc = aio_sock_accept(listen_sd);
	err = errno;
      );
    }
#else
    printf("socket_new_conn: aio not handled\n");
    exit(1);
#endif /* HAVE_AIO_LAYER */

    PRINT_TIME(listen_sd, &tnow, &tprev, 
	"socket_new_conn: aio initiated on %i", listen_sd);

    if (rc < 0) {
      check_and_record_aio_wait_errors(err);
    } else {
      num_idle--;
      PRINT_TIME(listen_sd, &tnow, &tprev, "socket_new_conn: "
	  "called aio_sock_accept num_idle = %d", num_idle);
      if (options.use_socket_aio) {
        info_listener[listen_sd].aio_accept_outstanding++;
      }
    }

    conns_off_if_needed();

  } else {

    ip = info_reserve();

#ifdef SEND
    if (options.use_accept_send) {
#ifdef HAVE_ACCEPT_SEND
      TRACE(EVT_ACCEPT,
	trace_fd = listen_sd;
	ip->addr.len = sizeof(ip->addr.u);
	rc = accept_send(listen_sd, &ip->addr.u.sa, &ip->addr.len, &events);
	err = errno;
      );
      sd = rc;
      PRINT_TIME(sd, &tnow, &tprev, 
	  "socket_new_conn: accepted - events = 0x%x", events);
#else
      printf("Not compiled with ACCEPT_SEND defined\n");
      exit(1);
#endif
    } else {
#endif /* SEND */

    TRACE(EVT_ACCEPT,
			trace_fd = listen_sd;
			ip->addr.len = sizeof(ip->addr.u);
      errno = 0;
			rc = accept(listen_sd, &ip->addr.u.sa, &ip->addr.len);
			err = errno;
    );
	}

      sd = rc;
      PRINT_TIME(listen_sd, &tnow, &tprev, "socket_new_conn: accept rc = %d "
          "errno %d", sd, err);

      if (sd > 0 && err != 0) {
        printf("ACCEPT sd = %d and errno = %d\n", sd, err);
      }
#ifdef SEND
    }
#endif /* SEND */

#ifdef HAVE_SSL
	//ok, so we've got the connection.
	//now, do the ssl handshake and all that jazz
    if (is_ssl_listener(listen_sd) && sd > 0) {
	ldbg("is ssl socket, doing accept\n");
	TRACE(EVT_ACCEPT,
      		trace_fd = listen_sd;
      		ip->addr.len = sizeof(ip->addr.u);
      		client_ssl = do_ssl_accept(sd);
		//listen_sd, &ip->addr.u.sa, &ip->addr.len);
      		err = errno;
    	);
	//so the thing is accepted. Now, put all of that info into the info struct
	ldbg("completed accept\n");
	
	//I have to do the post sock accept stuff at the end, because it sets it to non-blocking
	//which appears to screw up the SSL handshaking
	x = post_sock_accept(listen_sd, ip, sd, err);
	if (x > 0) {
		post_sock_ssl_accept(client_ssl,ip);
        }
    } else {
       x = post_sock_accept(listen_sd, ip, sd, err);
    }
#else
       x = post_sock_accept(listen_sd, ip, sd, err);
#endif /* HAVE_SSL */


  PRINT_TIME(listen_sd, &tnow, &tprev, "socket_new_conn: sd = %d returning %d", 
            sd, x);
  return(x);
}

/*----------------------------------------------------------------------*/
/* set up the new socket - listen_sd is the parent / listener
 * that accepted the new connection
 */

int
socket_setup(int listen_sd, int sd)
{
  struct info *ip;
  int num_open = 0;
  int rc = 0;
#ifdef DEBUG_SOCKBUF
  int size, len;
#endif /* DEBUG_SOCKBUF */

  if (!options.use_socket_aio) {
    rc = set_client_sockopts(listen_sd, sd);
    if (rc != 1) {
      return rc;
    }
  }

  notification_off();

  num_connections++;

  if (sd > max_sd) {
    max_sd = sd;

    if (max_sd > max_fds) {
      printf("EXITING because max_fds is too small for max_sd being used\n");
      printf("max_sd = %d max_fds = %d num_idle = %d\n", 
	  max_sd, max_fds, num_idle);
      printf("Trying using -f to increase max_fds\n");
      printf("max_fds >= 2 * maxconns may work\n");
      exit(1);
    }
  }

  /* if we are using aio accept we must decrement num_idle at initiation
   * time because at this point it may be too late
   */
  if (!options.use_aio_accept) {
    num_idle--;
  } 

  if (num_idle == options.free_fd_thold) {
    PRINT_TIME(sd, &tnow, &tprev, 
      "socket_setup: ATTENTION: NUM_IDLE == %d", options.free_fd_thold);
  }
  assert(num_idle >= 0);

  num_open = maxconns - num_idle;
  if (num_open > num_max_open) {
    num_max_open = num_open;
  }

  num_total_open += (double) num_open;

  if (num_idle <= options.free_fd_thold) {
    if (new_connections_on) {
      notification_on();
      socket_new_conn_off();
      notification_off();
    }
  }

  if (options.use_max_open_sd) {
    FD_SET(sd, &openfds);
    if (sd > max_opensd) {
      max_opensd = sd;
    }
  }
  /* 
  printf("socket_setup: sd = %6d max_opensd now = %6d  conns = %6d\n",
      sd, max_opensd, maxconns - num_idle);
  */

  ip = info_ptr(sd);
  assert(ip);

  ip->parent_sd = listen_sd;
  set_fsm_state(ip, FSM_CONNECTING);

  ip->conn_num = num_connections;

  /* 
    DEBG(MSG_CONN, "ip->sd = %d ip->conn_num = %d\n",
        ip->sd, ip->conn_num);
  */

  /* set the type to client until we know differently */
  ip->type = INFO_CLIENT;

  ip->rep.fd = -1;
  ip->rep.is_corked = 0;
  ip->rep.http_status = 0; /* Don't assume it will be a "200 OK" */ 

  ip->req.type = REQ_INVALID;
  PRINT_TIME(sd, &tnow, &tprev, "ip->req.type = %d", ip->req.type);
  ip->req.state = PARSE_METHOD;
  ip->req.skip_lws = 0;
  ip->req.method = HTTP_NONE;

  assert(ip->req.buf == ip->req.static_buf);
  assert(ip->req.buf_size == options.read_buffer_size);
  assert(ip->req.cur == ip->req.buf);
  assert(ip->req.end == ip->req.cur);

  ip->req.tmp_len = 0;
  ip->req.cookie_len = 0;
  ip->req.content_len = 0;
  ip->req.padding_len = 0;
  ip->req.ifmodsince = (time_t) 0;

  /* good for debugging... */
  ip->req.uri[0] = '\0';
  /* rep.buf may not point to anything yet. */
  /* ip->rep.buf[0] = '\0'; */

  /* if a root document directory has been specified put it into the 
   * buffer now and we'll append the incoming uri to it
   */
  if (options.doc_root[0] != '\0') {
    strncpy(ip->req.uri, options.doc_root, MAX_URI_LEN);
  }


  PRINT_TIME(sd, &tnow, &tprev, "socket_setup: lru_access on ip->sd = %d", ip->sd);
  lru_access(ip->sd);

  set_fsm_state(ip, FSM_READING_REQUEST);

  /* tell the system we are initializing this sd and
   * let the event handler specific code determine 
   * exactly what to do
   */
  if (options.use_epoll2) {
    interest_set_change(sd, ISET_INIT | ISET_READABLE | ISET_WRITABLE);
  } else {
    interest_set_change(sd, ISET_INIT | ISET_READABLE);
  }

  if ((options.process_sds_order == OPT_PROCESS_SDS_LIFO) ||
      (options.process_sds_order == OPT_PROCESS_SDS_FIFO)) {
    q_to_add(sd);
  }


  if (options.call_readable_from_new_conn) {
    PRINT_TIME(sd, &tnow, &tprev, "socket_setup: calling socket_readable");
    notification_on();
    if (options.loop_on_readable >= 2) {
      /* THIS WAS THE WAY t-event2 did things */
      while (socket_readable (ip) > 0);
    } else {
      socket_readable(ip);
    }
    notification_off();
  }

  notification_on();
  PRINT_TIME(sd, &tnow, &tprev, "socket_setup: returning ip = %p", ip);
  return 1;
} /* socket_setup */


/*----------------------------------------------------------------------*/
#ifdef MACCEPT
void
multiaccept_init(int listen_sd)
{
    int size;

    /*
     * TODO: note if we are limiting the number of consecutive accepts
     *       this size can be set to the size of the max # of consecutive
     *       accepts - this could save a lot of overhead on each call
     *       due to the copying of this array in and out of the kernel.
     */
    syscall_args.fd = listen_sd;
    size = sizeof(struct maccept) * max_fds;

    syscall_args.arr = (struct maccept *) alloc_aligned(cache_line_size, size,
	ALLOC_MALLOC, "multiaccept_init: (syscall_args.arr)");

    printf("syscall_args.arr = %p size = %d\n", syscall_args.arr, size);
    syscall_args.buflen = size;

}

/*----------------------------------------------------------------------*/
int
multiaccept(int listen_sd)
{
  int i;
  int rc = 0;
  int x;
  int size = 0;
  int count = 0;
  int sd;
  int err;
  int trace_fd = 0;

  PRINT_TIME(listen_sd, &tnow, &tprev, "multiaccept: entered");

  if (!maccept_init_done) {
    printf("Initializing multiaccept\n");
    multiaccept_init();
    maccept_init_done = 1;
  }

  /* Limit the number of new connections we are willing to accept
   * at one time to be limited by the max_fds or the number of idle
   * fds we have available (i.e., to the lower of the two). 
   *
   * Also permit there to be a user controlled limit on the max 
   * number of connections we'll accept
   */
  
  DEBG(MSG_MULTIACCEPT, 
      "size = %d max_fds = %d num_idle = %d maccept_max = %d\n",
      size, max_fds, num_idle, options.multiaccept_max);
  size = max_fds;

  DEBG(MSG_MULTIACCEPT, 
      "size = %d max_fds = %d num_idle = %d maccept_max = %d\n",
      size, max_fds, num_idle, options.multiaccept_max);

  /* pick the min of max_fds, num_idle or multiaccept_max */
  if (num_idle < max_fds) {
    size = num_idle;
    DEBG(MSG_MULTIACCEPT, 
	"size = %d max_fds = %d num_idle = %d maccept_max = %d\n",
	 size, max_fds, num_idle, options.multiaccept_max);
  } 
  
  if (options.multiaccept_max != OPT_MULTIACCEPT_MAX_UNLIMITED) {
    if (options.multiaccept_max < size) {
      size = options.multiaccept_max;
      DEBG(MSG_MULTIACCEPT, 
	  "size = %d max_fds = %d num_idle = %d maccept_max = %d\n",
	   size, max_fds, num_idle, options.multiaccept_max);
    }
  }

  assert(size <= max_fds);
  assert(size <= num_idle);
  if (options.multiaccept_max != OPT_MULTIACCEPT_MAX_UNLIMITED) {
    assert(size <= options.multiaccept_max);
  }

  DEBG(MSG_MULTIACCEPT, 
      "size = %d max_fds = %d num_idle = %d maccept_max = %d\n",
       size, max_fds, num_idle, options.multiaccept_max);
  syscall_args.buflen = size * sizeof(struct maccept);

  DEBG(MSG_MULTIACCEPT, "buflen = %d\n", syscall_args.buflen);

  num_accept_calls++;
  TRACE(EVT_MACCEPT,
    trace_fd = syscall_args.fd;
    rc = socketcall(SYS_MACCEPT, &syscall_args);
    err = errno;
  );

  if (rc < 0) {
    DEBG(MSG_MULTIACCEPT, "rc = %d errno = %d\n", rc, errno);
    if (err == EINVAL) {
      printf("multiaccept system call not supported\n");
      exit(1);
    }
    process_accept_errs(rc, err);
    return(0);
  } else {
    assert(rc <= num_idle);
    assert(rc <= size);
    if (options.multiaccept_max != OPT_MULTIACCEPT_MAX_UNLIMITED) {
      assert(rc <= options.multiaccept_max);
    }
    num_accept_successful++;
    DEBG(MSG_MULTIACCEPT, "multiaccept: rc = %d\n", rc);

    for (i=0; i<rc; i++) {
      sd = syscall_args.arr[i].rc;
      if (options.accepts_only) {
	info_set_fsm_state(sd, FSM_CLOSING);
	close_sock(sd);
	count++;
      } else {
	set_fsm_state(sd, FSM_READING_REQUEST);
	x = socket_setup(listen_sd, sd);
	if (x == 1) {
	  count++;
	}
      }
    } /* for */

  }
  PRINT_TIME(listen_sd, &tnow, &tprev, "multiaccept: returning");
  return(count);
}
#endif /* MACCEPT */


/*----------------------------------------------------------------------*/
void
process_accept_errs(int sd, int err)
{
  PRINT_TIME(sd, &tnow, &tprev, "process_accept_errs: err = %d", err);
  switch (err) {
    case ECONNABORTED:
#if !defined(FreeBSD) && !defined(DARWIN)
    case EPROTO:
#endif
      num_accept_aborts++;
      PRINT_TIME(sd, &tnow, &tprev, "socket_new_conn: EPROTO");
      break;

    case EAGAIN:
    /* These are the same on Linux */
#if !defined(__linux__) && !defined(__sun__) && \
    !defined(__CYGWIN__) && !defined(FreeBSD)  && !defined(DARWIN) && !defined(__SUNPRO_C)
    case EWOULDBLOCK:
#endif
      num_accept_eagain++;
      PRINT_TIME(sd, &tnow, &tprev, "socket_new_conn: EAGAIN");
      break;

    case EINTR:
      PRINT_TIME(sd, &tnow, &tprev, "socket_new_conn: EINTR");
      num_accept_eintr++;
      break;

    case EMFILE:
      num_accept_emfile++;
      PRINT_TIME(sd, &tnow, &tprev, "socket_new_conn: EMFILE");
      break;

    case ENFILE:
      num_accept_enfile++;
      PRINT_TIME(sd, &tnow, &tprev, "socket_new_conn: ENFILE");
      break;

    default :
      num_accept_eunknown++;
      PRINT_TIME(sd, &tnow, &tprev, "socket_new_conn: Euntracked");
      break;
  } /* switch */

} /* process_accept_errs */

/*----------------------------------------------------------------------*/
void
check_and_record_aio_wait_errors(int aio_wait_errno)
{
  /* handle all of the various errors */
  switch (aio_wait_errno) {

    case 0:
      num_aio_wait_failed_noerror++;
      break;

    case EAGAIN:
      num_aio_wait_eagain++;
      break;

    case EINTR:
      printf("Bizarre failure\n");
      printf("Should exit the aio loop when getting EINTR\n");
      exit(1);
      break;

    default :
      num_aio_wait_eother++;
      fprintf(stderr, "aio wait_failed: %d\n", aio_wait_errno);
      break;
  } /* switch */
} /* check_and_record_aio_wait_errors */


/*----------------------------------------------------------------------*/
/* This is done after the accept call has happened */

/* Note that listen_sd is only used here to update the
 * lru data structure that gets used for timing out connections.
 *
 * Some interfaces (e.g., SEND and likely some AIO interfaces won't
 * have access to the listen_sd - in these cases it's sufficient
 * to pass a value < 0 (meaning unknown).
 *
 * Maybe a better way to do this is to update the lru structure
 * whenever accept is called/initiated rather than when/if it
 * is successful.
 */

int
post_sock_accept(int listen_sd, struct info *ip, int sd, int err)
{
  struct info_listen_struct *ilp;
  int rc;

  ldbg("post_sock_accept(): listen_sd %d ip %p sd %d err %d\n",
      listen_sd, ip, sd, err);
  PRINT_TIME(listen_sd, &tnow, &tprev,
      "post_sock_accept(): listen_sd %d ip %p sd %d err %d",
      listen_sd, ip, sd, err);

  if (sd < 0) {
    PRINT_TIME(listen_sd, &tnow, &tprev, 
	"post_sock_accept: accept failed sd = %d  errno = %d", sd, err);
    /*
     * If the caller has reserved an info struct for us to use, unreserve it.
     */
    if (ip != NULL) {
      info_unreserve(ip);
    }
    process_accept_errs(sd, err);
    PRINT_TIME(listen_sd, &tnow, &tprev, "post_sock_accept: returning ");

    if (options.epoll_edge_triggered && err == EAGAIN) {
      sd_state_listener_not_readable(listen_sd);
    }
    return(0);
  }

  if (sd > max_sock_sd) {
    print_all();
    printf("EXITING from  post_sock_accept: new sd = %d is > max_sock_sd = %d\n",
      sd, max_sock_sd);
    exit(1);
  }

  assert(sd > min_usable_sd);

  /*
   * This should be a "fresh" sd, and so it should not already be associated
   * with an info struct at this point.
   * TBB - this is not true if aio is being used
   */
  if (!options.aio_read_before_accept) {
    assert(!info_map_valid(sd));
  }
  if (ip != NULL) {
    /*
     * The caller has reserved an info struct for our use.  Also, it is
     * assumed that the caller has filled in ip->addr ahead of time, e.g.
     * through accept().  All we have to do is register the sd with the
     * info struct.
     */
    info_register(ip, sd);
  } else {
    /*
     * We need to allocate an info struct for this sd.  We also have to fill
     * in ip->addr with the client's IP address and port number.
     */
    ip = info_add(sd);
    get_sock_addr(sd);
  }

  if (listen_sd != SOCK_LISTENER_UNKNOWN) {
    ilp = &info_listener[listen_sd];
    ilp->accept_count++;
    ilp->cur_conns++;
    if (options.use_socket_aio) {
      ilp->aio_accept_outstanding--;
    }

    if (ilp->cur_conns > ilp->max_cur_conns) {
      ilp->max_cur_conns = ilp->cur_conns;
    }

    PRINT_TIME(sd, &tnow, &tprev, "post_sock_accept: listen_sd = %d "
      "cur_conns = %d",
      listen_sd, ilp->cur_conns);
  } else {
    /* TODO */
    printf("post_sock_accept: code not completed\n");
    assert(0);
    exit(1);
  }

  num_accept_successful++;
  cur_conns++;
  PRINT_TIME(sd, &tnow, &tprev, "post_sock_accept: accepted cur_conns = %d",
      cur_conns);

#ifdef FIXHUP
  ip->got_hup = 0;
#endif /* FIXHUP */

#ifdef TRACK_STUCK_CONNS
  /* clear the history because the socket is getting used again now */
  ip->prev_fsm_state_history = ip->fsm_state_history;
  ip->fsm_state_history = 0;
  ip->read_count = 0;
  ip->read_nread = -88;
  ip->read_errno = -88;
  ip->session_len = 0;
#endif /* TRACK_STUCK_CONNS */

  if (options.accepts_only) {
    ip->sd = sd;
    ip->parent_sd = listen_sd;
    set_fsm_state(ip, FSM_CONNECTING);
    PRINT_TIME(sd, &tnow, &tprev, "post_sock_accept: accepted");

    /* this is done at initiation time for aio calls */
    if (!options.use_aio_accept) {
      num_idle--;

      if (num_idle == options.free_fd_thold) {
	PRINT_TIME(sd, &tnow, &tprev, 
	  "post_sock_accept: ATTENTION: NUM_IDLE == %d", 
	  options.free_fd_thold);
      }
    }
    set_fsm_state(ip, FSM_CLOSING);
    PRINT_TIME(sd, &tnow, &tprev, "post_sock_accept: closing");
    /* TODO: check if this is correct */
    /* close_sock(sd); */
    do_close(ip, REASON_CLOSE_AFTER_ACCEPT);

    set_fsm_state(ip, FSM_CLOSED);
    //  num_idle++;
    PRINT_TIME(sd, &tnow, &tprev, "post_sock_accept: closed");
    rc = 1;
  } else {
    rc = socket_setup(listen_sd, sd);
    if (rc) {
      if (options.close_after_sock_init) {
	do_close(ip, REASON_CLOSE_AFTER_INIT);
      } else {
/*
#define SEND_PRIME_IO
*/
#ifdef SEND_PRIME_IO
	/* TODO: XXX: in some environments (e.g., HP-UX clients
	 * the new connection won't get any new events delivered
	 * so we sort of fake them.
	 * I believe that this may occur when the new connection
	 * immediately has data on it at connection time - so there isn't
	 * a new event generated after the connection is establied.
	 */
        if ((options.get_connections == OPT_CONN_WITH_SEND_EVTS) &&
	     (!options.auto_accept)) {

          PRINT_TIME(sd, &tnow, &tprev, "post_sock_accept: send_prime_io");
          send_prime_io(ip);
        }
#endif /* SEND */
      }
    }
  }

  rc = 2;

  return rc;
} /* post_sock_accept */

/*----------------------------------------------------------------------*/

static int
set_client_sockopts(int listen_sd, int sd)
{
  int flags;
  struct linger l;
  const int one = 1;

  PRINT_TIME(sd, &tnow, &tprev, "set_client_sockopts: entered");
  assert(sd > 2);

#ifdef DEBUG_SOCKBUF
  len = sizeof(size);
  if (getsockopt(sd, SOL_SOCKET, SO_SNDBUF, (void *) &size, &len) < 0) {
    perror("set_client_sockopts: getsockopt");
    exit(-1);
  } else {
    DEBG(MSG_CONN, "set_client_sockopts: SNDBUF = %d\n", size);
  }

  len = sizeof(size);
  if (getsockopt(sd, SOL_SOCKET, SO_RCVBUF, (void *) &size, &len) < 0) {
    perror("set_client_sockopts: getsockopt");
    exit(-1);
  } else {
    DEBG(MSG_CONN, "set_client_sockopts: RCVBUF = %d\n", size);
  }
#endif /* DEBUG_SOCKBUF */


  if (((options.get_connections == OPT_CONN_WITH_SEND_EVTS) ||
       (options.get_connections == OPT_CONN_WITH_SEND_SELECT)) &&
      (options.send_io_events || !options.auto_accept)) {
#ifdef SEND
    PRINT_TIME(sd, &tnow, &tprev, "set_client_sockopts: F_SETOWN");
    if (fcntl(sd, F_SETOWN, getpid()) < 0) {
      printf("fcntl F_SETSIG - %i\n",errno);
      exit(-1);
    }

    PRINT_TIME(sd, &tnow, &tprev, "set_client_sockopts: F_GETFL");
    if ((flags = fcntl(sd, F_GETFL, 0)) < 0) {
        perror("fcntl F_GETFL failed\n");
	exit (1);
    }

    PRINT_TIME(sd, &tnow, &tprev, "set_client_sockopts: O_NONBLOCK | O_ASYNC");
    if (fcntl(sd, F_SETFL, flags | O_NONBLOCK | O_ASYNC) < 0) {
      perror("fcntl F_SETFL O_NONBLOCK | O_ASYNC failed\n");
      exit(1);
    }

    PRINT_TIME(sd, &tnow, &tprev, "set_client_sockopts: IO_GRP_NUM");
    if (fcntl(sd, F_SETSIG, IO_GRP_NUM) < 0) {
      printf("fcntl F_SETSIG - %i\n",errno);
      exit(-1);
    }
#endif /* SEND */
  } else {
    if ((flags = fcntl(sd, F_GETFL, 0)) < 0) {
	/* perror ("set_client_sockopts: fcntl"); */
	num_set_client_sockopts_failed++;
	close_sock(sd);
	return 0;
	/* exit (-1); */
    }

    if (fcntl(sd, F_SETFL, flags | O_NONBLOCK) < 0) {
	/* perror ("set_client_sockopts: fcntl"); */
	num_set_client_sockopts_failed++;
	close_sock(sd);
	return 0;
	/* exit (-1); */
    }
  }

  if (!options.use_tcp_cork) {
    /* Disable Nagle algorithm so we don't delay needlessly when
       pipelining requests.  */
    if (setsockopt(sd, SOL_TCP, TCP_NODELAY, &one, sizeof(one)) < 0) {
      /* perror ("set_client_sockopts: setsockopt(TCP_NODELAY)"); */
      num_set_client_sockopts_failed++;
      close_sock(sd);
      return 0;
      /* exit (-1); */
    }
  }

  if (options.close_with_reset) {
    l.l_onoff = 1;
    l.l_linger = 0;
    if (setsockopt(sd, SOL_SOCKET, SO_LINGER, &l, sizeof(l)) < 0) {
      perror ("set_client_sockopts: setsockopt(SO_LINGER)");
      printf("set_client_sockopts: setsocketopt(SO_LINGER failed on sd = %d "
	  "errno = %d\n", sd, errno);
      exit (-1);
    }
  }

  return 1;
} /* set_client_sockopts */
/*----------------------------------------------------------------------*/
