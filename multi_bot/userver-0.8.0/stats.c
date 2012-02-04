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


/*
 * Title   : stats.c
 * Author  : Tim Brecht
 * Date    : Fri Apr 13 12:59:55 PDT 2001
 *
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "debug.h"
#include "stats.h"
#include "options.h"
#include "common.h"
#include "syscalls.h"
#include "do_new_connections.h"
#include "netstat.h"
#include "rusage.h"
#include "state.h"
#include "stats.h"
#include "util.h"
#include "sock_listener.h"
#include "aio_layer.h"

#define USECS_PER_SEC (1000000.0)

void stats_print_accept(void);
void stats_print_close(void);
void stats_print_signals(void);
void stats_print_read(void);
void stats_print_read_file(void);
void stats_print_write(void);
void stats_print_select(void);
void stats_print_poll(void);
void stats_print_epoll(void);

#ifdef HAVE_AIO_LAYER
void stats_print_aio_stats(void);
void stats_print_aio_wait(void);
void stats_print_aio_sock_getevents(void);
void stats_print_aio_sock_mem_reg(void);
#endif /* HAVE_AIO_LAYER */

#ifdef SEND
void stats_print_send(void);
#endif /* SEND */
void stats_print_cache(void);
void stats_print_ifmodsince();
void stats_print_conns_on_off(void);
void stats_print_fcgi(void);
void stats_print_netstats(void);
void stats_print_rusage(void);
int have_send_events(void);

/* ---- Globals ------------------------------------------------------- */

struct timeval timer_start = {0, 0};
struct timeval timer_end = {0, 0};
double elapsed_time = 0.0;


int num_closed_state[FSM_STATE_MAX];

int num_do_new_conns_from[FROM_MAX];

int num_notification_off = 0;
int num_notification_on = 0;

int num_event_calls_successful = 0;
int num_total_fds_from_events = 0;

int num_requests = 0;
int num_replies = 0;
int num_good_gets = 0;
int num_failed_parse = 0;
int num_failed_request = 0;
int num_failed_open = 0;
int num_failed_stat = 0;

int num_accept_calls = 0;
int num_accept_aborts = 0;
int num_accept_eagain = 0;
int num_accept_enfile = 0;
int num_accept_emfile = 0;
int num_accept_eintr = 0;
int num_accept_eunknown = 0;
int num_accept_successful = 0;
int num_accept_max_reached = 0;
int num_accept_ready_but_no_idle = 0;
int num_socket_setup_failed = 0;
int num_set_client_sockopts_failed = 0;
int num_sigpipes = 0;
int num_sigurg = 0;
int num_extra_attempts = 0;

int num_connections = 0;
int num_do_new_connections_calls = 0;
int num_do_new_connections_no_idle = 0;
int num_do_new_connections_conns_off = 0;
int num_max_consecutive_accepts = 0;
int num_max_consecutive_requests = 0;

int num_socket_read_calls = 0;
int num_socket_read_eagain = 0;
int num_socket_read_eagain2 = 0;
int num_socket_read_retry_failed = 0;
int num_socket_read_nothing_first = 0;
int num_socket_read_nothing_other = 0;
int num_socket_read_reset = 0;
int num_socket_read_failed_others = 0;
int num_socket_read_successful = 0;
double num_socket_read_bytes = 0.0;

int num_file_read_calls = 0;
int num_file_read_successful = 0;
double num_file_read_bytes = 0.0;

int num_send_reply_failed = 0;
int num_socket_send_again = 0;

double num_socket_write_bytes_total = 0.0;

int num_socket_write_calls = 0;
int num_socket_write_eagain = 0;
int num_socket_write_epipe = 0;
int num_socket_write_ebusy = 0;
int num_socket_write_reset = 0;
int num_socket_write_failed_others = 0;
int num_socket_write_zero = 0;
int num_socket_write_successful = 0;
double num_socket_write_bytes = 0.0;


int num_socket_writev_calls = 0;
int num_socket_writev_eagain = 0;
int num_socket_writev_epipe = 0;
int num_socket_writev_reset = 0;
int num_socket_writev_failed_others = 0;
int num_socket_writev_zero = 0;
int num_socket_writev_successful = 0;
double num_socket_writev_bytes = 0.0;

int num_socket_sendfile_calls = 0;
int num_socket_sendfile_eagain = 0;
int num_socket_sendfile_epipe = 0;
int num_socket_sendfile_ebusy = 0;
int num_socket_sendfile_reset = 0;
int num_socket_sendfile_enotconn = 0;
int num_socket_sendfile_failed_others = 0;
int num_socket_sendfile_zero = 0;
int num_socket_sendfile_successful = 0;
double num_socket_sendfile_bytes = 0.0;

int num_socket_corked = 0;
int num_socket_uncorked = 0;

int num_skips = 0;
int num_cache_skips = 0;

int num_close_calls = 0;
int num_close_failed = 0;
int num_close_idle = 0;
int num_close_send_normal = 0;
int num_close_send_early_fin = 0;

int num_sock_close_calls = 0;
int num_sock_close_failed = 0;
int num_sock_close_enotconn = 0;
int num_sock_close_failed_other = 0;

int num_suspends = 0;
int num_interrupts = 0;
int num_sigios = 0;
int num_sigio_races = 0;
int num_sigio_false = 0;
int num_block_sigio = 0;
int num_unblock_sigio = 0;
int num_fd_limits = 0;
int num_new_conns_on = 0;
int num_new_conns_off = 0;
int num_try_accepts = 0;

int num_select_calls = 0;
int num_max_fds_from_select = 0;
int num_total_fds_from_select = 0;
int num_select_successful = 0;
int num_select_eagain = 0;
int num_select_eother = 0;
int num_select_failed_noerror = 0;
int num_select_interrupted = 0;
int num_select_timed_out = 0;

int num_poll_calls = 0;
int num_max_fds_from_poll = 0;
int num_total_fds_from_poll = 0;
int num_poll_successful = 0;
int num_poll_eagain = 0;
int num_poll_eother = 0;
int num_poll_failed_noerror = 0;
int num_poll_interrupted = 0;
int num_poll_timed_out = 0;

int num_epoll_calls = 0;
int num_max_fds_from_epoll = 0;
int num_total_fds_from_epoll = 0;
int num_epoll_successful = 0;
int num_epoll_eagain = 0;
int num_epoll_eother = 0;
int num_epoll_epollerr = 0;
int num_epoll_epollhup = 0;
int num_epoll_failed_noerror = 0;
int num_epoll_interrupted = 0;
int num_epoll_timed_out = 0;
int num_epoll_waits_nothing_to_do = 0;
int num_epoll_ctlv_calls = 0;
int num_max_count_to_epoll_ctlv = 0;
int num_total_count_to_epoll_ctlv = 0;
int num_epoll_ctlv_no_change = 0;
int num_epoll_ctl_calls = 0;

int num_send_handler_calls = 0;
int num_send_handler_interrupts = 0;

int num_reqs_if_modified_since = 0;
int num_reply_http_not_modified = 0;
int num_extra_fstat = 0;
int num_cached_fstat = 0;

int num_cache_add_calls = 0;
int num_cache_hits = 0;
int num_max_open = 0;
double num_total_open = 0.0;
double total_usable_sd = 0.0;
int num_cur_conn_total = 0;
int num_cur_conn_samples = 0;
int num_max_cur_conns = 0;

#ifdef SEND
#ifdef ARRAY_OF_BUFS
int num_events[EVT_BUFS];
#else
int num_events = 0;
#endif /* ARRAY_OF_BUFS */
#endif /* SEND */
int num_pollout_consumed = 0;
int num_pollout_on_closed_consumed = 0;
int num_pollerr_on_closed_consumed = 0;
int num_pollfin_on_closed_consumed = 0;
int num_pollhint_on_closed_consumed = 0;
int num_pollhint_only_consumed = 0;
int num_pollhint_server_consumed = 0;
int num_empty_events_consumed = 0;
int num_pollerr_on_open_socket = 0;
int num_release_queued_evts = 0;

int num_evt_sig = 0;
int num_evt_msg = 0;
int num_evt_ipaccept = 0;
int num_evt_ioready = 0;
int num_evt_dispatch = 0;
int num_evt_synch = 0;
int num_evt_unknown = 0;

int num_aio_dont_wait = 0;
int num_aio_wait_calls = 0;
int num_aio_wait_interrupted = 0;
int num_aio_wait_timed_out = 0;
int num_aio_wait_successful = 0;
int num_total_events_from_aio_wait = 0;
int num_max_events_from_aio_wait = 0;
int num_aio_wait_failed_noerror = 0;
int num_aio_wait_eagain = 0;
int num_aio_wait_eother = 0;

int num_aio_sock_getevents_calls;
int num_aio_sock_getevents_successful;
int num_aio_sock_getevents_failed;
int num_max_events_from_aio_sock_getevents;
int num_total_events_from_aio_sock_getevents;

int num_aio_sock_close_calls = 0;
int num_aio_sock_close_failed = 0;
int num_aio_sock_destroy_failed = 0;

int num_aio_sock_mem_reg_bytes = 0;
int num_aio_sock_mem_dereg_bytes = 0;
int num_aio_sock_mem_reg_bytes_cur = 0;
int num_aio_sock_mem_reg_bytes_max = 0;

int num_socket_setup = 0;
int num_bytes_malloced = 0;

int num_reply_http_not_found_count = 0;
int num_reply_http_bad_request = 0;
int num_reply_http_forbidden = 0;
int num_reply_http_unauthorized = 0;
int num_reply_http_service_unavailable = 0;
int num_reply_http_not_implemented = 0;

int max_dyn_buffers_used = 0;
int dyn_buffers_used = 0;
int dyn_buffers_used_sample_count = 0;
int out_of_buffers_count = 0;
int occ_bufs;
int no_freeable_bufs = 0;

#ifdef FIXHUP
int num_convert_to_hupped = 0;
#endif /* FIXHUP */

#ifdef HAVE_ENCRYPTION
int num_encrypt_tot_reqs = 0;
int num_encrypt_size_ok = 0;
int num_encrypt_encrypted = 0;
long long num_encrypt_bytes = 0;

static void stats_print_encrypt();
#endif /* HAVE_ENCRYPTION */

/* -------------------------------------------------------------------- */
void
stats_print(void)
{
  extern void cache_print_stats();
#ifdef DEBUG_NOT_CLOSED
  extern fd_set *rdfds;
  extern fd_set *wrfds;
#endif

  /* Calculate elapsed time first since later routines use this */
  if( timer_start.tv_sec > 0 && timer_end.tv_sec > 0 ) {
    elapsed_time = (1000000.0 - ((double)timer_start.tv_usec)) / 1000000.0;
    elapsed_time += ((double)(timer_end.tv_sec - (timer_start.tv_sec + 1)));
    elapsed_time += ((double) timer_end.tv_usec) / 1000000.0;    
    printf("-------------------------------------------------------------\n");
    printf("Elapsed time = %12.4f\n", elapsed_time );
  }

  stats_print_accept();
  stats_print_listeners();
  stats_print_read();
  stats_print_write();
  stats_print_close();

  stats_print_select();
  stats_print_poll();
  stats_print_epoll();

#ifdef HAVE_AIO_LAYER
  stats_print_aio_stats();
#endif /* HAVE_AIO_LAYER */

  stats_print_read_file();


#ifdef SEND
  stats_print_send();
#endif /* SEND */
  stats_print_signals();
  stats_print_cache();
  stats_print_ifmodsince();

#ifdef HAVE_NETSTAT
  stats_print_netstats();
#endif /* HAVE_NETSTAT */

  stats_print_rusage();

  stats_print_conns_on_off();

  stats_print_fcgi();

#ifdef HAVE_ENCRYPTION
  stats_print_encrypt();
#endif /* HAVE_ENCRYPTION */

  printf("-------------------------------------------------------------\n");
  printf("Misc stats:\n");
  printf("Total bytes malloced     %12d   %12d MB\n", 
          num_bytes_malloced, num_bytes_malloced / 1024 / 1024);
  printf("Bytes alloc_aligned      %21Zu   %21Zu MB\n", 
          alloced_bytes, alloced_bytes / 1024 / 1024);
  printf("max sd value             %12d\n", max_sd);
  printf("num max open             %12d\n", num_max_open);
  printf("avg num open             %12.1f\n", 
          num_total_open / (double) num_connections);

  printf("max cur_conns            %12d\n", num_max_cur_conns);
  printf("avg cur_conns            %12.1f\n", 
         (double) num_cur_conn_total / num_cur_conn_samples);

  printf("-------------------------------------------------------------\n");
  printf("Current values of some variables:\n");
  printf("max_sd                   %12d\n", max_sd);
  printf("num_idle                 %12d\n", num_idle);
  printf("maxconns-num_idle = open %12d\n", maxconns - num_idle);
  printf("max_opensd               %12d\n", max_opensd);
  printf("cur_conns                %12d\n", cur_conns);
#ifdef FIXHUP
  printf("num_convert_to_hupped    %12d\n", num_convert_to_hupped);
#endif /* FIXHUP */

#ifdef DEBUG_NOT_CLOSED
  if (rdfds) {
    print_fdset("rdfds:\n", rdfds, max_sd);
    print_fdset("wrfds:\n", wrfds, max_sd);
    if (num_idle != maxconns) {
      printf("Doing debug_fdset on rdfds\n");
      debug_fdset(rdfds, max_sd);
      printf("num_idle                 %12d\n", num_idle);
      printf("maxconns-num_idle = open  %12d\n", maxconns - num_idle);
      printf("Num sockets not closed   %12d   %5.1f %%\n", 
	      (num_connections - num_close_calls - num_close_failed),
	      (double) (num_connections - 
	      num_close_calls - num_close_failed) *
	      100.0 / (double) num_connections);
      print_fdset("rdfds:\n", rdfds, max_sd);
      print_fdset("wrfds:\n", wrfds, max_sd);
    }
  }
#endif /* DEBUG_NOT_CLOSED */

  printf("-------------------------------------------------------------\n");
  fflush(stdout);
  fflush(stderr);

} /* stats_print */


/* -------------------------------------------------------------------- */
void
stats_clear(void)
{
  int i;

  timer_start.tv_sec = 0;
  timer_start.tv_usec = 0;
  timer_end.tv_sec = 0;
  timer_end.tv_usec = 0;
  elapsed_time = 0.0;
  
  num_notification_off = 0;
  num_notification_on = 0;
  num_event_calls_successful = 0;
  num_total_fds_from_events = 0;

  num_requests = 0;
  num_replies = 0;
  num_good_gets = 0;
  num_failed_parse = 0;
  num_failed_request = 0;
  num_failed_open = 0;
  num_failed_stat = 0;

  num_accept_calls = 0;
  num_accept_aborts = 0;
  num_accept_eagain = 0;
  num_accept_enfile = 0;
  num_accept_emfile = 0;
  num_accept_successful = 0;
  num_accept_max_reached = 0;
  num_socket_setup_failed = 0;
  num_set_client_sockopts_failed = 0;
  num_sigpipes = 0;
  num_sigurg = 0;
  num_extra_attempts = 0;

  num_do_new_connections_calls = 0;
  num_do_new_connections_no_idle = 0;
  num_do_new_connections_conns_off = 0;
  num_connections = 0;
  num_max_consecutive_accepts = 0;
  num_max_consecutive_requests = 0;
  num_accept_ready_but_no_idle = 0;

  num_socket_read_calls = 0;
  num_socket_read_eagain = 0;
  num_socket_read_eagain2 = 0;
  num_socket_read_retry_failed = 0;
  num_socket_read_nothing_first = 0;
  num_socket_read_nothing_other = 0;
  num_socket_read_failed_others = 0;
  num_socket_read_reset = 0;
  num_socket_read_successful = 0;
  num_socket_read_bytes = 0.0;

  num_file_read_calls = 0;
  num_file_read_successful = 0;
  num_file_read_bytes = 0.0;

  num_send_reply_failed = 0;
  num_socket_send_again = 0;

  num_socket_write_bytes_total = 0.0;
  num_socket_write_calls = 0;
  num_socket_write_eagain = 0;
  num_socket_write_epipe = 0;
  num_socket_write_reset = 0;
  num_socket_write_failed_others = 0;
  num_socket_write_zero = 0;
  num_socket_write_successful = 0;
  num_socket_write_bytes = 0.0;

  num_socket_writev_calls = 0;
  num_socket_writev_eagain = 0;
  num_socket_writev_epipe = 0;
  num_socket_writev_reset = 0;
  num_socket_writev_failed_others = 0;
  num_socket_writev_zero = 0;
  num_socket_writev_successful = 0;
  num_socket_writev_bytes = 0.0;

  num_socket_sendfile_calls = 0;
  num_socket_sendfile_eagain = 0;
  num_socket_sendfile_epipe = 0;
  num_socket_sendfile_ebusy = 0;
  num_socket_sendfile_reset = 0;
  num_socket_sendfile_enotconn = 0;
  num_socket_sendfile_failed_others = 0;
  num_socket_sendfile_zero = 0;
  num_socket_sendfile_successful = 0;
  num_socket_sendfile_bytes = 0.0;

  num_socket_corked = 0;
  num_socket_uncorked = 0;

  num_close_calls = 0;
  num_close_failed = 0;
  num_close_idle = 0;

  num_skips = 0;
  num_cache_skips = 0;

  num_sock_close_calls = 0;
  num_sock_close_failed = 0;
  num_sock_close_enotconn = 0;
  num_sock_close_failed_other = 0;

  num_suspends = 0;
  num_interrupts = 0;
  num_sigios = 0;
  num_sigio_races = 0;
  num_sigio_false = 0;
  num_block_sigio = 0;
  num_unblock_sigio = 0;
  num_fd_limits = 0;
  num_new_conns_on = 0;
  num_new_conns_off = 0;
  num_try_accepts = 0;

  num_select_calls = 0;
  num_select_successful = 0;
  num_select_eagain = 0;
  num_select_eother = 0;
  num_select_failed_noerror = 0;
  num_select_interrupted = 0;
  num_select_timed_out = 0;
  num_max_fds_from_select = 0;
  num_total_fds_from_select = 0;

  num_poll_calls = 0;
  num_poll_successful = 0;
  num_poll_eagain = 0;
  num_poll_eother = 0;
  num_poll_failed_noerror = 0;
  num_poll_interrupted = 0;
  num_poll_timed_out = 0;
  num_max_fds_from_poll = 0;
  num_total_fds_from_poll = 0;

  num_epoll_calls = 0;
  num_epoll_successful = 0;
  num_epoll_eagain = 0;
  num_epoll_eother = 0;
  num_epoll_epollerr = 0;
  num_epoll_epollhup = 0;
  num_epoll_failed_noerror = 0;
  num_epoll_interrupted = 0;
  num_epoll_timed_out = 0;
  num_max_fds_from_epoll = 0;
  num_total_fds_from_epoll = 0;
  num_epoll_waits_nothing_to_do = 0;

  num_epoll_ctlv_calls = 0;
  num_max_count_to_epoll_ctlv = 0;
  num_total_count_to_epoll_ctlv = 0;
  num_epoll_ctlv_no_change = 0;
  num_epoll_ctl_calls = 0;

  num_reqs_if_modified_since = 0;
  num_reply_http_not_modified = 0;
  num_extra_fstat = 0;
  num_cached_fstat = 0;

  num_cache_add_calls = 0;
  num_cache_hits = 0;
  num_max_open = 0;
  num_max_open = 0;
  num_total_open = 0.0;
  total_usable_sd = 0.0;

  for (i=0; i<FROM_MAX; i++) {
    num_do_new_conns_from[i] = 0;
  }

#ifdef SEND
#ifdef ARRAY_OF_BUFS
  for (i=0; i<EVT_BUFS; i++) {
    num_events[i] = 0;
  }
#else
  num_events = 0;
#endif /* ARRAY_OF_BUFS */
#endif /* SEND */
  num_pollout_consumed = 0;
  num_pollout_on_closed_consumed = 0;
  num_pollerr_on_closed_consumed = 0;
  num_pollfin_on_closed_consumed = 0;
  num_pollhint_on_closed_consumed = 0;
  num_pollhint_only_consumed = 0;
  num_pollhint_server_consumed = 0;
  num_empty_events_consumed = 0;
  num_pollerr_on_open_socket = 0;
  num_release_queued_evts = 0;

  num_evt_sig = 0;
  num_evt_msg = 0;
  num_evt_ipaccept = 0;
  num_evt_ioready = 0;
  num_evt_dispatch = 0;
  num_evt_synch = 0;
  num_evt_unknown = 0;


  num_aio_dont_wait = 0;
  num_aio_wait_calls = 0;
  num_aio_wait_interrupted = 0;
  num_aio_wait_timed_out = 0;
  num_aio_wait_successful = 0;
  num_total_events_from_aio_wait = 0;
  num_max_events_from_aio_wait = 0;
  num_aio_wait_failed_noerror = 0;
  num_aio_wait_eagain = 0;
  num_aio_wait_eother = 0;

  num_aio_sock_getevents_calls = 0;
  num_aio_sock_getevents_successful = 0;
  num_aio_sock_getevents_failed = 0;
  num_max_events_from_aio_sock_getevents = 0;
  num_total_events_from_aio_sock_getevents = 0;

  num_aio_sock_close_calls = 0;
  num_aio_sock_close_failed = 0;
  num_aio_sock_destroy_failed = 0;

  num_aio_sock_mem_reg_bytes = 0;
  num_aio_sock_mem_dereg_bytes = 0;
  num_aio_sock_mem_reg_bytes_cur = 0;
  num_aio_sock_mem_reg_bytes_max = 0;

  num_socket_setup = 0;
  num_bytes_malloced = 0;

  num_reply_http_not_found_count = 0;
  num_reply_http_bad_request = 0;
  num_reply_http_forbidden = 0;
  num_reply_http_unauthorized = 0;
  num_reply_http_service_unavailable = 0;
  num_reply_http_not_implemented = 0;

  max_dyn_buffers_used=0;
  dyn_buffers_used=0;
  dyn_buffers_used=0;
	out_of_buffers_count=0;                                                                                                                            
	occ_bufs=0;
	no_freeable_bufs=0;

#ifdef FIXHUP
  num_convert_to_hupped = 0;
#endif 


#ifdef HAVE_ENCRYPTION
  num_encrypt_tot_reqs = 0;
  num_encrypt_size_ok = 0;
  num_encrypt_encrypted = 0;
  num_encrypt_bytes = 0;
#endif /* HAVE_ENCRYPTION */

#ifdef HAVE_AIO_LAYER
  aio_reset_stats(AIO_PARTIAL_RESET);
#endif
  cache_clear_stats();
  netstat_init();
  rusage_init();

  max_sd = min_usable_sd;
} /* stats_clear */

/* -------------------------------------------------------------------- */
void
stats_print_accept(void)
{
  int i;

  printf("-------------------------------------------------------------\n");
  if (options.do_multiaccept == OPT_MULTIACCEPT_BY_SYSCALL) {
    printf("Multiaccept related stats:\n");
  } else {
    printf("Accept related stats:\n");
  }

  if( elapsed_time > 0.0 ) {
    printf("accept-rate (successful)    %12.4f\n", 
	   ((double)(num_accept_successful))/elapsed_time  );
  }

  printf("accept calls             %12d\n",
          num_accept_calls);
  printf("accept would blocks      %12d   %5.1f %%\n",
          num_accept_eagain, 
	  (double) num_accept_eagain * 100.0 / (double) num_accept_calls);
  printf("accept eintr             %12d   %5.1f %%\n",
          num_accept_eintr, 
	  (double) num_accept_eintr * 100.0 / (double) num_accept_calls);
  printf("accept eunknown          %12d   %5.1f %%\n",
          num_accept_eunknown, 
	  (double) num_accept_eunknown * 100.0 / (double) num_accept_calls);
  printf("accept enfile            %12d   %5.1f %%\n",
          num_accept_enfile, 
	  (double) num_accept_enfile * 100.0 / (double) num_accept_calls);
  printf("accept emfile            %12d   %5.1f %%\n",
          num_accept_emfile, 
	  (double) num_accept_emfile * 100.0 / (double) num_accept_calls);
  printf("accept aborts            %12d   %5.1f %%\n",
          num_accept_aborts,
	  (double) num_accept_aborts * 100.0 / (double) num_accept_calls);
  printf("accept successful        %12d   %5.1f %%\n",
          num_accept_successful,
	  (double) num_accept_successful * 100.0 / (double) num_accept_calls);
  printf("accept max reached       %12d   %5.1f %%\n",
          num_accept_max_reached,
	  (double) num_accept_max_reached * 100.0 / 
	  (double) num_accept_successful);
  printf("accept ready but no idle %12d\n",
          num_accept_ready_but_no_idle);
  printf("\n");
  printf("accept num_conns and avg %12d   %5.1f (avg)\n",
          num_connections,
	  (double) num_connections / (double) num_accept_calls);
  printf("calls to do_new_conn     %12d\n",
          num_do_new_connections_calls);
  printf("do_new_conn with no idle %12d   %5.1f %%\n",
          num_do_new_connections_no_idle, 
	  (double) num_do_new_connections_no_idle * 100.0 / 
	  (double) num_do_new_connections_calls);
  printf("do_new_conn - conns off  %12d   %5.1f %%\n",
          num_do_new_connections_conns_off, 
	  (double) num_do_new_connections_conns_off * 100.0 / 
	  (double) num_do_new_connections_calls);
  printf("num extra attempts       %12d\n",
          num_extra_attempts);

  for (i=0; i<FROM_MAX; i++) {
    printf("do_new_conns from [%d]    %12d  %s\n",
	i, num_do_new_conns_from[i], called_from[i]);
  }
  printf("\n");

  printf("Max consecutive accepts  %12d\n",
          num_max_consecutive_accepts);
  printf("Num conns accepted       %12d\n",
          num_accept_successful);
  printf("Num conns completed      %12d\n",
          num_connections);


  printf("\n");
  printf("socket setup             %12d\n",
          num_socket_setup);
  printf("socket setup failed      %12d\n",
          num_socket_setup_failed);
  printf("client sockopts failed   %12d\n",
          num_set_client_sockopts_failed);
  printf("\n");
}


/* -------------------------------------------------------------------- */
void
stats_print_close(void)
{
  int i;
  
  printf("-------------------------------------------------------------\n");
  printf("Close related stats:\n");
  printf("Num idle sockets closed  %12d\n",
          num_close_idle);
  printf("Num sockets close calls  %12d   %5.1f %%\n",
          num_close_calls,
	  (double) num_close_calls * 100.0 / 
	  (double) num_accept_successful);
  printf("Num sockets close failed %12d   %5.1f %%\n",
          num_close_failed,
	  (double) num_close_failed * 100.0 / (double) num_close_calls);
  printf("Num sockets not closed   %12d   %5.1f %%\n", 
          (num_accept_successful - num_close_calls - num_close_failed),
	  (double) (num_accept_successful - num_close_calls - 
		    num_close_failed) * 100.0 / 
	  (double) num_accept_successful);

  printf("\nClosed from state stats:\n");
  for (i=0; i<FSM_STATE_MAX; i++) {
    if (num_closed_state[i] != 0) {
      printf("closed while in state %25s   %12d   %5.1f %%\n", 
              fsm_string[i], num_closed_state[i], 
	      (double) num_closed_state[i] * 100.0 / 
	      (double) num_close_calls);
    }
  }

  if (have_send_events()) {
    printf("\nClose in send related stats:\n");
    printf("Num close normal         %12d   %5.1f %%\n",
	    num_close_send_normal,
	    (double) num_close_send_normal * 100.0 / 
	    (double) num_close_calls);
    printf("Num close early fin      %12d   %5.1f %%\n",
	    num_close_send_early_fin,
	    (double) num_close_send_early_fin * 100.0 / 
	    (double) num_close_calls);
    printf("Num close from err       %12d   %5.1f %%\n",
	    num_pollerr_on_open_socket,
	    (double) num_pollerr_on_open_socket * 100.0 / 
	    (double) num_close_calls);
    printf("\n");
  }


  printf("\n");
  printf("Num sockets close calls        %12d   %5.1f %%\n",
          num_sock_close_calls,
	  (double) num_sock_close_calls * 100.0 / 
	  (double) num_accept_successful);
  printf("Num sockets close failed       %12d   %5.1f %%\n",
          num_sock_close_failed,
	  (double) num_sock_close_failed * 100.0 / 
	  (double) num_sock_close_calls);
  printf("Num sockets close enotconn     %12d   %5.1f %%\n",
          num_sock_close_enotconn,
	  (double) num_sock_close_enotconn * 100.0 / 
	  (double) num_sock_close_calls);
  printf("Num sockets close failed other %12d   %5.1f %%\n",
          num_sock_close_failed_other,
	  (double) num_sock_close_failed_other * 100.0 / 
	  (double) num_sock_close_calls);

#ifdef HAVE_AIO_LAYER
  if (options.use_socket_aio) {
  printf("Num aio sockets close failed   %12d\n",
          num_aio_sock_close_failed);
  printf("Num aio sockets destroy failed %12d\n",
          num_aio_sock_destroy_failed);
  }
#endif
}


/* -------------------------------------------------------------------- */
void
stats_print_signals(void)
{
  if (num_sigios) {
    printf("-------------------------------------------------------------\n");
    printf("Signal related stats:\n");
    printf("sigios                   %12d\n", num_sigios);
    printf("sigio_races              %12d\n", num_sigio_races);
    printf("sigio_false              %12d\n", num_sigio_false);
    printf("sigsuspends              %12d\n", num_suspends);
    printf("num block sigio          %12d\n", num_block_sigio);
    printf("num unblock sigio        %12d\n", num_unblock_sigio);
    if (options.count_sigpipes) {
      printf("num sigpipes             %12d\n", num_sigpipes);
    }
    printf("num sigurg               %12d\n", num_sigurg);
  }
}


/* -------------------------------------------------------------------- */
void
stats_print_read(void)
{
  printf("-------------------------------------------------------------\n");
  printf("Socket reading related stats:\n");
  printf("read calls               %12d\n", num_socket_read_calls);
  printf("read EAGAIN              %12d   %5.1f %%\n", 
          num_socket_read_eagain,
	  (double) num_socket_read_eagain * 100.0 / 
	  (double) num_socket_read_calls);
  printf("read EAGAIN: on 2nd call %12d   %5.1f %%\n", 
          num_socket_read_eagain2,
	  (double) num_socket_read_eagain2 * 100.0 / 
	  (double) num_socket_read_calls);
  printf("read EAGAIN retry failed %12d   %5.1f %%\n", 
          num_socket_read_retry_failed,
	  (double) num_socket_read_retry_failed * 100.0 / 
	  (double) num_socket_read_calls);
  printf("read RESET               %12d   %5.1f %%\n",
          num_socket_read_reset,
	  (double) num_socket_read_reset * 100.0 / 
	  (double) num_socket_read_calls);
  printf("read nothing first       %12d   %5.1f %%\n",
          num_socket_read_nothing_first,
	  (double) num_socket_read_nothing_first * 100.0 / 
	  (double) num_socket_read_calls);
  printf("read nothing other       %12d   %5.1f %%\n",
          num_socket_read_nothing_other,
	  (double) num_socket_read_nothing_other * 100.0 / 
	  (double) num_socket_read_calls);
  printf("read failed others       %12d   %5.1f %%\n",
          num_socket_read_failed_others,
	  (double) num_socket_read_failed_others * 100.0 / 
	  (double) num_socket_read_calls);
  printf("bytes per success read   %12.1f\n",
         (double) num_socket_read_bytes / 
         (double) num_socket_read_successful);
  /* not quite the number of successful reads */
  printf("read successful          %12d   %5.1f %%\n",
         num_socket_read_successful,
         (double) num_socket_read_successful * 100.0 / 
         (double) num_socket_read_calls);
  if( elapsed_time > 0 ) {
    printf("read-rate (successful)  %12.4f\n", (double) num_socket_read_successful / elapsed_time );
  }

  printf("\n");
  printf("good gets                %12d\n", num_good_gets);
  printf("failed parse             %12d\n", num_failed_parse);
  printf("failed request           %12d\n", num_failed_request);
  printf("failed open              %12d\n", num_failed_open);
  printf("failed stat              %12d\n", num_failed_stat);
  printf("requests                 %12d\n", num_requests);
} /* stats_print_read */


/* -------------------------------------------------------------------- */
void
stats_print_read_file(void)
{
  printf("-------------------------------------------------------------\n");
  printf("File reading related stats:\n");
  printf("read calls                    %12d\n", num_file_read_calls);
  printf("bytes per success file read     %12.1f\n",
         (double) num_file_read_bytes / 
         (double) num_file_read_successful);
  /* not quite the number of successful reads */
  printf("read successful file          %12d   %5.1f %%\n",
         num_file_read_successful,
         (double) num_file_read_successful * 100.0 / 
         (double) num_file_read_calls);
  if (elapsed_time > 0) {
    printf("read-rate file (successful) %12.4f\n", (double) num_file_read_successful / elapsed_time );
  }

} /* stats_print_read_file */


/* -------------------------------------------------------------------- */
void
stats_print_write(void)
{
  printf("-------------------------------------------------------------\n");
  printf("Socket writing/replying related stats:\n");

    printf("Total bytes written = %22.0f = %22.2f MB\n",
           (double) num_socket_write_bytes_total, 
           (double) num_socket_write_bytes_total / 1024.0 / 1024.0); 

  if (num_socket_write_calls > 0) {
    printf("write calls              %12d\n", num_socket_write_calls);
    printf("write EAGAIN             %12d   %5.1f %%\n",
           num_socket_write_eagain,
           (double) num_socket_write_eagain * 100.0 / 
           (double) num_socket_write_calls);
    printf("write EPIPE              %12d   %5.1f %%\n",
           num_socket_write_epipe,
           (double) num_socket_write_epipe * 100.0 / 
           (double) num_socket_write_calls);
    printf("write RESET              %12d   %5.1f %%\n",
           num_socket_write_reset,
           (double) num_socket_write_reset * 100.0 / 
           (double) num_socket_write_calls);
    printf("write failed others      %12d   %5.1f %%\n",
           num_socket_write_failed_others,
           (double) num_socket_write_failed_others * 100.0 / 
           (double) num_socket_write_calls);
    printf("write zero bytes         %12d   %5.1f %%\n",
           num_socket_write_zero,
           (double) num_socket_write_zero * 100.0 / 
           (double) num_socket_write_calls);
    printf("bytes per success write  %12.1f\n",
           (double) num_socket_write_bytes / 
           (double) num_socket_write_successful);
    printf("write successful         %12d   %5.1f %%\n",
           num_socket_write_successful,
           (double) num_socket_write_successful * 100.0 / 
           (double) num_socket_write_calls);
    if( elapsed_time > 0 ) {
      printf("write-rate (successful)  %12.4f\n", (double)num_socket_write_successful / elapsed_time   );
    }
    printf("\n");
  }

  if (num_socket_writev_calls > 0) {
    printf("writev calls             %12d\n", num_socket_writev_calls);
    printf("writev EAGAIN            %12d   %5.1f %%\n",
	    num_socket_writev_eagain,
	    (double) num_socket_writev_eagain * 100.0 / 
	    (double) num_socket_writev_calls);
    printf("writev EPIPE             %12d   %5.1f %%\n",
	    num_socket_writev_epipe,
	    (double) num_socket_writev_epipe * 100.0 / 
	    (double) num_socket_writev_calls);
    printf("writev RESET             %12d   %5.1f %%\n",
	    num_socket_writev_reset,
	    (double) num_socket_writev_reset * 100.0 / 
	    (double) num_socket_writev_calls);
    printf("writev failed others     %12d   %5.1f %%\n",
	    num_socket_writev_failed_others,
	    (double) num_socket_writev_failed_others * 100.0 / 
	    (double) num_socket_writev_calls);
    printf("writev zero bytes        %12d   %5.1f %%\n",
	    num_socket_writev_zero,
	    (double) num_socket_writev_zero * 100.0 / 
	    (double) num_socket_writev_calls);
    printf("writev successful        %12d   %5.1f %%\n",
	    num_socket_writev_successful,
	    (double) num_socket_writev_successful * 100.0 / 
	    (double) num_socket_writev_calls);
    printf("bytes per success writev %12.1f\n",
	    (double) num_socket_writev_bytes / 
	    (double) num_socket_writev_successful);
  }

  if (num_socket_sendfile_calls > 0) {
    printf("sendfile calls             %12d\n", 
	    num_socket_sendfile_calls);
    printf("sendfile EAGAIN            %12d   %5.1f %%\n",
	    num_socket_sendfile_eagain,
	    (double) num_socket_sendfile_eagain * 100.0 / 
	    (double) num_socket_sendfile_calls);
    printf("sendfile EPIPE             %12d   %5.1f %%\n",
	    num_socket_sendfile_epipe,
	    (double) num_socket_sendfile_epipe * 100.0 / 
	    (double) num_socket_sendfile_calls);
    printf("sendfile EBUSY             %12d   %5.1f %%\n",
	    num_socket_sendfile_ebusy,
	    (double) num_socket_sendfile_ebusy * 100.0 / 
	    (double) num_socket_sendfile_calls);
    printf("sendfile RESET             %12d   %5.1f %%\n",
	    num_socket_sendfile_reset,
	    (double) num_socket_sendfile_reset * 100.0 / 
	    (double) num_socket_sendfile_calls);
    printf("sendfile ENOTCONN          %12d   %5.1f %%\n",
	    num_socket_sendfile_enotconn,
	    (double) num_socket_sendfile_enotconn * 100.0 / 
	    (double) num_socket_sendfile_calls);
    printf("sendfile failed others     %12d   %5.1f %%\n",
	    num_socket_sendfile_failed_others,
	    (double) num_socket_sendfile_failed_others * 100.0 / 
	    (double) num_socket_sendfile_calls);
    printf("sendfile zero bytes        %12d   %5.1f %%\n",
	    num_socket_sendfile_zero,
	    (double) num_socket_sendfile_zero * 100.0 / 
	    (double) num_socket_sendfile_calls);
    printf("sendfile successful        %12d   %5.1f %%\n",
	    num_socket_sendfile_successful,
	    (double) num_socket_sendfile_successful * 100.0 / 
	    (double) num_socket_sendfile_calls);
    printf("bytes per success sendfile %12.1f\n",
	    (double) num_socket_sendfile_bytes / 
	    (double) num_socket_sendfile_successful);

    printf("socket cork calls          %12d\n", num_socket_corked);
    printf("socket uncork calls        %12d\n", num_socket_uncorked);
  }


  printf("\n");
  printf("socket send reply failed %12d\n", num_send_reply_failed);
  printf("send agains needed       %12d\n", num_socket_send_again);
  printf("replies                  %12d\n", num_replies);
  if ( elapsed_time > 0 ) {
    printf("reply-rate (successful)  %12.4f\n", (double) num_replies / (double) elapsed_time );
  }
  if ( num_accept_successful > 0 ) {
    printf("replies/accepts          %12.4f\n", 
           (double) num_replies / (double) num_accept_successful);
    printf("request/conn             %12.4f\n", 
           (double) num_good_gets / (double) num_accept_successful);
  }

  printf("------------\n");
  printf("Count of HTTP_NOT_FOUND            %12d\n", num_reply_http_not_found_count);
  printf("Count of HTTP_BAD_REQUEST          %12d\n", num_reply_http_bad_request);
  printf("Count of HTTP_FORBIDDEN            %12d\n", num_reply_http_forbidden);
  printf("Count of HTTP_UNAUTHORIZED         %12d\n", num_reply_http_unauthorized);
  printf("Count of HTTP_SERVICE_UNAVAILABLE  %12d\n", num_reply_http_service_unavailable);
  printf("Count of HTTP_NOT_IMPLEMENTED      %12d\n", num_reply_http_not_implemented);

  printf("\n");
  printf("Count of total victim skips        %12d\n", num_skips);
  printf("Count of total cache  skips        %12d\n", num_cache_skips);

}

/* -------------------------------------------------------------------- */
void
stats_print_select(void)
{
  if (num_select_calls) {
    printf("-------------------------------------------------------------\n");
    printf("Select related stats:\n");
    printf("select calls             %12d\n", num_select_calls);
    printf("select EINTR             %12d   %5.1f %%\n", 
	    num_select_interrupted,
	    (double) num_select_interrupted * 100.0 / 
	    (double) num_select_calls);
    printf("select EAGAIN            %12d   %5.1f %%\n",
	    num_select_eagain,
	    (double) num_select_eagain * 100.0 / 
	    (double) num_select_calls);
    printf("select other errors      %12d   %5.1f %%\n",
	    num_select_eother,
	    (double) num_select_eother * 100.0 / 
	    (double) num_select_calls);
    printf("select failed no errors  %12d   %5.1f %%\n",
	    num_select_failed_noerror,
	    (double) num_select_failed_noerror * 100.0 / 
	    (double) num_select_calls);
    printf("select timed out         %12d   %5.1f %%\n",
	    num_select_timed_out,
	    (double) num_select_timed_out * 100.0 / 
	    (double) num_select_calls);
    printf("select successful        %12d   %5.1f %%\n",
	    num_select_successful,
	    (double) num_select_successful * 100.0 / 
	    (double) num_select_calls);
    if( elapsed_time > 0 ) {
      printf("select-rate (successful)  %12.4f\n", (double) num_select_successful / elapsed_time );
    }
    printf("\n");
    printf("max num fds from select  %12d\n", num_max_fds_from_select);
    printf("total fds from select    %12d\n", num_total_fds_from_select);
    printf("avg fds from select        %12.1f\n", 
	    (double) num_total_fds_from_select / 
	    (double) num_select_successful);
    printf("avg max sd into select     %12.1f\n", 
	    (double) total_usable_sd / (double) num_select_calls);
  }
}



/* -------------------------------------------------------------------- */
void
stats_print_poll(void)
{
  if (num_poll_calls) {
    printf("-------------------------------------------------------------\n");
    printf("poll related stats:\n");
    printf("poll calls             %12d\n", num_poll_calls);
    printf("poll EINTR             %12d   %5.1f %%\n", 
	    num_poll_interrupted,
	    (double) num_poll_interrupted * 100.0 / 
	    (double) num_poll_calls);
    printf("poll EAGAIN            %12d   %5.1f %%\n",
	    num_poll_eagain,
	    (double) num_poll_eagain * 100.0 / 
	    (double) num_poll_calls);
    printf("poll other errors      %12d   %5.1f %%\n",
	    num_poll_eother,
	    (double) num_poll_eother * 100.0 / 
	    (double) num_poll_calls);
    printf("poll failed no errors  %12d   %5.1f %%\n",
	    num_poll_failed_noerror,
	    (double) num_poll_failed_noerror * 100.0 / 
	    (double) num_poll_calls);
    printf("poll timed out         %12d   %5.1f %%\n",
	    num_poll_timed_out,
	    (double) num_poll_timed_out * 100.0 / 
	    (double) num_poll_calls);
    printf("poll successful        %12d   %5.1f %%\n",
	    num_poll_successful,
	    (double) num_poll_successful * 100.0 / 
	    (double) num_poll_calls);

    printf("\n");
    printf("max num fds from poll  %12d\n", num_max_fds_from_poll);
    printf("total fds from poll    %12d\n", num_total_fds_from_poll);
    printf("avg fds from poll        %12.1f\n", 
	    (double) num_total_fds_from_poll / 
	    (double) num_poll_successful);
    printf("avg max sd into poll     %12.1f\n", 
	    (double) total_usable_sd / (double) num_poll_calls);
  }
}


/* -------------------------------------------------------------------- */
void
stats_print_epoll(void)
{
  if (num_epoll_calls) {
    printf("-------------------------------------------------------------\n");
    printf("epoll related stats:\n");
    printf("epoll_wait calls        %12d\n", num_epoll_calls);
    printf("epoll_wait nothing todo %12d   %5.1f %%\n", 
            num_epoll_waits_nothing_to_do,
	    (double) num_epoll_waits_nothing_to_do * 100.0 / 
	    (double) num_epoll_calls);
    printf("epoll EINTR             %12d   %5.1f %%\n", 
	    num_epoll_interrupted,
	    (double) num_epoll_interrupted * 100.0 / 
	    (double) num_epoll_calls);
    printf("epoll EAGAIN            %12d   %5.1f %%\n",
	    num_epoll_eagain,
	    (double) num_epoll_eagain * 100.0 / 
	    (double) num_epoll_calls);
    printf("epoll EPOLLHUP           %12d   %5.1f %%\n",
            num_epoll_epollhup,
            (double) num_epoll_epollhup * 100.0 /
            (double) num_epoll_calls);
    printf("epoll EPOLLERR           %12d   %5.1f %%\n",
            num_epoll_epollerr,
            (double) num_epoll_epollerr * 100.0 /
            (double) num_epoll_calls);
    printf("Note: EPOLLERR and EPOLLHUP often occur together\n");
    printf("epoll other errors      %12d   %5.1f %%\n",
	    num_epoll_eother,
	    (double) num_epoll_eother * 100.0 / 
	    (double) num_epoll_calls);
    printf("epoll failed no errors  %12d   %5.1f %%\n",
	    num_epoll_failed_noerror,
	    (double) num_epoll_failed_noerror * 100.0 / 
	    (double) num_epoll_calls);
    printf("epoll timed out         %12d   %5.1f %%\n",
	    num_epoll_timed_out,
	    (double) num_epoll_timed_out * 100.0 / 
	    (double) num_epoll_calls);
    printf("epoll successful        %12d   %5.1f %%\n",
	    num_epoll_successful,
	    (double) num_epoll_successful * 100.0 / 
	    (double) num_epoll_calls);

    printf("\n");
    printf("max num fds from epoll_wait  %12d\n", num_max_fds_from_epoll);
    printf("total fds from epoll_wait    %12d\n", num_total_fds_from_epoll);
    printf("avg fds from epoll_wait      %12.1f\n", 
	    (double) num_total_fds_from_epoll / 
	    (double) num_epoll_successful);
    /* TODO 
    printf("avg max sd into epoll_wait   %12.1f\n", 
	    (double) total_usable_sd / (double) num_epoll_calls);
    */

    printf("\n");
    printf("epoll_ctl  calls             %12d\n", num_epoll_ctl_calls);

    printf("\n");
    printf("epoll_ctlv calls             %12d\n", num_epoll_ctlv_calls);
    printf("max num fds to epoll_ctlv    %12d\n", num_max_count_to_epoll_ctlv);
    printf("total fds to epoll_ctlv      %12d\n", num_total_count_to_epoll_ctlv);
    printf("avg fds to epoll_ctlv        %12.3f\n", 
	    (double) num_total_count_to_epoll_ctlv / 
	    (double) num_epoll_ctlv_calls);
    printf("epoll_ctlv_no_change         %12d\n", num_epoll_ctlv_no_change);

  }
}


#ifdef HAVE_AIO_LAYER
void
stats_print_aio_stats()
{
  stats_print_aio_wait();
  stats_print_aio_sock_getevents();
}

/* -------------------------------------------------------------------- */
void
stats_print_aio_wait(void)
{
  if (num_aio_dont_wait) {
    printf("-------------------------------------------------------------\n");
    printf("aio_wait not used simply looping instead\n");
    printf("aio_dont_wait count =      %12d\n", num_aio_dont_wait);
  }

  if (num_aio_wait_calls) {
    printf("-------------------------------------------------------------\n");
    printf("aio_wait related stats:\n");
    printf("aio_wait calls             %12d\n", num_aio_wait_calls);
    printf("aio_wait EINTR             %12d   %5.1f %%\n", 
	    num_aio_wait_interrupted,
	    (double) num_aio_wait_interrupted * 100.0 / 
	    (double) num_aio_wait_calls);
    printf("aio_wait EAGAIN            %12d   %5.1f %%\n",
	    num_aio_wait_eagain,
	    (double) num_aio_wait_eagain * 100.0 / 
	    (double) num_aio_wait_calls);
    printf("aio_wait other errors      %12d   %5.1f %%\n",
	    num_aio_wait_eother,
	    (double) num_aio_wait_eother * 100.0 / 
	    (double) num_aio_wait_calls);
    printf("aio_wait failed no errors  %12d   %5.1f %%\n",
	    num_aio_wait_failed_noerror,
	    (double) num_aio_wait_failed_noerror * 100.0 / 
	    (double) num_aio_wait_calls);
    printf("aio_wait timed out         %12d   %5.1f %%\n",
	    num_aio_wait_timed_out,
	    (double) num_aio_wait_timed_out * 100.0 / 
	    (double) num_aio_wait_calls);
    printf("aio_wait successful        %12d   %5.1f %%\n",
	    num_aio_wait_successful,
	    (double) num_aio_wait_successful * 100.0 / 
	    (double) num_aio_wait_calls);
    if( elapsed_time > 0 ) {
      printf("aio_wait_rate (successful)  %12.4f\n", (double) num_aio_wait_successful / elapsed_time );
    }
    printf("\n");
    printf("max num events from aio_wait  %12d\n", 
	num_max_events_from_aio_wait);
    printf("total events from aio_wait    %12d\n",
	num_total_events_from_aio_wait);
    printf("avg events from aio_wait      %12.1f\n", 
	    (double) num_total_events_from_aio_wait / 
	    (double) num_aio_wait_successful);
    /* Not used / doesn't make sense here
    printf("avg max sd into aio_wait     %12.1f\n", 
	    (double) total_usable_sd / (double) num_aio_wait_calls);
    */

  }
}


/* -------------------------------------------------------------------- */
void
stats_print_aio_sock_mem_reg(void)
{
  if (num_aio_sock_mem_reg_bytes) {
    printf("-------------------------------------------------------------\n");
    printf("aio_sock_mem_reg_bytes        %12d\n",
	num_aio_sock_mem_reg_bytes);
    printf("aio_sock_mem_dereg_bytes      %12d\n",
	num_aio_sock_mem_dereg_bytes);
    printf("aio_sock_mem_reg_bytes_cur    %12d\n",
	num_aio_sock_mem_reg_bytes_cur);
    printf("aio_sock_mem_reg_bytes_max    %12d\n",
	num_aio_sock_mem_reg_bytes_max);
  }
}

/* -------------------------------------------------------------------- */
void
stats_print_aio_sock_getevents(void)
{
  extern void aio_sock_print_purged();
  if (num_aio_sock_getevents_calls) {
    printf("-------------------------------------------------------------\n");
    printf("aio_sock_getevents related stats:\n");
    printf("aio_sock_getevents calls                %12d\n", num_aio_sock_getevents_calls);
    printf("aio_sock_getevents failed               %12d   %5.1f %%\n",
	    num_aio_sock_getevents_failed,
	    (double) num_aio_sock_getevents_failed * 100.0 / 
	    (double) num_aio_sock_getevents_calls);
    printf("aio_sock_getevents successful           %12d   %5.1f %%\n",
	    num_aio_sock_getevents_successful,
	    (double) num_aio_sock_getevents_successful * 100.0 / 
	    (double) num_aio_sock_getevents_calls);
    if (elapsed_time > 0) {
      printf("aio_sock_getevents_rate (successful)  %12.4f\n", 
         (double) num_aio_sock_getevents_successful / elapsed_time );
    }

    printf("\n");
    printf("max num events from aio_sock_getevents  %12d\n", 
	num_max_events_from_aio_sock_getevents);
    printf("total events from aio_sock_getevents    %12d\n",
	num_total_events_from_aio_sock_getevents);
    printf("avg events from aio_sock_getevents      %12.1f\n", 
	    (double) num_total_events_from_aio_sock_getevents / 
	    (double) num_aio_sock_getevents_successful);


    aio_sock_print_purged();
  }
}


/* -------------------------------------------------------------------- */
void
stats_reg_bytes(int size)
{
    num_aio_sock_mem_reg_bytes += size;
    num_aio_sock_mem_reg_bytes_cur += size;
    if (num_aio_sock_mem_reg_bytes_cur > num_aio_sock_mem_reg_bytes_max) {
      num_aio_sock_mem_reg_bytes_max = num_aio_sock_mem_reg_bytes_cur;
    }
}

/* -------------------------------------------------------------------- */
void
stats_dereg_bytes(int size)
{
    num_aio_sock_mem_dereg_bytes += size;
    num_aio_sock_mem_reg_bytes_cur -= size;
    PRINT_TIME(NOFD, &tnow, &tprev, "stats_dereg_bytes: "
	"num_aio_sock_mem_reg_bytes = %d", num_aio_sock_mem_reg_bytes_cur);
}
#endif /* HAVE_AIO_LAYER */

/* -------------------------------------------------------------------- */
#ifdef SEND
void
stats_print_send(void)
{
  unsigned int tmp;
  double total_events = 0.0;
#ifdef ARRAY_OF_BUFS
  int i;
#endif /* ARRAY_OF_BUFS */

  if (have_send_events()) {
    printf("-------------------------------------------------------------\n");
    printf("SEND related stats:\n");
    printf("send_handler calls          %12d\n", 
	num_send_handler_calls);
    printf("send_handler_interrupts     %12d\n", 
	num_send_handler_interrupts);
    printf("\n");

#ifdef ARRAY_OF_BUFS
    for (i=0; i<EVT_BUFS; i++) {
      printf("send event count buf %2d     %12d\n", i, num_events[i]);
      total_events += (double) num_events[i];
    }
#else
    printf("send event count            %12d\n", num_events);
    total_events = (double) num_events;
#endif /* ARRAY_OF_BUFS */
    printf("\n");


    printf("send evt sig                %12d\n", num_evt_sig);
    printf("send evt msg                %12d\n", num_evt_msg);
    printf("send evt ipaccept           %12d\n", num_evt_ipaccept);
    printf("send evt ioready            %12d\n", num_evt_ioready);
    printf("send evt dispatch           %12d\n", num_evt_dispatch);
    printf("send evt synch              %12d\n", num_evt_synch);
    printf("send evt unknown            %12d\n", num_evt_unknown);
    printf("\n");

    tmp = 0;
    printf("pollhint_server_consumed    %12d   %5.1f %%\n",
            num_pollhint_server_consumed,
	    (double) num_pollhint_server_consumed * 100.0 / 
	    total_events);
    tmp += num_pollhint_server_consumed;

    printf("pollhint_only_consumed      %12d   %5.1f %%\n",
           num_pollhint_only_consumed,
	   (double) num_pollhint_only_consumed * 100.0 / 
	   total_events);
    tmp += num_pollhint_only_consumed;

    printf("pollhint_on_close_consumed  %12d   %5.1f %%\n",
            num_pollhint_on_closed_consumed,
	    (double) num_pollhint_on_closed_consumed * 100.0 / 
	    total_events);
    tmp += num_pollhint_on_closed_consumed;

    printf("pollerr_only_consumed       %12d   %5.1f %%\n",
            num_pollerr_on_closed_consumed,
	    (double) num_pollerr_on_closed_consumed * 100.0 / 
	    total_events);
    tmp += num_pollerr_on_closed_consumed;

    printf("pollerr_on_open_socket      %12d   %5.1f %%\n",
            num_pollerr_on_open_socket,
	    (double) num_pollerr_on_open_socket * 100.0 / 
	    total_events);
    tmp += num_pollerr_on_open_socket;

    printf("pollfin_on_closed_consumed  %12d   %5.1f %%\n",
            num_pollfin_on_closed_consumed,
	    (double) num_pollfin_on_closed_consumed * 100.0 / 
	    total_events);
    tmp += num_pollfin_on_closed_consumed;

    printf("pollout_consumed            %12d   %5.1f %%\n",
            num_pollout_consumed,
	    (double) num_pollout_consumed * 100.0 / 
	    total_events);
    tmp += num_pollout_consumed;

    printf("pollout_on_closed_consumed  %12d   %5.1f %%\n",
            num_pollout_on_closed_consumed,
	    (double) num_pollout_on_closed_consumed * 100.0 / 
	    total_events);
    tmp += num_pollout_on_closed_consumed;

    printf("empty_consumed              %12d   %5.1f %%\n",
            num_empty_events_consumed,
	    (double) num_empty_events_consumed * 100.0 / 
	    total_events);
    tmp += num_empty_events_consumed;

    printf("Total consumed              %12d   %5.1f %%\n",
           tmp,
	   (double) tmp * 100.0 / 
	   total_events);

  }
}
#endif /* SEND */


/* -------------------------------------------------------------------- */
void
stats_print_cache(void)
{
  printf("-------------------------------------------------------------\n");
  if (options.caching_on) {
    printf("Cache related stats:\n");
    cache_print_stats();
  } else {
    printf("Caching not turned on\n");
  }
}


void stats_print_ifmodsince() {
  double percent;

  printf("-------------------------------------------------------------\n");
  printf("If-modified-since stats:\n\n");
  printf("If-modified-since requests:%12d\n", num_reqs_if_modified_since);
  percent = (double) num_reply_http_not_modified / (double) num_reqs_if_modified_since * 100.0;
  printf("Number of HTTP 304 replies:%12d  %5.1f %%\n",
          num_reply_http_not_modified, percent);
  percent = (double) num_cached_fstat / (double) num_reqs_if_modified_since * 100.0;
  printf("fstat() caching hit rate:                %5.1f %%\n", percent);
  percent = (double) num_extra_fstat / (double) num_reqs_if_modified_since * 100.0;
  printf("fstat() caching miss rate:               %5.1f %%\n", percent);
}


/* -------------------------------------------------------------------- */
void
stats_print_conns_on_off(void)
{
#ifdef SEND
#ifdef ARRAY_OF_BUFS
  int i;
#endif /* ARRAY_OF_BUFS */
#endif /* SEND */
  printf("-------------------------------------------------------------\n");
  printf("Connections on and off stats:\n");
  printf("num new conns on         %12d\n", num_new_conns_on);
  printf("num new conns off        %12d\n", num_new_conns_off);
  printf("num try accepts          %12d\n", num_try_accepts);
#ifdef SEND
#ifdef ARRAY_OF_BUFS
  if (ecb) {
    for (i=0; i<EVT_BUFS; i++) {
      if (num_events[i] != 0) {
        printf("num release queued evts  %12d\n", 
	    num_release_queued_evts);
	break;
      }
    }
  }
#else
  if (num_events != 0) {
    printf("num release queued evts  %12d\n", 
	num_release_queued_evts);
  }
#endif /* ARRAY_OF_BUFS */
#endif /* SEND */
}

/* -------------------------------------------------------------------- */
void
stats_print_fcgi(void)
{
  int num_fcgi_servers;
  app_stats_t appstats;			// aggregate app stats
  appserver_stats_t appserverstats;	// aggregate appserver stats
  app_t *app;
  appserver_t *appserver;
  int i;

  num_fcgi_servers = 0;
  memset(&appstats, '\0', sizeof(appstats));
  memset(&appserverstats, '\0', sizeof(appserverstats));

  for (i = 0; i < options.app_count; i++) {
    app = &options.app[i];
    if (app->req_type != REQ_FASTCGI) {
      continue;
    }
    memset(&app->serverstats, '\0', sizeof(app->serverstats));
    appserver = app->server;
    assert(appserver != NULL);

    do {
      num_fcgi_servers++;
      /* update aggregate appserver stats for this app */
      app->serverstats.num_serviced += appserver->stats.num_serviced;
      app->serverstats.num_successful_reads += appserver->stats.num_successful_reads;
      app->serverstats.num_successful_data_reads += appserver->stats.num_successful_data_reads;
      app->serverstats.total_bytes_read += appserver->stats.total_bytes_read;
      app->serverstats.total_data_bytes_read += appserver->stats.total_data_bytes_read;
      app->serverstats.num_successful_writes += appserver->stats.num_successful_writes;
      app->serverstats.total_bytes_written += appserver->stats.total_bytes_written;
      appserver = appserver->next;
    } while (appserver != NULL);

    /* update aggregate app and appserver stats */
    appstats.num_requests += app->stats.num_requests;
    appstats.num_queued += app->stats.num_queued;
    appstats.total_req_queue_size += app->stats.total_req_queue_size;
    appstats.max_req_queue_size += app->stats.max_req_queue_size;
    appserverstats.num_serviced += app->serverstats.num_serviced;
    appserverstats.num_successful_reads += app->serverstats.num_successful_reads;
    appserverstats.num_successful_data_reads += app->serverstats.num_successful_data_reads;
    appserverstats.total_bytes_read += app->serverstats.total_bytes_read;
    appserverstats.total_data_bytes_read += app->serverstats.total_data_bytes_read;
    appserverstats.num_successful_writes += app->serverstats.num_successful_writes;
    appserverstats.total_bytes_written += app->serverstats.total_bytes_written;
  }

  if (num_fcgi_servers > 0) {
    printf("-------------------------------------------------------------\n");
    printf("Aggregate FastCGI stats:\n");
    printf(" requests                              %19d\n",
	appstats.num_requests);
    printf(" serviced                              %19d\n",
	appserverstats.num_serviced);
    printf(" queued                                %19d\n",
	appstats.num_queued);
    printf(" avg queue size                        %19.2f\n",
	(double) appstats.total_req_queue_size / appstats.num_requests);
    printf(" max queue size                        %19d\n",
	appstats.max_req_queue_size);
    printf(" num successful reads                  %19d\n",
	appserverstats.num_successful_reads);
    printf(" total bytes read                      %19Ld\n",
	appserverstats.total_bytes_read);
    printf(" avg bytes read                        %19.2f\n",
	(double) appserverstats.total_bytes_read / appserverstats.num_successful_reads);
    printf(" num successful data reads             %19d\n",
	appserverstats.num_successful_data_reads);
    printf(" total data bytes read                 %19Ld\n",
	appserverstats.total_data_bytes_read);
    printf(" avg data bytes read                   %19.2f\n",
	(double) appserverstats.total_data_bytes_read / appserverstats.num_successful_data_reads);
    printf(" num successful writes                 %19d\n",
	appserverstats.num_successful_writes);
    printf(" total bytes written                   %19Ld\n",
	appserverstats.total_bytes_written);
    printf(" avg bytes written                     %19.2f\n",
	(double) appserverstats.total_bytes_written / appserverstats.num_successful_writes);

    printf(" max dynamic buffers used		   %19d\n",
	max_dyn_buffers_used);
    printf(" avg dynamic buffers used		   %19.2f\n",
	(double) dyn_buffers_used/dyn_buffers_used_sample_count);
		printf(" times ran out of dynamic buffers			%19d\n", out_of_buffers_count);
		printf(" times couldn't free a dynamic buffer     %19d\n", no_freeable_bufs);

    if (num_fcgi_servers > 1) {
      for (i = 0; i < options.app_count; i++) {
	app = &options.app[i];
	if (app->req_type != REQ_FASTCGI) {
	  continue;
	}
	printf("\nApp '%s' stats:\n", app->uri);
	printf(" requests                              %19d\n",
	    app->stats.num_requests);
	printf(" serviced                              %19d\n",
	    app->serverstats.num_serviced);
	appserver = app->server;
	if (appserver->next != NULL) {
	  do {
	    printf("  by %-43s%10d\n",
		appserver->path, appserver->stats.num_serviced);
	    appserver = appserver->next;
	  } while (appserver != NULL);
	}
	printf(" queued                                %19d\n",
	    app->stats.num_queued);
	printf(" avg queue size                        %19.2f\n",
	    (double) app->stats.total_req_queue_size / app->stats.num_requests);
	printf(" max queue size                        %19d\n",
	    app->stats.max_req_queue_size);
	printf(" num successful reads                  %19d\n",
	    app->serverstats.num_successful_reads);
	appserver = app->server;
	if (appserver->next != NULL) {
	  do {
	    printf("  from %-41s%10d\n",
		appserver->path, appserver->stats.num_successful_reads);
	    appserver = appserver->next;
	  } while (appserver != NULL);
	}
	printf(" total bytes read                      %19Ld\n",
	    app->serverstats.total_bytes_read);
	appserver = app->server;
	if (appserver->next != NULL) {
	  do {
	    printf("  from %-32s%19Ld\n",
		appserver->path, appserver->stats.total_bytes_read);
	    appserver = appserver->next;
	  } while (appserver != NULL);
	}
	printf(" avg bytes read                        %19.2f\n",
	    (double) app->serverstats.total_bytes_read / app->serverstats.num_successful_reads);
	appserver = app->server;
	if (appserver->next != NULL) {
	  do {
	    printf("  from %-32s%19.2f\n",
		appserver->path, (double) appserver->stats.total_bytes_read / appserver->stats.num_successful_reads);
	    appserver = appserver->next;
	  } while (appserver != NULL);
	}
	printf(" num successful data reads             %19d\n",
	    app->serverstats.num_successful_data_reads);
	appserver = app->server;
	if (appserver->next != NULL) {
	  do {
	    printf("  from %-41s%10d\n",
		appserver->path, appserver->stats.num_successful_data_reads);
	    appserver = appserver->next;
	  } while (appserver != NULL);
	}
	printf(" total data bytes read                 %19Ld\n",
	    app->serverstats.total_data_bytes_read);
	appserver = app->server;
	if (appserver->next != NULL) {
	  do {
	    printf("  from %-32s%19Ld\n",
		appserver->path, appserver->stats.total_data_bytes_read);
	    appserver = appserver->next;
	  } while (appserver != NULL);
	}
	printf(" avg data bytes read                   %19.2f\n",
	    (double) app->serverstats.total_data_bytes_read / app->serverstats.num_successful_data_reads);
	appserver = app->server;
	if (appserver->next != NULL) {
	  do {
	    printf("  from %-32s%19.2f\n",
		appserver->path, (double) appserver->stats.total_data_bytes_read / appserver->stats.num_successful_data_reads);
	    appserver = appserver->next;
	  } while (appserver != NULL);
	}
	printf(" num successful writes                 %19d\n",
	    app->serverstats.num_successful_writes);
	appserver = app->server;
	if (appserver->next != NULL) {
	  do {
	    printf("  to %-43s%10d\n",
		appserver->path, appserver->stats.num_successful_writes);
	    appserver = appserver->next;
	  } while (appserver != NULL);
	}
	printf(" total bytes written                   %19Ld\n",
	    app->serverstats.total_bytes_written);
	appserver = app->server;
	if (appserver->next != NULL) {
	  do {
	    printf("  to %-34s%19Ld\n",
		appserver->path, appserver->stats.total_bytes_written);
	    appserver = appserver->next;
	  } while (appserver != NULL);
	}
	printf(" avg bytes written                     %19.2f\n",
	    (double) app->serverstats.total_bytes_written / app->serverstats.num_successful_writes);
	appserver = app->server;
	if (appserver->next != NULL) {
	  do {
	    printf("  to %-34s%19.2f\n",
		appserver->path, (double) appserver->stats.total_bytes_written / appserver->stats.num_successful_writes);
	    appserver = appserver->next;
	  } while (appserver != NULL);
	}
      }
    }
  }
}
/* -------------------------------------------------------------------- */
#ifdef HAVE_ENCRYPTION
static void
stats_print_encrypt()
{
  double dv1, dv2;

  printf("-------------------------------------------------------------\n");
  printf("Encryption stats:\n");
  printf("total candidates         %12d\n", num_encrypt_tot_reqs);
  printf("candidates not too large %12d\n", num_encrypt_size_ok);
  printf("requests encrypted       %12d\n", num_encrypt_encrypted);
  printf("total bytes encrypted    %12lld\n", num_encrypt_bytes);
  dv1 = (num_encrypt_tot_reqs == 0) ?
     0.0 : ((double)num_encrypt_encrypted/(double)num_encrypt_tot_reqs)*100.0;
  dv2 = (num_encrypt_size_ok == 0) ?
     0.0 : ((double)num_encrypt_encrypted/(double)num_encrypt_size_ok)*100.0;
  printf("percentage encrypted     %15.2f %%\n", dv1);
  printf("percentage of size ok    %15.2f %%\n", dv2);
}
#endif /* HAVE_ENCRYPTION */

/* -------------------------------------------------------------------- */
int
have_send_events(void)
{
  int ret_val = 0;
#ifdef SEND
#ifdef ARRAY_OF_BUFS
  int i;
  for (i=0; i<EVT_BUFS; i++) {
    if (num_events[i] != 0) {
      ret_val = 1;
      break;
    }
  }
#else
  ret_val = num_events;
#endif /* ARRAY_OF_BUFS */
#endif /* SEND */
  return(ret_val);
}

/* -------------------------------------------------------------------- */
void
stats_print_netstats(void)
{
  netstat_print();
}

/* -------------------------------------------------------------------- */
void 
stats_print_rusage(void)
{
  rusage_print();
}

/* -------------------------------------------------------------------- */
void 
num_closed_from_state(int state)
{
  num_closed_state[state]++;
}

/* ----------------------------------------------------------------------  */
struct print_info {
  char *hdrtop;      /* top line of the header */
  char *hdrbot;      /* bottom line of the header */
  int *ivalue;       /* if it's an integer value pointer to the int */
  double *dvalue;    /* if it's a double value pointer to the double */
  char *hdrfmt;      /* fmt used for printing header */
  char *valfmt;      /* fmt used for printing the value (this and above
			should be the same width */
  char *description; /* description of what each column means */
};


void
print_description(struct print_info *pi)
{
  int i=0;
  printf("\n");
  printf("Running stats information\n");
  while (pi[i].description != 0) {
    printf("  ");
    printf("%-8s", pi[i].hdrtop);
    printf("\n");
    printf("  ");
    printf("%-8s", pi[i].hdrbot);
    printf("  : %s\n", pi[i].description);
    printf("%-8s\n", "--------");
    i++;
  }
  printf("\n");
}

void
print_header(struct print_info *pi)
{
  int i;

  /* print the top row of the header */
  i = 0;
  while (pi[i].hdrtop != 0) {
    printf(pi[i].hdrfmt, pi[i].hdrtop);
    i++;
  }
  printf("\n");

  /* print the bottom row of the header */
  i = 0;
  while (pi[i].hdrbot != 0) {
    printf(pi[i].hdrfmt, pi[i].hdrbot);
    i++;
  }
  printf("\n");
}

void
print_data(struct print_info *pi)
{
  int i = 0;
  while (pi[i].hdrfmt != 0) {
    if (pi[i].ivalue != 0) {
      printf(pi[i].valfmt, *(pi[i].ivalue));
    } else {
      printf(pi[i].valfmt, *(pi[i].dvalue));
    }

    i++;
  }
  printf("\n");
}

/* -------------------------------------------------------------------- */

static double accepts_per_sec = 0.0;
static double requests_per_sec = 0.0;
static double replies_per_sec = 0.0;
static double read_first_fails_per_sec = 0.0;
static double get_event_calls_per_sec = 0.0;
static double new_events_per_sec = 0.0;
static double events_per_event_call_per_sec = 0.0;
static double accepts_minus_replies_per_sec = 0.0;
static double requests_minus_replies_per_sec = 0.0;
static double Mbps = 0.0;
static int pending = 0;
static int cur_usec_interval = 0;
static int cur_sec_interval = 0;
static int buffers_used=0;

extern int n_total;
extern cacheStats stats;

struct print_info running_stats[] = 
{
/* hdrtop, hdrbot, addr if int,  addr if double,          hdr fmt, data fmt */
  {"acc/", "sec",     0, &accepts_per_sec,               "%8s",  "%8.1f",
   "Accepts per second" 
  },
  {"req/", "sec",     0, &requests_per_sec,               "%8s",  "%8.1f",
   "Requests per second" 
  },
  {"rep/", "sec",     0, &replies_per_sec,                "%8s",  "%8.1f",
   "Replies per second" 
  },
  {"req-", "rep/s",   0, &requests_minus_replies_per_sec, "%10s", "%10.1f",
   "Requests minus replies per second"
  },
  {"acc-", "rep/s",   0, &accepts_minus_replies_per_sec, "%10s", "%10.1f",
   "Accepts minus replies per second"
  },
  {"1read", "fail/s", 0, &read_first_fails_per_sec,      "%10s", "%10.1f",
   "Times First call to read (1r) fails per second (client closed before read)"
  },
  {"ecalls", "sec",   0, &get_event_calls_per_sec,        "%9s",  "%9.1f",
   "Get event calls per second (select/poll/epoll/aio_wait ..."
  },
  {"evts/", "sec",     0, &new_events_per_sec,             "%12s",  "%12.1f",
   "Events per second returned by event calls"
  },
  {"evts/", "call/s", 0, &events_per_event_call_per_sec,  "%9s",  "%9.1f",
   "Events returned per event call per second"
  },
  {"Mbits/", "sec",   0, &Mbps,                           "%7s",  "%7.1f",
   "Bandwidth being written per second in megabits per second"
  },
  {"cur", "ws",    0,  &stats.workingSetCur,                "%12s",  "%12.0lf",
   "Cur working set size",
  },
  {"avg", "ws",    0,  &stats.workingSetAvg,                "%12s",  "%12.0lf",
   "Avg working set size",
  },
  {"Pend", "conns",   &pending, 0,                           "%6s",  "%6d",
   "Current number of pending connections (value at the time stat is printed)"
  },
  {"Cur", "conns",    &cur_conns, 0,                         "%6s",  "%6d",
   "Current number of active connections (value at the time stat is printed)"
  },
  {"usec", "itvl",    &cur_usec_interval, 0,                  "%10s",  "%10d",
   "Number of usecs between print intervals"
  },
	{"bufs", "used",    &buffers_used, 0,                       "%6s", "%6d",
   "Number of buffers used for dynamic requests"},
/*
  {"Cur", "conns",    &cur_conns, 0,                         "%6s",  "%6d",
   "Current number of active connections (value at the time stat is printed"
  },
  {"sec", "itvl",    &cur_sec_interval, 0,                    "%10s",  "%10d",
   "Number of secs between print intervals"
  },
  {"aio", "evts",    &n_total, 0,                    "%10s",  "%10d",
   "Number of current events from aio"
  },
*/
  {0, 0, 0, 0, 0, 0}
};



void 
print_running_stats(struct timeval *time_now)
{
  static struct timeval last_time = {0, 0};
  static double interval_usecs = 0.0;
  static double interval_secs = 0.0;
  static double actual_interval_secs = 0.0;
  static int count = 0;
  static int last_num_replies = 0;
  static int last_num_good_gets = 0; /* requests */
  static int last_num_accept_successful = 0;
  static int last_num_event_calls_successful = 0;
  static int last_num_total_fds_from_events = 0;
  static int last_num_socket_read_nothing_first = 0;
  static double last_num_socket_write_bytes_total = 0.0;
  static double adjusted_interval_usecs = 0.0;

  struct timeval tv;
  double usecs = 0.0;

  /*
  printf("print_running_stats: options.stats_interval = %d\n", 
      options.stats_interval);
  */

  if (options.stats_interval == 0) {
    return;
  } else if (interval_usecs == 0.0) {
    interval_usecs = options.stats_interval * USECS_PER_SEC;
    interval_secs = options.stats_interval;
    adjusted_interval_usecs = interval_usecs - 500.0;
  }


  if (count == 0) {
    usecs = interval_usecs;
  } else {
    timersub(time_now, &last_time, &tv);
    usecs = tv.tv_sec * USECS_PER_SEC + tv.tv_usec;
  }
  cur_usec_interval = (int) usecs;

  
  /*
  printf("print_running_stats: usecs = %20.0f interval_usecs = %20.0f\n",
      usecs, interval_usecs);
  */

  if (usecs >= adjusted_interval_usecs) {

    if (count == 0) {
      print_description(running_stats);
    }

    /* print the heading */
    if (count % 20 == 0) {
      print_header(running_stats);
    }

    actual_interval_secs = (usecs / USECS_PER_SEC);
    cur_sec_interval = (int) actual_interval_secs;

    replies_per_sec = (double) (num_replies - last_num_replies) /
        actual_interval_secs;

    requests_per_sec = (double) (num_good_gets - last_num_good_gets) /
        actual_interval_secs;

    accepts_per_sec = (double) (num_accept_successful - 
	last_num_accept_successful) / actual_interval_secs;

    accepts_minus_replies_per_sec = accepts_per_sec - replies_per_sec;

    requests_minus_replies_per_sec = requests_per_sec - replies_per_sec;

    read_first_fails_per_sec = (double) (num_socket_read_nothing_first - 
	last_num_socket_read_nothing_first) / actual_interval_secs;

    get_event_calls_per_sec = (double) (num_event_calls_successful - 
	last_num_event_calls_successful) / actual_interval_secs;

    new_events_per_sec = (double) (num_total_fds_from_events - 
	last_num_total_fds_from_events) / actual_interval_secs;

    Mbps = (double) ((num_socket_write_bytes_total - 
	last_num_socket_write_bytes_total) * 8.0 / 1000000.0) / 
        actual_interval_secs;

		buffers_used = occ_bufs;

    if (get_event_calls_per_sec == 0.0) {
      events_per_event_call_per_sec = 0.0;
    } else {
       events_per_event_call_per_sec = new_events_per_sec / 
         get_event_calls_per_sec;
    }

    pending = (maxconns - num_idle) - cur_conns;

    print_data(running_stats);

    count++;
    last_num_replies = num_replies;
    last_num_good_gets = num_good_gets;
    last_num_event_calls_successful = num_event_calls_successful;
    last_num_total_fds_from_events = num_total_fds_from_events;
    last_num_accept_successful = num_accept_successful;
    last_num_socket_write_bytes_total = num_socket_write_bytes_total;
    last_num_socket_read_nothing_first = num_socket_read_nothing_first;
    last_time = *time_now;
  }
} /* print_running_stats */

/* -------------------------------------------------------------------- */
