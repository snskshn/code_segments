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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "util.h"
#include "debug.h"
#include "app.h"
#include "inet.h"
#include "options.h"

struct opts options;

void
options_set(void)
{
  const char *ptr;
#ifdef HAVE_CPUSET
  int i;
#endif

  /*
   * To start with, zero out all of the options.
   * Technically, there's no point in zeroing any individual fields after
   * this, but we do.  -CAB
   */
  memset(&options, '\0', sizeof(options));

  options.free_fd_thold = 0;
  options.get_connections_on_close = 0;
  options.do_multiaccept = OPT_MULTIACCEPT_BY_REPEATING;
  options.use_accept_send = 0;
  options.multiaccept_max = OPT_MULTIACCEPT_MAX_UNLIMITED;
  options.get_connections = OPT_CONN_WITH_SELECT_POLL_EPOLL;
  options.conn_on = OPT_CONN_WITH_SELECT_POLL_EPOLL;
  options.conn_off = OPT_CONN_WITH_SELECT_POLL_EPOLL;
  options.call_readable_from_new_conn = 0;
  options.loop_on_readable = 0;
  options.call_writable_from_readable = 0;
  options.get_connections_in_server_loop = 0;
  options.suspend_on_idle = 0;
  options.process_sds_order = OPT_PROCESS_SDS_DOWN;
  options.delay = 0;
  options.state_delay = OPT_NO_STATE_DELAY;
  options.listenq = 128;
  options.read_sock_buf_size = OPT_USE_DEFAULT_SOCK_BUF_SIZE;
  options.write_sock_buf_size = OPT_USE_DEFAULT_SOCK_BUF_SIZE;
  options.read_sock_buf_lowat = OPT_USE_DEFAULT_SOCK_LOWAT;
  options.write_sock_buf_lowat = OPT_USE_DEFAULT_SOCK_LOWAT;
  options.use_max_open_sd = 0;
  options.count_sigpipes = 0;
  options.extra_freq = INT_MAX;
  options.dont_intr_select = 0;
  options.close_with_reset = 0;
  options.accepts_only = 0;
  options.port = OPT_DEFAULT_PORT;
#ifdef HAVE_SSL
  options.ssl_port = OPT_DEFAULT_SSL_PORT;
#endif /* HAVE_SSL */
  options.use_memcpy = 0;
  options.interactive = 0;
  options.sigio_cntl = OPT_SIGIO_USE_FCNTL;
  options.send_events = 0;
  options.select_timeout = OPT_SELECT_TIMEOUT_DEFAULT;
  options.idle_threshold = OPT_IDLE_THRESHOLD_DEFAULT;
  options.send_loop = 0;
  options.use_poll = 0;
  options.use_epoll = 0;
  options.use_epoll2 = 0;
  options.use_epoll_ctlv = 0;
  options.epoll_edge_triggered = 0;
  options.epoll_modify_listeners = 0;
  options.asynch_mode = 0;
  options.send_io_events = 0;
  options.auto_accept = 0;
  options.auto_accept_aperture = 0;
  options.send_kdebug = 0;
  options.send_block_for_events = 0;
  options.ecb_threshold_low = OPT_DEFAULT_ECB_THRESHOLD_LOW;
  options.ecb_threshold_high = OPT_DEFAULT_ECB_THRESHOLD_HIGH;
  options.ecb_size = OPT_DEFAULT_ECB_SIZE;
  options.kinfo = 0;
  options.noaccepts = 0;
  options.close_after_sock_init = 0;
  options.close_after_read = 0;
  options.close_after_parse = 0;
  options.close_idle_conns = 0;
  options.send_polls_for_accepts = 0;
  options.caching_on = 0;
  options.cache_table_size = OPT_DEFAULT_CACHE_TABLE_SIZE;
  options.cache_max_bytes = OPT_DEFAULT_CACHE_MAX_BYTES;
  options.cache_max_file_size = OPT_DEFAULT_CACHE_MAX_FILE_SIZE;
  options.cache_max_load_factor = (double) OPT_DEFAULT_CACHE_MAX_LOAD_FACTOR;
  options.cache_lock_pages = 0;
  options.cache_table_print = 0;
  options.cache_for_spec = 0;
#if defined(HAVE_SENDFILE) && defined(USE_SENDFILE)
  options.use_sendfile = 1;
  options.blocking_sendfile = 0;
#ifdef __linux__
  options.use_tcp_cork = 1;
#else
  options.use_tcp_cork = 0;
#endif /* __linux */
#else
  options.use_sendfile = 0;
  options.blocking_sendfile = 0;
  options.use_tcp_cork = 0;
#endif /* HAVE_SENDFILE && USE_SENDFILE */
  options.use_madvise = 0;
  options.send_selects_for_accepts = 0;
  options.rejection_rate = 0;
  options.trace_summary = 0;
  options.trace_summary_only = 0;
  options.content_type = 0;
  options.ip_addr_count = 0;
  options.use_aio_accept = 0;
  options.use_aio_read = 0;
  options.use_aio_write = 0;
  options.use_aio_close = 0;
  options.use_socket_aio = 0;
  options.use_aio_wait = 0;
  options.aio_write_events_limit = 0;
  options.aio_read_events_limit = 0;
  options.aio_accept_events_limit = 0;
  options.aio_accept_thold = 0;
  options.aio_read_before_accept = 0;
  options.aio_read_before_write = 0;
  options.aio_dont_wait = 0;
  options.aio_dont_close = 0;
  options.aio_initial_prepost_only = 0;

  options.read_buffer_size = OPT_DEFAULT_READ_BUFFER_SIZE;
  options.reply_buffer_size = OPT_DEFAULT_REPLY_BUFFER_SIZE;

  options.dyn_buffer_size = OPT_DEFAULT_DYN_BUFFER_SIZE;
  options.num_dyn_buffers = OPT_DEFAULT_NUM_DYN_BUFFERS;
  options.num_dyn_buffers_per_app = OPT_DEFAULT_NUM_DYN_BUFFERS_PER_APP;
  options.num_dyn_buffers_per_appserver =
    OPT_DEFAULT_NUM_DYN_BUFFERS_PER_APPSERVER;
  options.dyn_lock_pages = 0;
  options.dyn_touch_pages = 0;
  options.dyn_use_shared_mem = 0;

  // TODO
  // I was thinking of having the default .pid and .trace filenames reflect
  // the basename of the executable, but for now I'm backing off because it
  // might break some scripts (e.g. TRUN).  -CAB
  //sprintf(options.pid_filename, "%s.pid", prog_name);
  //sprintf(options.trace_filename, "%s.trace", prog_name);
  strcpy(options.pid_filename, OPT_DEFAULT_PID_FILENAME);
  strcpy(options.trace_filename, OPT_DEFAULT_TRACE_FILENAME);

  options.acting_as = OPT_ACTING_AS_HTTPD;
  options.stats_interval = 0;
  options.ignore_fd_setsize = 0;

#ifdef HAVE_CPUSET
  for (i=0; i<CPU_SETSIZE; i++) {
    CPU_SET(i, &options.cpu_mask);
    CPU_SET(i, &options.appserv_cpu_mask);
  }
#else
  options.cpu_mask = OPT_DEFAULT_CPU_MASK;
  options.appserv_cpu_mask = OPT_DEFAULT_CPU_MASK;
#endif
  options.doc_root[0] = '\0';
  options.doc_root_len = 0;

  ptr = inet_gethostname(NULL);
  if (ptr == NULL) {
    printf("options_set: can't get hostname\n");
    options.hostname[0] = '\0';
  } else if (strlen(ptr) >= OPT_MAX_HOSTNAME_LEN) {
    printf("options_set: hostname too long\n");
    options.hostname[0] = '\0';
  } else {
    strcpy(options.hostname, ptr);
  }

  options.app_count = 0;
  options.appserver_count = 0;
  options.appserver_startinfo = NULL;
  options.appserver_startinfo_count = 0;

  options.app_req_queue_size = -1;  // -1 means take on same value as maxconns

  options.cache_warm = 0;
  options.cache_warm_file[0] = '\0';
  options.cache_warm_size = 0;
  options.cache_warm_loops = 1;
  options.cache_warm_stride = 0;     /* 0 means pagesize */

  options.victim = 0;
  options.victim_skip = 0;
  options.victim_str[0] = '\0';

  options.cache_miss_skip = 0;

  options.use_logging = 0;
#ifdef CALL_STATS
  options.call_stats = 0;
#endif /* CALL_STATS */

  options.server_priority = OPT_SERVER_PRIORITY_NO_CHANGE;
  options.server_nice_level = OPT_SERVER_NICE_LEVEL_NO_CHANGE;

	options.no_exit_on_no_buffers = 0;

#ifdef HAVE_ENCRYPTION
  options.encrypt_data = 0;
  options.encrypt_percent = 0;
  options.encrypt_max_file_size = 0UL;
  srand(1);  /* seed if less than 100% of files being encrypted */
#endif /* HAVE_ENCRYPTION */

  /* permit the specification of a min and/or a maximum file size
   * to server -- any files not in this range are not served
   */
  options.min_file_size = OPT_NO_SIZE_LIMIT;
  options.max_file_size = OPT_NO_SIZE_LIMIT;
  options.skip_header = 0;

  options.aio_complq_count = OPT_AIO_DEFAULT_COMPL_QUEUES;
  strncpy(options.aio_completion_order, 
          OPT_AIO_DEFAULT_COMPLETION_ORDER, OPT_AIO_COMPL_ORDER_LEN);

  options.server_id = 0;

  options.fake_writing = 0;

} /* options_set */

void
options_print(void)
{
  int i, j;
  app_t *app;
  appserver_t *appserver;

  printf("--------------------------------------------------------\n");
  printf("options.free_fd_thold = %d\n", 
          options.free_fd_thold);
  printf("options.get_connections_on_close = %d\n",
          options.get_connections_on_close);
  printf("options.do_multiaccept = %d\n",
          options.do_multiaccept);
  printf("options.use_accept_send = %d\n",
          options.use_accept_send);
  printf("options.multiaccept_max = %d\n",
          options.multiaccept_max);
  printf("options.get_connections = %d\n",
          options.get_connections);
  printf("options.conn_on = %d\n",
          options.conn_on);
  printf("options.conn_off = %d\n",
          options.conn_off);
  printf("options.call_readable_from_new_conn = %d\n",
          options.call_readable_from_new_conn);
  printf("options.loop_on_readable = %d\n",
          options.loop_on_readable);
  printf("options.call_writable_from_readable = %d\n",
          options.call_writable_from_readable);
  printf("options.get_connections_in_server_loop = %d\n",
          options.get_connections_in_server_loop);
  printf("options.suspend_on_idle = %d\n",
          options.suspend_on_idle);
  printf("options.process_sds_order = %d\n",
          options.process_sds_order);
  printf("options.delay = %d\n",
          options.delay);
  printf("options.state_delay = %d\n",
          options.state_delay);
  printf("options.listenq = %d\n",
          options.listenq);
  printf("options.read_sock_buf_size = %d\n",
          options.read_sock_buf_size);
  printf("options.write_sock_buf_size = %d\n",
          options.write_sock_buf_size);
  printf("options.read_sock_buf_lowat = %d\n",
          options.read_sock_buf_lowat);
  printf("options.write_sock_buf_lowat = %d\n",
          options.write_sock_buf_lowat);
  printf("options.use_max_open_sd = %d\n",
          options.use_max_open_sd);
  printf("options.count_sigpipes = %d\n",
          options.count_sigpipes);
  printf("options.extra_freq = %d\n",
          options.extra_freq);
  printf("options.dont_intr_select = %d\n",
          options.dont_intr_select);
  printf("options.close_with_reset = %d\n",
          options.close_with_reset);
  printf("options.accepts_only = %d\n",
          options.accepts_only);
  printf("options.port = %d\n",
          options.port);
#ifdef HAVE_SSL
  printf("options.ssl_port = %d\n",
	  options.ssl_port);
#endif
  printf("options.use_memcpy = %d\n",
          options.use_memcpy);
  printf("options.interactive = %d\n",
          options.interactive);
  printf("options.sigio_cntl = %d\n",
          options.sigio_cntl);
  printf("options.send_events = %d\n",
          options.send_events);
  printf("options.select_timeout = %d\n",
          options.select_timeout);
  printf("options.idle_threshold = %d\n",
          options.idle_threshold);
  printf("options.send_loop = %d\n",
          options.send_loop);
  printf("options.use_poll = %d\n",
          options.use_poll);
  printf("options.use_epoll = %d\n",
          options.use_epoll);
  printf("options.use_epoll2 = %d\n",
          options.use_epoll2);
  printf("options.use_epoll_ctlv = %d\n",
          options.use_epoll_ctlv);
  printf("options.epoll_edge_triggered = %d\n",
          options.epoll_edge_triggered);
  printf("options.epoll_modify_listeners = %d\n",
          options.epoll_modify_listeners);
  printf("options.asynch_mode = %d\n",
          options.asynch_mode);
  printf("options.send_io_events = %d\n",
          options.send_io_events);
  printf("options.auto_accept = %d\n",
          options.auto_accept);
  printf("options.auto_accept_aperture = %d\n",
          options.auto_accept_aperture);
  printf("options.send_kdebug = 0x%x\n",
          options.send_kdebug);
  printf("options.send_block_for_events = %d\n",
          options.send_block_for_events);
  printf("options.ecb_threshold_low = %d\n",
          options.ecb_threshold_low);
  printf("options.ecb_threshold_high = %d\n",
          options.ecb_threshold_high);
  printf("options.ecb_size = %d\n",
          options.ecb_size);
  printf("options.kinfo = %d\n",
          options.kinfo);
  printf("options.noaccepts = %d\n",
          options.noaccepts);
  printf("options.close_after_sock_init = %d\n",
          options.close_after_sock_init);
  printf("options.close_after_read = %d\n",
          options.close_after_read);
  printf("options.close_after_parse = %d\n",
          options.close_after_parse);
  printf("options.close_idle_conns = %d\n",
          options.close_idle_conns);
  printf("options.send_polls_for_accepts = %d\n",
          options.send_polls_for_accepts);
  printf("options.caching_on = %d\n",
          options.caching_on);
  printf("options.cache_table_size = %d\n",
          options.cache_table_size);
  printf("options.cache_max_bytes = %.0f\n",
          options.cache_max_bytes);
  printf("options.cache_max_file_size = %.0f\n",
          options.cache_max_file_size);
  printf("options.cache_max_load_factor = %.2lf\n",
          options.cache_max_load_factor);
  printf("options.cache_table_print = %d\n",
          options.cache_table_print);
  printf("options.cache_for_spec = %d\n",
          options.cache_for_spec);
  printf("options.cache_lock_pages = %d\n",
          options.cache_lock_pages);
  printf("options.use_madvise = %d\n",
          options.use_madvise);
  printf("options.use_sendfile = %d\n",
          options.use_sendfile);
  printf("options.blocking_sendfile = %d\n",
          options.blocking_sendfile);
  printf("options.send_selects_for_accepts = %d\n",
          options.send_selects_for_accepts);
  printf("options.rejection_rate = %d\n",
          options.rejection_rate);
  printf("options.use_getpid = %d\n",
          options.use_getpid);
  printf("options.trace_summary = %d\n",
          options.trace_summary);
  printf("options.trace_summary_only = %d\n",
          options.trace_summary_only);
  printf("options.use_tcp_cork = %d\n",
          options.use_tcp_cork);
  printf("options.content_type = %d\n",
          options.content_type);
  for (i = 0; i < options.ip_addr_count; i++) {
    printf("options.ip_addr[%d] = [%s]\n", i, options.ip_addr[i]);
  }
  printf("options.use_aio_accept = %d\n",
          options.use_aio_accept);
  printf("options.use_aio_read = %d\n",
          options.use_aio_read);
  printf("options.use_aio_write = %d\n",
          options.use_aio_write);
  printf("options.use_aio_close = %d\n",
          options.use_aio_close);
  printf("options.use_socket_aio = %d\n",
          options.use_socket_aio);
  printf("options.use_aio_wait = %d\n",
          options.use_aio_wait);
  printf("options.aio_accept_thold = %d\n",
          options.aio_accept_thold);
  printf("options.aio_read_before_accept = %d\n",
          options.aio_read_before_accept);
  printf("options.aio_read_before_write = %d\n",
          options.aio_read_before_write);
  printf("options.aio_dont_wait = %d\n",
          options.aio_dont_wait);
  printf("options.aio_dont_close = %d\n",
          options.aio_dont_close);
  printf("options.aio_initial_prepost_only = %d\n",
          options.aio_initial_prepost_only);

  printf("options.aio_write_events_limit = %d\n",
          options.aio_write_events_limit);
  printf("options.aio_read_events_limit = %d\n",
          options.aio_read_events_limit);
  printf("options.aio_accept_events_limit = %d\n",
          options.aio_accept_events_limit);

  printf("options.read_buffer_size = %d\n",
          options.read_buffer_size);
  printf("options.reply_buffer_size = %d\n",
          options.reply_buffer_size);
  printf("options.dyn_buffer_size = %d\n",
          options.dyn_buffer_size);
  printf("options.num_dyn_buffers = %d\n",
          options.num_dyn_buffers);
  printf("options.dyn_lock_pages = %d\n",
          options.dyn_lock_pages);
  printf("options.dyn_touch_pages = %d\n",
          options.dyn_touch_pages);

  printf("options.app_count = %d\n",
          options.app_count);
  for (i = 0; i < options.app_count; i++) {
    app = &options.app[i];
    printf("options.app[%d].uri        = [%s]\n", i, app->uri);
    printf("options.app[%d].req_type   = [%s]\n",
	i, req_type_str(app->req_type));
    appserver = app->server;
    for (j = 0; appserver != NULL; j++, appserver = appserver->next) {
      printf("options.app[%d].server[%d]  = [%s]\n", i, j, appserver->path);
    }
  }
  printf("options.appserver_count = %d\n",
          options.appserver_count);
  printf("options.app_req_queue_size = %d\n",
          options.app_req_queue_size);

  printf("options.pid_filename = %s\n",
          options.pid_filename);
  printf("options.trace_filename = %s\n",
          options.trace_filename);
  printf("options.stats_interval = %d\n",
          options.stats_interval);
  printf("options.ignore_fd_setsize = %d\n",
          options.ignore_fd_setsize);
#ifdef HAVE_CPUSET
  print_cpuset_values("options.cpu_mask = ", &options.cpu_mask);
  printf("\n");
  print_cpuset_values("options.appserv_cpu_mask = ", &options.appserv_cpu_mask);
  printf("\n");
#else
  printf("options.cpu_mask = 0x%lx\n",
          options.cpu_mask);
  printf("options.appserv_cpu_mask = 0x%lx\n",
          options.appserv_cpu_mask);
#endif
  printf("options.doc_root = %s\n",
          options.doc_root);
  printf("options.hostname = %s\n",
          options.hostname);
  printf("options.cache_warm = %d\n",
          options.cache_warm);
  printf("options.cache_warm_file = %s\n",
          options.cache_warm_file);

  printf("options.victim = %s\n",
          options.victim_str);
  printf("options.victim_skip = %d\n",
          options.victim_skip);

  printf("options.cache_miss_skip = %d\n",
          options.cache_miss_skip);

#ifdef HAVE_ENCRYPTION
  printf("options.encrypt_data = %d\n",
          options.encrypt_data);
  printf("options.encrypt_percent = %d\n",
          options.encrypt_percent);
  printf("options.encrypt_max_file_size = %lu\n",
          options.encrypt_max_file_size);
#endif /* HAVE_ENCRYPTION */

  printf("options.aio_completion_order = [%s]   r=read, w=write a=accept\n",
          options.aio_completion_order);

  printf("options.min_file_size = %.0f\n",
          options.min_file_size);
  printf("options.max_file_size = %.0f\n",
          options.max_file_size);
  printf("options.skip_header = %d\n",
          options.skip_header);
  printf("options.aio_complq_count = %d\n",
	  options.aio_complq_count);
  printf("options.use_logging = %d\n", options.use_logging);
#ifdef CALL_STATS
  printf("options.call_stats = %d\n", options.call_stats);
#endif /* CALL_STATS */
  printf("options.fake_writing = %d\n", options.fake_writing);

  printf("--------------------------------------------------------\n");
}

void options_print_xml(FILE* destf)
{
  int i, j;
  app_t *app;
  appserver_t *appserver;

  fprintf(destf, "<options>\n\n");

  fprintf(destf, "  <category name='general'>\n");
  fprintf(destf, "    <opt name='content_type'> %d </opt>\n", options.content_type);
  fprintf(destf, "    <opt name='count_sigpipes'> %d </opt>\n", options.count_sigpipes);

#ifdef HAVE_CPUSET
  fprintf(destf, "    <opt name='cpu_mask'>\n");
  fprint_cpuset(destf, NULL, &options.cpu_mask);
  fprintf(destf, "\n    </opt>\n");
  
  fprintf(destf, "    <opt name='appserv_cpu_mask'>\n");
  fprint_cpuset(destf, NULL, &options.appserv_cpu_mask);
  fprintf(destf, "\n    </opt>\n");
#else
  fprintf(destf, "    <opt name='cpu_mask'> 0x%lx </opt>\n", options.cpu_mask);
  fprintf(destf, "    <opt name='appserv_cpu_mask'> 0x%lx </opt>\n", options.appserv_cpu_mask);
#endif

  fprintf(destf, "    <opt name='delay'> %d </opt>\n", options.delay);
  fprintf(destf, "    <opt name='state_delay'> %d </opt>\n", options.state_delay);
  fprintf(destf, "    <opt name='doc_root'> %s </opt>\n", options.doc_root);
  fprintf(destf, "    <opt name='hostname'> %s </opt>\n", options.hostname);
  fprintf(destf, "    <opt name='interactive'> %d </opt>\n", options.interactive);
  fprintf(destf, "    <opt name='min_file_size'> %.0f </opt>\n", options.min_file_size);
  fprintf(destf, "    <opt name='max_file_size'> %.0f </opt>\n", options.max_file_size);
  fprintf(destf, "    <opt name='numprocs'> %d </opt>\n", numprocs);
  fprintf(destf, "    <opt name='pid_filename'> %s </opt>\n", options.pid_filename);
  fprintf(destf, "    <opt name='port'> %d </opt>\n", options.port);
  fprintf(destf, "    <opt name='sigio_cntl'> %d </opt>\n", options.sigio_cntl);
  fprintf(destf, "    <opt name='stats_interval'> %d </opt>\n", options.stats_interval);
  fprintf(destf, "    <opt name='skip_header'> %d </opt>\n", options.skip_header);
  fprintf(destf, "    <opt name='trace_filename'> %s </opt>\n", options.trace_filename);
  fprintf(destf, "    <opt name='trace_summary'> %d </opt>\n", options.trace_summary);
  fprintf(destf, "    <opt name='trace_summary_only'> %d </opt>\n", options.trace_summary_only);
  fprintf(destf, "    <opt name='use_logging'> %d </opt>\n", options.use_logging);
#ifdef CALL_STATS
  fprintf(destf, "    <opt name='call-stats'> %d </opt>\n", options.call_stats);
#endif /* CALL_STATS */
  fprintf(destf, "    <opt name='fake-writing'> %d </opt>\n", options.fake_writing);
  for (i = 0; i < options.ip_addr_count; i++) 
    fprintf(destf, "    <ip-addr index='%d'> %s </opt>\n", i, options.ip_addr[i]);
  fprintf(destf, "  </category>\n\n");
  
  fprintf(destf, "  <category name='file-descriptors'>\n");
  fprintf(destf, "    <opt name='free_fd_thold'> %d </opt>\n", options.free_fd_thold);
  fprintf(destf, "    <opt name='ignore_fd_setsize'> %d </opt>\n", options.ignore_fd_setsize);
  fprintf(destf, "    <opt name='max_fds'> %d </opt>\n", max_fds);
  fprintf(destf, "    <opt name='process_sds_order'> %d </opt>\n", options.process_sds_order);
  fprintf(destf, "    <opt name='use_max_open_sd'> %d </opt>\n", options.use_max_open_sd);
  fprintf(destf, "  </category>\n\n");

  fprintf(destf, "  <category name='connections'>\n");
  fprintf(destf, "    <opt name='conn_off'> %d </opt>\n", options.conn_off);
  fprintf(destf, "    <opt name='conn_on'> %d </opt>\n", options.conn_on);
  fprintf(destf, "    <opt name='do_multiaccept'> %d </opt>\n", options.do_multiaccept);
  fprintf(destf, "    <opt name='extra_freq'> %d </opt>\n", options.extra_freq);
  fprintf(destf, "    <opt name='get_connections'> %d </opt>\n", options.get_connections);
  fprintf(destf, "    <opt name='get_connections_in_server_loop'> %d </opt>\n", options.get_connections_in_server_loop);
  fprintf(destf, "    <opt name='get_connections_on_close'> %d </opt>\n", options.get_connections_on_close);
  fprintf(destf, "    <opt name='idle_threshold'> %d </opt>\n", options.idle_threshold);
  fprintf(destf, "    <opt name='listenq'> %d </opt>\n", options.listenq);
  fprintf(destf, "    <opt name='maxconns'> %d </opt>\n", maxconns);
  fprintf(destf, "    <opt name='multiaccept_max'> %d </opt>\n", options.multiaccept_max);
  fprintf(destf, "  </category>\n\n");

  fprintf(destf, "  <category name='sockets'>\n");
  fprintf(destf, "    <opt name='close_with_reset'> %d </opt>\n", options.close_with_reset);
  fprintf(destf, "    <opt name='dont_intr_select'> %d </opt>\n", options.dont_intr_select);
  fprintf(destf, "    <opt name='read_sock_buf_lowat'> %d </opt>\n", options.read_sock_buf_lowat);
  fprintf(destf, "    <opt name='read_sock_buf_size'> %d </opt>\n", options.read_sock_buf_size);
  fprintf(destf, "    <opt name='select_timeout'> %d </opt>\n", options.select_timeout);
  fprintf(destf, "    <opt name='use_tcp_cork'> %d </opt>\n", options.use_tcp_cork);
  fprintf(destf, "    <opt name='write_sock_buf_lowat'> %d </opt>\n", options.write_sock_buf_lowat);
  fprintf(destf, "    <opt name='write_sock_buf_size'> %d </opt>\n", options.write_sock_buf_size);
  fprintf(destf, "   </category>\n\n");

  fprintf(destf, "  <category name='memory'>\n");
  fprintf(destf, "    <opt name='blocking_sendfile'> %d </opt>\n", options.blocking_sendfile);
  fprintf(destf, "    <opt name='call_readable_from_new_conn'> %d </opt>\n", options.call_readable_from_new_conn);
  fprintf(destf, "    <opt name='call_writable_from_readable'> %d </opt>\n", options.call_writable_from_readable);
  fprintf(destf, "    <opt name='loop_on_readable'> %d </opt>\n", options.loop_on_readable);
  fprintf(destf, "    <opt name='use_madvise'> %d </opt>\n", options.use_madvise);
  fprintf(destf, "    <opt name='use_memcopy'> %d </opt>\n", options.use_memcpy);
  fprintf(destf, "    <opt name='use_sendfile'> %d </opt>\n", options.use_sendfile);
  fprintf(destf, "  </category>\n\n");

  fprintf(destf, "  <category name='buffers'>\n");
  fprintf(destf, "    <opt name='dyn_buffer_size'> %d </opt>\n", options.dyn_buffer_size);
  fprintf(destf, "    <opt name='dyn_lock_pages'> %d </opt>\n", options.dyn_lock_pages);
  fprintf(destf, "    <opt name='dyn_touch_pages'> %d </opt>\n", options.dyn_touch_pages);
  fprintf(destf, "    <opt name='num_dyn_buffers'> %d </opt>\n", options.num_dyn_buffers);
  fprintf(destf, "    <opt name='num_dyn_buffers_per_app'> %d </opt>\n", options.num_dyn_buffers_per_app);
  fprintf(destf, "    <opt name='num_dyn_buffers_per_app_server'> %d </opt>\n", options.num_dyn_buffers_per_appserver);
  fprintf(destf, "    <opt name='read_buffer_size'> %d </opt>\n", options.read_buffer_size);
  fprintf(destf, "    <opt name='reply_buffer_size'> %d </opt>\n", options.reply_buffer_size);
  fprintf(destf, "  </category>\n\n");

  fprintf(destf, "  <category name='apps'>\n");
  fprintf(destf, "    <opt name='app_count'> %d </opt>\n", options.app_count);
  fprintf(destf, "    <opt name='appserver_count'> %d </opt>\n", options.appserver_count);
  fprintf(destf, "    <opt name='app_req_queue_size'> %d </opt>\n",options.app_req_queue_size);
  for (i = 0; i < options.app_count; i++) {
    app = &options.app[i];
    fprintf(destf, "    <app index='%d' uri='%s' req_type='%s'>\n", i, app->uri, req_type_str(app->req_type));
    appserver = app->server;
    for (j = 0; appserver != NULL; j++, appserver = appserver->next)
      fprintf(destf, "      <appserver index='%d' path='%s'/>", i, appserver->path);
  }
  fprintf(destf, "  </category>\n\n");

  fprintf(destf, "  <category name='polling-method'>\n");
  fprintf(destf, "    <opt name='epoll_edge_triggered'> %d </opt>\n", options.epoll_edge_triggered);
  fprintf(destf, "    <opt name='epoll_modify_listeners'> %d </opt>\n", options.epoll_modify_listeners);
  fprintf(destf, "    <opt name='use_epoll'> %d </opt>\n", options.use_epoll);
  fprintf(destf, "    <opt name='use_epoll2'> %d </opt>\n", options.use_epoll2);
  fprintf(destf, "    <opt name='use_epoll_ctlv'> %d </opt>\n", options.use_epoll_ctlv);
  fprintf(destf, "    <opt name='use_poll'> %d </opt>\n", options.use_poll);
  fprintf(destf, "  </category>\n\n");

  fprintf(destf, "  <category name='caching'>\n");
  fprintf(destf, "    <opt name='caching_on'> %d </opt>\n", options.caching_on);
  fprintf(destf, "    <opt name='cache_for_spec'> %d </opt>\n", options.cache_for_spec);
  fprintf(destf, "    <opt name='cache_max_bytes'> %f </opt>\n", options.cache_max_bytes);
  fprintf(destf, "    <opt name='cache_max_file_size'> %f </opt>\n", options.cache_max_file_size);
  fprintf(destf, "    <opt name='cache_max_load_factor'> %f </opt>\n", options.cache_max_load_factor);
  fprintf(destf, "    <opt name='cache_lock_pages'> %d </opt>\n", options.cache_lock_pages);
  fprintf(destf, "    <opt name='cache_table_print'> %d </opt>\n", options.cache_table_print);
  fprintf(destf, "    <opt name='cache_table_size'> %d </opt>\n", options.cache_table_size);
  fprintf(destf, "    <opt name='cache_warm_file'> %s </opt>\n", options.cache_warm_file);
  fprintf(destf, "    <opt name='victim'> %s </opt>\n", options.victim_str);
  fprintf(destf, "    <opt name='victim_skip'> %d </opt>\n", options.victim_skip);
  fprintf(destf, "    <opt name='cache_miss_skip'> %d </opt>\n", options.cache_miss_skip);
  fprintf(destf, "  </category>\n\n");

  fprintf(destf, "  <category name='request-life'>\n");
  fprintf(destf, "    <opt name='noaccepts'> %d </opt>\n", options.noaccepts);
  fprintf(destf, "    <opt name='accepts_only'> %d </opt>\n", options.accepts_only);
  fprintf(destf, "    <opt name='close_after_sock_init'> %d </opt>\n", options.close_after_sock_init);
  fprintf(destf, "    <opt name='close_after_read'> %d </opt>\n", options.close_after_read);
  fprintf(destf, "    <opt name='close_after_parse'> %d </opt>\n", options.close_after_parse);
  fprintf(destf, "  </category>\n\n");

  fprintf(destf, "</options>\n\n");
}

/* check for and report conflicts in the options */
void
options_valid(void)
{
#ifdef MOD_SPECWEB99
  app_t *app;
  int i;
#endif

  if (options.ecb_threshold_low >= options.ecb_threshold_high) {
    printf("Can't set ecb lower threshold not lower than high threshold\n");
    printf("low = %d high = %d\n",
       	options.ecb_threshold_low, options.ecb_threshold_high);
    exit(1);
  }

  if (options.process_sds_order == OPT_PROCESS_SDS_LRU) {
    if (options.get_connections != OPT_CONN_WITH_SELECT_POLL_EPOLL) {
      printf("Can't use OPT_PROCESS_SDS_LRU unless get_connections == "
	  "OPT_CONN_WITH_SELECT_POLL_EPOLL\n");
      exit(1);
    }
  }

#ifdef __linux__
  if (options.listenq > 128) {
    printf("WARNING: Listenq max is 128 on most Linux versions. "
	"It may not set it to %d\n",
            options.listenq);
  }
#endif /* __linux__ */

#ifdef MOD_SPECWEB99
  for (i = 0; i < options.app_count; i++) {
    app = &options.app[i];
    if (app->req_type == REQ_SPECWEB99) {
      if (!options.use_sendfile) {
	printf("The use of SPECweb99 applications requires --use-sendfile\n");
	exit(1);
      }
      break;
    }
  }
#endif
}


