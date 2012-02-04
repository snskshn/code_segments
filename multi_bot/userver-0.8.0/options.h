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


#ifndef OPTIONS_H
#define OPTIONS_H

#ifdef HAVE_CPUSET
#include <sched.h>
#endif
#include "app.h"

#include <limits.h>
#include <stdio.h>

#define OPT_MAX_IP_ADDRS                    (20)
#define OPT_MAX_IP_ADDR_LEN                 (50)
#define OPT_IP_ADDR_ANY                     "0.0.0.0"
#define OPT_IP_ADDR_LOOPBACK                "127.0.0.1"
#define OPT_IP_ADDR_BROADCAST               "255.255.255.255"

#define OPT_MAX_HOSTNAME_LEN                (100)

/* Number of seconds to use as timeout value to select/poll/aio_wait/etc */
#define OPT_SELECT_TIMEOUT_DEFAULT          (60*60)

/* Amount of time a connection doesn't do anything before
 * it's considered idle (in seconds)
 */
#define OPT_IDLE_THRESHOLD_DEFAULT          (15)

#define OPT_CONN_WITH_SELECT_POLL_EPOLL     (0)  /* Inform of new connrequests through select */
#define OPT_CONN_WITH_SIGIO                 (1)  /* sigio for notification */
#define OPT_CONN_WITH_SEND_EVTS             (2)  /* send for new connections */
#define OPT_CONN_WITH_SEND_SELECT           (3)  /* Use send except for new connections */
#define OPT_CONN_WITH_AIO_ACCEPT            (4)  /* aio accept for new conns */

#define OPT_PROCESS_SDS_UP                  (0)
#define OPT_PROCESS_SDS_DOWN                (1)
#define OPT_PROCESS_SDS_WRITES_FIRST_DOWN   (2)
#define OPT_PROCESS_SDS_WRITES_FIRST_UP     (3)
#define OPT_PROCESS_SDS_LRU                 (4)
#define OPT_PROCESS_SDS_LIFO                (5)
#define OPT_PROCESS_SDS_FIFO                (6)

#define OPT_MULTIACCEPT_MAX_UNLIMITED       (-1)
#define OPT_MULTIACCEPT_OFF                 (0)
#define OPT_MULTIACCEPT_BY_REPEATING        (1)
#define OPT_MULTIACCEPT_BY_SYSCALL          (2)

#define OPT_SIGIO_USE_FCNTL                 (0)
#define OPT_SIGIO_USE_PROCMASK              (1)

#define OPT_DEFAULT_MAX_FDS                 (FD_SETSIZE-1)
#define OPT_DEFAULT_MAX_CONNS               ((FD_SETSIZE-20) / 2)
/* leave a bit of room */

#define OPT_DEFAULT_PORT                    (6800)
#ifdef HAVE_SSL
#define OPT_DEFAULT_SSL_PORT                (6443)
#endif

#define OPT_USE_DEFAULT_SOCK_BUF_SIZE       (-1)
#define OPT_USE_DEFAULT_SOCK_LOWAT          (-1)

#define OPT_DEFAULT_ECB_SIZE                (-1)
#define OPT_DEFAULT_ECB_THRESHOLD_LOW       (-2)
#define OPT_DEFAULT_ECB_THRESHOLD_HIGH      (-1)

#define OPT_DEFAULT_READ_BUFFER_SIZE        (512)
#define OPT_DEFAULT_REPLY_BUFFER_SIZE       (1024)

#define OPT_DEFAULT_DYN_BUFFER_SIZE         (1*1024*1024)
#define OPT_DEFAULT_NUM_DYN_BUFFERS         (-1)
#define OPT_DEFAULT_NUM_DYN_BUFFERS_PER_APP (-1)
#define OPT_DEFAULT_NUM_DYN_BUFFERS_PER_APPSERVER (2)

#define MB                                  (1024*1024)

#define OPT_MAX_FILENAME_LEN                (200)
#define OPT_DEFAULT_PID_FILENAME            "userver.pid"
#define OPT_DEFAULT_TRACE_FILENAME          "userver.trace"

#define OPT_MAX_PATH_LEN                    (200)

/* Different types of servers */
#define OPT_ACTING_AS_HTTPD                  (1)
#define OPT_ACTING_AS_BITTORRENT             (2)

/* Should be more larger than the maximum number of files in the cache */
#define OPT_DEFAULT_CACHE_TABLE_SIZE        (100) 
/* This is the maximum number of bytes permitted in the cache */
#define OPT_DEFAULT_CACHE_MAX_BYTES         ((double) (20 * MB))
/* Files larger than this don't get cached */
#define OPT_DEFAULT_CACHE_MAX_FILE_SIZE     ((double) (1 * MB)) 
/* Hash table for cache not allowed to get more that this % full */
#define OPT_DEFAULT_CACHE_MAX_LOAD_FACTOR   (2.0)

#define OPT_DEFAULT_CPU_MASK                ((unsigned long) 0xffffffff)

#define OPT_AIO_LOWEST_SD                   (7)
#define OPT_AIO_MAX_LISTENERS               (10)
#define OPT_CACHE_LINE_EST                  (128)

#define OPT_AIO_MEM_REG_ALIGN               (128)

#define OPT_AIO_READ_COMPL_EVTS                ('a')
#define OPT_AIO_WRITE_COMPL_EVTS               ('w')
#define OPT_AIO_ACCEPT_COMPL_EVTS              ('r')
#define OPT_AIO_ALL_COMPL_EVTS                 ('A')

#define OPT_AIO_COMPL_ORDER_LEN                (64)

/* Handle write completions then reads, then accepts */
#define OPT_AIO_GOOD_COMPLETION_ORDER          "wra"
#define OPT_AIO_TEST_COMPLETION_ORDER          "rwa"
#define OPT_AIO_SAFE_COMPLETION_ORDER          "arw"
#define OPT_AIO_EXAMPLE_COMPLETION_ORDER       "wrwarw"
#define OPT_AIO_ONE_QUEUE_COMPLETION_ORDER     "A"

/* NOTE: if you change the number you need to change the order */
#define OPT_AIO_DEFAULT_COMPL_QUEUES           3
#define OPT_AIO_DEFAULT_COMPLETION_ORDER       OPT_AIO_GOOD_COMPLETION_ORDER

#define OPT_NO_SIZE_LIMIT                      (-1.0)

#define OPT_SERVER_PRIORITY_NO_CHANGE          (INT_MIN)
#define OPT_SERVER_NICE_LEVEL_NO_CHANGE        (INT_MIN)

#define OPT_NO_STATE_DELAY                     (-1)

#define OPT_SKIP_DO_MADVISE                    (-1)
#define OPT_SKIP_MADVISE_SIZE                  (100 * 1024)


struct opts {
    int is_cfg_filename;
    int free_fd_thold;
    int get_connections_on_close;
    int call_readable_from_new_conn;
    int loop_on_readable;
    int call_writable_from_readable;
    int do_multiaccept;
    int use_accept_send;
    int multiaccept_max;
    int get_connections;
    int conn_on;
    int conn_off;
    int get_connections_in_server_loop;
    int suspend_on_idle;		/* FIXME: This doesn't seem to be used for anything. */
    int process_sds_order;
    int delay;
    int state_delay;
    int listenq;
    int read_sock_buf_size;
    int write_sock_buf_size;
    int read_sock_buf_lowat;
    int write_sock_buf_lowat;
    int use_max_open_sd;
    int count_sigpipes;
    int extra_freq;
    int dont_intr_select;
    int close_with_reset;
    int accepts_only;
    int port;
#ifdef HAVE_SSL
    int ssl_port;
#endif
    int use_memcpy;
    int interactive;
    int sigio_cntl;
    int send_events;
    int select_timeout;
    int idle_threshold;
    int send_loop;
    int use_poll;
    int use_epoll;
    int use_epoll2; /* variation of epoll that tries to reduce epoll_ctl calls
                       * possibly at the expense of more events returned that
         * aren't of interest currently.
         */
    int use_epoll_ctlv; /* collect changes and use new syscall epoll_ctlv to
              * do the changes. ? does this work with epoll and epoll2
             */
    int epoll_edge_triggered;
    int epoll_modify_listeners;
    int asynch_mode;
    int send_io_events;
    int auto_accept;
    int auto_accept_aperture;
    int send_kdebug;
    int send_block_for_events;
    int ecb_threshold_low;
    int ecb_threshold_high;
    int ecb_size;
    int kinfo;
    int noaccepts;
    int close_after_sock_init;
    int close_after_read;
    int close_after_parse;
    int close_idle_conns;
    int send_polls_for_accepts;
    int caching_on;
    int cache_table_size;
    int cache_max_bytes_dummy;
    double cache_max_bytes;
    int cache_max_file_size_dummy;
    double cache_max_file_size;
    int cache_max_load_factor_dummy;
    double cache_max_load_factor;
    int cache_lock_pages;
    int cache_table_print;
    int cache_for_spec;
    int use_madvise;
    int use_sendfile;
    int blocking_sendfile;
    int send_selects_for_accepts;
    int rejection_rate;		/* FIXME: This doesn't seem to be used for anything. */
    int use_getpid;
    int trace_summary;
    int trace_summary_only;
    int use_tcp_cork;
    int content_type;
    /* Enough room to store
     * 192.168.10.20:65000
     * or
     * hostname.domain:port
     */
    int is_ip_addr;
    char ip_addr[OPT_MAX_IP_ADDRS][OPT_MAX_IP_ADDR_LEN];
    int ip_addr_count;
    /* used as the address/flag for getopt_long (value is meaningless
     * and is destroyed by getopt_long call.
     */
    int use_aio_accept;
    int use_aio_read;
    int use_aio_write;
    int use_aio_close;
    int use_socket_aio;
    int use_aio_wait;
    int aio_write_events_limit;
    int aio_read_events_limit;
    int aio_accept_events_limit;
    int aio_accept_thold;
    int aio_read_before_accept;
    int aio_read_before_write;
    int aio_dont_wait;
    int aio_dont_close;
    int aio_initial_prepost_only;
    int read_buffer_size;
    int reply_buffer_size;
    int dyn_buffer_size;
    int num_dyn_buffers;
    int num_dyn_buffers_per_app;
    int num_dyn_buffers_per_appserver;
    int dyn_lock_pages;
    int dyn_touch_pages;
    int dyn_use_shared_mem;
    int is_pid_filename;
    char pid_filename[OPT_MAX_FILENAME_LEN];
    int is_trace_filename;
    char trace_filename[OPT_MAX_FILENAME_LEN];
    int acting_as;
    int stats_interval;
    int ignore_fd_setsize;
    int cpu_mask_dummy;
    int appserv_cpu_mask_dummy;
#ifdef HAVE_CPUSET
    cpu_set_t cpu_mask;
    cpu_set_t appserv_cpu_mask;
#else
    unsigned long cpu_mask;
    unsigned long appserv_cpu_mask;
#endif
    int is_doc_root;
    char doc_root[OPT_MAX_PATH_LEN];
    int doc_root_len;
    int is_hostname;
    char hostname[OPT_MAX_HOSTNAME_LEN];
    int is_app;
    app_t app[MAX_APPS];
    int app_count;
    int appserver_count;
    int app_req_queue_size;
    int is_start_app_server;
    appserver_startinfo_t *appserver_startinfo;
    int appserver_startinfo_count;
    int no_exit_on_no_buffers;

    int cache_warm;
    int cache_warm_size;
    int cache_warm_stride;
    int cache_warm_loops;
    char cache_warm_file[OPT_MAX_FILENAME_LEN];

    int victim;
    char victim_str[OPT_MAX_FILENAME_LEN];
    int victim_skip;
    int cache_miss_skip;
#ifdef HAVE_ENCRYPTION
    int encrypt_data;
    int encrypt_percent;
    unsigned long encrypt_max_file_size;
#endif /* HAVE_ENCRYPTION */

    /* so we can tell if someone is specifying an order */
    int is_aio_completion_order;    
    char aio_completion_order[OPT_AIO_COMPL_ORDER_LEN];

    /* permit the specification of a min and/or a maximum file size
     * to server -- any files not in this range are not served
     */
    int min_file_size_dummy;
    double min_file_size;
    int max_file_size_dummy;
    double max_file_size;
    int skip_header;
    int aio_complq_count;
    int use_logging;

    int server_priority;
    int server_nice_level;

    int server_id;
#ifdef CALL_STATS
    int call_stats;
#endif /* CALL_STATS */
    int fake_writing;
};

extern struct opts options;

void options_set();
void options_print();
void options_print_xml(FILE*);
void options_valid();

#endif /* OPTIONS_H */
