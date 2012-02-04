/*
    userver -- (pronounced you-server or micro-server).
    This file is part of the userver, a high-performance web server designed for
    performance experiments.

    This file is Copyright (C) 2006  David Pariag

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

/* This module is used to generate userver logs in the Common Log Format.
 * This log format uses the following format for each entry:
 *
 *   remotehost rfc931 authuser [date] "request" status bytes
 *
 * where:
 *   <remotehost> is the hostname or IP address of the requesting machine
 *   <rfc931>     is the remote logname of the requesting user
 *   <authuser>   is the username as which the user has authenticated himself
 *   <date>       is the date at which the request was completed
 *   <request>    is the request line exactly as it came from the client
 *   <status>     is the HTTP status code that the server sends to the client
 *   <bytes>      is the size of the HTTP reply sent by the server (incl. header?)
 *
 * Notes: The <rfc931> and <authuser> fields may be unavaliable, in which case
 * a "-" is printed as a placeholder. The <request> field does not contain HTTP 
 * headers. The date field is in a specific format that is required by the
 * Common Log Format. An example is: [3/Jan/2006:13:55:36 -0700]
 */

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <string.h>
#include "assert.h"
#include "logging.h"


static FILE * log = NULL;

int
logging_init(char * logfile) 
{
  assert(logfile != NULL);
  log = fopen(logfile, "w+");
  if (log == NULL) {
    printf("Error: Could not open logfile %s for writing\n", logfile);
  }
  return(log != NULL);
}


void
log_request(struct info * ip)
{ 
  struct in_addr * client_addr;  
  char * client_address;
  time_t now;
  struct tm * local_time;
  char timestr[32];
  char * end = NULL;

  /* Make sure the log file has been initialized before we write to it */
  assert(log != NULL);
  assert(ip != NULL);

  /* First get the address of the client making the request */
  client_addr = &(ip->addr.u.sa_in.sin_addr);
  client_address = inet_ntoa(*client_addr);

  /* Now get the current time */
  now = time(NULL);
  local_time = localtime(&now);
  strftime(timestr, 32, "[%d/%b/%Y:%H:%M:%S %z]", local_time );

  /* Now get a portion of the request string out of the request buffer */
  end = strchr(ip->req.uri, '\r');
  if (end != NULL) {
     *end = '\0';
  }

  /* FIXME: Should the last field include header bytes or not? Right now, this
     is not clear. For now, it does. */
  fprintf(log, "%s - - %s \"%s\" %d %d \n", 
	  client_address, timestr, ip->req.uri, ip->rep.http_status, ip->rep.nwritten );

  if (end != NULL) {
    *end = '\r';
  }
}


void
logging_cleanup()
{
  fclose(log);
  log = NULL;
}

