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


#ifndef INFO_H
#define INFO_H

#include <limits.h>
#include <time.h>

#include "cache.h"
#include "util.h"
#include "app.h"
#include "dll.h"
#ifdef HAVE_SSL
#include "openssl/ssl.h"
#endif /* HAVE_SSL */

/* NOTE: I'd like to try to keep these sizes defined so that
 * the size of a struct info is a multiple of the cache line size
 * while also trying to keep them small enough to keep
 * the total bytes consumed for the info array reasonable.
 * Note that we may allocate 15000 - 30000 struct info's.
 */
#ifdef IA64
#define MAX_TMP_LEN		   (64)
#define MAX_URI_LEN		   (64+48+12)  /* part of req */
#define MAX_COOKIE_LEN		   (32+8+30)   /* part of req */
#define MAX_SPECWEB99_OBJ_NAME_LEN (32+28+2)   /* part of rep */
#else
#define MAX_TMP_LEN		   (64)
#define MAX_URI_LEN		   (64+64+12)  /* part of req */
#define MAX_COOKIE_LEN		   (32+8+62)   /* part of req */
#define MAX_SPECWEB99_OBJ_NAME_LEN (32+28+2)   /* part of rep */
#endif

#define MAX_CONTENT_LEN		(INT_MAX)
#define NULL_INDEX		(-1)

#ifdef  CALL_STATS
#define MAX_CLIENT_ID   	(UINT_MAX)
#define MAX_CALL_ID   	        (UINT_MAX)
#endif /* CALL_STATS */

extern int *sd_map;			/* mapping from sd to info index */
extern struct info *info;		/* info about each connection */

/* Reasons for closing the connection.
 * These are all negative values, in case we want
 * to specify errno values as reasons.
 * Negate these values to find string entry in reason_str (in info.c)
 *
 */

#define REASON_NO_FAILURE              ( 0)
#define REASON_CLIENT_CLOSED           (-1)
#define REASON_IDLE                    (-2)
#define REASON_NO_SPACE                (-3)
#define REASON_WRITE_COMPL_ERROR       (-4)
#define REASON_SEND_POLL_ERROR         (-5)
#define REASON_SEND_POLL_FIN           (-6)
#define REASON_CLOSE_AFTER_ACCEPT      (-7)
#define REASON_CLOSE_AFTER_INIT        (-8)
#define REASON_READ_CONN_ERROR         (-9)
#define REASON_CLOSE_AFTER_READ        (-10)
#define REASON_CLOSE_AFTER_PARSE       (-11)
#define REASON_HTTP_1_0                (-12)
#define REASON_ESC_SEQUENCE            (-13)
#define REASON_FSTAT_FAILED            (-14)
#define REASON_SEND_REPLY_FAILED       (-15)

enum info_req_state
{
  /* STATIC states */
  PARSE_METHOD = 0,
  PARSE_URI,
  PARSE_VERSION,
  PARSE_SKIP,
  PARSE_CR_1,
  PARSE_LF_1,
  PARSE_HEADER_NAME,
  PARSE_COLON,
  PARSE_COOKIE,
  PARSE_IF_MODIFIED_SINCE,
  PARSE_CONTENT_LENGTH,
#ifdef CALL_STATS
  PARSE_CLIENT_ID,
#endif /* CALL_STATS */
  PARSE_CR_2,
  PARSE_LF_2,
  PARSE_DONE,
  /* FASTCGI states */
  FCGI_READING_HEADER,
  FCGI_READING_END_REQUEST,
  FCGI_READING_STDOUT,
  FCGI_READING_STDERR,
  FCGI_READING_GET_VALUES_RESULT,
  FCGI_READING_UNKNOWN_TYPE,
#ifdef MOD_SPECWEB99
  /* SPECweb99 states */
  SPECWEB99_REQUEST_DONE,
  SPECWEB99_RESPONSE_DONE,
  SPECWEB99_SOCKET_CLOSED
#endif
};

enum info_req_method
{
  HTTP_NONE,
  HTTP_SIMPLE,
  HTTP_GET,
  HTTP_HEAD,
  HTTP_POST,
  HTTP_NOT_IMPL,
  HTTP_RETURN_ERROR
};

struct req
{
  int num;
  req_type_t type;
  app_t *app;
  enum info_req_state state;
  int skip_lws;			/* flag: skip LWS to get to next token? */
  enum info_req_method method;
  /*
   * We essentially use "close" as an HTTP version flag.  A value of 0 means
   * HTTP/1.1, while a value of 1 means HTTP/1.0.  If a new version of HTTP
   * comes out and we want to support it, then maybe we'll store a proper
   * HTTP version number.
   */
  int close;
  char *query_string;		/* points into uri[], or NULL if n/a */
  int cookie_len;
  int content_len;
#ifdef CALL_STATS
  unsigned int client_id;
  unsigned int call_id;
#endif /* CALL_STATS */
  int padding_len;
  char *static_buf;		/* pre-allocated buffer */
  char *dyn_buf;		/* dynamically-allocated buffer */
  char *buf;			/* current buffer (e.g. one of the above) */
  int buf_size;		        /* size of current buffer */
  char *cur;			/* beginning of data in current buffer */
  char *end;			/* next spot for data in current buffer */
// #ifdef HAVE_AIO_LAYER
  void *read_reg_handle;
// #endif
  int tmp_len;
  /*
   * The tmp[] array is used for building up a single token (the HTTP
   * method, the HTTP version, or a header name).  It must be large enough
   * to hold the longest _supported_ header name (plus NUL terminator).
   * The tmp_len variable refers to the amount of data currently in tmp[].
   */
  char tmp[MAX_TMP_LEN];
  char cookie[MAX_COOKIE_LEN];
  char uri[MAX_URI_LEN];
  time_t ifmodsince;		/* from If-Modified-Since header */
};

struct rep
{
  int fd;			/* associated file or socket descriptor */

  cacheinfo *cache;
  char *static_buf;		/* pre-allocated buffer */
  char *dyn_buf;		/* dynamically-allocated buffer */
  char *buf;			/* current buffer (e.g. one of the above) */
  int buf_size;		        /* size of current buffer */
  char *cur;			/* beginning of data in current buffer */
  char *end;			/* next spot for data in current buffer */
// #ifdef HAVE_AIO_LAYER
  void *write_reg_handle;
// #endif
  int total_len;
  int nwritten;
  int offset;
  struct iovec iovec[2];     /* iovec[0] for header iovec[1] for file */
  char use_cache;   /* if set no need to open, read from or close file */
  char is_corked;
  int http_status;              /* HTTP status code of the reply (e.g. 200, 404, etc.) */
#ifdef MOD_SPECWEB99
  char objectname[MAX_SPECWEB99_OBJ_NAME_LEN];
  int objectlen;
#endif

#ifdef QUEUE_BUFS
	struct dll_element *buf_dll;
#endif

#ifdef IA64
  char __pad[8];
#endif
};

enum info_type
{
  INFO_CLIENT = 0,
  INFO_LISTENER,
  INFO_FASTCGI
};

enum info_fsm_state
{
  /*  0 */  FSM_UNKNOWN = 0,
  /* client states */
  /*  1 */ FSM_INITIAL_STATE,
  /*  2 */ FSM_CONNECTING,
  /*  3 */ FSM_READING_REQUEST,
  /*  4 */ FSM_READ_ARRIVED_WHILE_WRITING,
  /*  5 */ FSM_WRITING_REPLY,
  /*  6 */ FSM_READING_NEXT_REQUEST,
  /*  7 */ FSM_CLOSING,
  /*  8 */ FSM_CLOSED,
  /*  9 */ FSM_LISTENING,
  /* FastCGI request states */
  /* 10 */ FSM_FCGI_WRITING_SET_VALUES,
  /* 11 */ FSM_FCGI_WRITING_BEGIN_REQUEST,
  /* 12 */ FSM_FCGI_WRITING_PARAMS_1,
  /* 13 */ FSM_FCGI_WRITING_PARAMS_2,
  /* 14 */ FSM_FCGI_WRITING_END_PARAMS,
  /* 15 */ FSM_FCGI_WRITING_STDIN,
  /* 16 */ FSM_FCGI_WRITING_END_STDIN,
  /* 17 */ FSM_FCGI_WAITING_FOR_REPLY,
  /* 18 */ FSM_FCGI_REQUEST_QUEUED,
  /* 19 */ FSM_FCGI_CLEARING_BUFFER,
  /* 20 */ FSM_FCGI_UNBLOCKING,
  /* 21 */ FSM_CONN_ERROR,
  /* 22 */ FSM_STATE_MAX
};

struct info
{
  int sd;
  int skip;               /* used to determine if this request is a victim/should be treated differently */
  off_t skip_info;        /* used to determine track info related to skipping -- use depends on skipping method */
  sockaddr_t addr;
  enum info_type type;
	
#ifdef QUEUE_BUFS
	int done_write;
#endif

  // TODO
  // All fields below this point are dependent upon type (INFO_CLIENT, etc.).
  // It would make sense to just have a void *ptr that pointed to this
  // type-specific data.

#ifdef HAVE_SSL
  // int is_ssl;
  // SSL_CTX *ssl_ctx;
  SSL *ssl;
#endif /* HAVE_SSL */

  app_t *app;
  appserver_t *appserver;
  int parent_sd;        /* socket that accepted this connection */
  int conn_num;
  int next_free;
// To try to get sizing figured out whether AIO is being used or not.
// #ifdef HAVE_AIO_LAYER
  int save_result;      /* save out of order aio completion info */
  int save_err;
// #endif /* HAVE_AIO_LAYER */

  unsigned int sd_state;

  /* NOTE: if you change this be sure to change the fsm_string in state.c */
  enum info_fsm_state fsm_state;
  /* can't actually track this inside of the state changes
   * because we need to transition through the different states
   * while communicating with the appserver
   */
  unsigned int got_hup;
#ifdef TRACK_STUCK_CONNS
  unsigned int fsm_state_history;
  unsigned int prev_fsm_state_history;
  unsigned int read_count;
  unsigned int read_nread;
  unsigned int read_errno;
  int session_len;
#endif

#ifdef MOD_SPECWEB99
  void *priv;		/* private handle to a buffer - same as TUX */
#endif

  struct rep rep;
  struct req req;
};

extern struct info *info_add(int sd);
extern void info_delete(int sd);
extern int info_get_fsm_state(int sd);
extern void info_set_fsm_state(int sd, enum info_fsm_state state);
extern struct info *info_ptr(int sd);
extern struct req *info_req_ptr(int sd);
extern struct rep *info_rep_ptr(int sd);
extern int info_index(int sd);
extern int info_init(void);
extern void info_print_status(void);
extern struct info *info_reserve(void);
extern void info_unreserve(struct info *ip);
extern void info_register(struct info *ip, int sd);
extern int info_map_valid(int sd);
extern int info_close_rep_fd(struct info *ip);
extern void info_reset_rep_buf(struct info *ip);
extern const char *info_req_state_str(enum info_req_state state);
extern const char *info_req_method_str(enum info_req_method method);
void store_sock_addr(int sd, const sockaddr_t *addr);
void get_sock_addr(int sd);
void get_peer_addr_str(int sd, char *str, int maxlen);

int is_client_conn(struct info *ip);
int is_app_conn(struct info *ip);
int is_listening_conn(struct info *ip);
int is_hungup(struct info *ip);

#endif /* INFO_H */
