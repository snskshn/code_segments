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



#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/resource.h>
#include <ctype.h>
#include <time.h>

#ifdef HPUX
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include "debug.h"
#include "stats.h"
#include "common.h"
#include "options.h"
#define GNU_SOURCE
#include "getopt.h"
#undef GNU_SOURCE

#ifdef HAVE_SCHED_AFFINITY
#ifdef PROVIDE_SCHED_AFFINITY_HEADERS
#include "sys_sched_affinity.h"
#else
#include "sched.h"
#endif
#endif

#include "util.h"
#include "app.h"

#ifdef CALL_STATS
#include "call_stats.h"
#endif /* CALL_STATS */


/* NOTE: DIST_NAME comes from Makefile.base */
char *version = VERSION;
static int do_version = 0;

int need_fd_setsize();

#define SERVER_IDENT_LEN   (400)
char server_ident[SERVER_IDENT_LEN];

void adjust_connections(int maxconns);

/* Fields are:
 * name, arguments, addr of int value to set, val to use
 * If addr is 0 then val is returned when getopt is called.
 * E.g., {"name1", no_argument, 0, 'Q'} will look like -Q was used.
 * If addr is not 0 then val is stored in the addr provided.
 * E.g., {"name1", no_argument, &opt, 'Q'} will store 'Q' in &opt
 *
 * For now we rely on everything using the original single
 * character values to fall through for processing.
 */

#ifdef USE_GETOPT_LONG
static struct option long_options[] =
{
  {"cfg-filename",          required_argument, &options.is_cfg_filename, 0},
  {"accepts-only",          no_argument,       0, 'A'},
  {"close-after-accept",    no_argument,       0, 'A'},
  {"block-for-send_events", no_argument,       0, 'B'},
  {"caching-on",            no_argument,       0, 'C'},
  {"debug-mask",            required_argument, 0, 'D'},
  {"send-events",           no_argument,       0, 'E'},
  {"reset-on-close",        no_argument,       0, 'F'},
  {"use-getpid",            no_argument,       0, 'G'},
  {"help",                  no_argument,       0, 'H'},
  {"rcv-sock-low-wat",      required_argument, 0, 'I'},
  {"snd-sock-low-wat",      required_argument, 0, 'J'},
  {"kdebug-mask",           required_argument, 0, 'K'},
  {"full-read",             required_argument, 0, 'L'},
  {"multi-accept",          required_argument, 0, 'M'},
  {"send-loop",             no_argument,       0, 'N'},
  {"select-timeout",        required_argument, 0, 'O'},
  {"count-sigpipes",        no_argument,       0, 'P'},
  {"ecb-hi-water",          required_argument, 0, 'Q'},
  {"read-sockbuf-size",     required_argument, 0, 'R'},
  {"dont-intr-select",      required_argument, 0, 'S'},
  {"port",                  required_argument, 0, 'T'},
  {"auto-accept",           no_argument,       0, 'U'},
  {"auto-accept-aperture",  required_argument, 
                                &options.auto_accept_aperture, 0},
#ifdef HAVE_SENDFILE
  {"use-sendfile",          no_argument,       0, 'V'},
  {"blocking-sendfile",     no_argument, &options.blocking_sendfile, 1},
#endif /* HAVE_SENDFILE */
  {"write-sockbuf-size",    required_argument, 0, 'W'},
  {"send-selects-for-accepts", no_argument,    0, 'X'},
  {"send-io-events",        no_argument,       0, 'Y'},
  {"content-type",          no_argument,       0, 'Z'},
  {"async-mode",            no_argument,       0, 'a'},
  {"ecb-buf-entries",       required_argument, 0, 'b'},
  {"max-conns",             required_argument, 0, 'c'},
  {"delay",                 required_argument, 0, 'd'},
  {"extra-accepts",         required_argument, 0, 'e'},
  {"max-fds",               required_argument, 0, 'f'},
  {"accept-on-close",       no_argument,       0, 'g'},
  {"help",                  no_argument,       0, 'h'},
  {"interactive",           no_argument,       0, 'i'},
  {"kernel-info",           no_argument,       0, 'j'},
  {"sigio-use-procmask",    no_argument,       0, 'k'},
  {"listenq",               required_argument, 0, 'l'},
  {"accept-count",          required_argument, 0, 'm'},
  {"no-accepts",            no_argument,       0, 'n'},
  {"listen-only",           no_argument,       0, 'n'},
  {"process-fd-order",      required_argument, 0, 'o'},
  {"procs",                 required_argument, 0, 'p'},
  {"ecb-low-water",         required_argument, 0, 'q'},
  {"eager-reads",           no_argument,       0, 'r'},
  {"conns-in-server-loop",  no_argument,       0, 's'},
  {"free-fd-threshold",     required_argument, 0, 't'},
  {"track-max-fd",          no_argument,       0, 'u'},
  {"use-poll",              no_argument,       0, 'v'},
  {"eager-writes",          no_argument,       0, 'w'},
  {"sigio-accepts",         no_argument,       0, 'x'},
  {"memcpy",                no_argument,       0, 'y'},
  {"rejection-rate",        no_argument,       0, 'z'},
  {"help",                  no_argument,       0, '?'},
#ifdef HAVE_EPOLL
  {"use-epoll",             no_argument,       0, '1'},
  {"use-epoll2",            no_argument,       0, '2'},
  {"use-epoll-ctlv",        no_argument,       0, '3'},
  {"epoll-edge-triggered",  no_argument,       0, '4'},
  {"epoll-modify-listeners", no_argument,      0, '5'},
  {"epoll-good",            no_argument,       0, '6'},
#endif /* HAVE_EPOLL */

#ifdef HAVE_SSL
  {"ssl-port",              required_argument, 0, '7'},
#endif /* HAVE_SSL */

  {"skip-header",           no_argument, &options.skip_header, 1},
  /* Note: dummy variable here just used to trigger the option */
  {"min-file-size",         required_argument, &options.min_file_size_dummy,
     0},
  {"max-file-size",         required_argument, &options.max_file_size_dummy,
     0},
  {"close-after-sock-init", no_argument, &options.close_after_sock_init, 1},
  {"close-after-read",      no_argument, &options.close_after_read, 1},
  {"close-after-parse",     no_argument, &options.close_after_parse, 1},
  {"close-idle-conns",      no_argument, &options.close_idle_conns, 1},
  {"send-polls-for-accepts", no_argument, 
                            &options.send_polls_for_accepts, 1},
  {"cache-table-size",      required_argument, 
                            &options.cache_table_size, 0},
  {"cache-max-bytes",       required_argument, 
                            &options.cache_max_bytes_dummy, 0},
  {"cache-max-file-size",   required_argument, 
                            &options.cache_max_file_size_dummy, 0},
  {"cache-max-load-factor", required_argument, 
                            &options.cache_max_load_factor_dummy, 0},
  {"cache-lock-pages",      no_argument, &options.cache_lock_pages, 1},
  {"cache-table-print",     no_argument, &options.cache_table_print, 1},
  {"cache-for-spec",        no_argument, &options.cache_for_spec, 1},
  {"cache-warm",            required_argument, &options.cache_warm, 1},
  {"cache-warm-size",       required_argument, &options.cache_warm_size, 1},
  {"cache-warm-stride",     required_argument, &options.cache_warm_stride, 1},
  {"cache-warm-loops",     required_argument, &options.cache_warm_loops, 1},
#ifdef HAVE_ENCRYPTION
  {"encrypt-data",          no_argument, &options.encrypt_data, 1},
  {"encrypt-percent",       required_argument, &options.encrypt_percent, 0},
  {"encrypt-max-file-size", required_argument,
                            &options.encrypt_max_file_size_dummy, 0},
#endif /* HAVE_ENCRYPTION */
  {"trace-summary",         no_argument, &options.trace_summary, 1},
  {"trace-summary-only",    no_argument, &options.trace_summary_only, 1},
  {"use-tcp-cork",          no_argument, &options.use_tcp_cork, 1},
  {"use-madvise",           no_argument, &options.use_madvise, 1},
  {"use-accept-send",       no_argument, &options.use_accept_send, 1},
  {"ip-addr",               required_argument, &options.is_ip_addr, 0},
  {"version",               no_argument, &do_version, 1},
  {"use-aio-accept",        no_argument, &options.use_aio_accept, 1},
  {"use-aio-read",          no_argument, &options.use_aio_read, 1},
  {"use-aio-write",         no_argument, &options.use_aio_write, 1},
  {"use-aio-close",         no_argument, &options.use_aio_close, 1},
  {"use-socket-aio",        no_argument, &options.use_socket_aio, 1},
  {"use-aio-wait",          no_argument, &options.use_aio_wait, 1},
  {"aio-read-before-accept", no_argument, &options.aio_read_before_accept, 1},
  {"aio-read-before-write",  no_argument, &options.aio_read_before_write, 1},
  {"aio-write-events-limit",  required_argument, 
                              &options.aio_write_events_limit, 0},
  {"aio-read-events-limit",   required_argument, 
                              &options.aio_read_events_limit, 0},
  {"aio-accept-events-limit", required_argument, 
                              &options.aio_accept_events_limit, 0},
  {"aio-accept-thold",         required_argument, 
                              &options.aio_accept_thold, 0},
  {"aio-completion-order",    required_argument, 
                              &options.is_aio_completion_order, 0},
  {"aio-dont-wait",           no_argument, &options.aio_dont_wait, 1},
  {"aio-dont-close",          no_argument, &options.aio_dont_close, 1},
  {"aio-initial-prepost-only",no_argument, &options.aio_initial_prepost_only,1},
  {"aio-complq-count",      required_argument, &options.aio_complq_count, 0},
  {"pid-filename",          required_argument, &options.is_pid_filename, 0},
  {"trace-filename",        required_argument, &options.is_trace_filename, 0},
  {"read-buffer-size",      required_argument, &options.read_buffer_size, 0},
  {"reply-buffer-size",     required_argument, &options.reply_buffer_size, 0},
  {"dyn-buffer-size",       required_argument, &options.dyn_buffer_size, 0},
  {"num-dyn-buffers",       required_argument, &options.num_dyn_buffers, 0},
  {"num-dyn-buffers-per-app", required_argument,
			      &options.num_dyn_buffers_per_app, 0},
  {"num-dyn-buffers-per-appserver", required_argument,
			      &options.num_dyn_buffers_per_appserver, 0},
  {"dyn-lock-pages",        no_argument, &options.dyn_lock_pages, 0},
  {"dyn-touch-pages",       no_argument, &options.dyn_touch_pages, 0},
  {"stats-interval",        required_argument, &options.stats_interval, 0},
  {"ignore-fd-setsize",     no_argument, &options.ignore_fd_setsize, 1},
  {"use-cpu-mask",          required_argument, &options.cpu_mask_dummy, 0},
  {"idle-threshold",        required_argument, 
                              (int *) &options.idle_threshold, 0},
  {"doc-root",              required_argument, &options.is_doc_root, 0},
  {"hostname",              required_argument, &options.is_hostname, 0},
  {"app",                   required_argument, &options.is_app, 0},
  {"app-req-queue-size",    required_argument, &options.app_req_queue_size, 0},
  {"start-app-server",      required_argument, &options.is_start_app_server, 0},
  {"use-logging",           no_argument, &options.use_logging, 1},
#ifdef CALL_STATS
  {"call-stats",           required_argument, &options.call_stats, 0},
#endif /* CALL_STATS */
  {"no-exit-on-no-buffers", no_argument, &options.no_exit_on_no_buffers, 1},
  {"server-nice-level",       required_argument, &options.server_nice_level, 0},
  {"server-priority",       required_argument, &options.server_priority, 0},
  {"server-id",             required_argument, &options.server_id, 0},
  {"state-delay",           required_argument, &options.state_delay, 0},
  {"victim",                required_argument, &options.victim, 1},
  {"victim-skip",           required_argument, &options.victim_skip, 1},
  {"cache-miss-skip",       required_argument, &options.cache_miss_skip, 1},
  {"fake-writing",          no_argument, &options.fake_writing, 1},
  {0, 0, 0, 0}
};
#endif


/* ---------------------------------------------------------------------- */
void
usage(void)
{
#ifdef USE_GETOPT_LONG
   int i = 0;
#endif /* USE_GETOPT_LONG */

   printf("Usage: %s [options]\n", prog_name);
   printf("  -A {only process accepts - see how fast we can do this}\n");
   printf("  -B {block waiting for SEND events when there aren't any}\n");
   printf("  -C {Caching is turned on}\n");
   printf("  -D # {set debug mask to # (# is specified in hex)}\n");
   printf("  -E {use SEND events}\n");
   printf("  -F {close on reset / fast close (use SO_LINGER to try to "
       "close sockets faster)}\n");
   printf("  -H {print help/command line option info}\n");
   printf("  -I # {set low waTer mark for read/receive socket buffer to #}\n");
   printf("  -J # {set low waTer mark for write/send socket buffer to #}\n");
   printf("  -K {turn on kernel debugging for SEND events}\n");
   printf("  -L {loop on readable when it's called after select\n");
   printf("  -M # {multiaccept syscall - up to #}\n");
   printf("  -N {use send loop}\n");
   printf("  -O # {seconds used to select timeout}\n");
   printf("  -P {pipe (count SIGPIPES received)}\n");
   printf("  -Q # {ecb empty queue high water mark / threshold}\n");
   printf("  -R # {read socket buffer size is set to #}\n");
   printf("  -S {select protected from being interrupted}\n");
   printf("  -T # {use port number #}\n");
   printf("  -U {use auto accept for incoming connection requests}\n");
   printf("  -V {use sendfile system call}\n");
   printf("  -W # {write/send socket buffer size is set to #}\n");
   printf("  -Y {use send events for socket I/O also}\n");
   printf("  -Z {return Content-Type information in header\n");
   printf("  -a {asynchronous accepts - using SIGIO}\n");
   printf("  -b # {size of the ecb buffer (number of events it will hold}\n");
   printf("  -c # {maximum number of connections}\n");
   printf("  -d # {delay for # useconds - faking work}\n");
   printf("  -e {extra accepts done during process_sds}\n");
   printf("  -f {fds maximum}\n");
   printf("  -g {get new connections on close (multiple if -m used)}\n");
   printf("  -h {print help/command line option info}\n");
   printf("  -i {enter interactive mode - used for debugging}\n");
   printf("  -j {use kinfo stuff}\n");
   printf("  -k {use sigprocmask for blocking/unblocking SIGIOs}\n");
   printf("     {must be used with -a}\n");
   printf("     {without -k, -a uses fcntl calls to disable/enable SIGIOs}\n");
   printf("  -l # {listen queue size is set to #}\n");
   printf("  -m # {multiple accepts when getting new connection - up to #}\n");
   printf("  -o how {order the work done on fds according to how}\n");
   printf("          where how can be one of:}\n");
   printf("     up           {lowest fd to highest}\n");
   printf("     down         {highest fd to lowest}\n");
   printf("     writes-down  {writes first, highest fd to lowest}\n");
   printf("     writes-up    {writes first, lowest fd to highest}\n");
   printf("     lru          {least recently used fd (CURRENTLY SLOW!)}\n");
   printf("     lifo         {last in first out}\n");
   printf("     fifo         {first in first out}\n");
   printf("  -p {processes to use (not sure if this works anymore}\n");
   printf("  -q # {ecb empty queue low water mark / threshold}\n");
   printf("  -r {readable is called when new connection is made}\n");
   printf("  -s {server loop gets new connections}\n");
   printf("  -t # {threshold for the number of free fds needed before}\n");
   printf("        accepting new connections}\n");
   printf("  -u {use max open fd (instead of max sd for select and process "
       "sds)}\n");
   printf("  -v {use poll to obtain events}\n");
   printf("  -w {writable is called from readable}\n");
   printf("  -y {use memcpy to copy file descriptors prior to select}\n");
   printf("  -z # {reject # in 100 connection requests}\n");
   printf("  -? {print help/command line option info}\n");

#ifdef USE_GETOPT_LONG
   /* now loop over the long versions of the arguments */
   i = 0;
   while (long_options[i].name != 0) {
     if (long_options[i].flag) {
       printf("      --%s", long_options[i].name);
     } else {
       printf("  -%c  --%s", long_options[i].val, long_options[i].name);
     }

     if (long_options[i].has_arg) {
       printf(" arg\n");
     } else {
       printf("\n");

     }
     i++;
   }
#endif /* USE_GETOPT_LONG */
}


typedef struct argvector {
  int argc;
  char **argv;
  char *buf;
  int size;
} argvector_t;

argvector_t *
create_argvector(int argc, char **argv)
{
  argvector_t *av;

  av = (argvector_t *) malloc(sizeof(argvector_t));
  assert(av != NULL);
  av->argc = argc;
  av->size = argc + 1;
  av->argv = (char **) malloc(av->size * sizeof(char *));
  av->buf = NULL;
  assert(av->argv != NULL);
  memcpy(av->argv, argv, av->size * sizeof(char *));

  return av;
}

void
free_argvector(argvector_t *av)
{
  assert(av != NULL);
  assert(av->argv != NULL);
  /*
   * If av->buf is non-NULL then we must have previously allocated memory,
   * and we are responsible for freeing it.
   */
  if (av->buf != NULL) {
    free(av->buf);
  }
  free(av->argv);
  free(av);
}


/* Read the command line options contained in the given
 * config file. Return an argvector 
 */
argvector_t *
read_cfg_file(const char *cfg_file)
{
  struct stat st;
  FILE *fp;
  argvector_t *av;
  int sol;		// flag -- at start of line
  int comment;		// flag -- in a comment
  int ws;		// flag -- currently reading whitespace
  int inquotes;           // flag -- currently reading quoted text "e.g. this"
  off_t off;

  assert(cfg_file != NULL);

  /* Stat the file, exit if we can't */
  if (stat(cfg_file, &st) < 0) {
    printf("Can't stat config file '%s'\n", cfg_file);
    exit(1);
  }

  /* Open the file, exit if we can't */
  fp = fopen(cfg_file, "rt");
  if (fp == NULL) {
    printf("Can't open config file '%s'\n", cfg_file);
    exit(1);
  }

  av = (argvector_t *) malloc(sizeof(argvector_t));
  assert(av != NULL);

  /*
   * Read the contents of cfg_file into av->buf[].  Along the way, we make
   * note of how many arguments are present (av->argc).
   */
  av->buf = (char *) malloc(st.st_size + strlen(cfg_file) + 2);
  assert(av->buf != NULL);
  av->argc = 1;
  sol = 1;      /* We're at the start of the first line */
  comment = 0;  /* Assume it's not a comment */
  ws = 1;       /* Assume it's a whitespace character (check later) */
  inquotes = 0;   /* No quotes to start */

  for (off = 0; off < st.st_size; off++) {
    /* Read the file one character at a time */
    av->buf[off] = getc(fp);
    assert(av->buf[off] != EOF);

    if (av->buf[off] == '\n') {
      /* We're at the start of a new line */
      if ( inquotes ) {
        printf("Error parsing config file: unmatched quotes\n");
        exit(1);
      }
      sol = 1;
      comment = 0;
    }  else if ( !comment && av->buf[off] == '"') {
      /* We're either entering or leaving quoted text */
      inquotes = !inquotes;
    
    } else {
      if (sol) {
        /* If we're at the start of a line, check if it's a comment */
	if (av->buf[off] == '#') {
	  comment = 1;
	}
      }
      if (comment) {
        /* Skip thru the comment */
	continue;
      }
    } /* if-else */

    /* It's not a comment if we got here */
    if (ws) {
      if (!isspace((int) av->buf[off])) {
        /* Whenever we hit unquoted whitespace followed by non-whitespace,
           we've got a new argument, so increment argc */
	ws = 0;
	av->argc++;
      }
    } else {
      if (!inquotes && isspace((int) av->buf[off])) {
	ws = 1;
      }
    }
  }
  fclose(fp);

  /* Allocate an argvector of the appropriate size */
  av->size = av->argc + 1;
  av->argv = (char **) malloc(av->size * sizeof(char *));
  assert(av->argv != NULL);
  av->argv[0] = av->buf + st.st_size + 1;
  strcpy(av->argv[0], cfg_file);

  /* Now finish filling in the argvector_t fields.  This involves re-scanning
   * the buffer to locate and null-terminate each argument.  Much of this
   * code is a duplicate of the code just above.
   */
  av->argc = 1;
  sol = 1;
  comment = 0;
  ws = 1;

  for (off = 0; off < st.st_size; off++) {
    if (av->buf[off] == '\n') {
      /* We're at the start of a new line */
      sol = 1;
      comment = 0;

    } else if (!comment && av->buf[off] == '"' ) {
      /* We're either entering or leaving quoted text */
      inquotes = !inquotes;

    } else {
      if (sol) {
        /* If we're at the start of a line, check if it's a comment */
	if (av->buf[off] == '#') {
	  comment = 1;
	}
      }
      if (comment) {
        /* Skip thru the comment */
	continue;
      }
    }

    /* It's not a comment if we got here */
    if (ws) {
      if (!isspace((int) av->buf[off])) {
        /* Whenever we hit unquoted whitespace followed by non-whitespace,
           we've got a new argument, so transfer the arg and increment argc */
	av->argv[av->argc] = av->buf + off;
	av->argc++;
	ws = 0;
      }
    } else {
      if (!inquotes && isspace((int) av->buf[off])) {
	av->buf[off] = '\0';
	ws = 1;
      }
    }
  } /* for */
  
  av->buf[off] = '\0';
  av->argv[av->argc] = NULL;
  return av;
}

/* ---------------------------------------------------------------------- */

/*
 * It is possible and sometimes convenient for a config file to "include"
 * other config files (by using --cfg-filename, as you would from the
 * command line).  We don't explicitly check for infinite recursion.
 * Instead, we just limit the depth of recursion to something reasonable.
 * Setting this to 0 would effectively disable config file support; setting
 * this to 1 would allow a config file but no recursion.
 */
#define MAX_CFG_FILE_DEPTH 5


#define MAX_HOSTNAME_LEN   (100)

int cfg_file_depth = 0;

void
process_args(int argc, char **argv)
{
  argvector_t *av;

  int ch, i;
#ifdef USE_GETOPT_LONG
  int longindex = 0;
  int *flag = 0;
#endif /* USE_GETOPT_LONG */
  struct stat stat_buf;
  int len,exec_strlen=0;
  char *uri_arg, *type_arg, *path_arg, *port_arg, *count_arg, *exec_arg, *mask_arg;
  req_type_t req_type = REQ_INVALID;
  int appserver_flags;
  app_t *app;
  char serverpath[MAX_APP_SERVER_PATH_LEN + 12];
  sockaddr_t addr;
  int count;
  char *end;
  int rc = 0;
  int done = 0;
  char hostname[MAX_HOSTNAME_LEN];
  int hostnamelen = MAX_HOSTNAME_LEN-1;
  struct tm *local = 0;
  time_t t;

#ifdef HAVE_ENCRYPTION
  char *p;
#endif /* HAVE_ENCRYPTION */

#ifdef HAVE_SCHED_AFFINITY
  unsigned long bitmap;
  unsigned long appserv_bitmap = (ULONG_MAX);
#ifdef HAVE_CPUSET
  cpu_set_t tmp_mask;
  cpu_set_t appserv_cpu_mask;
  CPU_ZERO(&tmp_mask);
  CPU_ZERO(&appserv_cpu_mask);
  /* appserver by default run on any cpu */
  for (i=0; i<CPU_SETSIZE; i++) {
    CPU_SET(i, &appserv_cpu_mask);
  } 
#else
  unsigned long appserv_cpu_mask = (ULONG_MAX); 
#endif
#endif /* HAVE_SCHED_AFFINITY */

  strncpy(server_ident, version, SERVER_IDENT_LEN);
  
  if (cfg_file_depth == 0) {
    printf("Invoked as:\n");
    printf("%s", prog_name);
    for (i = 1; i < argc; i++) {
      printf(" %s", argv[i]);
    }
    printf("\n");
    optind = 1;
  } else {
    if (cfg_file_depth > MAX_CFG_FILE_DEPTH) {
      printf("Config file depth exceeded %d; aborting.\n",
	  MAX_CFG_FILE_DEPTH);
      exit(1);
    }
    printf("Additional options from file '%s':\n", argv[0]);
    for (i = 1; i < argc; i++) {
      printf("%s%s", i == 1 ? "" : " ", argv[i]);
    }
    printf("\n");
    optind = 0;
  }

  gethostname(hostname, hostnamelen);
  printf("Run on hostname: %s\n", hostname);


  t = time(NULL);
  local = localtime(&t);
  printf("Run at: %s\n", asctime(local));

  while (1) {
#ifdef USE_GETOPT_LONG
    ch = getopt_long(argc, argv,
	"+ABCD:EFGHI:J:K:L:M:NO:PQ:R:ST:UVW:XYZ"
	"ab:c:d:e:f:ghijkl:m:no:p:q:rst:uvwxyz:?"
	"123",
	long_options, &longindex);
    if (ch < 0) {
      done = 1;
    }
#else
    ch = getopt(argc, argv,
	"ABCD:EFGHI:J:K:L:M:NO:PQ:R:ST:UVW:XYZ"
	"ab:c:d:e:f:ghijkl:m:no:p:q:rst:uvwxyz:?-"
	"123");
    if (ch == EOF) {
      done = 1;
    }
#endif

    if (done) {
      if (optind < argc) {
        printf("Unable to parse command line (error occurred around option %d)!\n", optind);
        exit(1);
      } else {
        break;
      }
    }

    switch (ch) {

#ifdef USE_GETOPT_LONG
      case 0:
	flag = long_options[longindex].flag;

	if (flag == (int *) &options.is_cfg_filename) {
	  if (strlen(optarg) >= OPT_MAX_FILENAME_LEN) {
	    printf("Cfg filename specified is too long %s\n", optarg);
	    printf("Use shorter name or increase OPT_MAX_FILENAME_LEN = %d\n",
		    OPT_MAX_FILENAME_LEN);
	    exit(1);
	  }
	  av = read_cfg_file(optarg);
	  assert(av != NULL);
	  cfg_file_depth++;
	  i = optind;
	  process_args(av->argc, av->argv);
	  optind = i;
	  cfg_file_depth--;
	  free_argvector(av);
	} else if (flag == &options.state_delay) {
	  options.state_delay = atoi(optarg);
	} else if (flag == &options.cache_table_size) {
	  options.cache_table_size = atoi(optarg);
	} else if (flag == &do_version) {
	  printf("Version = %s\n", version);
	  exit(1);
	} else if (flag == &options.use_aio_accept) {
	  options.get_connections = OPT_CONN_WITH_AIO_ACCEPT;
	  options.conn_on = OPT_CONN_WITH_AIO_ACCEPT;
	  options.conn_off = OPT_CONN_WITH_AIO_ACCEPT;
	} else if (flag == &options.cache_max_bytes_dummy) {
	  options.cache_max_bytes = atof(optarg);
	} else if (flag == &options.stats_interval) {
	  options.stats_interval = atoi(optarg);
	  /* blocking select/poll/epoll/etc calls need to timeout
	   * in order to print stats
	   */
	  options.select_timeout = options.stats_interval;
	} else if (flag == (int *) &options.aio_complq_count) {
	  options.aio_complq_count = atoi(optarg);
	  if (options.aio_complq_count == 1) {
	    strncpy(options.aio_completion_order,
		   OPT_AIO_DEFAULT_COMPLETION_ORDER,
		   OPT_AIO_COMPL_ORDER_LEN);
	  } else if (options.aio_complq_count == 3) {
	    strncpy(options.aio_completion_order,
		   OPT_AIO_ONE_QUEUE_COMPLETION_ORDER,
		   OPT_AIO_COMPL_ORDER_LEN);
	  } else {
	    printf("Code currently only handles 1 or 3 aio "
		   "completion queues\n");
	    printf("No support for %d queues\n", options.aio_complq_count);
	    exit(1);
	  }

	} else if (flag == &options.cache_max_file_size_dummy) {
	  options.cache_max_file_size = atof(optarg);
	} else if (flag == &options.read_buffer_size) {
	  options.read_buffer_size = atoi(optarg);
#ifdef HAVE_AIO_LAYER
	  if (options.read_buffer_size % OPT_CACHE_LINE_EST != 0) {
	    printf("read buffer size needs to be multiple of %d\n",
		  OPT_CACHE_LINE_EST);
	  }
#endif
	} else if (flag == &options.reply_buffer_size) {
	  options.reply_buffer_size = atoi(optarg);
#ifdef HAVE_AIO_LAYER
	  if (options.reply_buffer_size % OPT_CACHE_LINE_EST != 0) {
	    printf("reply buffer size needs to be multiple of %d\n",
		  OPT_CACHE_LINE_EST);
	  }
#endif
	} else if (flag == &options.dyn_buffer_size) {
	  options.dyn_buffer_size = atoi(optarg);
#ifdef HAVE_AIO_LAYER
	  if (options.dyn_buffer_size % OPT_CACHE_LINE_EST != 0) {
	    printf("dynamic content buffer size needs to be multiple of %d\n",
		  OPT_CACHE_LINE_EST);
	  }
#endif
	} else if (flag == &options.num_dyn_buffers) {
	  options.num_dyn_buffers = atoi(optarg);
	} else if (flag == &options.num_dyn_buffers_per_app) {
	  options.num_dyn_buffers_per_app = atoi(optarg);
	} else if (flag == &options.num_dyn_buffers_per_appserver) {
	  options.num_dyn_buffers_per_appserver = atoi(optarg);
	} else if (flag == &options.dyn_lock_pages) {
	  options.dyn_lock_pages = 1;
	} else if (flag == &options.dyn_touch_pages) {
	  options.dyn_touch_pages = 1;
	} else if (flag == &options.min_file_size_dummy) {
	  options.min_file_size = atof(optarg);
	} else if (flag == &options.max_file_size_dummy) {
	  options.max_file_size = atof(optarg);
	} else if (flag == &options.cache_max_load_factor_dummy) {
	  options.cache_max_load_factor = atof(optarg);
	} else if (flag == &options.aio_write_events_limit) {
	  options.aio_write_events_limit = atoi(optarg);
	} else if (flag == &options.aio_read_events_limit) {
	  options.aio_read_events_limit = atoi(optarg);
	} else if (flag == &options.aio_accept_events_limit) {
	  options.aio_accept_events_limit = atoi(optarg);
	} else if (flag == &options.aio_accept_thold) {
	  options.aio_accept_thold = atoi(optarg);
	} else if (flag == &options.is_aio_completion_order) {
	  strncpy(options.aio_completion_order,
		  optarg, OPT_AIO_COMPL_ORDER_LEN);
	} else if (flag == &options.idle_threshold) {
	  options.idle_threshold = atoi(optarg);
	} else if (flag == &options.cpu_mask_dummy) {
#ifdef HAVE_SCHED_AFFINITY
	  if (!strncmp(optarg, "0x", 2)) {
	    bitmap = strtoul(optarg, &end, 16);
	  } else {
	    bitmap = strtoul(optarg, &end, 10);
	  }
	  if (errno == ERANGE || end == optarg || *end != '\0')
	  {
	    fprintf(stderr, "%s: illegal CPU mask '%s'\n",
		     prog_name, optarg);
	    exit(1);
	  }

#ifdef HAVE_CPUSET
          CPU_ZERO(&tmp_mask);
          CPU_ZERO(&options.cpu_mask);
          for (i=0; i<CPU_SETSIZE; i++) {
            if (bitmap & 1) {
              CPU_SET(i, &options.cpu_mask);
            }
            bitmap = bitmap >> 1;
          } 

	  print_cpuset_values("Using CPU mask: \n", &options.cpu_mask);
	  printf("\n");
#else
          options.cpu_mask = bitmap;
	  printf("Using CPU mask: 0x%lx\n", options.cpu_mask);
#endif


#ifdef OLDWAY
	  rc = sched_setaffinity(getpid(), sizeof(options.cpu_mask),
	      &options.cpu_mask);
	    
	  if (rc < 0) {
	    printf("process_args: unable to set processor affinity mask\n");
	    printf("process_args: call returns %d errno = %d\n", rc, errno);
	    perror("unable to sched_setaffinity");
	    exit(1);
	  }
	  rc = sched_getaffinity(getpid(), sizeof(tmp_mask), &tmp_mask);
	  if (rc < 0) {
	    printf("process_args: unable to get processor affinity mask\n");
	    printf("process_args: call returns %d errno = %d\n", rc, errno);
	    perror("unable to sched_getaffinity");
	    exit(1);
	  }

#ifdef HAVE_CPUSET
	  // print_cpuset_values("Effective CPU mask: \n", &tmp_mask);
	  // printf("\n");
#else
	  // printf("Effective CPU mask: 0x%lx\n", tmp_mask);
#endif /* HAVE_CPUSET */
#endif /* OLDWAY */

#else
	  printf("process_args: sched affinity has not been enabled\n");
	  exit(1);
#endif /* HAVE_SCHED_AFFINITY */

	} else if (flag == &options.server_priority) {
	  options.server_priority = atoi(optarg);

	} else if (flag == &options.server_nice_level) {
	  options.server_nice_level = atoi(optarg);

	} else if (flag == (int *) &options.is_pid_filename) {
	  if (strlen(optarg) >= OPT_MAX_FILENAME_LEN) {
	    printf("Pid filename specified is too long %s\n", optarg);
	    printf("Use shorter name or increase OPT_MAX_FILENAME_LEN = %d\n",
		    OPT_MAX_FILENAME_LEN);
	    exit(1);
	  }
	  strncpy(options.pid_filename, optarg, OPT_MAX_FILENAME_LEN);
	  if (options.pid_filename[0] != '\0') {
	    printf("Using pid filename = [%s]\n", options.pid_filename);
	  } else {
	    printf("Not using a pid file\n");
	  }
	} else if (flag == (int *) &options.is_trace_filename) {
	  if (strlen(optarg) >= OPT_MAX_FILENAME_LEN) {
	    printf("Trace filename specified is too long %s\n", optarg);
	    printf("Use shorter name or increase OPT_MAX_FILENAME_LEN = %d\n",
		    OPT_MAX_FILENAME_LEN);
	    exit(1);
	  }
	  strncpy(options.trace_filename, optarg, OPT_MAX_FILENAME_LEN);
	  printf("Using trace filename = [%s]\n", options.trace_filename);

	} else if (flag == &options.cache_warm) {
	  if (strlen(optarg) >= OPT_MAX_FILENAME_LEN) {
	    printf("Warm cache filename specified is too long %s\n", optarg);
	    printf("Use shorter name or increase OPT_MAX_FILENAME_LEN = %d\n",
	    OPT_MAX_FILENAME_LEN);
	    exit(1);
	  }
	  strncpy(options.cache_warm_file, optarg, OPT_MAX_FILENAME_LEN);
	  printf("Using warm cache filename = [%s]\n", options.cache_warm_file);

	} else if (flag == &options.cache_warm_size) {

	  options.cache_warm_size = atoi(optarg);
	  if (options.cache_warm_size == 0) {
	    options.cache_warm_size = getpagesize();
	  }
	  printf("Using warm cache size = %d\n", options.cache_warm_size);

	} else if (flag == &options.cache_warm_stride) {
	  options.cache_warm_stride = atoi(optarg);
	  printf("Using warm cache stride = %d\n", options.cache_warm_stride);

	} else if (flag == &options.cache_warm_loops) {
	  options.cache_warm_loops = atoi(optarg);
	  printf("Using warm cache loops = %d\n", options.cache_warm_loops);

	} else if (flag == &options.victim) {
	  if (strlen(optarg) >= OPT_MAX_FILENAME_LEN) {
	    printf("Victim filename specified is too long %s\n", optarg);
	    printf("Use shorter name or increase OPT_MAX_FILENAME_LEN = %d\n",
	    OPT_MAX_FILENAME_LEN);
	    exit(1);
	  }
	  strncpy(options.victim_str, optarg, OPT_MAX_FILENAME_LEN);
	  printf("Setting victim string = [%s]\n", options.victim_str);

	} else if (flag == &options.victim_skip) {
	  options.victim_skip = atoi(optarg);
	  printf("Setting victim skip to %d\n", options.victim_skip);

          if (options.victim_skip == OPT_SKIP_DO_MADVISE) {
	    printf("Victim to use madvise\n");
          }


	} else if (flag == &options.cache_miss_skip) {
	  options.cache_miss_skip = atoi(optarg);
	  printf("Setting cache miss skip to %d\n", options.cache_miss_skip);


#ifdef HAVE_ENCRYPTION
	} else if (flag == (int *) &options.encrypt_percent) {
	   options.encrypt_percent = atoi(optarg);
	   if ((options.encrypt_percent < 0) || (options.encrypt_percent > 100)) {
	     printf("Invalid encryption percentage; must be >= 0 and <= 100\n");
	     printf("Encryption percentage set to 0\n");
	     options.encrypt_percent = 0;
	   }  

	} else if (flag == (int *) &options.encrypt_max_file_size) {
	  options.encrypt_max_file_size = strtoul(optarg, &p, 0);
	  if (*p != '\0') {
	    printf("Invalid encryption maximum file size: using 0\n");
	    options.encrypt_max_file_size = 0UL;
	  }
#endif /* HAVE_ENCRYPTION */

	} else if (flag == (int *) &options.is_ip_addr) {
	  if (options.ip_addr_count == OPT_MAX_IP_ADDRS) {
	    printf("Too many IP addresses specified\n");
	    exit(1);
	  }
	  strncpy(options.ip_addr[options.ip_addr_count],
		  optarg, OPT_MAX_IP_ADDR_LEN);
	  printf("Listening on IP addr [%d] = [%s]\n",
	      options.ip_addr_count, options.ip_addr[options.ip_addr_count]);
	  options.ip_addr_count++;
	  printf("ip_addr_count now = %d\n", options.ip_addr_count);
	} else if (flag == &options.use_sendfile) {
	  options.use_sendfile = 1;
        } else if (flag == &options.blocking_sendfile) {
          options.blocking_sendfile = 1;

	} else if (flag == &options.auto_accept_aperture) {
#ifdef SEND_AUTO_ACCEPT_CONTROL
	  options.auto_accept_aperture = atoi(optarg);
	  printf("auto-accept-aperture set to %d\n",
	      options.auto_accept_aperture);
	  if (options.auto_accept_aperture < 0) {
	    printf("auto-accept-aperture must be >= 0\n");
	    exit(1);
	  }
#else
	  printf("auto-accept-aperture not supported in this version\n");
	  printf("define SEND_AUTO_ACCEPT_CONTROL in the "
		 "Makefile and recompile\n");
	  exit(1);
#endif /* SEND_AUTO_ACCEPT_CONTROL */
	} else if (flag == (int *) &options.is_doc_root) {
	  len = strlen(optarg);
	  /* see if we'll need to append a slash */
	  if (optarg[len - 1] != '/') {
	    len++;
	  }
	  if (len >= OPT_MAX_PATH_LEN) {
	    printf("document root specified is too long %s\n", optarg);
	    printf("Use shorter name or increase OPT_MAX_PATH_LEN = %d\n",
		    OPT_MAX_PATH_LEN);
	    exit(1);
	  }
	  strcpy(options.doc_root, optarg);

	  rc = stat(options.doc_root, &stat_buf);
	  if (rc < 0) {
	    perror("stat failed");
	    printf("failed on doc_root = %s\n", options.doc_root);
	    exit(1);
	  }

	  if (!S_ISDIR(stat_buf.st_mode)) {
	    printf("doc_root = %s is not a directory\n", options.doc_root);
#ifdef SUNOS
	    printf("doc_root stat_buf.st_mode = 0x%lx\n", stat_buf.st_mode);
#else
	    printf("doc_root stat_buf.st_mode = 0x%x\n", stat_buf.st_mode);
#endif
	    exit(1);
	  }

	  /* if necessary, append a slash (len was incremented above) */
	  if (options.doc_root[len - 1] == '\0') {
	    options.doc_root[len - 1] = '/';
	    options.doc_root[len] = '\0';
	  }
	  options.doc_root_len = len;
	  printf("Using doc root = [%s]\n", options.doc_root);

	} else if (flag == (int *) &options.is_hostname) {
	  if (strlen(optarg) >= OPT_MAX_HOSTNAME_LEN) {
	    printf("Server path too long\n");
	    exit(1);
	  }
	  strcpy(options.hostname, optarg);

	} else if (flag == (int *) &options.is_app) {
	  /*
	   * Split the argument into fields.  Expected format is:
	   *   uri,type[,path[,count]]
	   */
	  uri_arg = optarg;
	  type_arg = strchr(uri_arg, ',');
	  if (type_arg != NULL) {
	    *type_arg++ = '\0';
	    path_arg = strchr(type_arg, ',');
	    if (path_arg != NULL) {
	      *path_arg++ = '\0';
	      count_arg = strchr(path_arg, ',');
	      if (count_arg != NULL) {
		*count_arg++ = '\0';
	      }
	    } else {
	      count_arg = NULL;
	    }
	  } else {
	    path_arg = NULL;
	    count_arg = NULL;
	  }

	  if (type_arg == NULL) {
	    printf("No application type specified!\n");
	    printf("Expected format: uri,type[,path[,count]]\n");
	    exit(1);
	  } else if (!stricmp(type_arg, "FastCGI")) {
	    if (path_arg == NULL) {
	      printf("No application path specified!\n");
	      printf("Expected format for FastCGI: uri,type,path[,count]\n");
	      exit(1);
	    }
	    req_type = REQ_FASTCGI;
	    appserver_flags = 0;
	  } else if (!stricmp(type_arg, "FastCGI-sharedmem")) {
	    if (path_arg == NULL) {
	      printf("No application path specified!\n");
	      printf("Expected format for FastCGI-sharedmem: uri,type,path[,count]\n");
	      exit(1);
	    }
	    req_type = REQ_FASTCGI;
	    appserver_flags = APPSERVER_SHAREDMEM;
	    options.dyn_use_shared_mem++;
#ifdef MOD_SPECWEB99
	  } else if (!stricmp(type_arg, "SPECweb99")) {
	    if (path_arg != NULL) {
	      printf("Application path specified!\n");
	      printf("Expected format for SPECweb99: uri,type\n");
	      exit(1);
	    }
	    req_type = REQ_SPECWEB99;
	    appserver_flags = 0;
#endif
	  } else {
	    printf("Unknown application type [%s]\n", type_arg);
	    exit(1);
	  }

	  /* strip leading slashes from the URI */
	  while (*uri_arg == '/') {
	    uri_arg++;
	  }

	  /* find an existing application matching the URI, or create one */
	  app = find_app(uri_arg);
	  if (app == NULL) {
	    app = create_app(uri_arg, req_type);
	    if (app == NULL) {
	      printf("Failed to create application\n");
	      exit(1);
	    }
	  } else {
	    printf("Adding server to existing application\n");
	  }

	  /* add application server(s) */
	  if (path_arg != NULL) {
	    if (count_arg != NULL) {
	      /*
	       * This optional count argument is a quick way to specify a
	       * range of IP addresses.
	       * So specifying uri,type,ipaddr:port,n is the same as
	       * specifying all of these:
	       *    uri,type,ipaddr:port
	       *    uri,type,ipaddr:port+1
	       *    ...
	       *    uri,type,ipaddr:port+(n-1)
	       * Obviously, this shortcut only works for IP addresses, not
	       * UNIX domain paths.
	       */
	      switch (req_type) {
		case REQ_FASTCGI:
		  break;
		default:
		  printf("Count value is only valid for FastCGI apps\n");
		  exit(1);
	      }
	      count = atoi(count_arg);
	      if (count < 1) {
		printf("Bad server count: %d\n", count);
		exit(1);
	      }
	      if (sock_path_to_addr(path_arg, &addr, 1, OPT_IP_ADDR_ANY,
		    DEF_APP_SERVER_PORT) < 0) {
		printf("Can't convert '%s' to sock addr\n", path_arg);
		exit(1);
	      }
	      switch (addr.u.sa.sa_family) {
		case AF_INET:
		  break;
		default:
		  printf("Count value is only valid for IP addresses\n");
		  exit(1);
	      }
	      if (strlen(path_arg) >= MAX_APP_SERVER_PATH_LEN) {
		printf("Server path too long\n");
		exit(1);
	      }
	      port_arg = strchr(path_arg, ':');
	      if (port_arg != NULL) {
		*port_arg = '\0';  // trim off the port specification
	      }
	      for (i = 0; i < count; i++) {
		sprintf(serverpath, "%s:%d", path_arg,
		    ntohs(addr.u.sa_in.sin_port) + i);
		if (app_add_server(app, serverpath, appserver_flags) < 0) {
		  printf("Failed to add application server\n");
		  exit(1);
		}
	      }
	    } else {
	      if (app_add_server(app, path_arg, appserver_flags) < 0) {
		printf("Failed to add application server\n");
		exit(1);
	      }
	    }
	  }

#ifdef CALL_STATS
	} else if (flag == &options.call_stats) {
	  options.call_stats = atoi(optarg);
	  printf("call-stats size = %d\n", options.call_stats);
          call_stats_init(options.call_stats);
#endif /* CALL_STATS */
	} else if (flag == &options.app_req_queue_size) {
	  options.app_req_queue_size = atoi(optarg);
	  if (options.app_req_queue_size < 0) {
	    printf("app-req-queue-size [%d] must be >= 0\n",
		options.app_req_queue_size);
	    exit(1);
	  }

	} else if (flag == (int *) &options.is_start_app_server) {
	  /*
	   * Split the argument into fields.  Expected format is:
	   *   path[,count][="exec string without a comma"][,appserv_cpu_mask]
	   */
	  path_arg = optarg;
	  exec_arg = strchr(path_arg, '=');
	  if (exec_arg != NULL) {
	    *exec_arg++ = '\0';
            mask_arg = strchr(exec_arg, ',');
            if(mask_arg != NULL) {
              *mask_arg++ = '\0'; 
              if(*mask_arg != '\0') {
#ifdef HAVE_SCHED_AFFINITY
		  if (!strncmp(mask_arg, "0x", 2)) {
		    appserv_bitmap = strtoul(mask_arg, &end, 16);
		  } else {
		    appserv_bitmap = strtoul(mask_arg, &end, 10);
		  }
		  printf("appserv_bitmap = 0x%lx\n", appserv_bitmap);
		  if (errno == ERANGE || end == mask_arg || *end != '\0') {
		    fprintf(stderr, "%s: illegal CPU mask '%s'\n",
			prog_name, mask_arg);
		    exit(1);
		  }
#ifdef HAVE_CPUSET
		  CPU_ZERO(&appserv_cpu_mask);
		  for (i=0; i<CPU_SETSIZE; i++) {
		    if (appserv_bitmap & 1) {
		      CPU_SET(i, &appserv_cpu_mask);
		    }
		    appserv_bitmap = appserv_bitmap >> 1;
		  } 

		  printf("appserv_bitmap = 0x%lx\n", appserv_bitmap);
		  print_cpuset_values("Using appserver CPU mask: \n", &appserv_cpu_mask);
#else
                  appserv_cpu_mask = appserv_bitmap;
	          printf("Using appserver CPU mask: 0x%lx\n", appserv_cpu_mask);
#endif /* HAVE_CPUSET */
#else
	 	printf("CPU mask/affinity was not enabled during compilation\n");
			exit(1);
#endif /* HAVE_SCHED_AFFINITY */
	      }
            } 
            if (*exec_arg == '"' ) exec_arg++;  /*Strip the quotes if present */
            exec_strlen = strlen(exec_arg);
            if (exec_arg[exec_strlen - 1] == '"' ) exec_arg[--exec_strlen] = '\0'; 
	  }
	  count_arg = strchr(path_arg, ',');
	  if (count_arg != NULL) {
	    *count_arg++ = '\0';
	  }

	  /* record the application server start info (for later use) */
	  if (count_arg != NULL) {
	    /*
	     * This optional count argument is a quick way to specify a
	     * range of IP addresses.
	     * So specifying ipaddr:port,n[="exec string"] is the same as
	     * specifying all of these:
	     *    ipaddr:port[="exec string"]
	     *    ipaddr:port+1[="exec string"]
	     *    ...
	     *    ipaddr:port+(n-1)[="exec string"]
	     * Obviously, this shortcut only works for IP addresses, not
	     * UNIX domain paths.
	     */
	    count = atoi(count_arg);
	    if (count < 1) {
	      printf("Bad server count: %d\n", count);
	      exit(1);
	    }
            if (count > options.appserver_count) {
              printf("appserver_startupinfo_count: %d more than appserver_count: %d\n", count,options.appserver_count);
              exit(1);
            }
	    if (sock_path_to_addr(path_arg, &addr, 1, OPT_IP_ADDR_ANY,
		  DEF_APP_SERVER_PORT) < 0) {
	      printf("Can't convert '%s' to sock addr\n", path_arg);
	      exit(1);
	    }
	    switch (addr.u.sa.sa_family) {
	      case AF_INET:
		break;
	      default:
		printf("Count value is only valid for IP addresses\n");
		exit(1);
	    }
	    if (strlen(path_arg) >= MAX_APP_SERVER_PATH_LEN) {
	      printf("Server path too long\n");
	      exit(1);
	    }
	    port_arg = strchr(path_arg, ':');
	    if (port_arg != NULL) {
	      *port_arg = '\0';  // trim off the port specification
	    }
	    for (i = 0; i < count; i++) {
	      sprintf(serverpath, "%s:%d", path_arg,
		      ntohs(addr.u.sa_in.sin_port) + i);
#ifdef HAVE_SCHED_AFFINITY
	      if (app_add_startinfo(serverpath, exec_arg, appserv_cpu_mask) < 0) {
#else
	      if (app_add_startinfo(serverpath, exec_arg) < 0) {
#endif /* HAVE_SCHED_AFFINITY */
		printf("Failed to add application server start info\n");
         	exit(1);
	      }
	    }
	  } else {
            if(options.appserver_count > 1) {
              printf("appserver_startinfo_count: 1 more than appserver_count: %d\n",options.appserver_count);
              exit(1);
            }
#ifdef HAVE_SCHED_AFFINITY
	    if (app_add_startinfo(path_arg, exec_arg, appserv_cpu_mask) < 0) {
#else
	    if (app_add_startinfo(path_arg, exec_arg) < 0) {
#endif /* HAVE_SCHED_AFFINITY */
	      printf("Failed to add application server start info\n");
              exit(1);
	    }
	  }

	} else if (flag == (int *) &options.server_id) {
	  options.server_id = atoi(optarg);
	  if (options.server_id < 0) {
	    printf("server-id must be >= 0\n");
	    exit(1);
	  }
	} else {
	  printf("long option index = %d name = %s flag = %p\n",
	     longindex, long_options[longindex].name, flag);
	  printf("long option value = %d\n", *flag);
	}
	break;
#endif /* USE_GETOPT_LONG */

      case '-':
	printf("-- long options form not supported on this system\n");
	exit(-1);
	break;

      case 'A':
	options.accepts_only = 1;
	options.get_connections_in_server_loop = 1;
	break;

      case 'B':
	options.send_block_for_events = 1;
	break;

      case 'C':
	options.caching_on = 1;
	break;

      case 'D':
#ifdef DEBUG_ON
	if (!strncmp(optarg, "0x", 2)) {
	  MSG_MASK = strtoul(optarg, &end, 16);
	} else {
	  MSG_MASK = strtoul(optarg, &end, 10);
	}
	if (errno == ERANGE || end == optarg || *end != '\0')
	{
	  fprintf(stderr, "%s: illegal debug mask '%s'\n",
	      prog_name, optarg);
	  exit(1);
	}
	printf("Message dmask is now = 0x%x\n", MSG_MASK);
#else
	printf("-%c option - not enabled\n", ch);
	printf("ignoring option\n");
	/* exit(1); */
#endif /* DEBUG_ON */
	break;

      case 'E':
	options.get_connections = OPT_CONN_WITH_SEND_EVTS;
	options.conn_on = OPT_CONN_WITH_SEND_EVTS;
	options.conn_off = OPT_CONN_WITH_SEND_EVTS;
	options.send_events = 1;
	break;

      case 'F':
	options.close_with_reset = 1;
	break;

      case 'G':
	options.use_getpid = 1;
	break;

      case 'H':
	usage();
	exit(0);
	break;

#ifndef SUNOS
      case 'I':
	options.read_sock_buf_lowat = atoi(optarg);
	break;

      case 'J':
	options.write_sock_buf_lowat = atoi(optarg);
	break;
#else
      case 'I':
      case 'J':
	printf("Changing socket buffer low water mark not permitted on "
	    "SUNOS\n");
	exit(1);
	break;
#endif /* ! SUNOS */

      case 'K':
	if (!strncmp(optarg, "0x", 2)) {
	  options.send_kdebug = strtoul(optarg, &end, 16);
	} else {
	  options.send_kdebug = strtoul(optarg, &end, 10);
	}
	if (errno == ERANGE || end == optarg || *end != '\0')
	{
	  fprintf(stderr, "%s: illegal kdebug mask '%s'\n",
	      prog_name, optarg);
	  exit(1);
	}
	printf("Send kernel debug mask is now = 0x%x\n",
	    options.send_kdebug);
	break;

      case 'L':
	if (optarg == 0 || optarg[0] == '-') {
	  printf("ERROR: incorrect specification of loop on readable %s\n",
		  optarg);
	  usage();
	  exit(1);
	}
	options.loop_on_readable = atoi(optarg);
	break;

      case 'M':
	if (optarg == 0 || optarg[0] == '-') {
	  printf("ERROR: incorrect specification of max connections %s\n",
		  optarg);
	  usage();
	  exit(1);
	}
	options.do_multiaccept = OPT_MULTIACCEPT_BY_SYSCALL;
	options.multiaccept_max = atoi(optarg);
	if (options.multiaccept_max <= 0) {
	  options.multiaccept_max = OPT_MULTIACCEPT_MAX_UNLIMITED;
	}
#ifndef MACCEPT
	printf("ERROR: multiaccept system call not installed/supported\n");
	exit(1);
#endif
	break;

      case 'N':
	options.send_loop = 1;
	break;

      case 'O':
	options.select_timeout = atoi(optarg);
	break;

      case 'P':
	options.count_sigpipes = 1;
	break;

      case 'Q':
	options.ecb_threshold_high = atoi(optarg);
	break;

      case 'R':
	options.read_sock_buf_size = atoi(optarg);
	break;

      case 'S':
	options.dont_intr_select = 1;
	break;

      case 'T':
	options.port = atoi(optarg);
	break;

      case 'U':
	options.auto_accept = 1;
	break;

      case 'V':
	options.use_sendfile = 1;
	break;

      case 'W':
	options.write_sock_buf_size = atoi(optarg);
	break;

      case 'X':
	options.send_selects_for_accepts = 1;
	options.get_connections = OPT_CONN_WITH_SEND_SELECT;
	options.conn_on = OPT_CONN_WITH_SEND_SELECT;
	options.conn_off = OPT_CONN_WITH_SEND_SELECT;
	break;

      case 'Y':
	options.send_io_events = 1;
	/* options.send_loop = 1; */
	break;

      case 'Z':
	options.content_type = 1;
	break;

      case 'a':
	options.asynch_mode = 1;
	/* some options that might work well with 'a' are: */
	/* options.suspend_on_idle = 1; */
	/* options.get_connections_in_server_loop = 1; */
	break;

      case 'b':
	options.ecb_size = atoi(optarg);
	break;

      case 'c':
	maxconns = atoi(optarg);
	printf("process_args: maxconns = %d\n", maxconns);
	break;

      case 'd':
	options.delay = atoi(optarg);
	break;

      case 'e':
	options.extra_freq = atoi(optarg);
	break;

      case 'f':
	max_fds = atoi(optarg);
	printf("Using max_fds = %d\n", max_fds);
	if (max_fds < 10) {
	  printf("A minimum number of fds are needed - setting max_fds to 10\n");
	  max_fds = 10;
	}

	break;

      case 'g':
	options.get_connections_on_close = 1;
	break;

      case 'h':
	usage();
	exit(0);
	break;

      case '?':
	usage();
	exit(optopt != '?');
	break;

      case 'i':
	options.interactive = 1;
	break;

      case 'j':
	options.kinfo = 1;
	break;

      case 'k':
	options.sigio_cntl = OPT_SIGIO_USE_PROCMASK;
	break;

      case 'l':
	options.listenq = atoi(optarg);
	break;

      case 'n':
	options.noaccepts = 1;
	break;

      case 'm':
	if (optarg == 0 || optarg[0] == '-') {
	  printf("ERROR: incorrect specification of max connections %s\n",
		  optarg);
	  usage();
	  exit(1);
	}
	options.do_multiaccept = OPT_MULTIACCEPT_BY_REPEATING;
	options.multiaccept_max = atoi(optarg);
	if (options.multiaccept_max <= 0) {
	  options.multiaccept_max = OPT_MULTIACCEPT_MAX_UNLIMITED;
	}
	break;

      case 'o':
	if (strcmp(optarg,"up") == 0) {
	  options.process_sds_order = OPT_PROCESS_SDS_UP;
	} else if (strcmp(optarg,"down") == 0) {
	  options.process_sds_order = OPT_PROCESS_SDS_DOWN;
	} else if (strcmp(optarg,"writes-down") == 0) {
	  options.process_sds_order = OPT_PROCESS_SDS_WRITES_FIRST_DOWN;
	} else if (strcmp(optarg,"writes-up") == 0) {
	  options.process_sds_order = OPT_PROCESS_SDS_WRITES_FIRST_UP;
	} else if (strcmp(optarg,"lru") == 0) {
	  options.process_sds_order = OPT_PROCESS_SDS_LRU;
	  printf("WARNING: current implementation of LRU is VERY SLOW\n");
	} else if (strcmp(optarg,"lifo") == 0) {
	  options.process_sds_order = OPT_PROCESS_SDS_LIFO;
	} else if (strcmp(optarg,"fifo") == 0) {
	  options.process_sds_order = OPT_PROCESS_SDS_FIFO;
	} else {
	  printf("Unrecognized option: -o %s\n", optarg);
	  usage();
	  exit(1);
	}
	break;

      case 'p':
	numprocs = atoi(optarg);
	if (numprocs <= 0)
	  {
	    fprintf(stderr, "%s: argument to -p should be positive\n",
		     prog_name);
	    exit(-1);
	  }
	break;

      case 'q':
	options.ecb_threshold_low = atoi(optarg);
	break;

      case 'r':
	options.call_readable_from_new_conn = 1;
	break;

      case 's':
	options.get_connections_in_server_loop = 1;
	break;

      case 't':
	options.free_fd_thold = atoi(optarg);
	break;

      case 'u':
	options.use_max_open_sd = 1;
	break;

      case 'v':
	options.use_poll = 1;
	break;

      case 'w':
	options.call_writable_from_readable = 1;
	break;

      case 'x':
	/* use sigio interrupts to get new connections */
	options.get_connections = OPT_CONN_WITH_SIGIO;
	options.conn_on = OPT_CONN_WITH_SIGIO;
	options.conn_off = OPT_CONN_WITH_SIGIO;
	options.asynch_mode = 1;
	break;

      case 'y':
	options.use_memcpy = 1;
	break;

      case 'z':
	options.rejection_rate = atoi(optarg);
	if (options.rejection_rate < 0 || options.rejection_rate > 100) {
		printf("Option error: [-z/--rejection-rate] must be "
		       "between 0 and 100, inclusive");
		exit(1);
	}
	break;

      case '1':
#ifdef HAVE_EPOLL
	options.use_epoll = 1;
#else
	printf("userver not compiled with EPOLL support or\n");
	printf("This system does not support epoll\n");
	exit(1);
#endif
	break;

      case '2':
#ifdef HAVE_EPOLL
	options.use_epoll2 = 1;
#else
	printf("userver not compiled with EPOLL support or\n");
	printf("This system does not support epoll\n");
	exit(1);
#endif
	break;

      case '3':
#ifdef HAVE_EPOLL_CTLV
	options.use_epoll_ctlv = 1;
#else
	printf("userver not compiled with EPOLL support or\n");
	printf("This system does not support epoll_ctlv\n");
	exit(1);
#endif
	break;

      case '4':
	options.epoll_edge_triggered = 1;
	break;

      case '5':
	options.epoll_modify_listeners = 1;
	break;

      /* --epoll-good : this represents a good combination
       *                    of parameters to use with epoll
       * Other combations may be broken (temporarily)
       */

      case '6':
	options.epoll_modify_listeners = 1;
	options.epoll_edge_triggered = 1;
	options.use_epoll2 = 1;
	break;

#ifdef HAVE_SSL 
      case '7':
	options.ssl_port = atoi(optarg);
	printf("using ssl port = %d\n", options.ssl_port);
        break;
#endif /*  HAVE_SSL */

      default:
	printf("long option index = %d name = %s flag = %p\n",
	     longindex, long_options[longindex].name, flag);
	printf("long option value = %d\n", *flag);
	usage();
	exit(1);
    }
  }

  if (cfg_file_depth > 0) {
    return;
  }

  if (do_version) {
    printf("Version = %s\n", version);
    exit(1);
  }

  printf("Server identification: %s\n", server_ident);

  if (options.use_socket_aio) {
    options.use_aio_accept = 1;
    options.use_aio_read = 1;
    options.use_aio_write = 1;
    options.use_aio_close = 1;
    options.get_connections = OPT_CONN_WITH_AIO_ACCEPT;
    options.conn_on = OPT_CONN_WITH_AIO_ACCEPT;
    options.conn_off = OPT_CONN_WITH_AIO_ACCEPT;
    options.use_sendfile = 0;
    options.blocking_sendfile = 0;
    options.use_tcp_cork = 0;
#ifdef AIO_FAKE
    options.use_aio_wait = 1;
#endif /* AIO_FAKE */
  }
} /* process_args */


/* Check the command line options that were supplied. Certain
 * combinations of options are incompatible, and we aim to check
 * for those combinations here. Also, some options are inconsistent
 * with system limits, so we also check for those cases here.
 */ 
void 
check_options() 
{
  int i;
  
  if (options.victim_skip == OPT_SKIP_DO_MADVISE && !options.caching_on) {
    printf("If specifying --victim-skip %d you must have caching turned on\n", OPT_SKIP_DO_MADVISE);
    exit(1);
  } 

  if (options.victim_skip != 0) {
    if (options.victim_str[0] == '\0') {
      printf("If specifying --victim-skip you must also specify a victim string using --victim\n");
      exit(1);
    }
  }

  if (options.ignore_fd_setsize && need_fd_setsize()) {
    printf("ERROR IN OPTION SELECTION\n");
    printf("Trying to avoid using FD_SETSIZE but the options chosen require it\n");
    printf("You may be using select and/or --use-max-open-sd\n");
    printf("and --ignore-fd-setsize is not compatible with these options\n");
    exit(1);
  }

  /* Check that number of connections is <= FD_SETSIZE */
  if (maxconns > FD_SETSIZE) {
    printf("Trying to allow more open connections than permitted by FD_SETSIZE\n");
    printf("FD_SETSIZE = %d maxconns = %d\n", FD_SETSIZE, maxconns);
    if (options.ignore_fd_setsize && !need_fd_setsize()) {
      printf("Ignoring maxconns > FD_SETSIZE\n");
    } else {
      printf("ERROR IN OPTION SELECTION\n");
      printf("Increase FD_SETSIZE (see README file for instructions\n");
      exit(1);
    }
  }

  /* Check that number of open files is <= FD_SETSIZE */
  if (max_fds > FD_SETSIZE) {
    printf("Trying to allow more open files than permitted by FD_SETSIZE\n");
    printf("FD_SETSIZE = %d max_fds = %d\n", FD_SETSIZE, max_fds);
    if (options.ignore_fd_setsize && !need_fd_setsize()) {
      printf("Ignoring max_fds > FD_SETSIZE\n");
    } else {
      printf("ERROR IN OPTION SELECTION\n");
      printf("Increase FD_SETSIZE (see README file for instructions\n");
      exit(1);
    }
  }

  if (options.trace_summary_only) {
    options.trace_summary = 1;
  }

  /* Check epoll options */
  if (options.use_epoll && options.use_epoll2) {
    printf("ERROR IN OPTION SELECTION\n");
    printf("Error: Can't use both epoll and epoll2 at the same time\n");
    exit(1);
  }

  if (options.use_epoll2 && options.appserver_count > 0) {
    printf("ERROR IN OPTION SELECTION\n");
    printf("Error: epoll2 is not currently implemented for dynamic workloads\n");
    exit(1);
  }

  if (options.epoll_edge_triggered && options.appserver_count > 0) {
    printf("ERROR IN OPTION SELECTION\n");
    printf("Error: edge-triggered epoll is not currently implemented for dynamic workloads\n");
    exit(1);
  }

  if (options.send_loop && options.use_poll) {
    printf("ERROR IN OPTION SELECTION\n");
    printf("Error: Can't use both send events and poll at the same time\n");
    exit(1);
  }

  if ((options.use_poll && options.use_epoll) ||
      (options.use_poll && options.epoll_edge_triggered) ||
      (options.use_poll && options.use_epoll2)) {
    printf("ERROR IN OPTION SELECTION\n");
    printf("Can't use both poll and any form of epoll at the same time\n");
    exit(1);
  }

  if (max_fds < maxconns) {
    printf("ERROR IN OPTION SELECTION\n");
    printf("ERROR: max_fds = %d is too small for maxconns = %d\n",
	max_fds, maxconns);
    exit(1);
  }

#ifdef HAVE_AIO_LAYER
  /* for the aio layer we want sds to go from OPT_AIO_LOWEST 
   * to (OPT_AIO_LOWEST + maxconns -1) inclusive
   */
  if (maxconns + OPT_AIO_LOWEST_SD - 1 > max_fds) {
    printf("ERROR IN OPTION SELECTION\n");
    printf("max_fds = %d isn't large enough\n", max_fds);
    printf("must be > maxconns + OPT_AIO_LOWEST_SD = %d + %d = %d\n",
            maxconns, OPT_AIO_LOWEST_SD, (maxconns + OPT_AIO_LOWEST_SD));
    exit(1);
  }
#endif /* HAVE_AIO_LAYER */

  if (options.get_connections == OPT_CONN_WITH_SIGIO) {
    if (options.get_connections_on_close == 0 && 
	options.get_connections_in_server_loop == 0) {
       printf("\n");
       printf("WARNING: when using SIGIO for accept likely also need\n");
       printf("         another method for getting these events\n");
    }
  }

  if (options.sigio_cntl == OPT_SIGIO_USE_PROCMASK) {
    if (options.get_connections != OPT_CONN_WITH_SIGIO) {
      printf("ERROR IN OPTION SELECTION\n");
      printf("Error in specifying options - must use -a with -k\n");
      exit(1); 
    }
  }

  if (options.get_connections == OPT_CONN_WITH_SIGIO) {
    if (max_fds < (2 * maxconns)) {
      printf("WARNING: max_fds = %d may not be large enough for "
	  "maxconns = %d\n", max_fds, maxconns);
      printf("WARNING: likely better to use max_fds >= 2 * maxconns\n");
    }
  }

  printf ("%s: %d processes, up to %d concurrent connections/process.\n",
	  prog_name, numprocs, maxconns);

  if ((options.auto_accept) && (!options.accepts_only)) {
    printf("Turning on options.send_io_events\n");
    printf("   auto_accept is on and not doing accepts only\n");
    options.send_io_events = 1;
  }


  if ((options.use_tcp_cork) && (!options.use_sendfile)) {
    printf("ERROR IN OPTION SELECTION\n");
    printf("Error: Current implementation only supports use-tcp-cork with "
	"use-sendfile\n");
    exit(-1);
  }

  if ((options.blocking_sendfile) && (!options.use_sendfile)) {
    printf("ERROR IN OPTION SELECTION\n");
    printf("Error: Must specify --use-sendfile whenever --blocking-sendfile is specified\n");
    exit(-1);
  }

  /* blocking select/poll/epoll/etc calls need to timeout 
   * in order to print stats 
   */
  if (options.stats_interval) {
    if (options.select_timeout < 0) {
      printf("ERROR IN OPTION SELECTION\n");
      printf("Can't use a stats interval and an indefinite timeout value\n");
      printf("Change either --stats-interval or --select-timeout\n");
      exit(1);
    } else {
      options.select_timeout = options.stats_interval;
      printf("Setting select timeout to stats interval = %d\n", options.stats_interval);
    }
  }

  /* check that the order specified for handling aio completions is valid */
  for (i = 0; i < (int) strlen(options.aio_completion_order); i++) {
    switch (options.aio_completion_order[i]) {
      case 'a':
      case 'A':
      case 'r':
      case 'R':
      case 'w':
      case 'W':
         break;
   
      default:
        printf("ERROR IN OPTION SELECTION\n");
        printf("Unknown character in aio completion order option = %c\n", 
                options.aio_completion_order[i]);
        printf("The only valid characters are [rwa]\n");
        exit(1);
    }
  }

  assert(options.appserver_count < MAX_APPS );
  if (options.appserver_count == 0) {
    options.num_dyn_buffers = 0;
  } else {
    if (options.num_dyn_buffers == -1) {
      if (options.num_dyn_buffers_per_app == -1) {
	options.num_dyn_buffers =
	  options.num_dyn_buffers_per_appserver * options.appserver_count;
      } else {
	options.num_dyn_buffers =
	  options.num_dyn_buffers_per_app * options.app_count;
      }
    }
    if (options.num_dyn_buffers < 1) {
      printf("ERROR IN OPTION SELECTION\n");
      printf("Error: num-dyn-buffers [%d] must be > 0\n",
	  options.num_dyn_buffers);
      exit(1);
    }
    if (options.app_req_queue_size == -1) {
      options.app_req_queue_size = maxconns;
    }
    if (options.app_req_queue_size < 0) {
      printf("ERROR IN OPTION SELECTION\n");
      printf("Error: app-req-queue-size [%d] must be >= 0\n",
	  options.app_req_queue_size);
      exit(1);
    }
  }

  if (app_match_startinfo_all() < 0) {
    printf("ERROR IN OPTION SELECTION\n");
    printf("Error: Application servers must be defined using --app before they can be started!\n");
    exit(1);
  }

#if !defined(__linux__) && !defined(HPUX) && \
    !defined(__CYGWIN__) && !defined(FreeBSD)
  if (options.use_sendfile) {
    printf("ERROR IN OPTION SELECTION\n");
    printf("Error: The use of sendfile hasn't been implemented on "
	"this platform\n");
    exit(1);
  }
#endif

#if !defined(__linux__)
  if (options.use_tcp_cork) {
    printf("IGNORING --use-tcp-cork on non Linux system\n");
  }
#endif

#if !defined(__FreeBSD__)
  if (options.cache_miss_skip) {
    printf("--cache-miss-skip not available on non BSD systems\n");
    exit(1);
  }
#endif

  if (options.cache_warm) {
    if (!options.caching_on) {
      printf("ERROR IN OPTION SELECTION\n");
      printf("Error: trying to warm cache without turning on caching\n");
      exit(1);
    }
  }

  if (options.fake_writing) {
    if (options.use_sendfile) {
      printf("ERROR IN OPTION SELECTION\n");
      printf("Can't use --fake-writing with --use-sendfile\n");
      exit(1);
    }

    if (options.caching_on) {
      printf("ERROR IN OPTION SELECTION\n");
      printf("Can't use --fake-writing with --caching-on\n");
      exit(1);
    }
  }
} /* check_options */


/* ---------------------------------------------------------------------- */

void
adjust_connections(int maxconns)
{
  struct rlimit rlim;

  /* check to see what limits are placed on us */
  if (getrlimit(RLIMIT_NOFILE, &rlim) < 0) {
    perror("getrlimit");
    exit(-1);
  }

  /* if our current limits aren't sufficient try to bump them. */
  if ((unsigned) maxconns > rlim.rlim_max - 10) {
		   
    /* Note that to make things easier to run experiments I've
     * been using a modified version of the kernel that allows
     * a regular user to change these limits 
     */

    /* on BSD this isn't a long int so cast it */
    printf("Trying to set max file limit - currently = %ld\n", 
            (long int) rlim.rlim_max);
    rlim.rlim_cur = FD_SETSIZE;
    rlim.rlim_max = FD_SETSIZE;

    if (setrlimit(RLIMIT_NOFILE, &rlim) < 0) {
      printf("Note: on most systems only root can change these limits\n");
      printf("      I often use a modified kernel that permits\n");
      printf("      regular users to do so\n");
      perror("setrlimit");

      if (getrlimit(RLIMIT_NOFILE, &rlim) < 0) {
	perror("getrlimit");
	exit(-1);
      }

      fprintf(stderr, "%s: argument to -c must be in range 1..%d\n",
	      prog_name,(int) rlim.rlim_max - 10);
      exit(-1);
    } else {
      if (getrlimit(RLIMIT_NOFILE, &rlim) < 0) {
	perror("getrlimit");
	exit(-1);
      }
    }
  }

  /* on BSD these aren't long ints so cast them */
  printf("rlim.rlim_cur = %ld\n", (long int) rlim.rlim_cur);
  printf("rlim.rlim_max = %ld\n", (long int) rlim.rlim_max);

  /* ensure that max_fds is at least as large as maxconns */
  /* note that it may be set explicitly using -f */
  if (max_fds != OPT_DEFAULT_MAX_FDS) {
    max_fds = maxconns;
  }

}

/* ---------------------------------------------------------------------- */
int
need_fd_setsize(void)
{
  int rc = 1;

  if (options.use_poll) {
    rc = 0; 
  }

  if (options.use_epoll) {
    rc = 0; 
  }

  if (options.use_epoll2) {
    rc = 0; 
  }

  if (options.use_socket_aio) {
    rc = 0; 
  }

  /* this test must be last !! */
  if (options.use_max_open_sd) {
    rc = 1;
  }

  return(rc);
}
