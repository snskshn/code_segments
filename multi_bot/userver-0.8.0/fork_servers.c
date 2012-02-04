/*
    userver -- (pronounced you-server or micro-server).
    This file is part of the userver, a high-performance web server designed for
    performance experiments.

    Copyright (C) 2005-2010 Tim Brecht
    Based on the file originally Copyright (C) 2004  Hewlett-Packard Company

    Authors: Tim Brecht <brecht@cs.uwaterloo.ca>
             David Pariag <db2paria@cs.uwaterloo.ca>
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
#include <sys/time.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sched.h>
#include "common.h"
#include "options.h"
#include "debug.h"
#include "sigstuff.h"
#include "logging.h"
#ifdef SEND
#include "cntl_intr.h"
#include "send.h"
#endif /* SEND */


#ifdef HAVE_SCHED_AFFINITY  
#ifdef PROVIDE_SCHED_AFFINITY_HEADERS
#include "sys_sched_affinity.h"
#else
#include "sched.h"
#endif
#endif

void poll_loop(void);

#ifdef HAVE_EPOLL
void epoll_loop(void);
extern int epoll_fd,epoll_fd_array[];
#endif /* HAVE_EPOLL */

#ifdef HAVE_AIO_LAYER
void aio_loop(void);
#endif /* HAVE_AIO_LAYER */

pid_t my_pid = 0;
int my_proc_id = 0;

extern void cleanup();
extern int fcgi_create(app_t *app, appserver_t *appserver);
static int configure_appserver(int proc_id);
static void start_appservers();
static void alloc_dyn_buf();

void set_userver_cpu_affinity(); 
void set_server_priority(); 
void set_server_nice_level(); 

void
fork_servers(int numprocs)
{
  pid_t child;
  int i;
  char myfile[100];
  char logfile[MAX_LOGFILE_LEN];
  int rc;
  int out_fd = 0;
#ifdef SEND
  int init;
#endif /* SEND */
  my_pid = getpid();

  for (i=0; i<numprocs - 1; ++i) {
    child = fork();
    if (child == -1) {
	perror ("fork");
	exit (-1);
    }
    if (child == 0) {
      my_pid = getpid();
      my_proc_id = i+1;
      break;
    }
  }

#ifdef SUNOS
  printf("my_proc_id = %d pid = %ld\n", my_proc_id, my_pid);
#else
  printf("my_proc_id = %d pid = %d\n", my_proc_id, my_pid);
#endif

#ifdef HAVE_EPOLL
  epoll_fd=epoll_fd_array[my_proc_id]; 
#endif 

    alloc_dyn_buf(); /* allocate dynamic buffers for the appservers to use */
   
    /*configure the data structures for appservers if any */
    configure_appserver(my_proc_id);

    /*start the appservers if any and connect to all of them */
    start_appservers();
    
    /*set the affinity to required mask */
    set_userver_cpu_affinity();

		/* Note that I think prioirties are inherited so
		 * this needs to be done after starting the appservers
		 */
#if defined(__linux__)
    set_server_priority();
#endif
    set_server_nice_level();
     
    install_sig_handlers();
    rc = atexit(cleanup);
    if (rc < 0) {
      printf("fork_servers: atexit failed rc = %d errno = %d\n", rc, errno);
      exit(1);
    }

  /* Now we try to change where stdout and stderr go.
   * We'll try a simple hack to try to get the output for
   * each process to go to a different file
   */

  if (numprocs > 1) {
#ifdef OLDWAY
    sprintf(myfile, "userver-%d.log", my_proc_id);
#else
    sprintf(myfile, "userver-%d.log", my_proc_id + numprocs * options.server_id);
#endif /* OLDWAY */
    printf("Re-directing stdout to %s\n", myfile);
    out_fd = open(myfile, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
    if (out_fd < 0) {
      printf("Can't open file = %s\n", myfile);
      perror("Open failed");
      exit(1);
    }

    rc = dup2(out_fd, 1);
    if (rc < 0) {
      printf("dup2 failed arg1 = %d arg2 = %d\n", out_fd, 1);
      perror("dup2 failed");
      exit(1);
    }
    close(out_fd);
  }
  /* Initialize logging for each server */
  if( options.use_logging ) {
    snprintf(logfile, MAX_LOGFILE_LEN, "httpd-%d.log", my_proc_id);
    rc = logging_init(logfile);
    if( rc == 0 ) {
      /* Initializing the log file failed! */
      printf("Error initializing logfile: %s. Exiting...\n", logfile);
      exit(1);
    }
  }

	printf("INIT DONE\n");

#ifdef SEND
  if (options.get_connections == OPT_CONN_WITH_SEND_EVTS) {
    /* assumes that we start with notification on - so for
     * to avoid assertions turn notification off and then on 
     */
    /* TODO - fix this all up */
    notification_off();
    notification_on();
    DEBG(MSG_SEND, "fork_servers: setting queue mask\n");
    init = SIG_GRP;
    if (options.auto_accept) {
      init |= AUTO_ACCEPT_GRP;
    } else {
      init |= ACCEPT_IO_GRP;
      /* Not sure if this is needed  */
      /* init |= IO_GRP; */
    }

    if (!options.accepts_only) {
      init |= IO_GRP;
    }
    init |= MSG_GRP;
    set_queue(init);

    DEBG(MSG_SEND, "fork_servers: setting notify mask\n");
    init = SIG_GRP;
    if (options.asynch_mode) {
      if (options.auto_accept) {
	init |= AUTO_ACCEPT_GRP;
      } else {
	init |= ACCEPT_IO_GRP;
      }

      if (!options.accepts_only) {
	init |= IO_GRP;
      }
    }

    set_notify(init);

    PRINT_TIME(NOFD, &tnow, &tprev, 
	"fork_servers: queue = 0x%x notify = 0x%x",
      ecb->queue, ecb->notify);
  }


  if (options.send_loop) {
    PRINT_TIME(NOFD, &tnow, &tprev, "fork_servers: calling send_loop");
    send_loop();
  } 
#endif /* SEND */

  if (options.use_poll) {
    PRINT_TIME(NOFD, &tnow, &tprev, "fork_servers: calling poll_loop");
    poll_loop();
  }

#ifdef HAVE_EPOLL
  if (options.use_epoll || 
      options.epoll_edge_triggered ||
      options.use_epoll2) {
    PRINT_TIME(NOFD, &tnow, &tprev, "fork_servers: calling epoll_loop");
    epoll_loop();
  }
#endif /* HAVE_EPOLL */

#ifdef HAVE_AIO_LAYER
  if (options.use_socket_aio) {
    PRINT_TIME(NOFD, &tnow, &tprev, "fork_servers: calling aio_loop");
    aio_loop();
  }
#endif /* HAVE_AIO_LAYER */

  if (!options.send_loop) {
    assert(options.send_loop == 0);
    PRINT_TIME(NOFD, &tnow, &tprev, "fork_servers: calling select_loop");
    select_loop();
  }
}


/* Fork an app server on the local machine, using the given exec_string  */
pid_t fork_app_server(const char * exec_string ) {
  pid_t pid;                 /* PID of app server */
  char * args[MAX_APP_ARGS]; /* Hold args to execve */
  char * env[] = {NULL};     /* Environment vars */
  char * copy; /* Copy of the exec_string */
  char * app;  /* Path to the application */
  //  int sd;      /* Only used by the child */
  int i = 0;
  int len;

  assert(exec_string != NULL );
  // printf("Starting app server with exec_string: %s\n", exec_string);

  /* Construct argvector to pass to execve */
  /* Copy the exec string since we don't want to modify it */
  copy = strdup_aligned( exec_string );
  assert( copy != NULL );

  /* May need to strip quotes from the exec string.
     Usually, the shell strips these, but they don't
     get stripped if we are started with a config file 
  */
  if (copy[0] == '"' ) copy++;
  len = strlen(copy);
  if (copy[len - 1] == '"' ) copy[len - 1] = '\0';

  /* Now construct the argvector, by parsing copy */
  app = (char*) copy;
  while( copy != NULL && i < (MAX_APP_ARGS -1) ) {
    args[i] = copy;
    i++;
    copy = strchr(copy, ' ');
    while(copy != NULL && *copy == ' ') {
      *copy++ = '\0';
    }
  }
  args[i] = NULL;
  pid = fork();
  
  if( pid == 0 ) {
    /* Child */
    for( i = 0; i < max_fds; i++ ) {
      close(i);
    }
    //sd = socket(PF_INET, SOCK_STREAM, 0 );
    //listen(sd,128);
    
    if( execve(app, args, env ) == -1 ) {
      perror("execve failed");
    }

    /* Make sure we never go back into the userver code (if we do
     * we'll start forking our own appservers and chaos ensues).
     * (Using _exit for now to ensure 'atexit'-registered functions
     * aren't called by appservers.)
		 */
    _exit(1);

  } else {
    if( pid == -1 ) {
      perror("fork of app server failed");
    } 
  }
  return pid;
}

/* This function corrects the ports for appservers for different uservers 
   and the exec_string used for forking them */
static int configure_appserver(int proc_id)
{
  app_t *app;
  appserver_t *appserver;
  appserver_startinfo_t *start;
  int i,port;
  char *path, *port_str, *exec_str;
  char serverpath[MAX_APP_SERVER_PATH_LEN + 12];

  for(i=0; i < options.app_count; i++) {
     app = &options.app[i];
     appserver = app->server;
     while(appserver) {
         /*first correct the server path */
        path = strdup_aligned(appserver->path);
        port_str = strchr(path,':');
        if(port_str==NULL) {
          printf("configure_appserver:No port specified. Failed to configure appserver: %s,%s\n",app->uri,path);
          return -1;
        }
        *port_str++ = '\0';
        port = atoi(port_str);
        port += proc_id * options.appserver_count;
        sprintf(serverpath,"%s:%d",path,port);
        free_aligned(appserver->path);
        appserver->path=strdup(serverpath); /*server path corrected*/

        /*now correct the startinfo structures*/
        if(appserver->startinfo) {
           start = appserver->startinfo;
           free_aligned(start->path);
           start->path = strdup(serverpath);/*path corrected*/
           exec_str = (char *) calloc(strlen(start->exec_string)+strlen(serverpath)+3,sizeof(char));
           sprintf(exec_str,"%s %s",start->exec_string,serverpath);
           free_aligned(start->exec_string);
           start->exec_string = strdup_aligned(exec_str);/*exec_string corrected*/
           free(exec_str);
        } else {
#ifdef OLDWAY
           printf("configure_appserver:No startinfo. Failed to configure appserver: %s,%s:%s\n",app->uri,path,port_str);
          return -1;
#endif
        }
        appserver=appserver->next;
     }
  }
  return 0;
}

/* This function starts the appservers if required and then connects to each appserver for each app */
static void start_appservers()
{
  app_t *app;
  appserver_t *appserver;
  appserver_startinfo_t *start;
  int i;
#ifdef HAVE_SCHED_AFFINITY
  int rc;
#endif /* HAVE_SCHED_AFFINITY */

  start = options.appserver_startinfo;
  while (start != NULL) {
    assert(start->server != NULL);
#ifdef HAVE_SCHED_AFFINITY

#ifdef HAVE_CPUSET
    cpu_set_t tmp_mask;
    CPU_ZERO(&tmp_mask);
    // print_cpuset_values("Using Appserver CPU mask\n", &start->cpu_mask);
    // printf("\n");
#else
    unsigned long tmp_mask;
    // printf("Using CPU mask: 0x%lx for %s appserver\n", start->cpu_mask,start->path);
#endif /* HAVE_CPUSET */
    rc = sched_setaffinity(getpid(), sizeof(start->cpu_mask),
	  &(start->cpu_mask));
    if (rc < 0) {
	printf("start_appservers: unable to set processor affinity mask for %s appserver\n",start->path);
	printf("start_appservers: call returns %d errno = %d\n", rc, errno);
	perror("unable to sched_setaffinity");
	exit(1);
    }
    rc = sched_getaffinity(getpid(), sizeof(tmp_mask), &tmp_mask);
    if (rc < 0) {
	printf("start_appservers: unable to get processor affinity mask for %s appserver\n",start->path);
	printf("start_appservers: call returns %d errno = %d\n", rc, errno);
	perror("unable to sched_getaffinity");
	exit(1);
    }

#ifdef HAVE_CPUSET
    // printf("Effective CPU mask: for %s appserver\n", start->path);
    // print_cpuset_values("Appserver CPU mask: \n", &tmp_mask);
    // printf("\n");
#else
    // printf("Effective CPU mask: 0x%lx for %s appserver\n", tmp_mask, start->path);
#endif /* HAVE_CPUSET */

#else 
    printf("start_appservers: SCHED_AFFINITY not defined. ignoring appserv_cpu_mask\n");
#endif /* HAVE_SCHED_AFFINITY */
    start->server->pid = fork_app_server(start->exec_string);
    start = start->next;
  }
  if(options.appserver_startinfo != NULL ) sleep(5);

  /* connect to applications */
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
        /* nothing to do for this type of application */
        break;
    }
  }
}


  /* This allocates dynamic buffer pool for the appservers to use */
static void alloc_dyn_buf()
{
  int flags;
  extern bufpool_t * dyn_bufpool;

  if (options.num_dyn_buffers > 0) {
    if (options.use_sendfile || options.dyn_use_shared_mem) {
      flags = ALLOC_MMAP_FILE;
    } else {
#ifdef OLDWAY
      flags = ALLOC_MALLOC;
#else
      flags = ALLOC_MMAP;
#endif
    }
    if (options.dyn_lock_pages) {
      flags |= ALLOC_MLOCK;
    } else if (options.dyn_touch_pages) {
      flags |= ALLOC_TOUCH;
    }
    dyn_bufpool = alloc_bufpool(options.num_dyn_buffers,
        options.dyn_buffer_size, flags);
    if (dyn_bufpool == NULL) {
      printf("alloc_dyn_buf: can't allocate dynamic buffer pool\n");
      exit(1);
    }
  }
}

void
set_server_nice_level() 
{
  int rc = 0;

  if (options.server_nice_level != OPT_SERVER_NICE_LEVEL_NO_CHANGE) {
    /* set nice_level of calling process */
		printf("Setting nice_level to %d\n", options.server_nice_level);
    rc = setpriority(PRIO_PROCESS, 0, options.server_nice_level);

    if (rc < 0) {
      printf("set_server_nice_level: unable to set nice_level to %d\n",
	       options.server_nice_level);
      perror("set_server_nice_level failed");
      exit(1);
    }
  }

	/* need to clear errno because -1 from getpriority can
	 * legitimately return -1
	 */
	errno = 0;
  rc = getpriority(PRIO_PROCESS, 0);
	if (rc == -1) {
		if (errno != 0) {
			printf("set_server_nice_level: getpriority call fails\n");
			perror("getpriority call fails\n");
			exit(1);
		}
	}
	printf("nice level is = %d\n", rc);
}

void
set_server_priority() 
{
#if !defined(__linux__)
	printf("Setting priority currently only works on Linux\n");
  exit(1);
#else
	struct sched_param params;
  int rc = 0;

  if (options.server_priority != OPT_SERVER_PRIORITY_NO_CHANGE) {

	  params.sched_priority = options.server_priority;

    /* set priority of calling process */
		printf("Setting priority to %d\n", options.server_priority);

    rc = sched_setscheduler(0, SCHED_RR, &params);

    if (rc < 0) {
      printf("set_server_priority: unable to set priority to %d\n",
	       options.server_priority);
      perror("sched_setscheduler failed");
      exit(1);
    }
  }

	rc = sched_getparam(0, &params);

	if (rc < 0) {
		printf("set_server_priority: unable to get scheduler parameters\n");
		perror("sched_getparams failed");
		exit(1);
	}

	printf("set_server_priority: priority = %d\n", params.sched_priority);

	rc = sched_getscheduler(0);

	if (rc < 0) {
		printf("set_server_priority: unable to get scheduler\n");
		perror("sched_getscheduler failed");
		exit(1);
	}

	switch (rc) {

    case SCHED_RR:
			printf("set_server_priority: scheduler = SCHED_RR\n");
			break;

    case SCHED_FIFO:
			printf("set_server_priority: scheduler = SCHED_FIFO\n");
			break;

    case SCHED_OTHER:
			printf("set_server_priority: scheduler = SCHED_OTHER\n");
			break;

		default:
			printf("set_server_priority: scheduler = %d is unknown\n", rc);
			exit(1);
			break;
	}
#endif 

}

void set_userver_cpu_affinity() {
#ifdef HAVE_SCHED_AFFINITY
#ifdef HAVE_CPUSET
    cpu_set_t tmp_mask;
    CPU_ZERO(&tmp_mask);
    print_cpuset_values("Using CPU mask: \n", &options.cpu_mask);
    printf("\n");
#else
    unsigned long tmp_mask;
    printf("Using CPU mask: 0x%lx\n", options.cpu_mask);
#endif /* HAVE_CPUSET */

    int rc = sched_setaffinity(getpid(), sizeof(options.cpu_mask),
           &options.cpu_mask);
    if (rc < 0) {
	printf("set_userver_cpu_affinity: unable to set processor affinity mask\n");
        printf("set_userver_cpu_affinity: call returns %d errno = %d\n", rc, errno);
        perror("unable to sched_setaffinity");
        exit(1);
    }
    rc = sched_getaffinity(getpid(), sizeof(tmp_mask), &tmp_mask);
    if (rc < 0) {
	printf("set_userver_cpu_affinity: unable to get processor affinity mask\n");
        printf("set_userver_cpu_affinity: call returns %d errno = %d\n", rc, errno);
        perror("unable to sched_getaffinity");
        exit(1);
    }
#ifdef HAVE_CPUSET
    print_cpuset_values("Effective CPU mask: \n", &tmp_mask);
    printf("\n");
#else
    printf("Effective CPU mask: 0x%lx for userver\n", tmp_mask);
#endif /* HAVE_CPUSET */
#endif
}
