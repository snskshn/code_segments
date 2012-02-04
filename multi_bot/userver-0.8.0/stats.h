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


#ifndef STATS_H
#define STATS_H

#ifdef SEND
#ifdef ARRAY_OF_BUFS
#include "send.h"
#endif /* ARRAY_OF_BUFS */
#endif /* SEND */

#include "do_new_connections.h"

/* --- STATS ----------------------------------------------------------- */

extern struct timeval timer_start;
extern struct timeval timer_end;
extern double elapsed_time;

extern int num_do_new_conns_from[];
extern int num_notification_off;
extern int num_notification_on;

extern int num_event_calls_successful;
extern int num_total_fds_from_events;

extern int num_requests;
extern int num_replies;

extern int num_do_new_connections_calls;
extern int num_do_new_connections_no_idle;
extern int num_do_new_connections_conns_off;
extern int num_accept_calls;
extern int num_accept_aborts;
extern int num_accept_eagain;
extern int num_accept_enfile;
extern int num_accept_eintr;
extern int num_accept_eunknown;
extern int num_accept_emfile;
extern int num_accept_successful;
extern int num_accept_max_reached;
extern int num_accept_ready_but_no_idle;
extern int num_connections;
extern int num_max_consecutive_accepts;
extern int num_max_consecutive_requests;
extern int num_socket_setup_failed;
extern int num_set_client_sockopts_failed;
extern int num_sigpipes;
extern int num_sigurg;
extern int num_extra_attempts;

extern int num_socket_read_calls;
extern int num_socket_read_eagain;
extern int num_socket_read_eagain2;
extern int num_socket_read_retry_failed;
extern int num_socket_read_nothing_first;
extern int num_socket_read_nothing_other;
extern int num_socket_read_failed_others;
extern int num_socket_read_reset;
extern int num_socket_read_successful;
extern double num_socket_read_bytes;


extern int num_file_read_calls;
extern int num_file_read_successful;
extern double num_file_read_bytes;

extern int num_send_reply_failed;
extern int num_socket_send_again;

extern double num_socket_write_bytes_total;

extern int num_socket_write_calls;
extern int num_socket_write_eagain;
extern int num_socket_write_epipe;
extern int num_socket_write_reset;
extern int num_socket_write_failed_others;
extern int num_socket_write_zero;
extern int num_socket_write_successful;
extern double num_socket_write_bytes;

extern int num_socket_writev_calls;
extern int num_socket_writev_eagain;
extern int num_socket_writev_epipe;
extern int num_socket_writev_reset;
extern int num_socket_writev_failed_others;
extern int num_socket_writev_zero;
extern int num_socket_writev_successful;
extern double num_socket_writev_bytes;

extern int num_socket_sendfile_calls;
extern int num_socket_sendfile_eagain;
extern int num_socket_sendfile_epipe;
extern int num_socket_sendfile_ebusy;
extern int num_socket_sendfile_reset;
extern int num_socket_sendfile_enotconn;
extern int num_socket_sendfile_failed_others;
extern int num_socket_sendfile_zero;
extern int num_socket_sendfile_successful;
extern double num_socket_sendfile_bytes;

extern int num_socket_corked;
extern int num_socket_uncorked;

extern int num_close_calls;
extern int num_close_failed;
extern int num_close_idle;
extern int num_close_send_normal;
extern int num_close_send_early_fin;

extern int num_interrupts;
extern int num_good_gets;
extern int num_failed_parse;
extern int num_failed_request;
extern int num_failed_open;
extern int num_failed_stat;
extern int num_fd_limits;

extern int num_sigios;
extern int num_suspends;
extern int num_sigio_races;
extern int num_sigio_false;
extern int num_block_sigio;
extern int num_unblock_sigio;
extern int num_new_conns_on;
extern int num_new_conns_off;

extern int num_max_fds_from_select;
extern int num_total_fds_from_select;
extern int num_select_calls;
extern int num_select_successful;
extern int num_select_eagain;
extern int num_select_eother;
extern int num_select_failed_noerror;
extern int num_select_timed_out;
extern int num_select_interrupted;

extern int num_max_fds_from_poll;
extern int num_total_fds_from_poll;
extern int num_poll_calls;
extern int num_poll_successful;
extern int num_poll_eagain;
extern int num_poll_eother;
extern int num_poll_failed_noerror;
extern int num_poll_timed_out;
extern int num_poll_interrupted;

extern int num_max_fds_from_epoll;
extern int num_total_fds_from_epoll;
extern int num_epoll_calls;
extern int num_epoll_successful;
extern int num_epoll_eagain;
extern int num_epoll_eother;
extern int num_epoll_epollerr;
extern int num_epoll_epollhup;
extern int num_epoll_failed_noerror;
extern int num_epoll_timed_out;
extern int num_epoll_interrupted;
extern int num_epoll_waits_nothing_to_do;

extern int num_epoll_ctlv_calls;
extern int num_max_count_to_epoll_ctlv;
extern int num_total_count_to_epoll_ctlv;
extern int num_epoll_ctlv_no_change;

extern int num_epoll_ctl_calls;

extern int num_cache_add_calls;
extern int num_cache_hits;

extern int num_send_handler_calls;
extern int num_send_handler_interrupts;

extern int num_max_open;
extern double num_total_open;
extern double total_usable_sd;

extern int num_cur_conn_total;
extern int num_cur_conn_samples;
extern int num_max_cur_conns;

#ifdef SEND
#ifdef ARRAY_OF_BUFS
extern int num_events[EVT_BUFS];
#else
extern int num_events;
#endif /* ARRAY_OF_BUFS */
#endif /* SEND */
extern int num_evt_sig;
extern int num_evt_msg;
extern int num_evt_ipaccept;
extern int num_evt_ioready;
extern int num_evt_dispatch;
extern int num_evt_synch;
extern int num_evt_unknown;
extern int num_pollout_consumed;
extern int num_pollout_on_closed_consumed;
extern int num_pollerr_on_closed_consumed;
extern int num_pollfin_on_closed_consumed;
extern int num_pollhint_on_closed_consumed;
extern int num_pollhint_only_consumed;
extern int num_pollhint_server_consumed;
extern int num_empty_events_consumed;
extern int num_pollerr_on_open_socket;
extern int num_release_queued_evts;

extern int num_aio_wait_calls;
extern int num_aio_wait_interrupted;
extern int num_aio_wait_timed_out;
extern int num_aio_wait_successful;
extern int num_total_events_from_aio_wait;
extern int num_max_events_from_aio_wait;
extern int num_aio_wait_failed_noerror;
extern int num_aio_wait_eagain;
extern int num_aio_wait_eother;
extern int num_aio_dont_wait;

extern int num_aio_sock_getevents_calls;
extern int num_aio_sock_getevents_successful;
extern int num_aio_sock_getevents_failed;
extern int num_max_events_from_aio_sock_getevents;
extern int num_total_events_from_aio_sock_getevents;

extern int num_aio_sock_close_calls;
extern int num_aio_sock_close_failed;
extern int num_aio_sock_destroy_failed;

extern int num_sock_close_calls;
extern int num_sock_close_failed;
extern int num_sock_close_enotconn;
extern int num_sock_close_failed_other;

extern int num_reqs_if_modified_since;    /* Number of if-modified-since requests */
extern int num_reply_http_not_modified;   /* Number of HTTP 304 replies generated */
extern int num_extra_fstat;               /* Number of extra fstat calls required to process if-mod-since*/
extern int num_cached_fstat;              /* Number of times cached fstat data used to process if-mod-since */

extern int num_reply_http_not_found_count;
extern int num_reply_http_bad_request;
extern int num_reply_http_forbidden;
extern int num_reply_http_unauthorized;
extern int num_reply_http_service_unavailable;
extern int num_reply_http_not_implemented;

extern int num_skips;
extern int num_cache_skips;

extern int num_fcgi_requests;
extern int num_fcgi_serviced;
extern int num_fcgi_queued;
extern int total_fcgi_req_queue_size;
extern int max_fcgi_req_queue_size;
extern int max_dyn_buffers_used;
extern int num_socket_setup;
extern int num_bytes_malloced;
extern int dyn_buffers_used;
extern int dyn_buffers_used_sample_count;
extern int out_of_buffers_count;
extern int occ_bufs;
extern int no_freeable_bufs;


#ifdef FIXHUP
extern int num_convert_to_hupped;
#endif


#ifdef HAVE_ENCRYPTION
extern int num_encrypt_tot_reqs;
extern int num_encrypt_size_ok;
extern int num_encrypt_encrypted;
extern long long num_encrypt_bytes;
#endif /* HAVE_ENCRYPTION */

void stats_print(void);
void stats_clear(void);
void num_closed_from_state(int state);
void print_running_stats(struct timeval *time_now);
void stats_reg_bytes(int size);
void stats_dereg_bytes(int size);

#endif /* STATS_H */
