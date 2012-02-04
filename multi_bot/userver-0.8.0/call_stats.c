/*
    userver -- (pronounced you-server or micro-server).
    This file is part of the userver, a high-performance web server designed for
    performance experiments.
          
    This file is Copyright (C) 2010  Tim Brecht

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


#ifdef CALL_STATS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/time.h>

// #define LOCAL_DEBUG
#include "local_debug.h"

#include "info.h"
#include "call_stats.h"

#define MIN(a, b)    (a < b ? a : b)

#define URI_SIZE 80 /* Maximum size allowed for a URI for a given call */

/* --call-stats=buf_size  
 * indicates max number of calls tracked by call_stats */
int buf_size = 0; 
int add_calls = 0; 

/* struct used to format the display of call statistics */
struct print_stats {
	char *hdr;		   /* header */
	char *units;	   /* units for data */
	char *hdr_frmt;	 /* format used for header and units*/
	char *data_frmt; /* format used for data, should be the same width as above */
};

/* Order here can't be changed without changing the print order */
struct print_stats print_stats_info[]={
  {"reqnum",     "",     "%-11s",  "%-11d"},
  {"client",     "",     "%-11s",  "%-11d"},
  {"call",       "",     "%-11s",  "%-11d"},
  {"size",       "",     "%-11s",  "%-11d"},
  {"sent",       "",     "%-11s",  "%-11d"},
  {"%",          "",     "%-4s",   "%-4d"},
  {"status",     "",     "%-7s",   "%-7d"},
  {"time",       "usec ","%-11s",  "%-11d"},
  {"fail",       "",     "%-5s",   "%-5d"},
  {"uri",        "",     "%s",     "%s"},
  {0,0,0}
};


/* struct used to gather per call statistics */
typedef struct {
	u_long time;                /* time until done sending or fail */
	unsigned int client_id;
	unsigned int call_id;
	u_long size;                /* file/reply size */
	u_long bytes_sent;          /* bytes sent */
	int status;                 /* HTTP status */
	int reason;                 /* if failure, reason for failure */
	char uri[URI_SIZE];
} call_data;

call_data *call_stats;


/* Initialize the data structures used for tracking per call statistics */
void call_stats_init(int size)
{
	int i;
        /* make this one bigger because requests start at 1 and not zero */
	buf_size = size + 1;
	
	call_stats = malloc (sizeof(call_data) * (buf_size));

	if (call_stats == NULL) {
		fprintf(stderr, "call_stats_init: malloc failed to allocate call_stats array\n");
		exit (1);
	}

	for (i=0; i<buf_size; i++) {
		call_stats[i].uri[0] = '\0';
		call_stats[i].time = 0;
		call_stats[i].size = -1; 
		call_stats[i].bytes_sent = 0;
		call_stats[i].status = 0; 
		call_stats[i].client_id = 0;
		call_stats[i].call_id = 0;
		call_stats[i].reason = 0;
	}
}


/* Use the request number as the index into the call stats array */
void 
call_stats_request(int req_num, char *uri)
{
   struct timeval cur;
   int index = req_num;

   ldbg("call_stats_request: req_num = %d\n", req_num);
   assert(index >= 0);

   if (index < buf_size) {
      strncpy(call_stats[index].uri, uri, URI_SIZE);
      gettimeofday(&cur, 0);
      /* usecs at the current time */
      call_stats[index].time = ((double) cur.tv_sec * 1000000.0) + (double) cur.tv_usec;
   }

   add_calls++;

}

/* Use the request number as the index into the call stats array 
 * Called when the request completes successfully
 */

void
call_stats_finished(int req_num, int size, int nwritten,
  int client_id, int call_id, int http_status)
{
   struct timeval cur;
   double cur_usecs;
   int index = req_num;

   ldbg("call_stats_finish: req_num = %d\n", req_num);

   if (index < buf_size) {
     gettimeofday(&cur, 0);
     cur_usecs = ((double) cur.tv_sec * 1000000.0) + (double) cur.tv_usec;
     call_stats[index].time = cur_usecs - call_stats[index].time;
     call_stats[index].client_id = client_id;
     call_stats[index].call_id = call_id;
     call_stats[index].size = size;
     call_stats[index].bytes_sent = nwritten;
     call_stats[index].status = http_status;
     call_stats[index].reason = REASON_NO_FAILURE;
   }

}

void
call_stats_failed(int req_num, int size, int nwritten, 
  int client_id, int call_id, int http_status, int reason)
{
   struct timeval cur;
   double cur_usecs;
   int index = req_num;

   ldbg("call_stats_failed: req_num = %d\n", req_num);
   
   if (index < buf_size) {
     if (call_stats[index].size == -1) {
       gettimeofday(&cur, 0);
       cur_usecs = ((double) cur.tv_sec * 1000000.0) + (double) cur.tv_usec;
       call_stats[index].time = cur_usecs - call_stats[index].time;
       call_stats[index].client_id = client_id;
       call_stats[index].call_id = call_id;
       call_stats[index].size = size;
       call_stats[index].bytes_sent = nwritten;
       call_stats[index].status = http_status;
       call_stats[index].reason = reason;
     } else {
       ldbg("call_stats_failed: req_num = %d closing connection\n", req_num);
     }
   }

}


void
print_headers()
{
  int i = 0;

  /* print column headers */
  while (print_stats_info[i].hdr != 0) {
    printf(print_stats_info[i].hdr_frmt, print_stats_info[i].hdr);
    i++;
  }
  printf("\n");

  i = 0;

  /* print column units */
  while (print_stats_info[i].hdr != 0) {
    printf(print_stats_info[i].hdr_frmt, print_stats_info[i].units);
    i++;
  }
  printf("\n");

}
/* Print a summary of the per call statistics. */
void call_stats_print()
{
  int i = 0;
  int j = 0;
  int percent = 0;
  char *failed = 0;

  printf("\n\n------------------------------------------------------\n");
  printf("Call Statistics\n");
  printf("add_calls = %d  buf_size = %d\n\n", add_calls, buf_size);

  if (add_calls > buf_size) {
     printf("WARNING NOT ALL CALLS WERE TRACKED\n");
     printf("May need to increase --call-stats size\n\n");
  }

  print_headers();

  /* print data */
  for (i=1; i<MIN(add_calls+1, buf_size); i++) {

    /* Compute needed values first */
    if (call_stats[i].size == 0) {
       percent = 100;
    } else {
      percent = (int) (double) call_stats[i].bytes_sent * 100.0 / (double) call_stats[i].size;
    }

    if (percent != 100.0) {
      failed = "yes";
    } else {
      failed = "no";
    }

    j = 0;
    /* print out a row of stats, one row = one call */
    printf(print_stats_info[j++].data_frmt, i);
    printf(print_stats_info[j++].data_frmt, call_stats[i].client_id);
    printf(print_stats_info[j++].data_frmt, call_stats[i].call_id);
    printf(print_stats_info[j++].data_frmt, call_stats[i].size);
    printf(print_stats_info[j++].data_frmt, call_stats[i].bytes_sent);
    printf(print_stats_info[j++].data_frmt, percent);
    printf(print_stats_info[j++].data_frmt, call_stats[i].status);
    printf(print_stats_info[j++].data_frmt, call_stats[i].time);
    printf(print_stats_info[j++].data_frmt, call_stats[i].reason);
    printf(print_stats_info[j++].data_frmt, call_stats[i].uri);
    printf("\n");
  }

  printf("------------------------------------------------------\n");
}

#endif /* CALL_STATS */
