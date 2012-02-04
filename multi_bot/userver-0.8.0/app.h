/*
    userver -- (pronounced you-server or micro-server).
    This file is part of the userver, a high-performance web server designed for
    performance experiments.
          
    This file is Copyright (C) 2004-2010  Tim Brecht and Craig Barkhouse

    Authors: Tim Brecht <brecht@cs.uwaterloo.ca>
    Authors: Craig Barkhouse <cabarkho@student.cs.uwaterloo.ca>
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


#ifndef APP_H
#define APP_H

#include "util.h"
#include "queue.h"


#define MAX_APPS		1000
#define MAX_APP_URI_LEN		256  // should match MAX_URI_LEN in info.h
#define MAX_APP_SERVER_PATH_LEN	128
#define MAX_APP_ARGS		32
#define DEF_APP_SERVER_PORT	8888


typedef enum req_type {
  REQ_INVALID = -1,
  REQ_STATIC = 0,
  REQ_FASTCGI,
#ifdef MOD_SPECWEB99
  REQ_SPECWEB99,
#endif
} req_type_t;

typedef struct appserver_startinfo appserver_startinfo_t;
typedef struct appserver_stats appserver_stats_t;
typedef struct appserver appserver_t;
typedef struct app_stats app_stats_t;
typedef struct app app_t;

struct appserver_startinfo {
  appserver_startinfo_t *next;		// for chaining
  app_t *app;				// associated app
  appserver_t *server;			// associated appserver
  char *path;				// hostname, IP address, or filename
  char *exec_string;			// string to pass to execl()
#ifdef HAVE_SCHED_AFFINITY
#ifdef HAVE_CPUSET
  cpu_set_t cpu_mask;                   // mask for cpu affinity
#else
  unsigned long cpu_mask;               // mask for cpu affinity
#endif /* HAVE_CPU_SET */
#endif /* HAVE_SCHED_AFFINITY */
};

struct appserver_stats {
  int num_serviced;
  int num_successful_reads;
  int num_successful_data_reads;
  long long total_bytes_read;
  long long total_data_bytes_read;
  int num_successful_writes;
  long long total_bytes_written;
};

/* appserver flags */
#define APPSERVER_SHAREDMEM	0x0001

struct appserver {
  appserver_t *next;			// for chaining
  app_t *app;				// associated app
  appserver_startinfo_t *startinfo;	// associated start info
  appserver_stats_t stats;		// application server stats
  int flags;				// flags
  int sd;				// sd of application server socket
  char *path;				// hostname, IP address, or filename
  pid_t pid;				// process ID (if local)
};

struct app_stats {
  int num_requests;
  int num_queued;
  long long total_req_queue_size;
  int max_req_queue_size;
};

struct app {
  char uri[MAX_APP_URI_LEN];		// fixed URI (TODO: wildcards/regexps)
  req_type_t req_type;			// store this in ip->req.type
  app_stats_t stats;			// application stats
  appserver_t *server;			// linked list of server paths
  appserver_stats_t serverstats;	// aggregate server stats
  Queue *req_q;				// sds of waiting clients
  int blocked;				// flag -- in blocked_app_q?
};


extern Queue *blocked_app_q;		// list of blocked app #s
extern Queue *idle_app_q;

const char *req_type_str(req_type_t req_type);
app_t *create_app(const char *uri, req_type_t req_type);
app_t *find_app(const char *uri);
appserver_t *find_appserver(const char *path);
int app_add_server(app_t *app, const char *path, int flags);

#ifdef HAVE_SCHED_AFFINITY
#ifdef HAVE_CPUSET
int app_add_startinfo(const char *path, const char *exec_string, cpu_set_t appserv_cpu_mask);
#else
int app_add_startinfo(const char *path, const char *exec_string, unsigned long appserv_cpu_mask);
#endif /* HAVE_CPUSET */
#else
int app_add_startinfo(const char *path, const char *exec_string);
#endif /* HAVE_SCHED_AFFINITY */

int app_match_startinfo(appserver_startinfo_t *startinfo);
int app_match_startinfo_all(void);
int init_app_queues(void);
int app_is_blocked(app_t *app);
void app_block(app_t *app);
app_t *app_unblock_next(void);
void print_app_info(void);


#endif /* APP_H */
