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


#include <string.h>

#include "myassert.h"
#include "util.h"
#include "options.h"
#include "app.h"

//#define LOCAL_DEBUG
#include "local_debug.h"

Queue *blocked_app_q;
Queue *idle_app_q;

const char *
req_type_str(req_type_t req_type)
{
  static char buf[16];

  switch (req_type) {
    case REQ_INVALID:
      return "INVALID";
    case REQ_STATIC:
      return "static";
    case REQ_FASTCGI:
      return "FastCGI";
#ifdef MOD_SPECWEB99
    case REQ_SPECWEB99:
      return "SPECweb99";
#endif
    default:
      sprintf(buf, "%d", req_type);
      return buf;
  }
}


app_t *
create_app(const char *uri, req_type_t req_type)
{
  app_t *app;
  printf("create_app: uri = [%s] req_type = %d\n", uri, req_type);

  assert(uri != NULL);
  app = find_app(uri);
  if (app != NULL) {
    printf("create_app: app uri '%s' already defined\n", uri);
    return NULL;
  }
  if (options.app_count == MAX_APPS) {
    printf("create_app: application limit exceeded\n");
    return NULL;
  }
  app = &options.app[options.app_count];

  if (strlen(uri) >= MAX_APP_URI_LEN) {
    printf("create_app: uri too long\n");
    return NULL;
  }
  strcpy(app->uri, uri);
  app->req_type = req_type;
  memset(&app->stats, '\0', sizeof(app->stats));
  app->server = NULL;
  memset(&app->serverstats, '\0', sizeof(app->serverstats));
  app->req_q = NULL;  // will be handled later, in init_app_queues()
  app->blocked = 0;
  ldbg("create_app: uri '%s' type %s\n", uri, req_type_str(req_type));

  options.app_count++;
  ldbg("create_app: app_count now %d\n", options.app_count);

  return app;
}


appserver_t *
find_appserver(const char *path)
{
  app_t *app;
  appserver_t *server;
  int i;

  for (i = 0; i < options.app_count; i++) {
    app = &options.app[i];
    server = app->server;
    while (server != NULL) {
      if (sock_path_compare(server->path, path) == 0) {
	return server;
      }
      server = server->next;
    }
  }

  return NULL;
}


app_t *
find_app(const char *uri)
{
  app_t *app;
  int i;
 
  ldbg("find_app: looking for uri = %s\n", uri);

  assert(uri != NULL);
  for (i = 0; i < options.app_count; i++) {
    app = &options.app[i];
    ldbg("find_app: comparing with app->uri = %s\n", app->uri);
    if (!strcmp(uri, app->uri)) {
      return app;
    }
  }

  return NULL;
}


int
app_add_server(app_t *app, const char *path, int flags)
{
  appserver_t *server, *tmp;
  int len;

  ldbg("app_add_server(): path '%s' flags 0x%08x\n", path, flags);
  assert(app != NULL);
  assert(path != NULL);
  len = strlen(path);
  if (len >= MAX_APP_SERVER_PATH_LEN) {
    printf("app_add_server: path too long (%d >= %d)\n", len,
	MAX_APP_SERVER_PATH_LEN);
    return -1;
  }

  server = find_appserver(path);
  if (server != NULL) {
    printf("app_add_server: app server '%s' already defined\n", path);
    // treat the redefinition as innocuous if the app is correct
    if (server->app == app) {
      return 0;
    }
    return -3;
  }

  server = (appserver_t *) alloc_aligned(ALLOC_NOALIGN, sizeof(appserver_t),
      ALLOC_MALLOC, "app_add_server");
  if (server == NULL) {
    printf("app_add_server: failed to allocate server\n");
    return -2;
  }

  server->next = NULL;
  server->app = app;
  server->startinfo = NULL;
  memset(&server->stats, '\0', sizeof(server->stats));
  server->flags = flags;
  server->sd = -1;
  server->path = strdup_aligned(path);
  server->pid = -1;
  if (server->path == NULL) {
    printf("app_add_server: failed to allocate path\n");
    return -2;
  }

  if (app->server == NULL) {
    app->server = server;
  } else {
    for (tmp = app->server; tmp->next != NULL; tmp = tmp->next);
    tmp->next = server;
  }
  ldbg("app_add_server: app '%s' added server '%s'\n", app->uri, path);

  options.appserver_count++;
  ldbg("app_add_server: appserver_count now %d\n", options.appserver_count);

  return 0;
}


int
#ifdef HAVE_SCHED_AFFINITY
#ifdef HAVE_CPUSET
app_add_startinfo(const char *path, const char *exec_string, cpu_set_t appserv_cpu_mask)
#else
app_add_startinfo(const char *path, const char *exec_string, unsigned long appserv_cpu_mask)
#endif /* HAVE_CPUSET */
#else
app_add_startinfo(const char *path, const char *exec_string)
#endif /* HAVE_SCHED_AFFINITY */
{
  appserver_startinfo_t *startinfo, *tmp;
  int len;

  assert(path != NULL);
  len = strlen(path);
  if (len >= MAX_APP_SERVER_PATH_LEN) {
    printf("app_add_startinfo: path too long (%d >= %d)\n", len,
	MAX_APP_SERVER_PATH_LEN);
    return -1;
  }

  startinfo = (appserver_startinfo_t *) alloc_aligned(ALLOC_NOALIGN,
      sizeof(appserver_startinfo_t), ALLOC_MALLOC, "app_add_startinfo");
  if (startinfo == NULL) {
    printf("app_add_startinfo: failed to allocate startinfo\n");
    return -2;
  }

  startinfo->next = NULL;
  startinfo->app = NULL;
  startinfo->server = NULL;

#ifdef HAVE_SCHED_AFFINITY
  startinfo->cpu_mask = appserv_cpu_mask;
#endif /* HAVE_SCHED_AFFINITY */

  startinfo->path = strdup_aligned(path);
  if (startinfo->path == NULL) {
    printf("app_add_startinfo: failed to allocate path\n");
    return -2;
  }
  if (exec_string != NULL) {
    startinfo->exec_string = strdup_aligned(exec_string);
    if (startinfo->exec_string == NULL) {
      printf("app_add_startinfo: failed to allocate exec_string\n");
      return -2;
    }
  } else {
    startinfo->exec_string = NULL;
  }

  if (options.appserver_startinfo == NULL) {
    options.appserver_startinfo = startinfo;
  } else {
    tmp = options.appserver_startinfo;
    while (1) {
      assert(tmp != NULL);
      if (sock_path_compare(tmp->path, path) == 0) {
	fprintf(stderr,"app_add_startinfo: server '%s' already has startinfo. userver now exiting\n", path);
	free_aligned(startinfo->exec_string);
	free_aligned(startinfo->path);
	free_aligned(startinfo);
        	//return 0;  earlier method.  if two appservers have same path, userver doesn't exit.
        return -1;  // if two appservers have same path userver should exit.
      }
      if (tmp->next == NULL) {
	break;
      }
      tmp = tmp->next;
    }
    tmp->next = startinfo;
  }
  ldbg("app_add_startinfo: server '%s' exec_string '%s'\n",
      path, exec_string ? exec_string : "(default)");

  options.appserver_startinfo_count++;
  ldbg("app_add_startinfo: appserver_startinfo_count now %d\n",
      options.appserver_startinfo_count);

  return 0;
}


int
app_match_startinfo(appserver_startinfo_t *startinfo)
{
  app_t *app;
  appserver_t *server;
  char buf[1024];
  int i;

  assert(startinfo != NULL);
  if (startinfo->server != NULL) {
    return 0;  // already matched
  }

  for (i = 0; i < options.app_count; i++) {
    app = &options.app[i];
    server = app->server;
    while (server != NULL) {
      if (sock_path_compare(server->path, startinfo->path) == 0) {
	startinfo->app = app;
	startinfo->server = server;
	if (startinfo->exec_string == NULL) {
	  snprintf(buf, sizeof(buf), "%s%s", options.doc_root, app->uri);
	  startinfo->exec_string = strdup_aligned(buf);
	  if (startinfo->exec_string == NULL) {
	    printf("app_match_startinfo: failed to allocate exec_string\n");
	    return -2;
	  }
	  ldbg("app_match_startinfo(): created exec_string '%s'\n", buf);
	}
	server->startinfo = startinfo;
	return 0;
      }
      server = server->next;
    }
  }

  printf("app_match_startinfo(): no match for '%s'\n", startinfo->path);
  return -1;
}


int
app_match_startinfo_all(void)
{
  appserver_startinfo_t *startinfo;

  startinfo = options.appserver_startinfo;
  while (startinfo != NULL) {
    if (app_match_startinfo(startinfo) < 0) {
      return -1;
    }
    startinfo = startinfo->next;
  }

  return 0;
}


int
init_app_queues(void)
{
  app_t *app;
  int i;

  blocked_app_q = alloc_queue(MAX_APPS);
  if (blocked_app_q == NULL) {
    printf("init_app_queues: could not allocate blocked_app_q\n");
    return -1;
  }

  idle_app_q = alloc_queue(MAX_APPS);
  if (idle_app_q == NULL) {
    printf("init_app_queues: could not allocate idle_app_q\n");
    return -1;
  }

  for (i = 0; i < options.app_count; i++) {
    app = &options.app[i];
    if (app->server != NULL) {
      app->req_q = alloc_queue(options.app_req_queue_size);
      if (app->req_q == NULL) {
	printf("init_app_queues: could not allocate req_q\n");
	return -1;
      }
    }
  }

  return 0;
}


int
app_is_blocked(app_t *app)
{
  assert(app != NULL);
  return app->blocked;
}


void
app_block(app_t *app)
{
  int appnum;
  int ret;

  appnum = app - options.app;
  assert(appnum >= 0);
  assert(appnum < options.app_count);
  assert(app == &options.app[appnum]);
  assert(!app->blocked);
  ldbg("app_block(): blocking app #%d\n", appnum);
  app->blocked = 1;
  ret = queue_put(blocked_app_q, appnum);
  assert(ret == 0);
}


app_t *
app_unblock_next(void)
{
  int appnum;
  app_t *app;

  appnum = queue_get(blocked_app_q);
  if (appnum < 0) {
    ldbg("app_unblock_next(): nothing to unblock\n");
    return NULL;
  }
  assert(appnum < options.app_count);
  app = &options.app[appnum];
  assert(app->blocked);
  ldbg("app_unblock_next(): unblocking app #%d\n", appnum);
  app->blocked = 0;

  return app;
}


#ifdef FIXHUP
void
print_server_info(appserver_t *server)
{
   printf("sd = %d path = %s pid = %d flags = %d\n",
     server->sd, server->path, (int) server->pid, server->flags);
}

void
print_app_info(void)
{
  int next_sd = -1;
  int appnum = -1;
  app_t *app = NULL;
  int sd = -1;
  appserver_t *server = NULL;

  printf("\nPrinting info for all apps\n");
  for (appnum=0; appnum<options.app_count; appnum++) {
    app = &options.app[appnum];
    printf("Printing req queue info for appnum = %d uri = %s is_blocked = %d\n", 
       appnum, app->uri, app->blocked);
    while ((next_sd = queue_get(app->req_q)) > 0) {
      printf("next_sd in queue = %d\n", next_sd);
    }
    printf("Done printing req queue info for appnum = %d\n", appnum);
  }

  if (app) {
    server = app->server;
    while (server != NULL) {
      print_server_info(server);
      server = server->next;
    }

    printf("Done printing info for all apps\n");
  }

  printf("Start of blocked_app_q:\n");
  while ((appnum = queue_get(blocked_app_q)) > 0) {
    printf("  blocked appnum = %d\n", appnum);
  }
  printf("End of blocked_app_q\n");
   
  printf("Start of idle_app_q:\n");
  while ((sd = queue_get(idle_app_q)) >= 0) {
    printf("  app sd = %d\n", sd);
  }
  printf("End of idle_app_q\n\n");
}
#endif /* FIXHUP */
