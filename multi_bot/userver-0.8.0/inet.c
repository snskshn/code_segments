/*
    userver -- (pronounced you-server or micro-server).
    This file is part of the userver, a high-performance web server designed for
    performance experiments.
          
    This file is Copyright (C) 2004  Craig Barkhouse

    Authors: Craig Barkhouse <cabarkho@uwaterloo.ca>
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

#if !defined(DARWIN)
#include <netinet/in.h>
#endif

#include <arpa/inet.h>
#include <netdb.h>
#if defined(__CYGWIN__) || defined(FreeBSD) || defined(HPUX)
#include <sys/socket.h>
#endif /* __CYGWIN__ || FreeBSD || HPUX */

#include "myassert.h"
#include "inet.h"


const struct hostent *
inet_gethostbyinaddr(struct in_addr inaddr)
{
  static struct hostent he;
  static char *h_addr_list_data[2];
  static struct in_addr h_inaddr;

  /*
   * Fill in a static hostent structure.  Note that the address should
   * be stored in network byte order.
   */
  h_inaddr = inaddr;
  h_addr_list_data[0] = (char *) &h_inaddr;
  h_addr_list_data[1] = NULL;
  memset(&he, '\0', sizeof(struct hostent));
  he.h_addr_list = &h_addr_list_data[0];
  he.h_length = sizeof(h_inaddr);
  he.h_addrtype = AF_INET;

  return &he;
}


const struct hostent *
inet_gethost(const char *host)
{
  struct in_addr inaddr;
  const struct hostent *he;

  assert(host != NULL);
  if ((inaddr.s_addr = inet_addr(host)) == INADDR_NONE) {
    he = gethostbyname(host);
  } else {
    he = gethostbyaddr((char *) &inaddr, sizeof(inaddr), AF_INET);
    if (he == NULL) {
      he = inet_gethostbyinaddr(inaddr);
    }
  }

  return he;
}


int
inet_resolvehost(const char *host, char **hostname, struct in_addr *inaddr)
{
  const struct hostent *he;

  assert(host != NULL);
  he = inet_gethost(host);
  if (he == NULL) {
    return -1;
  }
  if (inaddr != NULL) {
    *inaddr = *((struct in_addr *) he->h_addr);
  }
  if (hostname != NULL) {
    if (he->h_name == NULL) {
      return -2;
    }
    *hostname = (char *) he->h_name;
  }

  return 0;
}


const char *
inet_gethostname(const char *host)
{
  const struct hostent *he;
  char hostname[128];

  if (host == NULL) {
    if (gethostname(hostname, sizeof(hostname)) < 0) {
      return NULL;
    }
    host = hostname;
  }
  he = inet_gethost(host);
  if (he == NULL || he->h_name == NULL) {
    return NULL;
  }

  return he->h_name;
}


struct in_addr
inet_gethostinaddr(const char *host)
{
  const struct hostent *he;
  struct in_addr inaddr;

  assert(host != NULL);
  he = inet_gethost(host);
  if (he == NULL) {
    inaddr.s_addr = INADDR_NONE;
    return inaddr;
  }

  return *((struct in_addr *) he->h_addr);
}
