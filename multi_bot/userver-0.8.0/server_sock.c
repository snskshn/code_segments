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

#ifdef HAVE_SSL
#include <openssl/ssl.h>
#endif /* HAVE_SSL */

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#ifdef SUNOS
#include <stropts.h>
#endif /* SUNOS */
#include <stdlib.h>
#include <string.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/signal.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/ioctl.h>
#ifdef PROVIDE_EPOLL_HEADERS
#include "sys_epoll.h"
#else
#ifdef __linux__
#include <sys/epoll.h>
#endif
#endif

#include "debug.h"
#include "local_debug.h"
#include "stats.h"
#include "common.h"
#include "options.h"
#include "lru.h"
#include "interest_set.h"
#include "sock_listener.h"
#include "sock_special.h"
#include "aio_layer.h"
#include "fcgi.h"
#include "trace.h"
#include "reply_status.h"

#ifdef HAVE_SSL
#include "u_ssl_utils.h"
#endif /* HAVE_SSL */

// #define LOCAL_DEBUG
#include "local_debug.h"

#ifdef SEND
#include "send.h"
#endif

int numprocs = 1;
int maxconns = OPT_DEFAULT_MAX_CONNS;
int max_fds = OPT_DEFAULT_MAX_FDS;
int min_usable_sd = -1;
int first_regular_sd = -1;

#ifdef HAVE_SSL
int ssl_listen_sd = -1;
SSL_CTX *server_ctx = 0;
#endif /* HAVE_SSL */

bufpool_t *dyn_bufpool;

void listener_async_setup(int sd);
#ifdef SEND
void listener_send_async_setup(int sd);
#ifdef ARRAY_OF_BUFS
void send_init_event_mappings();
#endif /* ARRAY_OF_BUFS */
#endif /* SEND */

static int listener_create(const char *path, int init_mode, int is_ssl_listener);
static void server_extras(int listen_sd);
static void post_listener_create(int sd, int init_mode);
static void socket_bind(int sd, const sockaddr_t *addr);
static int socket_create(int domain, int type, int protocol);
static void post_socket_create(int sd);
static void socket_listen(int sd, int len);
static void listener_setopts(int sd);
static int listener_aio_setup(int sd);
static void socket_connect(int sd, const sockaddr_t *addr);
int fcgi_create(app_t *app, appserver_t *appserver);

/* ---------------------------------------------------------------------- */
#ifdef SEND
void
listener_send_async_setup(int sd)
{
  int i;
  int flags;

  /* if send_polls_for_accepts then we only need to ensure that the
   * listen_sd is put into non-blocking mode
   */

  if (fcntl(sd, F_SETOWN, getpid()) < 0) {
    printf("fcntl F_SETOWN - %i\n", errno);
    exit(-1);
  }

  DEBG(MSG_SEND, "listener_send_async_setup: sd = %d\n", sd);
  if ((flags = fcntl(sd, F_GETFL, 0)) < 0) {
    perror("fcntl F_GETFL failed\n");
    exit(1);
  }

  if (options.auto_accept) {
    if (fcntl(sd, F_SETFL, flags | O_NONBLOCK) < 0) {
      perror("fcntl F_SETFL O_NONBLOCK\n");
      exit(1);
    }
  } else {
    if (options.send_polls_for_accepts) {
      DEBG(MSG_SEND, "listener_send_async_setup: setting O_NOBLOCK\n");
      if (fcntl(sd, F_SETFL, flags | O_NONBLOCK) < 0) {
	perror("fcntl F_SETFL O_NONBLOCK failed\n");
	exit(1);
      }
    } else {
#if defined(__CYGWIN__)
      printf("listener_send_async_setup: async I/O not "
	     "supported in CYGWIN implementation\n");
      exit(1);
#else
      DEBG(MSG_SEND, "listener_send_async_setup: setting "
	  "O_NOBLOCK and O_ASYNC\n");
      if (fcntl(sd, F_SETFL, flags | O_NONBLOCK | O_ASYNC) < 0) {
	perror("fcntl F_SETFL O_NONBLOCK | O_ASYNC failed\n");
	exit(1);
      }
    }
#endif
  }

  /* Since the child sockets inherit properties of the parent */
  /* we don't want these set if we are only doing accepts */
  if (!options.accepts_only) {
    if (!options.send_polls_for_accepts) {
      /* The interface for getting events goes through fcntl */
      DEBG(MSG_SEND, "listener_send_async_setup: calling fcntl group num = %d\n",
	  IO_GRP_NUM);
      if (fcntl(sd, F_SETSIG, IO_GRP_NUM) < 0) {
	printf("fcntl F_SETSIG - %i\n", errno);
	exit(-1);
      }
    }
  }

  if (options.auto_accept) {
    if (options.accepts_only) {
      i = sd;
      DEBG(MSG_SEND, "Calling SIOCAUTOACCEPTNC i = %d\n", i);
      if (ioctl(sd, SIOCAUTOACCEPTNC, &i) < 0) {
	printf("ioctl SIOCAUTOACCEPTNC - failed %i\n", errno);
	exit(-1);
      }
      printf("SIOCAUTOACCEPTNC not supported\n");
      exit(-1);
    } else {
      i = sd;
      DEBG(MSG_SEND, "Calling SIOCAUTOACCEPT i = %d\n", i);
      if (ioctl(sd, SIOCAUTOACCEPT, &i) < 0) {
	printf("ioctl SIOCAUTOACCEPT - failed %i\n", errno);
	exit(-1);
      }
    }
    DEBG(MSG_SEND, "listener_send_async_setup: Called ioctl sd = %d\n", sd);
    DEBG(MSG_SEND, "listener_send_async_setup: Called ioctl i = %d\n", i);
  } else {
    if (!options.send_polls_for_accepts) {
      /* We are using SEND and the only other way to get events  */
      /* on connections is this way */
      /* The interface for getting events goes through fcntl */
      DEBG(MSG_SEND, "listener_send_async_setup: calling fcntl %d\n",
	  ACCEPT_IO_GRP_NUM);
      if (fcntl(sd, F_SETSIG, ACCEPT_IO_GRP_NUM) < 0) {
	printf("fcntl F_SETSIG - %i\n", errno);
	exit(-1);
      }
    } else {
      DEBG(MSG_SEND,
	  "listener_send_async_setup: send will poll for new connections\n");
    }
  }

  PRINT_TIME(sd, &tnow, &tprev, "listener_send_async_setup: returning");

} /* listener_send_async_setup */
#endif /* SEND */

/* ---------------------------------------------------------------------- */
void
listener_async_setup(int sd)
{
  int flags = 0;

  DEBG(MSG_ASYNC, "listener_async_setup: socket %d for asynchronous use\n", sd);


  /* As per pg 590 Stevens UNIX NP, Vol 1, 2nd edition
   *   - set the socket owner fcntl (F_SETOWN).
   *   - enable signal driven IO (fcntl, A_ASYNC).
   */

  if (fcntl(sd, F_SETOWN, getpid()) < 0) {
    perror("fcntl F_SETOWN failed\n");
    //close(sd);
    exit(1);
  }


  if ((flags = fcntl(sd, F_GETFL, 0)) < 0) {
    perror("fcntl F_GETFL failed\n");
    //close(sd);
    exit(1);
  }

#if defined(HPUX) || defined(__CYGWIN__)
  printf("async not finished\n");
  exit(1);
#else
#ifdef SUNOS
  if (fcntl(sd, F_SETFL, flags | O_NONBLOCK) < 0) {
    perror("fcntl F_SETFL O_NONBLOCK failed\n");
    //close(sd);
    exit(1);
  }
  if (0 > ioctl(sd, I_SETSIG, S_INPUT | S_ERROR)) {
    perror("ioctl(I_SETSIG)");
    //close(sd);
    exit(1);
  }
#else
  if (fcntl(sd, F_SETFL, flags | O_NONBLOCK | O_ASYNC) < 0) {
    perror("fcntl F_SETFL O_NONBLOCK | O_ASYNC failed\n");
    //close(sd);
    exit(1);
  }
#endif /* ! SUNOS */
#endif /* ! HPUX */

  PRINT_TIME(sd, &tnow, &tprev, "listener_async_setup: returning");
  DEBG(MSG_ASYNC, "Done setting up socket %d "
      "for asynchronous use\n", sd);
} /* listener_async_setup */



/* ---------------------------------------------------------------------- */
void
server_init(int init_mode)
{
#ifdef SEND
#ifdef ARRAY_OF_BUFS
  extern void init_ecb_array();
#endif /* ARRAY_OF_BUFS */
#endif /* SEND */

  int sd;
  int i;

            /*these are used for starting appservers */
  /*app_t *app;
  appserver_t *appserver;
  appserver_startinfo_t *start;*/
  
  char buf[1024];

  /* int flags; ****** used for dyn_bufpool allocation */

  printf("server_init: init_mode = 0x%x = %d\n",
          init_mode, init_mode);

  lru_init();

#ifdef SEND
  if (init_mode & LISTENER_SEND_INIT) {
#ifdef ARRAY_OF_BUFS
     init_ecb_array();
#else
     init_ecb();
#endif /* ARRAY_OF_BUFS */
  }
#endif /* SEND */

  /* get the sockets set up for listening - we listen later 
   * because we have other things that need to get set up 
   * before listening
   */
  if (options.ip_addr_count == 0) {
    sd = listener_create(NULL, init_mode, LISTENER_IS_NOT_SSL);
    post_listener_create(sd, init_mode);
    printf("listening socket is %d\n", sd);

#ifdef HAVE_SSL
    //add a listener for SSL 
    sd = listener_create(NULL, init_mode, LISTENER_IS_SSL);
    post_listener_create(sd, init_mode);
    printf("listening ssl socket is %d\n", sd);
#endif /* HAVE_SSL */

  } else {
    for (i = 0; i < options.ip_addr_count; i++) {
      sd = 0;
      sd = listener_create(options.ip_addr[i], init_mode, LISTENER_IS_NOT_SSL);
      post_listener_create(sd, init_mode);
      printf("listening socket is %d\n", sd);
    }
  }

#ifdef SEND
#ifdef ARRAY_OF_BUFS
  if (init_mode & LISTENER_SEND_INIT) {
    assert(ecb);
    send_init_event_mappings();
  }
#endif /* ARRAY_OF_BUFS */
#endif /* SEND */

  /* now listen on each of the sockets */
  for (i = sock_listener_min; i <= sock_listener_max; i++) {
    if (sock_is_listener(i)) {
      PRINT_TIME(i, &tnow, &tprev, "server_init: to listen on %d q = %d", i,
	  options.listenq);

      socket_listen(i, options.listenq);
    }
  }

#ifdef OLDWAY
  /* this is now done after forking uservers 
        ******* allocate dynamic buffer pool ******
  if (options.num_dyn_buffers > 0) {
    if (options.use_sendfile || options.dyn_use_shared_mem) {
      flags = ALLOC_MMAP_FILE;
    } else {
      flags = ALLOC_MALLOC;
    }
    if (options.dyn_lock_pages) {
      flags |= ALLOC_MLOCK;
    } else if (options.dyn_touch_pages) {
      flags |= ALLOC_TOUCH;
    }
    dyn_bufpool = alloc_bufpool(options.num_dyn_buffers,
	options.dyn_buffer_size, flags);
    if (dyn_bufpool == NULL) {
      printf("server_init: can't allocate dynamic buffer pool\n");
      exit(1);
    }
  } */
#endif /* OLDWAY */

  /*
   * A bit of FastCGI initialization here.  We compute the value of
   * fcgi_HTTP_hdr_len by doing a test sprintf().  This tells us how much
   * space we need to reserve at the beginning of each reply buffer so
   * we can prepend our HTTP headers.
   */
  fcgi_HTTP_hdr_len = sprintf(buf, HTTP_OK_STR_ID_LEN_NOBLANK,
      1, server_ident, ULONG_MAX);
  assert(fcgi_HTTP_hdr_len < (int) sizeof(buf));  // make sure we didn't overflow

#ifdef OLDWAY
  /* The following tasks are now done after forking uservers 

  *****start any applications that we are responsible for starting****** 
  start = options.appserver_startinfo;
  while (start != NULL) {
    assert(start->server != NULL);
    start->server->pid = fork_app_server(start->exec_string);
    start = start->next;
  }
  if(options.appserver_startinfo != NULL ) sleep(5);

  **** connect to applications *****
  for (i = 0; i < options.app_count; i++) {
    app = &options.app[i];
    switch (app->req_type) {
      case REQ_FASTCGI:
	appserver = app->server;
	assert(appserver != NULL);
	do {
	  fcgi_create(app, appserver);
	  appserver = appserver->next;
	} while (appserver != NULL);
	break;
      default:
	*******nothing to do for this type of application*******
	break;
    }
  } */   
#endif /* OLDWAY */

  info_listener_compute_limits();

} /* server_init */


/* ---------------------------------------------------------------------- */
static int
listener_create(const char *path, int init_mode, int is_ssl_listener)
{
  sockaddr_t addr;
  int sd;
  int port=options.port;

#ifdef HAVE_SSL
  if (is_ssl_listener == 1) {
	port = options.ssl_port;
	printf("creating ssl socket on port %d\n", port);
  }
#else
  // NOT SURE WHY THIS WAS HERE
  // printf("Need to compile with HAVE_SSL\n");
  // exit(1);
#endif /* HAVE_SSL */

  if (sock_path_to_addr(path, &addr, 1, OPT_IP_ADDR_ANY, port) < 0) {
    printf("listener_create: can't convert '%s' to sock addr\n", path);
    exit(1);
  }

  sd = socket_create(addr.u.sa.sa_family, SOCK_STREAM, 0);
  sock_set_listener(sd);

  printf("listener_create: Using sd = %d\n", sd);

  post_socket_create(sd);

  socket_bind(sd, &addr);

#ifdef HAVE_SSL
  if(is_ssl_listener) {
    ssl_listen_sd = sd;
    server_ctx = initialize_ssl(port);
		PRINT_TIME(ssl_listen_sd, &tnow, &tprev, "listener_create: got server context\n");
  }
#endif /* HAVE_SSL */



  if (!options.use_socket_aio) {
    listener_setopts(sd);
  } else {
    /* set socket options for aio sockets here */
  }

  return sd;
} /* listener_create */


/* ---------------------------------------------------------------------- */
static void
server_extras(int listen_sd)
{
#ifdef SEND
  extern unsigned int send_readfds;
#endif /* SEND */

#ifdef HAVE_EPOLL
  struct epoll_event ep_evt;
  extern int epoll_fd_array[];
  extern struct epoll_event *epoll_array;
  int i;
  int rc = 0;
  int trace_fd = listen_sd;
  int save_errno = 0;
#endif

  PRINT_TIME(listen_sd, &tnow, &tprev, "server_extras: listen_sd = %d", listen_sd);

  switch (options.get_connections)
  {
    case OPT_CONN_WITH_SELECT_POLL_EPOLL:
      FD_SET(listen_sd, &openfds);
      if (!options.noaccepts) {
#ifdef HAVE_EPOLL
        /* for each of the processes add all listeners to the interest set */
        if (options.use_epoll || options.epoll_edge_triggered || 
            options.use_epoll2) {
	  for (i=0; i<numprocs; i++) {
	    ep_evt.data.fd = listen_sd;
	    ep_evt.events = EPOLLIN;
#ifdef FIXME
	    if (options.epoll_edge_trigger_listeners) {
	      ep_evt = EPOLLIN | EPOLLET;
	    }
#else
#endif

#ifdef PROVIDE_EPOLL_HEADERS
	    TRACE(EVT_EPOLL_CTL,
	      trace_fd = listen_sd;
	      rc = sys_epoll_ctl(epoll_fd_array[i], EPOLL_CTL_ADD, listen_sd, &ep_evt);
	      save_errno = errno;
	    );
#else
	    TRACE(EVT_EPOLL_CTL,
	      trace_fd = listen_sd;
	      rc = epoll_ctl(epoll_fd_array[i], EPOLL_CTL_ADD, listen_sd, &ep_evt);
	      save_errno = errno;
	    );
#endif

	    if (rc < 0) {
	      perror("server_extras: epoll_ctl failed ");
	      exit(1);
	    }

            PRINT_TIME(listen_sd, &tnow, &tprev, "server_extras: set epoll_array listen_sd = %d", listen_sd);
            printf("server_extras: set epoll_array listen_sd = %d\n", listen_sd);
            printf("server_extras: &epoll_array = %p\n", &epoll_array);
            printf("server_extras: &epoll_array[listen_sd].data.fd = %p\n", 
	            &epoll_array[listen_sd].data.fd);
	           
            printf("server_extras: &epoll_array[listen_sd].data.fd = %p\n", 
	            &epoll_array[listen_sd].events);
	    epoll_array[listen_sd].data.fd = -1;
	    epoll_array[listen_sd].events = 0;

	  }
	} else {
	  interest_set_change(listen_sd, ISET_INIT | ISET_READABLE);
	}
#else
	interest_set_change(listen_sd, ISET_INIT | ISET_READABLE);
#endif /* HAVE_EPOLL */
      }
      break;

#ifdef SEND
    case OPT_CONN_WITH_SEND_SELECT:
      if (!options.noaccepts) {
        FD_SET(listen_sd, (fd_set *) &send_readfds);
      }
      break;
#endif /* SEND */
    
    case OPT_CONN_WITH_SIGIO:
    case OPT_CONN_WITH_AIO_ACCEPT:
      break;

    case OPT_CONN_WITH_SEND_EVTS:
#ifdef TBB_KINFO
      if (options.kinfo) {
        do_kinfo_init(kinfo_fd);
      }
#endif /* TBB_KINFO */
      break;

    default :
      printf("server_extras: options.get_connections = %d not handled\n",
              options.get_connections);
      exit(1);
      break;
  } /* switch */

  /* TODO: ensure that this move doesn't mess things up 
   * I think this should be done in all cases.
   */

} /* server_extras */

/* ---------------------------------------------------------------------- */
static void
post_listener_create(int sd, int init_mode)
{

  PRINT_TIME(sd, &tnow, &tprev, "post_listener_create: init_mode = 0x%x", 
	init_mode);

  if (init_mode & LISTENER_DO_ASYNC_INIT) {
    listener_async_setup(sd);
  } 
#ifdef SEND
  if (init_mode & LISTENER_SEND_INIT) {
    listener_send_async_setup(sd);
  }
#endif /* SEND */

  if (init_mode & LISTENER_AIO_INIT) {
    listener_aio_setup(sd);
  }

    server_extras(sd);
} /* post_listener_create */

/* ---------------------------------------------------------------------- */
static void 
socket_bind(int sd, const sockaddr_t *addr)
{
  int save_errno;
  int rc;

  printf("socket_bind: sd %d addr '%s'\n", sd, sock_addr_str(addr));
  PRINT_TIME(NOFD, &tnow, &tprev, "socket_bind: sd %d addr '%s'\n",
      sd, sock_addr_str(addr));

  if (options.use_socket_aio) {
#ifdef HAVE_AIO_LAYER
    rc = aio_sock_bind(sd, (struct sockaddr *) &addr->u.sa, addr->len);
    save_errno = errno;
#else
    printf("socket_bind: aio not handled\n");
    exit(1);
#endif /* HAVE_AIO_LAYER */
  } else {
    rc = bind(sd, &addr->u.sa, addr->len);
    save_errno = errno;
  }

  PRINT_TIME(sd, &tnow, &tprev, "socket_bind: bind returns %d ", rc);
  if (rc < 0) {
    printf("socket_bind: bind failed rc = %d errno = %d\n", rc, save_errno);
    perror("bind");
    //close(sd);
    exit(-1);
  } else {
    store_sock_addr(sd, addr);
  }
} /* socket_bind */

/* ---------------------------------------------------------------------- */
static int
socket_create(int domain, int type, int protocol)
{
  int sd;

  PRINT_TIME(NOFD, &tnow, &tprev, "socket_create: entered");

  if (options.use_socket_aio) {
#ifdef HAVE_AIO_LAYER
    sd = aio_sock_socket(domain, type, protocol);
#else
    printf("socket_create: aio not handled\n");
    exit(1);
#endif /* HAVE_AIO_LAYER */
  } else {
    sd = socket(domain, type, protocol);
  }

  if (sd < 0) {
    perror("socket");
    exit(-1);
  }

  // printf("socket_create: created socket sd %d\n", sd);
  return sd;
} /* socket_create */

/* ---------------------------------------------------------------------- */
static void
post_socket_create(int sd)
{
  const int one = 1;
  int flags = 0;
  struct info *ip;

  PRINT_TIME(NOFD, &tnow, &tprev, "post_socket_create: entered");

  if (!options.use_socket_aio) {
    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one)) < 0) {
      perror("setsockopt");
      //close(sd);
      exit(-1);
    }

    if ((flags = fcntl(sd, F_GETFL, 0)) < 0) {
      perror("server_init: fcntl F_GETFL failed on sd (becomes listen_sd)\n");
      //close(sd);
      exit(1);
    }

    if (fcntl(sd, F_SETFL, flags | O_NONBLOCK) < 0) {
      perror("server_init: fcntl F_SETFL O_NONBLOCK failed\n");
      //close(sd);
      exit(1);
    }
  }

  if (sd > max_sd) {
    max_sd = sd;
  }

  if (options.use_max_open_sd) {
    FD_SET(sd, &openfds);
    if (sd > max_opensd) {
      max_opensd = sd;
    }
  }

  assert(!info_map_valid(sd));
  if (!sock_is_listener(sd)) {
    ip = info_add(sd);
    assert(ip);
  }

  PRINT_TIME(NOFD, &tnow, &tprev, "post_socket_create: returning");
} /* post_socket_create */

/* ---------------------------------------------------------------------- */
static void
socket_listen(int sd, int len)
{
  int rc = -1;

  printf("socket_listen: sd %d len %d\n", sd, len);
  PRINT_TIME(NOFD, &tnow, &tprev, "socket_listen: sd %d len %d\n", sd, len);

  if (options.use_socket_aio) {
#ifdef HAVE_AIO_LAYER
    rc = aio_sock_listen(sd, len);
#else
    printf("socket_listen: aio not handled\n");
    exit(1);
#endif /* HAVE_AIO_LAYER */
  } else {
    rc = listen(sd, len);
  }
  printf("socket_listen: listen returns %d\n", rc);

  if (rc < 0) {
    printf("socket_listen: failed listen call\n");
    perror("listen");
    //close(sd);
    exit(-1);
  } else {
    printf("server_init: listening on socket = %d\n", sd);
  }
} /* socket_listen */

/* ---------------------------------------------------------------------- */
static void
listener_setopts(int sd)
{
  int size = 0;
  socklen_t optlen = 0;

  size = options.read_sock_buf_size;
  DEBG(MSG_SOCKBUF, "before setsockopt size = %d\n", size);
  if (size != OPT_USE_DEFAULT_SOCK_BUF_SIZE) {
    if (setsockopt(sd, SOL_SOCKET, SO_RCVBUF, 
	(void *) &size, sizeof(size)) < 0) {
      perror("listener_opts: setsockopt");
      //close(sd);
      exit(-1);
    }
  }
  DEBG(MSG_SOCKBUF, "after setsockopt size = %d\n", size);

  size = options.write_sock_buf_size;
  DEBG(MSG_SOCKBUF, "before setsockopt size = %d\n", size);
  if (size != OPT_USE_DEFAULT_SOCK_BUF_SIZE) {
    if (setsockopt(sd, SOL_SOCKET, SO_SNDBUF, 
	(void *) &size, sizeof(size)) < 0) {
      perror("listener_opts: setsockopt");
      //close(sd);
      exit(-1);
    }
  }
  DEBG(MSG_SOCKBUF, "after setsockopt size = %d\n", size);


#ifndef SUNOS
  size = options.read_sock_buf_lowat;
  DEBG(MSG_SOCKBUF, "before setsockopt lowat = %d\n", size);
  if (size != OPT_USE_DEFAULT_SOCK_LOWAT) {
    if (setsockopt(sd, SOL_SOCKET, SO_RCVLOWAT, 
	(void *) &size, sizeof(size)) < 0) {
      perror("listener_opts: setsockopt");
      printf("Unable to set SO_RCVLOWAT\n");
      //close(sd);
      exit(-1);
    }
  }
  DEBG(MSG_SOCKBUF, "after setsockopt lowat = %d\n", size);
#endif /* !SUNOS */

  size = options.write_sock_buf_lowat;
  DEBG(MSG_SOCKBUF, "before setsockopt lowat = %d\n", size);
  if (size != OPT_USE_DEFAULT_SOCK_LOWAT) {
    if (setsockopt(sd, SOL_SOCKET, SO_SNDLOWAT, 
	(void *) &size, sizeof(size)) < 0) {
      perror("listener_opts: setsockopt");
      printf("Unable to set SO_SNDLOWAT\n");
      //close(sd);
      exit(-1);
    }
  }
  DEBG(MSG_SOCKBUF, "after setsockopt lowat = %d\n", size);

  optlen = sizeof(size);
  if (getsockopt(sd, SOL_SOCKET, SO_SNDBUF, 
      (void *) &size, &optlen) < 0) {
    perror("listener_opts: getsockopt");
    //close(sd);
    exit(-1);
  } else {
    printf("listener_opts: SNDBUF = %d\n", size);
  }

  optlen = sizeof(size);
  if (getsockopt(sd, SOL_SOCKET, SO_RCVBUF, 
      (void *) &size, &optlen) < 0) {
    perror("listener_opts: getsockopt");
    //close(sd);
    exit(-1);
  } else {
    printf("listener_opts: RCVBUF = %d\n", size);
  }

#if !defined(SUNOS) && !defined(__CYGWIN__)
  optlen = sizeof(size);
  if (getsockopt(sd, SOL_SOCKET, SO_SNDLOWAT, 
      (void *) &size, &optlen) < 0) {
    perror("listener_opts: getsockopt");
    //close(sd);
    exit(-1);
  } else {
    printf("listener_opts: SNDLOWAT = %d\n", size);
  }

  optlen = sizeof(size);
  if (getsockopt(sd, SOL_SOCKET, SO_RCVLOWAT, 
      (void *) &size, &optlen) < 0) {
    perror("listener_opts: getsockopt");
    //close(sd);
    exit(-1);
  } else {
    printf("listener_opts: RCVLOWAT = %d\n", size);
  }
#endif /* !SUNOS && !__CYGWIN__ */
} /* listener_opts */

/* ---------------------------------------------------------------------- */
static int
listener_aio_setup(int sd)
{
  PRINT_TIME(sd, &tnow, &tprev, "listener_aio_setup entered");
  return 1;
} /* listener_aio_setup */

/* ---------------------------------------------------------------------- */
static void 
socket_connect(int sd, const sockaddr_t *addr)
{
  printf("socket_connect: sd %d addr '%s'\n", sd, sock_addr_str(addr));
  PRINT_TIME(NOFD, &tnow, &tprev, "socket_connect: sd %d addr '%s'\n",
      sd, sock_addr_str(addr));

  //TODO: aio_sock_connect() ?
  if (connect(sd, &addr->u.sa, addr->len) < 0) {
    printf("socket_connect: sd %d addr '%s'\n", sd, sock_addr_str(addr));
    perror("socket_connect: connect");
    exit(-1);
  }
} /* socket_connect */

/* ---------------------------------------------------------------------- */
int
fcgi_create(app_t *app, appserver_t *appserver)
{
  sockaddr_t addr;
  int sd;
  struct info *ip;

  assert(app != NULL);
  assert(appserver != NULL);

  if (sock_path_to_addr(appserver->path, &addr, 1,
	OPT_IP_ADDR_LOOPBACK, DEF_APP_SERVER_PORT) < 0) {
    printf("fcgi_create: can't convert '%s' to sock addr\n", appserver->path);
    exit(1);
  }

  appserver->sd = 
  sd = socket_create(addr.u.sa.sa_family, SOCK_STREAM, 0);
  if (sd < 0) {
    printf("fcgi_create: can't create socket\n");
    perror("can't create socket");
    exit(1);
  }
  appserver->sd = sd;

  /*
   * We momentarily delay calling post_socket_create() so that we can connect
   * to the FastCGI server synchronously.
   * An alternative is to modify socket_connect() to temporarily disable
   * non-blocking mode.
   * Another alternative is to add support for asynchronous connects.  This
   * will entail maintaining some extra state information.
   */
  socket_connect(sd, &addr);

  post_socket_create(sd);

  /* Mark the socket as special, so it isn't timed out by the lru code */
  sock_set_special(sd);

  ip = info_ptr(sd);
  store_sock_addr(sd, &addr);

  /* initialize key elements of the info struct */
  ip->type = INFO_FASTCGI;
  ip->app = app;
  ip->appserver = appserver;

  ip->req.state = FCGI_READING_HEADER;
  ip->req.buf = ip->req.static_buf;
  ip->req.buf_size = options.read_buffer_size;
  ip->req.end = ip->req.cur = ip->req.buf;
  ip->req.num = 0;
  ip->req.type = REQ_INVALID;


  ip->rep.fd = -1;
  ip->rep.buf = ip->rep.static_buf;
  ip->rep.buf_size = options.reply_buffer_size;
  ip->rep.end = ip->rep.cur = ip->rep.buf;
  ip->rep.total_len = 0;
  ip->rep.nwritten = 0;

  /*
   * A FastCGI socket is a bit different from a listener socket or an
   * ordinary client socket, in that we don't immediately expect to read or
   * write on the socket.  Still, we should initialize the sd in our
   * interest set so that later we can just make the sd readable and/or
   * writable at will.
   */
  interest_set_change(ip->sd, ISET_INIT);

#ifdef DO_FCGI_GET_SERVER_VALUES
  PRINT_TIME(sd, &tnow, &tprev, "fcgi_create: calling fcgi_get_server_values");
  if (fcgi_get_server_values(appserver) < 0) {
    printf("fcgi_get_server_values failed -- exiting\n");
    exit(1);
  }
#endif
  PRINT_TIME(sd, &tnow, &tprev, "fcgi_create: calling fcgi_set_server_values");
  if (fcgi_set_server_values(appserver) < 0) {
    printf("fcgi_set_server_values failed -- exiting\n");
    exit(1);
  }

  PRINT_TIME(sd, &tnow, &tprev, "fcgi_create: successful created sd = %d", sd);

  return sd;
} /* fcgi_create */
