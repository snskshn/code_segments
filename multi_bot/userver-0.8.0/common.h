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


#ifndef COMMON_H
#define COMMON_H

#ifdef HAVE_SSL
#include <openssl/ssl.h>
#endif /* HAVE_SSL */

#include <sys/types.h>
#include <sys/time.h>
#include "info.h"
#include "bufpool.h"

#define NELEMS(a)	((int) (sizeof(a) / sizeof((a)[0])))

#define _PASTE(x)	x
#define PASTE(x,y)	_PASTE(x)_PASTE(y)

#ifdef SENDFILE
# define OPT_SENDFILE	"-sendfile"
#else
# define OPT_SENDFILE	"-nosendfile"
#endif

#define IDENT	PASTE(PROG,OPT_SENDFILE)

#ifndef SOL_TCP
# define SOL_TCP 6	/* probably ought to do getprotlbyname() */
#endif

#define LISTENER_NOT_ASYNC_INIT  (0x01)
#define LISTENER_DO_ASYNC_INIT   (0x02)
#define LISTENER_SEND_INIT       (0x04)
#define LISTENER_AIO_INIT        (0x08)

#define SETUP_HTTP_CONTINUE   (2)


extern int max_sd;
extern int max_opensd;
extern int num_idle;
extern int num_select;
extern int cur_conns;

extern int saved_mode;
extern int in_cs;
extern int in_handler;
extern int sigio_blocked;
extern int max_sock_sd;
extern fd_set openfds;

extern int pagesize;
extern int cache_line_size;

extern char prog_name[];
extern char server_ident[];
extern int numprocs;
extern int maxconns;
extern int max_fds;
extern int min_usable_sd;
extern int first_regular_sd;

#ifdef HAVE_SSL
extern SSL_CTX *server_ctx;
extern int ssl_listen_sd;
#endif

extern bufpool_t *dyn_bufpool;

extern void do_sd(int sd, fd_set *readable, fd_set *writable);
extern int do_readable(int sd);
extern void do_writable(int sd);
extern void do_lru(int n, fd_set *readable, fd_set *writable);

extern void process_accept_errs(int sd, int err);
extern void server_init(int do_async);
extern void process_args(int argc, char *argv[]);
extern void check_options();
extern void set_fsm_state(struct info *ip, enum info_fsm_state state);
extern int parse_bytes(struct req *req);
extern int send_reply(int sd, struct rep *rep);
extern int socket_setup(int listen_sd, int sd);

extern void print_req(struct req *r);
extern void print_rep(struct rep *r);

extern int  do_close(struct info *ip, int reason);
extern int  socket_new_conn(int listen_fd);
#ifdef MACCEPT
extern int multiaccept(void);
#endif
extern int  socket_readable(struct info *ip);
extern int  setup_http(struct info *ip);
extern int  socket_writable(struct info *ip);
extern void process_sds(int n, fd_set *readable, fd_set *writable, 
                        int max_usable_sd);
extern void fork_servers(int num_servers);
extern pid_t fork_app_server(const char * exec_string);
extern void select_loop(void);
extern void send_loop(void);
extern void options_set();
extern void create_pidfile();
extern void delete_pidfile();

extern void close_idle_conns(struct timeval *tv);
extern int server_loop_top();
extern int do_write(int sd, struct rep *rep);

#if defined(HPUX) || ((defined(SUNOS) && !defined(__SUNPRO_C)) && (defined(SUNOS) && !defined(__sun__)))
#define timersub(a, b, result)                                      \
  do {                                                              \
    (result)->tv_sec = (a)->tv_sec - (b)->tv_sec;                   \
    (result)->tv_usec = (a)->tv_usec - (b)->tv_usec;                \
    if ((result)->tv_usec < 0) {                                    \
      --(result)->tv_sec;                                           \
      (result)->tv_usec += 1000000;                                 \
    }                                                               \
  } while (0)
#endif /* HPUX */

/*----------------------------------------------------------------------*/
#define do_extra_accepts(a, b) \
   if (((a) != 0) && ((a) % (b)) == 0) { \
     /* printf("a = %d b = %d\n", (a), (b)); */ \
     num_extra_attempts++; \
     do_new_connections(SOCK_LISTENER_USE_ALL, FROM_EXTRAS); \
   }

#endif /* COMMON_H */
