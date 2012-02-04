/*
    userver -- (pronounced you-server or micro-server).
    This file is part of the userver, a high-performance web server designed for
    performance experiments.

    This file Copyright (C) 2004  Hewlett-Packard Company

    Authors: Tim Brecht <brecht@cs.uwaterloo.ca>
             Done while working part-time for HP
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


#ifdef HAVE_AIO_LAYER
/*----------------------------------------------------------------------*/
/* The idea here is to write to an API that underneath is mapped to
 * a specific AIO implementation.
 *
 * We'd like to be able to use AIO on files and on sockets so we
 * are initially trying to explore interfaces that will work on 
 * both (even though the underlying implementations may be done 
 * using different API's).
 *
 * Socket AIO API's of interest:
 *    Extended Sockets, Linux AIO, POSIX AIO.
 *
 * File AIO API's of interest:
 *    Linux AIO, POSIX AIO.
 *
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <stdio.h>


/*----------------------------------------------------------------------*/
/* Socket related calls
 *
 * May want some way to specify a evt_compl_q (or have this done underneath)
 * For now we assume a pre specified set of completion event types 
 * and that the underlying layers know about and works with us
 * using those complete event types.
 * 
 * These could be mapped to Linux AIO (if ever supported), 
 * Posix AIO, or whatever.
 */

/* For now we assume that there will be four completion queues and
 * that these completion queues map to different types of calls
 */

/* Different types of completion events supported */

/* These are mapped to completion queues.
 * We create a completion queue for each of these completion types
 * and completion events of each type will be placed
 * in the appropriate queue.
 */

typedef int compl_t;
typedef int sock_compl_t;

/* Used to wait on specific types of event completions */
/* Can use a bitmask of events of interest */
#define SOCK_ACCEPT_COMPL      (1)
#define SOCK_READ_COMPL        (2)
#define SOCK_WRITE_COMPL       (3)
#define SOCK_CLOSE_COMPL       (4)
#define SOCK_MAX_COMPL         (5)

/* Used to wait on specific types of event completions */
/* Can use a bitmask of events of interest */
#define SOCK_ACCEPT_MASK       (0x1 << (SOCK_ACCEPT_COMPL-1))
#define SOCK_READ_MASK         (0x1 << (SOCK_READ_COMPL-1))
#define SOCK_WRITE_MASK        (0x1 << (SOCK_WRITE_COMPL-1))
#define SOCK_CLOSE_MASK        (0x1 << (SOCK_CLOSE_COMPL-1))

#define SOCK_ALL_MASK          (SOCK_ACCEPT_MASK | SOCK_READ_MASK | \
                                SOCK_WRITE_MASK  | SOCK_CLOSE_MASK)

/* File completion events */
#define FILE_READ_COMPL        (8)
#define FILE_WRITE_COMPL       (9)
#define FILE_CLOSE_COMPL       (10)
#define FILE_FSTAT_COMPL       (11)

#define FILE_READ_MASK         (0x1 << (FILE_READ_COMPL-1))
#define FILE_WRITE_MASK        (0x1 << (FILE_WRITE_COMPL-1))
#define FILE_CLOSE_MASK        (0x1 << (FILE_CLOSE_COMPL-1))
#define FILE_FSTAT_MASK        (0x1 << (FILE_FSTAT_COMPL-1))

#define FILE_ALL_MASK          (FILE_READ_MASK | FILE_WRITE_MASK | \
                                FILE_CLOSE_MASK | FILE_FSTAT_MASK)

#define ALL_MASK               (SOCK_ALL_MASK | FILE_ALL_MASK)

/* All calls are assumed to indicate an error condition in the
 * return value and to set errno on an error condition.
 */

/*
typedef void *conn_t;
*/
typedef int conn_t;

/* Used to get aio completion events from the underlying aio interface */
struct compl_event {
  /* NOTE! at least one of data or conn_handle must be completed */
  void *data;        /* user specified data if possible, 0 otherwise */

  /* NOTE: this assumes that we'll never do anything on stdin!! */
  conn_t handle;     /* handle if possible, 0 otherwise */
                     /* Note that in environments that use file descriptors
		      * this will be an fd, in others it is specific to 
		      * that environment */
  compl_t type;      /* the completion type */
  int result;        /* return value of the actual call */
  int result_errno;  /* errno from the actual call */
};

typedef struct compl_event compl_evt_t;

struct aio_vec {
  void *base;
  unsigned int len;
  void *reg_handle;
};


/* Underlying call is specific to the layer this is implemented on top of */

// Since the underlying system may be providing integers that could possibly 
// map into the same space as actual socket or file descriptors we may
// which to specify which range of sd values to use.
// If the underlying system can handle this it will guarantee
// that any handle returned as a socket will be in the range 
// low_sd_value to high_sd_value - inclusive.
int aio_sock_init(int low_sd_value, int high_sd_value);

/* Give the underlying layer a chance to clean up */
void aio_sock_uninit(void);

/* returns an integer handle - underlying layer maps to appropriate type */
/* ?? for now assume event_type is one of the completion types specified */
/* Returns negative value on failure */
/* Should be able to assume that there will be a relatively small 
 * number of queues - or create a call to specify the number of queues needed
 */
int aio_sock_create_compl_q(size_t len, int event_type);


/* Must register memory to be read and/or written before initiating request */
/* Note that in environments where registration is not required
 * these are just noops.
 *
 * Some environments require a region handle that is also 
 * used with reads and writes.
 * To be general we assume that the region handle is an arbitrary
 * pointer value which is returned to us at registration time.
 */
int aio_sock_mem_reg(void *addr, size_t size, void **reg_handle);
int aio_sock_mem_dereg(void *addr, size_t size, void *reg_handle);

/* I believe that these will be needed for compatibility with layers
 * that don't actually use file descriptors for connections/sockets 
 */
conn_t aio_sock_socket(int domain, int type, int protocol);
int aio_sock_bind(conn_t s, struct sockaddr *my_addr, socklen_t addrlen);
int aio_sock_listen(conn_t s, int backlog);

/* Parameters, error conditions and codes are the same
 * as for the regular system calls
 *
 * If these calls require a completion queue to be specified it is the
 * responsibility of the underlying layer to include the queue in the actual
 * call.
 *
 * NOTE: this are the only calls that in initiated asynchronously and
 * for which completion events are generated.
 */
/* 
int aio_sock_accept(conn_t sd, struct sockaddr *addr, socklen_t *addrlen);
*/
/* Note that the standard accept call returns the address in the call
 * for our purposes we don't care about the address so we don't worry
 * about how to get the addresses on a event completion.
 */
int aio_sock_accept(conn_t sd);

int aio_sock_read(conn_t sd, void *buf, size_t count, void *reg_handle);
int aio_sock_write(conn_t sd, void *buf, size_t count, void *reg_handle);
int aio_sock_close(conn_t sd);
/* When we get an error on a socket it's already been closed so now just 
 * destroy it
 */
int aio_sock_destroy(conn_t sd);

/* Initiate  a read and an accept. In some environments this provides
 * the address of the read buffer for data that is available prior
 * to the accept.
 * The assumption here is that the read is done on the newly accepted
 * connection and the underlying layer knows which socket/connection
 * that is (because at this point we don't).
 *
 * This call returns the socket descriptor that will be used
 * for the new connection.
 *
 * PROBLEM HERE: don't know which buf and reg_handle to use
 *               until we know what the sd will be.
 *   Could - introduce a call to get/reserve the next sd first?
 *         - pick/find an info slot to use anonymously until
 *           the call gives us the sd and then we can fill it in.
 */
int aio_sock_read_accept(conn_t listensd, void *buf, size_t count,
                         void *reg_handle);


/* This works like writev except that the aio_vec requires a handle
 * for each element in the vector.
 * NOTE: I'm not sure what the completion event returns other than
 * maybe the number of bytes that were written. So I'll assume that for now.
 */
int aio_sock_writev(conn_t sd, struct aio_vec *vec, size_t count);

/* returns the number of events in the specified queue */
int aio_sock_num_events(int compl_q);

/* get as many completion events as there are in the specified queue
 * up to the maximum specified.
 * Events are returned in the specified events array
 */
int aio_sock_getevents(int compl_q, int max, compl_evt_t *events);



/* For environments that permit and it's advantageous to batch
 * calls into a list like call (e.g., Linux AIO io_submit)
 *
 * If batch size = 1 calls are done immediately 
 * otherwise calls are added to an array (of e.g., aiocbp)
 * which are only submitted when the size of that array reaches
 * the batch size
 */

int aio_sock_batch_size(int size);

/* Take whatever has been added to the array for batch submit 
 * and go ahead and submit it.
 */

int aio_sock_batch_flush(void);

/* Wait for a completion on any of the event types */

#define AIO_WAIT_ERROR    (-2)
#define AIO_WAIT_TIMEOUT  (-1)

/* Wait for an event completion notification on one of the
 * specified queues or for the timeout to expire - whichever
 * comes first.
 *
 * The bitmask event_bitmap specifies which events types are of interest.
 * 
 * If evts is not NULL it should point to an array that must 
 *   already be allocated, initialized and valid and contain
 *   room for at least "max" event completions.
 *   Completion events obtained will be placed in the array.
 * If evts == NULL no events are returned and the caller simply
 *   wants to block until an event of interest occurs. Typically
 *   the actual events are obtained later through the 
 *   aio_sock_getevents() call.
 *   Note that if evts == NULL the max parameter is ignored
 *   as the call doesn't return any events.
 *
 * max specifies the maximum number of events the evts buffer can hold.
 *
 * The order in which completion events are returned in evts is
 * implementation dependent (i.e., it's up to the lower layer to decide).
 * and max specifies the maximum number of events that
 * can be placed in the evts array.
 *
 */
int aio_sock_wait(int event_bitmap, compl_evt_t *evts, int max,
    struct timeval *tv);

/* Works on sockets or files */
int aio_wait(int event_bitmap, compl_evt_t *evts, int max,
    struct timeval *tv);

#define AIO_PARTIAL_RESET  (0)
#define AIO_FULL_RESET     (1)

/* This function resets the counters to 0.
 * If full is true it resets all values - otherwise it 
 * performs a partial reset
 */
void aio_reset_stats(int full_reset);
  
/* This function returns the number of stats reported by aio_map.  See
 * below for how it might be used. 
 */
int aio_num_stats(void);

/*
    This is the function used to get the aio_map statistics.

    outf    - This is the file the statistics are to be output to (e.g.
              stdout).  If its value is NULL then the routine does not 
              attempt to output the statistics.
    stats   - If this value is not NULL then it an array of size elements
              where the counters will be placed.
    what    - If not NULL then this is an array of size elements where 
              pointers to short descriptions of the counters will be
              placed.
    size    - Describes the number of elements in stats and what if they
              are not NULL. 
    
    The functional return is the number of elements actually placed into
    stats and/or what.

    For example, to simply output the current values of the counters:

      aio_get_stats(stdout, NULL, NULL, 0);
*/

int aio_get_stats(FILE *outf, double *stats, char **what, int size);

/*----------------------------------------------------------------------*/
/* File related calls */

/* This is intentionally incomplete - it will get fleshed out in
 * more detail when the sockets calls are more complete.
 */

/* Ideally these can map to POSIX AIO and/or LINUX AIO */
/* The tricky part will be doing the mapping of data structures efficiently */

int aio_file_init(void);
int aio_file_open(const char *pathname, int flags);
int aio_file_fstat(int filedes, struct stat *buf);
int aio_file_read(int fd, void *buf, size_t count);
int aio_file_write(int fd, void *buf, size_t count);
int aio_file_close(int fd);

int aio_num_file_events(void);
int aio_get_file_events(void);

int aio_file_batch_size(int batch_size);
int aio_file_batch_flush(void);

/*----------------------------------------------------------------------*/
/* Calls related to both sockets and files */

/* Blocks and waits until an completion event occurs or timeout is reached */
int aio_sock_and_file_wait(int event_bitmap, struct timeval *tv);

/*----------------------------------------------------------------------*/
#endif /* HAVE_AIO_LAYER */
