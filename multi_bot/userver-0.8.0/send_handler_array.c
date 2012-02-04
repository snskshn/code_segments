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


#ifdef SEND

#ifdef ARRAY_OF_BUFS

#include <sys/poll.h>
#include <sys/socket.h>

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "send.h"
#include "debug.h"
#include "stats.h"
#include "trace.h"
#include "syscalls.h"
#include "q.h"
#include "send_utils.h"
#include "cntl_conns.h"
#include "sock_listener.h"

#define PROCESS_SD          (1)
#define DONT_PROCESS_SD     (0)

int num_evts_buf(int buf_index);
struct event *get_next_event(struct event_control *lecb, int *buf_index);
int num_evts_array();
int bufs_empty();
int buf_is_empty(int buf_index);
int which_buf_to_process(int buf_index);
int num_evts_in_buf(int buf_index);
extern void send_print_info_array();
void verify_evt_array(const struct event *evt, int buf_index);

#ifdef TBB_QLENS
static inline _syscall1(int, qlen, int, fd);
static inline _syscall1(int, qlen_young, int, fd);
static inline _syscall1(int, qlen_listenq, int, fd);
#endif /* TBB_QLENS */

static  struct evt_regs* t = NULL;
static struct evt_regs *bogus_regs = 0;
/* static  struct evt_regs old_regs; */
/* static  struct evt_regs* old_regs_addr; */

int send_errno = 0;
int saved_tail[EVT_BUFS_MAX];
int send_need_to_check_events = 0;
int evt_notify = 0;
int evt_queue = 0;
int send_intrs_to_handle = 0;
struct event_control *ecb = NULL;
int should_process_sds = 0;

extern fd_set readable;
extern fd_set writable;

sighandlerfn_t sigs[NSIG]={NULL,};

int send_do_io(int sd, struct info *ip, int events);
void restore_thread(struct evt_regs *regs, int notify);
int send_handle_new_conn(const struct event *evt, int listen_sd, int sd, int process);

extern void sig_kill_handler();
extern void sig_pipe_handler();
extern void sig_usr1_handler();
extern void sig_usr2_handler();
extern void sig_crash_handler();

int send_handler_evts_done = 0;
int consecutive = 0;

/*======================================================================*/
void start_critical(){};
/*======================================================================*/

/*----------------------------------------------------------------------*/
void
send_handler(struct event_control *lecb)
{
  extern fd_set *rdfds, *wrfds;
  const struct event *evt = 0;
  int rc = 0;
  int event_processed = 1;
  int save_errno = 0;
  int sd;
  struct info *ip;
  int called_explicitly = 0;
  int events;
  int evt_count;
  int fake_events = 0;
  int trace_fd = 0;
  static int init_done = 0;
  int i;
  int buf_index = -1;
  int listen_sd = -1;

  if (!init_done) {
    for (i=0; i<EVT_BUFS_MAX; i++) {
      saved_tail[i] = -1;
    }
    init_done = 1;
  }

  if (options.send_loop) {
    should_process_sds = 1;
  } else {
    should_process_sds = 0;
  }


  consecutive = 0;
  send_handler_evts_done = 0;

  in_handler = 1;
  /* turn off notification while processing the event */
  ecb->notify = SIG_GRP;
  
  if (lecb == NULL) {
    called_explicitly = 1;
    PRINT_TIME(NOFD, &tnow, &tprev,
       	"---> send_handler: entered handler explicitly");
    lecb = ecb;
    DEBG(MSG_SEND, "send_handler: lecb was NULL\n");
    DEBG(MSG_SEND, "send_handler: lecb now = %p\n", lecb);
    DEBG(MSG_SEND, "send_handler: lecb->regs = %p\n", lecb->regs);
    for (i=0; i<EVT_BUFS; i++) {
      DEBG(MSG_SEND, 
	  "send_handler: lecb->head[%d] = %d lecb->tail[%d] = %d\n", 
	   i, lecb->head[i], i, lecb->tail[i]);
    }
    t = bogus_regs;
    num_send_handler_calls++;
  } else {
    PRINT_TIME(NOFD, &tnow, &tprev,
       	"---> send_handler: entered thru interrupt");
    for (i=0; i<EVT_BUFS; i++) {
      DEBG(MSG_SEND, 
	  "send_handler: lecb->head[%d] = %d lecb->tail[%d] = %d\n", 
	   i, lecb->head[i], i, lecb->tail[i]);
    }
    DEBG(MSG_SEND, "send_handler: lecb->regs = %p\n", lecb->regs);
    num_send_handler_interrupts++;
		
    t = lecb->regs;
  }

  /* Changed to permit calling handler explicitly */
  DEBG(MSG_SEND, "send_handler: setting lecb->regs = 0xffffffff\n");
  lecb->regs = (void*)0xffffffff;
  save_errno = errno;

  if (t != bogus_regs && t != 0) {
    if (t->reason > 0 && t->eax < 0) {
      DEBG(MSG_SEND, "send_handler: syscall failure: reason = %i\n",
	  t->reason);
      dump_regs(t);
      dump_stack(t);
    }
    send_errno = (int) t->eax;
  } else {
    send_errno = 0;
  }

  DEBG(MSG_SEND, "send_handler: send_errno = %d\n", send_errno);

  /* either entering a critical section or in one so we just leave handler */
  if ((entering_cs || in_cs) && (!called_explicitly)) {
    PRINT_TIME(NOFD, &tnow, &tprev, 
	"<--- send_handler: race detected\n");
    num_sigio_races++;
    send_intrs_to_handle++;
    goto get_out;
  }

  if ((!new_connections_on || sigio_blocked) && (!called_explicitly)) {
    PRINT_TIME(NOFD, &tnow, &tprev,
       	"<--- send_handler: race detected - new connections not on\n");
    num_sigio_false++;
    send_intrs_to_handle++;
    goto get_out;
  }


  if (num_idle <= options.free_fd_thold) {
    DEBG(MSG_SEND, "WARNING! send_handler: entered num_idle = %d and "
	"thold = %d\n", num_idle, options.free_fd_thold);
  }

  for (i=0; i<EVT_BUFS_MAX; i++) {
    saved_tail[i] = lecb->tail[i];
  }
  consecutive = 0;

  while (!(bufs_empty())) { 

    /* decide which of the event buffers to process */
    buf_index = which_buf_to_process(buf_index);

    while ((!buf_is_empty(buf_index)) && (event_processed)) {
      PRINT_TIME(NOFD, &tnow, &tprev, "send_handler: at top of loop");
      event_processed = 0;
      conns_off_if_needed();
      evt_count = num_evts_array();
      PRINT_TIME(NOFD, &tnow, &tprev, 
	  "send_handler: evt_count = %d", evt_count);

      for (i=0; i<EVT_BUFS; i++) {
	PRINT_TIME(NOFD, &tnow, &tprev,
	    "send_handler: head[%d] = %d tail[%d] = %d",
	    i, lecb->head[i], i, lecb->tail[i]);
      }

      PRINT_TIME(NOFD, &tnow, &tprev, "send_handler: missed = %d",
	  ecb->missed_events);

#ifdef ONE_LISTENER
#ifdef TBB_KINFO
      if (kinfo) {
	 PRINT_TIME(NOFD, &tnow, &tprev, "send_handler:  "
	     "kinfo->qlen_young = %d kinfo->qlen = %d",
	     kinfo->qlen_young, kinfo->qlen);
	 PRINT_TIME(NOFD, &tnow, &tprev, "send_handler: syscall "
	     "qlen_young = %d        qlen = %d", 
	    qlen_young(server_sd), qlen(server_sd));
	 PRINT_TIME(NOFD, &tnow, &tprev, "send_handler: qlen_listenq = %d",
	     qlen_listenq(server_sd));
      }
#endif /* TBB_KINFO */
#endif /* ONE_LISTENER */

      evt = get_next_event(lecb, &buf_index);

#ifdef DEBUG_ON
      verify_evt_array(evt, buf_index);
#endif

      if ((MSG_MASK & MSG_TIME) || (MSG_MASK & MSG_SEND)) {
	print_event(evt, t);
      }

#ifdef ARRAY_OF_BUFS
      num_events[buf_index]++;
#else
      num_events++;
#endif /* ARRAY_OF_BUFS */

      switch(evt->type) {

	case EVT_SIG:
          /* XXX: for now - turn off notification and delivery */
	  /* because we are just going to print out some stats and exit */
	  ecb->notify = 0;
	  ecb->queue = 0;
	  num_evt_sig++;
	  event_processed = 1;
	  PRINT_TIME(NOFD, &tnow, &tprev, 
	      "send_handler: Processing EVT_SIG");
	  DEBG(MSG_SEND, "send_handler: Processing EVT_SIG\n");
	  DEBG(MSG_SEND, "send_handler: Received event: type = %d "
	      "id = %d\n", evt->type, evt->event_id);
	  if (t != bogus_regs) {
	    PRINT_TIME(NOFD, &tnow, &tprev, "send_handler: eip = %p", 
		t->eip);
	    DEBG(MSG_SEND, "send_handler: Received event: eip = %p\n", 
		t->eip);
	  }

	  if (sigs[evt->event_id] && sigs[evt->event_id] != 
	      (sighandlerfn_t)(-1)) {
	    PRINT_TIME(NOFD, &tnow, &tprev,
		"send_handler: signal = %d calling handler",
		evt->event_id);
	    sigs[evt->event_id](0,evt);
	  } else if (sigs[evt->event_id] != (sighandlerfn_t)(-1)) {
	    DEBG(MSG_SEND, "send_handler: No handler for %i\n",
		evt->event_id);
	    printf("send_handler: No handler for %i\n",evt->event_id);
	    fflush(stdout);
	    event_processed = 1;
	    if (t) {
	      dump_regs(t);
	    }
	    print_event(evt, t);
	    exit(1);
	  } else {
	    printf("send_handler: evt->event_id = %d "
		"sigs[evt->event_id] = %p\n",
		evt->event_id, sigs[evt->event_id]);
	    fflush(stdout);
	    /* event_processed = 1; */
	    if (t) {
	      dump_regs(t);
	    }
	    print_event(evt, t);
	    exit(1);
	  }
	  break;

	case EVT_MSG:
	  num_evt_msg++;
	  DEBG(MSG_SEND, "send_handler: Processing message from: %i\n",
		 evt->event_id);

	  /*
	   * A previous read call failed (EAGAIN) even though the event
	   * was generated saying that the socket was ready.
	   * So for now this is a work around where we've been 
	   * sent a message saying that we need to re-read 
	   * from the socket indicated in the msg.
	   */

	  if ((int) evt->data.msg.data > 0) {
	      sd = (int) evt->data.msg.data;
	      ip = info_ptr(sd);
	      assert(ip);
	      fake_events = POLLIN | POLLRDNORM | POLLOUT | 
		 POLLWRNORM | POLLREADRETRY;
	      rc = send_do_io(sd, ip, fake_events);
	  }
	  event_processed = 1;
	  break;

	case EVT_IPACCEPT:
	  num_evt_ipaccept++;
	  /* TODO: not sure this is in the right place? */
	  PRINT_TIME(NOFD, &tnow, &tprev,
	      "send_handler: consecutive = %d num_idle = %d",
	      consecutive, num_idle);
	    sd = evt->data.ipa.fd;
	    num_accept_calls++;
	    PRINT_TIME(NOFD, &tnow, &tprev,
		"send_handler: Processing EVT_IPACCEPT sd = %d", sd);
	    if (sd < 0) {
	      /* In this case the sd contains the negated errno */
	      process_accept_errs(sd, -sd);
	      event_processed = 1;
	    } else {
	      events = evt->data.io.events;
	      if (events & POLLFIN) {
		  /*
		   * We are going to bypass doing a bunch of work
		   * on a socket that has already been closed by
		   * the client. So first we have to do a bit of 
		   * setup that subsequent code depends on (preconditions)
		   */
		  ip = info_add(sd);
		  ip->sd = sd;
		  set_fsm_state(ip, FSM_CONNECTING);
		  num_connections++;
		  num_accept_successful++;
		  num_idle--;
		  if (num_idle == options.free_fd_thold) {
		    PRINT_TIME(sd, &tnow, &tprev, 
		      "send_handler: ATTENTION: NUM_IDLE == %d",
		      options.free_fd_thold);
		  }

		  PRINT_TIME(sd, &tnow, &tprev, "send_handler: POLL_FIN "
			     "calling do_close");
		  num_close_send_early_fin++;
		  rc = do_close(ip, REASON_SEND_POLL_FIN);
		  event_processed = 1;
	      } else {
		assert(sd <= max_fds);
		assert(sd > min_usable_sd);
		ip = info_add(sd);
		ip->sd = sd;
		set_fsm_state(ip, FSM_CONNECTING);
		/* Eventually we should add this to SEND */
                /* listen_sd = evt->data.ipa.parent_fd; */
		listen_sd = SOCK_LISTENER_USE_UNKNOWN;
		event_processed = send_handle_new_conn(evt, listen_sd, sd,
		    should_process_sds);

#ifdef NOT_SURE
		if ((options.do_multiaccept == 0 && consecutive == 1) || 
		    ((options.multiaccept_max != 
		      OPT_MULTIACCEPT_MAX_UNLIMITED) &&
		     (consecutive >= options.multiaccept_max))) {
		  PRINT_TIME(NOFD, &tnow, &tprev,
		      "send_handler: reached max accepts");
		  socket_new_conn_off();
		} 
#endif

		/* if the socket is has data to be read go ahead and 
		 * read it
		 */
		if (!options.accepts_only && should_process_sds) {
		  if (event_processed && evt->data.io.events & POLLIN) {
		    /* TODO: this may be wrong as the event has been 
		     * processed
		     * event_processed = 
		     * send_do_io(sd, ip, evt->data.io.events);
		     */
		    rc = send_do_io(sd, ip, evt->data.io.events);
		  }
		}
	      } /* else */
	    } /* else */
	  break;

	case EVT_IOREADY:
	  num_evt_ioready++;
	  /* TODO: not sure this is in the right place? */
	  PRINT_TIME(NOFD, &tnow, &tprev,
	      "send_handler: consecutive = %d num_idle = %d",
	      consecutive, num_idle);
	    sd = evt->data.io.fd;
	    events = evt->data.io.events;
	    PRINT_TIME(sd, &tnow, &tprev,
		"send_handler: Processing EVT_IOREADY");

	    if (sd < 0) {
	       PRINT_TIME(sd, &tnow, &tprev,
		   "send_handler: Processing EVT_IOREADY");
	       printf("send_handler: got negative sd on EVT_IOREADY\n");
	       print_event(evt, t);
	       event_processed = 1;
	    } else if (sock_is_listener(sd)) {
	      if (events & POLLIN) {
		listen_sd = sd;
		ip = info_add(sd);
		event_processed = send_handle_new_conn(evt, listen_sd, sd,
		    should_process_sds);
		/* TODO: This is just to test if it changes anything */
		/* send_async_setup(sd); */

	      } else {
		PRINT_TIME(sd, &tnow, &tprev,
		    "send_handler: listener_sd EVT_IOREADY but POLLIN "
		    "is not set events = 0x%x", events);
		/* is pollhint set and nothing else */
		if ((events | POLLHINT) == POLLHINT) {
		  PRINT_TIME(sd, &tnow, &tprev,
		      "send_handler: listener_sd no POLLIN but POLLHINT");
		  num_pollhint_server_consumed++;
		}
		event_processed = 1;
	      }
	    } else {
	      if (!options.accepts_only && should_process_sds) {
		ip = info_ptr(sd);
		assert(ip);
		event_processed = send_do_io(sd, ip, events);
		if (!event_processed) {
		  printf("send_handler: event not processed\n");
		  print_event(evt, t);
		  exit(1);
		}
	      } else {
		PRINT_TIME(sd, &tnow, &tprev,
		    "send_handler: skipping events 0x%x", events);
		PRINT_TIME(sd, &tnow, &tprev,
		    "send_handler: accepts_only = %d "
		    "should_process_sds = %d", 
		    options.accepts_only, should_process_sds);

		event_processed = 1;
	      }
	    }

	    if (!event_processed) {
	      printf("send_handler: event not processed\n");
	      print_event(evt, t);
	      exit(1);
	    }
	  break;

	case EVT_DISPATCH:
	  num_evt_dispatch++;
	  DEBG(MSG_SEND, "send_handler: Processing EVT_DISPATCH\n");

	case EVT_SYNCH:
	  num_evt_synch++;
	  DEBG(MSG_SEND, "send_handler: Processing EVT_SYNCH\n");

	default:
	  num_evt_unknown++;
	  PRINT_TIME(NOFD, &tnow, &tprev,
	      "send_handler: doing event");
	  DEBG(MSG_SEND, "send_handler: lecb->head[%d] = %d "
	       "lecb->tail[%d] = %d lecb->event_list_size[%d] = %d\n",
	       buf_index, lecb->head[buf_index], 
	       buf_index, lecb->tail[buf_index], 
	       buf_index, lecb->event_list_size[buf_index]);
	  DEBG(MSG_SEND, "send_handler: evt->type = %d\n", evt->type);
	  DEBG(MSG_SEND, "send_handler: Fell down into default\n");
	  print_event(evt, t);
	  exit(1);
	  break;

      } /* switch */

      /* NEW */
      /* Did not process the event this time around */
      /* so leave it in the buffer/queue to be processed later */
      if (event_processed) {
	/* num_events++; */
	send_handler_evts_done++;
	++lecb->head[buf_index];
	DEBG(MSG_SEND, 
	    "send_handler: event processed head for %d now = %d\n",
	    buf_index, lecb->head[buf_index]);
	/* evt->type= (u32) - 1; */
	send_need_to_check_events = 0;
      } else {
	DEBG(MSG_SEND, 
	    "send_handler: event not processed head for %d still = %d\n",
	    buf_index, lecb->head[buf_index]);
	PRINT_TIME(NOFD, &tnow, &tprev,
	    "send_handler: event not processed head for %d still = %d\n",
	    buf_index, lecb->head[buf_index]);
	printf("send_handler: event not processed head for %d still = %d\n",
	    buf_index, lecb->head[buf_index]);
	send_need_to_check_events = 1;
	printf("send_handler: if EVT_IPACCEPT fd = %d\n", evt->data.ipa.fd);
	printf("send_handler: if EVT_IOREADY fd = %d\n", evt->data.io.fd);
	printf("send_handler: calling exit\n");
	print_event(evt, t);
	exit(1);
      }
      DEBG(MSG_SEND, "send_handler: send_need_to_check_events = %d\n",
	  send_need_to_check_events);
      PRINT_TIME(NOFD, &tnow, &tprev,
	  "send_handler: at bottom of loop buf_index = %d "
	  "head = %d tail = %d",
	  buf_index, ecb->head[buf_index], ecb->tail[buf_index]);
    } /* while */
  
  } /* while */

  if (consecutive > num_max_consecutive_accepts) {
    num_max_consecutive_accepts = consecutive;
  }

get_out:

  if (consecutive > 0) {
    if (options.use_memcpy) {
      memcpy(readable, rdfds, sizeof(fd_set));
      memcpy(writable, wrfds, sizeof(fd_set));
    } else {
      *readable = *rdfds;
      *writable = *wrfds;
    }

    if (options.process_sds_order == OPT_PROCESS_SDS_LIFO) {
      q_sync(Q_ADD_TO_FRONT);
    } else if (options.process_sds_order == OPT_PROCESS_SDS_FIFO) {
      q_sync(Q_ADD_TO_REAR);
    }
  }

  /* Changed to allow handler to be called explicitly. */
  lecb->regs = NULL;

  if (t != bogus_regs) {
    if (t->reason > 0 && t->eax < 0) {
      DEBG(MSG_SEND, "send_handler: Syscall failure: %i\n",t->reason);
      dump_regs(t);
      dump_stack(t);
      exit(1);
    }
    send_errno = (int) t->eax;
  } else {
    send_errno = 0;
  }

  /* DEBG(MSG_SEND, "send_handler: About to do restore\n"); */
  DEBG(MSG_SEND, "send_handler: send_errno = %d\n", send_errno);
  DEBG(MSG_SEND, "send_handler: save_errno = %d\n", save_errno);
  DEBG(MSG_SEND, "send_handler: send_need_to_check_events = %d\n",
      send_need_to_check_events);

  PRINT_TIME(NOFD, &tnow, &tprev,
      "send_handler: consecutive = %d evts_done = %d",
      consecutive, send_handler_evts_done);

  /* This was used for debugging */
  /* memcpy(&old_regs, t, sizeof(old_regs)); */
  /* old_regs_addr = t; */
  errno = save_errno;
  if (t != bogus_regs) {
    PRINT_TIME(NOFD, &tnow, &tprev, "restore_thread: eip = %p", t->eip);
    PRINT_TIME(NOFD, &tnow, &tprev, "restore_thread: flags = %p", t->flags);

    TRACE(EVT_SENDHANDLER,
      trace_fd = 0;
      rc = send_handler_evts_done;
    );
    PRINT_TIME(NOFD, &tnow, &tprev, "<--- send_handler: restoring");
    DEBG(MSG_SEND, "send_handler: notify will be = 0x%x queue = 0x%x\n", 
	evt_notify, lecb->queue);
    in_handler = 0;
    /*  restore_thread(t,0); */
    restore_thread(t, evt_notify);
  } else {
    /* XXX: CHECK THIS */
    lecb->notify = evt_notify;
  }

  DEBG(MSG_SEND, "send_handler: No restore\n");
  PRINT_TIME(NOFD, &tnow, &tprev,
      "<--- send_handler: returning, notify = 0x%x queue = 0x%x", 
      lecb->notify, lecb->queue);
  in_handler = 0;
  /* return(evts_done); */
} /* send_handler */


/*----------------------------------------------------------------------*/
/* For now this pulls the next event of the buffer we are currently
 * processing
 *
 * Later we may wish to change this so that a different priority is used
 * in determining the order in which events are processed.
 *
 * We pass the address of the buffer index variable in case
 * we end up changing this code so that a different event buffer
 * is used - in which case we should change the buf_index to
 * whichever one is chosen to work on.
 *
 */
struct event *
get_next_event(struct event_control *lecb, int *buf_index)
{
  int element = lecb->head[*buf_index] % lecb->event_list_size[*buf_index];
  PRINT_TIME(NOFD, &tnow, &tprev, "get_next_event from buf_index = %d "
    "head/element = %d", *buf_index, element);
  return((struct event *) &lecb->eventbuf[*buf_index][element]);
}

/*----------------------------------------------------------------------*/
void 
restore_thread(struct evt_regs *regs, int notify)
{
  DEBG(MSG_SEND, "restore_thread: notify = 0x%x\n", notify);
  ecb->notify = notify;
  if (!notify) {
      printf("Bad notify\n");
  }
  asm("movl %0, %%esp \n\t"			\
      "popfl\n\t"				\
      "popal \n\t"				\
      "ret \n\t"				\
      ::"m" (&regs->flags));

}

/*----------------------------------------------------------------------*/
/* This is used only once - to set the initial default state */
void 
set_notify(int mode)
{
  evt_notify = mode;
  DEBG(MSG_SEND,
      "set_notify: entering notify = 0x%08x mode = 0x%08x num_idle = %d\n",
      ecb->notify, mode, num_idle);
  /* This changes delivery not clear we want to change delivery yet. */
  /* ecb->queue = mode; */
  if (!in_handler) {
    DEBG(MSG_SEND, "set_notify: setting notify to 0x%08x\n", mode);
    ecb->notify = mode;
  }
  DEBG(MSG_SEND,
      "set_notify: returning ecb->queue = 0x%x ecb->notify = 0x%08x\n", 
      ecb->queue, ecb->notify);
  PRINT_TIME(NOFD, &tnow, &tprev,
      "set_notify: ecb->queue = 0x%08x", ecb->queue);
}

/*----------------------------------------------------------------------*/
/* This is used only once - to set the initial default state */
void 
set_queue(int mode)
{
  DEBG(MSG_SEND, "set_queue: entering setting mode to 0x%08x\n", mode);

  /* This changes delivery */
  ecb->queue = mode;

  DEBG(MSG_SEND,
      "set_queue: returning ecb->queue = 0x%08x ecb->notify = 0x%08x\n",
       ecb->queue, ecb->notify);
  PRINT_TIME(NOFD, &tnow, &tprev,
      "set_queue: ecb->queue = 0x%08x", ecb->queue);
}

/*----------------------------------------------------------------------*/
int
num_evts_array(void)
{
  int i;
  int total = 0;
  for (i=0; i<EVT_BUFS; i++) {
    total += num_evts_in_buf(i);
  }
  PRINT_TIME(NOFD, &tnow, &tprev, "num_evts_array: returning %d", total);
  return(total);
}

int
num_evts_in_buf(int buf_index)
{
  int tmptail, tmphead;
  int evt_count = 0;

  /* The tail can/does change  */
  /* But not if we place/leave this inside of the critical section */
  tmptail = ecb->tail[buf_index];
  tmphead = ecb->head[buf_index];

  if (tmptail == tmphead) {
    evt_count = 0;
  } else if (tmptail > tmphead) {
    /* evt_count = tmptail - tmphead - 1; */
    evt_count = tmptail - tmphead;
  } else {
    evt_count = (ecb->event_list_size[buf_index]) - tmphead + tmptail - 1;
  }
  PRINT_TIME(NOFD, &tnow, &tprev, "num_evts_in_buf: buf = %d returning %d", 
    buf_index, evt_count);
  return(evt_count);
} /* num_evts_in_buf */


/*----------------------------------------------------------------------*/
int
free_ecb_slots_in_buf(int buf_index)
{
  int free_slots; 

  free_slots = ecb->event_list_size[buf_index] - num_evts_in_buf(buf_index);
  PRINT_TIME(NOFD, &tnow, &tprev,
      "free_ecb_slots_in_buf: buf = %d returning %d", 
      buf_index, free_slots);
  return(free_slots);
} /* free_ecb_slots */

/*----------------------------------------------------------------------*/
int
used_ecb_slots_in_buf(int buf_index)
{
  return(num_evts_in_buf(buf_index));
} /* used_ecb_slots */


/*----------------------------------------------------------------------*/
void
block_auto_accept_delivery(void)
{
  PRINT_TIME(NOFD, &tnow, &tprev,
      "block_auto_accept_delivery: before queue = 0x%x",
		  ecb->queue);
  assert(ecb);
  /* This was the blocking the wrong thing */
  /* ecb->queue = evt_queue & (~EVT_IPACCEPT); */
  ecb->queue = ecb->queue & (~AUTO_ACCEPT_GRP);
  PRINT_TIME(NOFD, &tnow, &tprev,
      "block_auto_accept_delivery: after  queue = 0x%x",
		  ecb->queue);
}

/*----------------------------------------------------------------------*/
void
unblock_auto_accept_delivery(void)
{
  assert(ecb);
  PRINT_TIME(NOFD, &tnow, &tprev,
      "unblock_auto_accept_delivery: before queue = 0x%x",
		  ecb->queue);
  /* This was the blocking the wrong thing and was just plain wrong */
  /* ecb->queue = evt_queue | (~EVT_IPACCEPT); */
  ecb->queue = ecb->queue | AUTO_ACCEPT_GRP;
  PRINT_TIME(NOFD, &tnow, &tprev,
      "unblock_auto_accept_delivery: after  queue = 0x%x",
      ecb->queue);
}
/*----------------------------------------------------------------------*/
int
which_buf_to_process(int buf_index)
{
  int ret_val = -1;
  int i;
  /* check for things in highest buffer first */
  for (i=EVT_BUFS-1; i>=0; i--) {
    if (!buf_is_empty(i)) {
       ret_val = i; 
    }
  }
  assert(ret_val != -1);
  PRINT_TIME(NOFD, &tnow, &tprev, 
    "which_buf_to_process: buf_index = %d "
    "head/element = %d", buf_index, ret_val);
  return(ret_val);
}

int
bufs_empty(void)
{
  int buf_index;
  int all_bufs_empty = 1;
  for (buf_index=0; buf_index<EVT_BUFS; buf_index++) {
    if (!buf_is_empty(buf_index)) {
      all_bufs_empty = 0;
      break;
    }
  }
  PRINT_TIME(NOFD, &tnow, &tprev, 
     "bufs_empty: buf_index = %d returning %d", 
     buf_index, all_bufs_empty);
  return(all_bufs_empty);
}

int
buf_is_empty(int buf_index)
{
  int buf_is_empty = 1;
  if (ecb->head[buf_index] != ecb->tail[buf_index]) {
     buf_is_empty = 0;
  }
  PRINT_TIME(NOFD, &tnow, &tprev, 
    "buf_is_empty: buf_index = %d returning %d",
    buf_index, buf_is_empty);
  return(buf_is_empty);
}
/*======================================================================*/
void end_critical(){};
/*======================================================================*/

/*----------------------------------------------------------------------*/
sighandlerfn_t
send_sig_kill_handler(int cpu, const struct event* evt)
{
  sig_kill_handler();
}

/*----------------------------------------------------------------------*/
sighandlerfn_t
send_sig_pipe_handler(void)
{
  sig_pipe_handler();
}

/*----------------------------------------------------------------------*/
sighandlerfn_t
send_sig_usr1_handler(void)
{
  sig_usr1_handler();
}

/*----------------------------------------------------------------------*/
sighandlerfn_t
send_sig_usr2_handler(void)
{
  sig_usr2_handler();
}

/*----------------------------------------------------------------------*/
sighandlerfn_t
send_sig_crash_handler(void)
{
  double x = 1.0;
  double y = 0.0;
  double z = 0.0;
  printf("Got SIGSEGV or SIGBUS \n");
  fflush(stdout);
  z = x / y;
  sig_crash_handler();
}

/*----------------------------------------------------------------------*/
sighandlerfn_t
send_sig_trap_handler(void)
{
  printf("Got SIGTRAP\n");
  fflush(stdout);
  sig_crash_handler();
}

/*----------------------------------------------------------------------*/
int
send_handle_new_conn(const struct event *evt, int listen_sd, int sd, int process_sd)
{
  int rc = 0;
  int i = 0;
  int event_processed = 0;
  struct linger l;
  int trace_fd = 0;
  struct info *ip;

  PRINT_TIME(sd, &tnow, &tprev,
      "send_handle_new_conn: process_sd = %d\n", process_sd);

  if (sd >  FD_SETSIZE) {
    printf("send_handle_new_conn: fd too large *** sd = %d "
	"FD_SETSIZE = %d\n", sd, FD_SETSIZE);
    printf("send_handle_new_conn: num_idle = %d maxconns = %d "
	"max_fds = %d\n", num_idle, maxconns, max_fds);
    print_event(evt, t);
    printf("send_handle_new_conn: Checking fds now\n");
    fflush(stdout);
    for (i=0; i<=FD_SETSIZE; i++) {
       check_fd(i);
    }
    sleep(10);
    printf("send_handle_new_conn: Checking fds AGAIN\n");
    fflush(stdout);
    for (i=0; i<=FD_SETSIZE; i++) {
       check_fd(i);
    }
    exit(1);
  }

  assert(sd > 2);
  assert(sd <= max_fds);

  /* In this case we do continue to accept new connections */
  /* the idea being we set the free_fd_thold so that we */
  /* don't run out of fds. */
  if (num_idle > 0) {
    if (!options.auto_accept) {
      PRINT_TIME(sd, &tnow, &tprev,
	  "send_handle_new_conn: calling do_new_connections");
      assert(sock_is_listener(listen_sd));
      do_new_connections(listen_sd, FROM_SEND_HANDLER);
    } else {
      /* Nothing to trace here because it is auto accepted in the kernel */
      TRACE(EVT_AUTO_ACCEPT,
        trace_fd = sd;
	rc = sd;
      );

      if (sd > max_sd) {
        max_sd = sd;
      }

      if (options.accepts_only) {
	// ip = info_ptr(sd);
	ip = info_add(sd);
	assert(ip);
	ip->sd = sd;
        num_idle--;
	if (num_idle == options.free_fd_thold) {
	  PRINT_TIME(sd, &tnow, &tprev, "send_handle_new_conn: "
	      "ATTENTION: NUM_IDLE == %d", options.free_fd_thold);
	}
        set_fsm_state(ip, FSM_CONNECTING);
	PRINT_TIME(sd, &tnow, &tprev,
	    "send_handle_new_conn: connection auto accepted");
	/* this typically gets counted in socket_setup but that's not
	 *  called in this case
	 */
	num_connections++;

        if (options.close_with_reset) {
          l.l_onoff = 1;
          l.l_linger = 0;
          if (setsockopt (sd, SOL_SOCKET, SO_LINGER, &l, sizeof(l)) < 0) {
            perror ("send_handle_new_conn: setsockopt(SO_LINGER)");
            printf("send_handle_new_conn: setsocketopt(SO_LINGER failed "
	           "on sd = %d errno = %d\n", sd, errno);
            exit (-1);
          }
        }

        set_fsm_state(ip, FSM_CLOSING);

	PRINT_TIME(sd, &tnow, &tprev,
	    "send_handle_new_conn: calling close");
	close_sock(sd);
        num_idle++;
        set_fsm_state(ip, FSM_CLOSED);
	PRINT_TIME(sd, &tnow, &tprev,
	    "send_handle_new_conn: closed");
	ip->sd = -1;

	if (should_enable_new_conns()) {
	    socket_new_conn_on();
	    send_release_queued_evts();
	    /* The pending connection requests / events */
	    /* are queued and will be released later */
            /* NO NEED TO do_new_connections(FROM_SEND_HANDLER); */
	}

      } else {
	PRINT_TIME(sd, &tnow, &tprev, 
	    "send_handle_new_conn: calling socket_setup");
	if (listen_sd != SOCK_LISTENER_USE_UNKNOWN) {
	  assert(sock_is_listener(listen_sd));
	}
	rc = socket_setup(listen_sd, sd);
	PRINT_TIME(sd, &tnow, &tprev,
	    "send_handle_new_conn: done socket_setup");
      } /* else */

      if (rc != 0) {
	num_accept_successful++;
	consecutive++;
      }
    }
    event_processed = 1;
  } else {
    /* Not clear what to do in this case - it's an event that
     * can't be processed right now - so it will need to be processed
     * later. One approach might be to dequeue it from the event
     * queue and add it to another queue to be processed later.
     */
    PRINT_TIME(sd, &tnow, &tprev, 
      "send_handle_new_conn: NOT calling do_new_connections "
      "because num_idle <= 0");

    printf("send_handle_new_conn: NOT calling do_new_connections\n");
    printf(    "num_idle = %d <= options.free_fd_thold %d\n", 
		    num_idle, options.free_fd_thold);

    send_print_info_array();

    num_accept_ready_but_no_idle++;
    /* We were unable to process the event at this point in time */
    /* So we'll need to deal with it later */
    event_processed = 0;
  }

  PRINT_TIME(sd, &tnow, &tprev,
      "send_handle_new_conn: returning %d", event_processed);
  return(event_processed);
} /* send_handle_new_conn */
/*----------------------------------------------------------------------*/

void
send_release_queued_evts(void)
{
  int rc;
  int origtimeout;
  int timeout;
  int trace_fd = 0;

  timeout = 0;
  origtimeout = timeout;

  if (ecb->num_queued != 0) {
    num_release_queued_evts++;
    PRINT_TIME(NOFD, &tnow, &tprev, "send_release_queued_evts: "
	"evtctl WAIT -- timeout = %d", timeout);
    PRINT_TIME(NOFD, &tnow, &tprev, "send_release_queued_evts: "
	"num_queued = %d head = %d tail = %d\n", 
	ecb->num_queued, ecb->head, ecb->tail);
    /* Note passing an addr of 0 waits indefinitely */
    TRACE(EVT_WAIT_EVTS,
      trace_fd = 0;
      rc = evtctl(EVT_ANY, WAIT_FOR_EVENT, (void *) ecb->notify,
	(int) &timeout);
    )
    if (rc < 0) {
      printf("evtctl failed errno = %d\n", errno);
      exit(1);
    }
    PRINT_TIME(NOFD, &tnow, &tprev, "send_release_queued_evts: "
	"evtctl returns %d\n", rc);
    PRINT_TIME(NOFD, &tnow, &tprev, "send_release_queued_evts: "
	"remaining timeout = %d  elapsed = %d",
	 timeout, (origtimeout - timeout));
    PRINT_TIME(NOFD, &tnow, &tprev, "send_release_queued_evts: "
	"num_queued = %d head = %d tail = %d\n",
	ecb->num_queued, ecb->head, ecb->tail);
  }
}

/*----------------------------------------------------------------------*/
int
send_prime_io(struct info *ip) 
{
  int rc = 0;
  unsigned int fake_events;
 
  /*
   * Pretend that we are ready to read and to write.
   * The hope/plan is that if we initiate a read or write that
   * if it's not ready we'll later get an event stating that it is.
   */
  fake_events = POLLIN | POLLOUT;
  PRINT_TIME(ip->sd, &tnow, &tprev, "send_prime_io: fake_events = 0x%x");
  assert(ip->sd > 2);
  rc = send_do_io(ip->sd, ip, fake_events);
  return(rc);
}

/*----------------------------------------------------------------------*/


#endif /* ARRAY_OF_BUFS */
#endif /* SEND */

