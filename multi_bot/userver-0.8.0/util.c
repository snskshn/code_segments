/*
    userver -- (pronounced you-server or micro-server).
    This file is part of the userver, a high-performance web server designed for
    performance experiments.

    Copyright (C) 2005-2010 Tim Brecht
    Based on the file originally Copyright (C) 2004  Hewlett-Packard Company

    Authors: Tim Brecht <brecht@cs.uwaterloo.ca>
             Craig Barkhouse <cabarkho@uwaterloo.ca>
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



#ifdef HAVE_CPUSET
#include <sched.h>
#endif 

#ifdef HAVE_EPOLL
#ifdef PROVIDE_EPOLL_HEADERS
#include "sys_epoll.h"
#else
#include <sys/epoll.h>
#endif /* PROVIED_EPOLL_HEADERS */
#endif /* HAVE_EPOLL */

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>
#include <sys/resource.h>

#include "debug.h"
#include "stats.h"
#include "common.h"
#include "options.h"
#include "util.h"
#include "inet.h"

#ifdef FreeBSD
#include <sys/mman.h>
#endif

/* AS: MAP_ANON is depricated in Linux but is used elsewhere */
#if defined(FreeBSD) || defined(DARWIN)
#define MAP_ANONYMOUS    (MAP_ANON)
#endif

//#define LOCAL_DEBUG
#include "local_debug.h"


#define KB     (1024)
#define MB     (1024*1024)

#ifdef HAVE_CPUSET
static void print_range_if_needed(int start, int end);
#endif /* HAVE_CPUSET */

int get_actual_sock_status(int sd, long timeout, char which);

alloc_track_t alloc_track[MAX_ALLOCED];

int alloc_count = 0;
size_t alloced_bytes = 0;
size_t freed_bytes = 0;

void *
alloc_aligned_track(unsigned int alignment, size_t size, int flags,
    const char *print_str, alloc_track_t **caller_track)
{
  int index;
  alloc_track_t *track;
  void *start = 0;
  void *adjusted = 0;
  uintptr_t alignment_mask = 0;
  uintptr_t diff = 0;
  unsigned int check = 0;
  int use_mmap = 0;
  int i;

  if (print_str == 0) {
    print_str = "";
  }

  ldbg("alloc_aligned_track: %s alignment = %u (0x%x) size = %Zu flags = 0x%x\n",
          print_str, alignment, alignment, size, flags);

  for (index = 0; index < MAX_ALLOCED; index++) {
    if (!alloc_track[index].in_use) {
      break;
    }
  }
  if (index >= MAX_ALLOCED) {
    printf("alloc_aligned_track: called too many times MAX_ALLOCED = %d\n",
	MAX_ALLOCED);
    exit(1);
  }
  track = &alloc_track[index];

  if (alignment != ALLOC_NOALIGN) {
    for (i = 1; i < (int) sizeof(unsigned int) * 8; i++) {
      check = 1 << i;
      alignment_mask = (alignment_mask << 1) | 1;
      ldbg("alloc_aligned_track: i = %d check = %u (0x%x)\n", i, check, check);
      if (alignment == check) {
	break;
      }
    }

    if (alignment != check) {
      printf("alloc_aligned_track: %s alignment = %u not a power of 2\n",
	  print_str, alignment);
      exit(1);
    }
  }

  errno = 0;

  switch (flags & 0xFFFF) {

    case ALLOC_MALLOC:
      start = malloc(size + alignment);
      break;

    case ALLOC_MMAP:
      use_mmap = 1;
      start = mmap(NULL, size + alignment, PROT_READ | PROT_WRITE,
	  MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
      if (start == MAP_FAILED) {
	start = NULL;
      }
      break;

    case ALLOC_MMAP_FILE:
      use_mmap = 1;
      strcpy(track->mmap_file, "/tmp/userver.XXXXXXXX");
      track->mmap_fd = mkstemp(track->mmap_file);
      if (track->mmap_fd < 0) {
        perror("alloc_aligned_track: mkstemp failed");
	printf("alloc_aligned_track: %s mkstemp '%s' failed errno = %d\n",
	    print_str, track->mmap_file, errno);
	exit(1);
      }
      if (ftruncate(track->mmap_fd, size + alignment) < 0) {
        perror("alloc_aligned_track: ftruncate failed");
	printf("alloc_aligned_track: %s ftruncate(size = %Zu) failed errno = %d\n",
	    print_str, size + alignment, errno);
	exit(1);
      }

      /*
       * Note that MAP_PRIVATE doesn't allow us to use sendfile(), since
       * changes we make do not get reflected in the file.  Therefore, we
       * use MAP_SHARED.
       */
      start = mmap(NULL, size + alignment, PROT_READ | PROT_WRITE,
	  MAP_SHARED, track->mmap_fd, 0);
      if (start == MAP_FAILED) {
	start = NULL;
      }
      break;

    default:
      printf("alloc_aligned_track: invalid flags 0x%x\n", flags);
      exit(1);
  }

  if (start == NULL) {
    perror("alloc_aligned_track: allocation failed");
    printf("alloc_aligned_track: %s %s failed errno = %d\n",
	print_str,
	(flags & (ALLOC_MMAP | ALLOC_MMAP_FILE)) ? "mmap" : "malloc",
	errno);
    exit(1);
  } else {
    // printf("alloc_aligned_track: start = %p\n", start);
    adjusted = start;
  }

  if (flags & ALLOC_MLOCK) {
#ifdef __linux__
    {
      /*
       * Linux supports a resource limit on the amount of memory a user can
       * lock.  Before calling mlock(), try to raise this limit to infinity.
       * Only root should be able to do this.  If setrlimit() fails, it's
       * not necessarily fatal; mlock() might still succeed.
       */
      struct rlimit rlim;

      rlim.rlim_cur = RLIM_INFINITY;
      rlim.rlim_max = RLIM_INFINITY;
      if (setrlimit(RLIMIT_MEMLOCK, &rlim) < 0) {
	ldbg("alloc_aligned_track: %s setrlimit failed errno = %d\n",
	    print_str, errno);
      }
    }
#endif /* __linux__ */

    /*
     * Although Linux allows you to pass any address to mlock(), POSIX
     * specifies that the address must be page-aligned.
     */
    diff = (uintptr_t) start % pagesize;
    if (mlock((char *) start - diff, size + alignment + diff) < 0) {
      printf("alloc_aligned_track: %s mlock failed errno = %d\n", print_str, errno);
      exit(1);
    }
  }

  if (flags & ALLOC_ZERO) {
    /*
     * Zero the newly allocated memory region.
     */
    memset(start, '\0', size + alignment);
  }

  if ((flags & ALLOC_TOUCH) && !(flags & (ALLOC_MLOCK | ALLOC_ZERO))) {
    /*
     * Touch at least one byte on each memory page.  We do this by touching
     * the first byte, and every pagesize'th byte after that, and finally the
     * last byte for good measure.
     * Note that we don't bother touching if we are already locking and/or
     * zeroing the memory, as it would be pointless.
     */
    for (diff = 0; diff < size + alignment; diff += pagesize) {
      ((char *) start)[diff] = '\0';
    }
    ((char *) start)[size + alignment - 1] = '\0';
  }

  if (alignment != 0) {
    alignment_mask = ~alignment_mask;
    if (((uintptr_t) adjusted & alignment_mask) != (uintptr_t) adjusted) {
      adjusted = (void *) (((uintptr_t) ((uintptr_t) start + alignment)) & alignment_mask);
      /* check alignment now */
      ldbg("alloc_aligned_track: start = %p adjusted = %p, alignment = 0x%x "
	     "alignment_mask = 0x%tx\n",
	    start, adjusted, alignment, alignment_mask);

      ldbg("alloc_aligned_track: adjusted & alignment_mask = 0x%tx\n",
	  (uintptr_t) adjusted & alignment_mask);

      if (((uintptr_t) adjusted & alignment_mask) != (uintptr_t) adjusted) {
	printf("alloc_aligned_track: %s failed to align addr = %p "
	    "alignment = 0x%x\n", print_str, adjusted, alignment);
	exit(1);
      }
    } else {
      ldbg("alloc_aligned_track: is aligned\n");
      ldbg("alloc_aligned_track: start = %p adjusted = %p, alignment = 0x%x "
	     "alignment_mask = 0x%tx\n",
	    start, adjusted, alignment, alignment_mask);

      ldbg("alloc_aligned_track: adjusted & alignment_mask = 0x%tx\n",
	  (uintptr_t) adjusted & alignment_mask);

    }
  } else {
    adjusted = start;
  }

  diff = (uintptr_t) adjusted - (uintptr_t) start;

  track->in_use = 1;
  track->adjusted_addr = adjusted;
  track->real_addr = start;
  track->orig_size = size;
  track->size = size + diff;
  track->flags = flags;
  alloc_count++;

  alloced_bytes += size;
  if (alloced_bytes < size) {
    printf("WARNING: alloced_bytes overflow\n");
    printf("total alloced_bytes = %Zu size = %Zu\n", alloced_bytes, size);
    // exit(1);
  }
#if 0
  printf("alloc_aligned_track: %s returning %p\n",
      print_str, adjusted);
  printf("alloc_aligned_track: alloced = %Zu (%Zu MB) total alloced = %Zu (%Zu MB)\n",
      size, size / MB, alloced_bytes, alloced_bytes / MB);
  printf("alloc_aligned_track: was adjusted by %ld\n\n", (long) diff);
#endif

  /* Does the caller want access to the alloc_track structure? */
  if (caller_track != NULL) {
    *caller_track = track;
  }

  return adjusted;
}


void *
alloc_aligned(unsigned int alignment, size_t size, int flags,
    const char *print_str)
{
  return alloc_aligned_track(alignment, size, flags, print_str, NULL);
}


char *
strdup_aligned(const char *src)
{
  char *dst;
  int len;

  assert(src != NULL);
  len = strlen(src);

  dst = (char *) alloc_aligned(ALLOC_NOALIGN, len + 1,
      ALLOC_MALLOC, "strdup_aligned");
  if (dst != NULL) {
    memcpy(dst, src, len + 1);
  }

  return dst;
}


static void
free_aligned_index(int index)
{
  alloc_track_t *track;

  assert(index >= 0);
  assert(index < MAX_ALLOCED);

  track = &alloc_track[index];
  if (!track->in_use) {
    printf("free_aligned_index: index %d not in use!\n", index);
    exit(1);
  }
  ldbg("free_aligned_index: freeing index %d addr %p flags 0x%x\n",
      index, track->adjusted_addr, track->flags);

  if (track->flags & ALLOC_MLOCK) {
    munlock(track->real_addr, track->orig_size);
  }

  switch (track->flags & 0xFFFF) {
    case ALLOC_MALLOC:
      free(track->real_addr);
      break;
    case ALLOC_MMAP:
      munmap(track->real_addr, track->orig_size);
      break;
    case ALLOC_MMAP_FILE:
      munmap(track->real_addr, track->orig_size);
      close(track->mmap_fd);
      //printf("free_aligned_index: Not deleting mmap file\n");
      unlink(track->mmap_file);
      break;
    default:
      printf("free_aligned_index: invalid flags 0x%x\n", track->flags);
      exit(1);
  }

  freed_bytes += track->orig_size;
  if (freed_bytes < track->orig_size) {
    printf("WARNING: freed_bytes overflow\n");
  }
  ldbg("free_aligned_index: freed %d bytes total freed %lu\n",
      track->orig_size, (unsigned long) freed_bytes);

  track->in_use = 0;
}


void
free_aligned(void *adjusted_start)
{
  int index;
  alloc_track_t *track;

  for (index = 0; index < MAX_ALLOCED; index++) {
    track = &alloc_track[index];
    if (track->adjusted_addr == adjusted_start) {
      free_aligned_index(index);
      return;
    }
  }

  printf("free_aligned: trying to free invalid ptr %lx\n",
      (unsigned long) adjusted_start);
  exit(1);
}


void
free_aligned_all(void)
{
  int index;
  alloc_track_t *track;

  for (index = MAX_ALLOCED - 1; index >= 0; index--) {
    track = &alloc_track[index];
    if (track->in_use) {
      free_aligned_index(index);
    }
  }

  if (freed_bytes != alloced_bytes) {
    printf("free_aligned_all: freed_bytes %Zu != alloced_bytes %Zu\n",
	freed_bytes, alloced_bytes);
    exit(1);
  }
}


int
ulong_digits(unsigned long x)
{
  int ret = 1;

  while (x >= 10) {
    x /= 10;
    ret++;
  }

  return ret;
}


int
max_ulong_digits(void)
{
  static int ret = -1;

  if (ret == -1) {
    ret = ulong_digits(ULONG_MAX);
  }

  return ret;
}


void
hexdump(const void *buf, int len)
{
  int offset = 0;
  int line_offset;

  while (offset < len) {
    printf("%08X:", offset);
    for (line_offset = 0; line_offset < 16; line_offset++) {
      if ((line_offset % 4) == 0)
	printf(" ");
      if (offset + line_offset < len)
	printf(" %02X", ((unsigned char *) buf)[offset + line_offset]);
      else
	printf("   ");
    }
    printf("  ");
    for (line_offset = 0; line_offset < 16; line_offset++) {
      if (offset + line_offset >= len)
	break;
      printf("%c", isprint(((unsigned char *) buf)[offset + line_offset]) ?
	  ((unsigned char *) buf)[offset + line_offset] : '.');
    }
    offset += 16;
    printf("\n");
  }
}


int
stricmp(const char *s1, const char *s2)
{
  assert(s1 != NULL);
  assert(s2 != NULL);

  while (tolower(*s1) == tolower(*s2) && *s1 != '\0') {
    s1++, s2++;
  }

  return tolower(*s2) - tolower(*s1);
}


int
strnicmp(const char *s1, const char *s2, size_t n)
{
  assert(s1 != NULL);
  assert(s2 != NULL);

  while (n > 0 && tolower(*s1) == tolower(*s2)) {
    assert(*s1 != '\0' || n == 1);
    s1++, s2++, n--;
  }

  return n > 0 ? tolower(*s2) - tolower(*s1) : 0;
}


#define REENTRANCY_THRESHOLD 5

const char *
nice_char(char c)
{
  static char buf[REENTRANCY_THRESHOLD][5];
  static int bufind = -1;

  bufind = (bufind + 1) % REENTRANCY_THRESHOLD;
  if (isprint((int) c) && c != '\\') {
    buf[bufind][0] = c;
    buf[bufind][1] = '\0';
  } else {
    switch (c) {
      case '\\':
	strcpy(buf[bufind], "\\\\");
	break;
      case '\r':
	strcpy(buf[bufind], "\\r");
	break;
      case '\n':
	strcpy(buf[bufind], "\\n");
	break;
      case '\t':
	strcpy(buf[bufind], "\\t");
	break;
      default:
	sprintf(buf[bufind], "\\%03o", (unsigned char) c);
	break;
    }
  }

  return buf[bufind];
}


const char *
nice_strn(const char *s, int len)
{
  static char buf[REENTRANCY_THRESHOLD][1024];
  static int bufind = -1;
  const char *end;
  int i;

  if (s == NULL) {
    return "(null)";
  }

  bufind = (bufind + 1) % REENTRANCY_THRESHOLD;

  end = s + len;
  i = 0;
  while (s < end && i < 1020) {
    if (isprint((int) *s) && *s != '\\') {
      buf[bufind][i++] = *s;
    } else {
      switch (*s) {
	case '\\':
	  strcpy(buf[bufind] + i, "\\\\");
	  i += 2;
	  break;
	case '\r':
	  strcpy(buf[bufind] + i, "\\r");
	  i += 2;
	  break;
	case '\n':
	  strcpy(buf[bufind] + i, "\\n");
	  i += 2;
	  break;
	case '\t':
	  strcpy(buf[bufind] + i, "\\t");
	  i += 2;
	  break;
	default:
	  sprintf(buf[bufind] + i, "\\%03o", (unsigned char) *s);
	  i += 4;
	  break;
      }
    }
    s++;
  }
  buf[bufind][i] = '\0';

  return buf[bufind];
}


const char *
nice_str(const char *s)
{
  if (s == NULL) {
    return "(null)";
  }

  return nice_strn(s, strlen(s));
}


const char *
format_str(const char *format, ...)
{
  static char buf[REENTRANCY_THRESHOLD][1024];
  static int bufind = -1;
  va_list argptr;

  bufind = (bufind + 1) % REENTRANCY_THRESHOLD;

  va_start(argptr, format);
  vsprintf(buf[bufind], format, argptr);
  va_end(argptr);

  return buf[bufind];
}


const char *
sock_addr_str(const sockaddr_t *addr)
{
  static char buf[REENTRANCY_THRESHOLD][1024];
  static int bufind = -1;

  bufind = (bufind + 1) % REENTRANCY_THRESHOLD;

  switch (addr->u.sa.sa_family) {
    case AF_INET:
      sprintf(buf[bufind], "%s:%d", inet_ntoa(addr->u.sa_in.sin_addr),
	  ntohs(addr->u.sa_in.sin_port));
      break;
    case AF_UNIX:
      assert(addr->len < sizeof(addr->u.sa_un.sun_path));
      memcpy(buf[bufind], addr->u.sa_un.sun_path, addr->len);
      buf[bufind][addr->len] = '\0';
      break;
    default:
      assert(0);
  }

  return buf[bufind];
}


int
sock_path_to_addr(const char *path, sockaddr_t *addr, int dolookup,
    const char *defhost, int defport)
{
  if (path == NULL) {
    path = "";
  }

  if (strchr(path, '/') != NULL) {
    return sock_path_to_addr_un(path, addr);
  } else {
    return sock_path_to_addr_in(path, addr, dolookup, defhost, defport);
  }
}


int
sock_path_to_addr_un(const char *path, sockaddr_t *addr)
{
  size_t len;

  len = strlen(path);
  if (len >= sizeof(addr->u.sa_un.sun_path)) {
    printf("sock_path_to_addr_un: path too long\n");
    return -1;
  }

  addr->len = ((size_t) ((struct sockaddr_un *) 0)->sun_path) + len;
  addr->u.sa_un.sun_family = AF_UNIX;
  memcpy(addr->u.sa_un.sun_path, path, len + 1);

  return 0;
}


int
sock_path_to_addr_in(const char *host, sockaddr_t *addr, int dolookup,
    const char *defhost, int defport)
{
  char *colon;
  char hostbuf[128];
  size_t hostlen;
  int port;
  const struct hostent *he;

  if ((colon = strchr(host, ':')) != NULL) {
    /*
     * We don't want to destroy the caller's data by setting *colon to '\0',
     * so we make a local copy of the hostname portion.
     */
    if ((hostlen = colon - host) >= sizeof(hostbuf)) {
      printf("sock_path_to_addr_in: hostname too long\n");
      return -1;
    }
    strncpy(hostbuf, host, hostlen);
    hostbuf[hostlen] = '\0';
    host = hostbuf;
    port = atoi(colon + 1);
  } else {
    port = defport;  // default
  }

  addr->len = sizeof(addr->u.sa_in);
  memset(&addr->u.sa_in, '\0', sizeof(addr->u.sa_in));  // zero any padding
  addr->u.sa_in.sin_family = AF_INET;
  addr->u.sa_in.sin_port = htons(port);
  if (host[0] == '\0') {
    host = defhost;
  }
  if (host != NULL) {
    addr->u.sa_in.sin_addr.s_addr = inet_addr(host);
    if (addr->u.sa_in.sin_addr.s_addr == INADDR_NONE) {
      if (!dolookup) {
	printf("Unable to convert host '%s' to network usable addr\n", host);
	return -1;
      }
      he = gethostbyname(host);
      if (he == NULL) {
	printf("Unable to look up host '%s'\n", host);
	return -1;
      }
      addr->u.sa_in.sin_addr = *((struct in_addr *) he->h_addr);
    }
  } else {
    addr->u.sa_in.sin_addr.s_addr = htonl(INADDR_NONE);
  }

  return 0;
}


int
sock_addr_compare(const sockaddr_t *addr1, const sockaddr_t *addr2)
{
  assert(addr1 != NULL);
  assert(addr2 != NULL);

  if (addr1->len < addr2->len) {
    return -1;
  } else if (addr1->len > addr2->len) {
    return 1;
  }

  return memcmp(&addr1->u.sa, &addr2->u.sa, addr1->len);
}


int
sock_path_compare(const char *path1, const char *path2)
{
  sockaddr_t addr1, addr2;

  if (sock_path_to_addr(path1, &addr1, 1, OPT_IP_ADDR_ANY,
	DEF_APP_SERVER_PORT) < 0) {
    return -2;
  } else if (sock_path_to_addr(path2, &addr2, 1, OPT_IP_ADDR_ANY,
	DEF_APP_SERVER_PORT) < 0) {
    return 2;
  }

  return sock_addr_compare(&addr1, &addr2);
}

int
is_sock_readable(int sd, long timeout)
{
  return(get_actual_sock_status(sd, timeout, 'R'));
}

int
is_sock_writable(int sd, long timeout)
{
  return(get_actual_sock_status(sd, timeout, 'W'));
}

int
is_sock_hupped(int sd, long timeout)
{
  return(get_actual_sock_status(sd, timeout, 'H'));
}

int
get_actual_sock_status(int sd, long timeout, char which)
{
  struct timeval tv;
  fd_set fdset;
  int n = 0;
  int result = 0;
#ifdef HAVE_EPOLL
  int save_errno = 0;
  struct epoll_event ep_evt;
  unsigned int target;
  int rc = 0;
  extern int epoll_fd;
#endif /* HAVE_EPOLL */

  assert(sd > 2);

  if (sd < 0) {
    return 0;
  }

#ifdef HAVE_EPOLL
  if (options.use_epoll) {
    ep_evt.data.fd = sd;

    switch(which) {
    case 'R':
      target = EPOLLIN;
      ep_evt.events = target;
      break;

    case 'W':
      target = EPOLLOUT;
      ep_evt.events = target;
      break;

    case 'H':
      target = EPOLLHUP;
      ep_evt.events = target;
      break;

    default:
      printf("get_actual_sock_status: incorrect use of which = %c\n", which);
      exit(1);
    }
    
    rc = epoll_wait(epoll_fd, &ep_evt, 1, 0);
    save_errno = errno;

    if (rc < 0) {
      printf("get_actual_sock_status: epoll_wait FAILED on %d with rc = %d errno = %d\n", 
        sd, rc, save_errno);
    } else {
      result = ep_evt.events & target;
    }
    
  }  else {
#endif /* HAVE_EPOLL */
    FD_ZERO(&fdset);
    FD_SET(sd, &fdset);

    if (timeout < 0) {
      tv.tv_sec = 0;
      tv.tv_usec = -timeout;
    } else {
      tv.tv_sec = timeout;
      tv.tv_usec = 0;
    }

    if (which == 'R') {
      n = select(sd+1, &fdset, NULL, NULL, &tv);
    } else if (which == 'W') {
      n = select(sd+1, NULL, &fdset, NULL, &tv);
    } else if (which == 'H') {
      result = 0;
      return(result);
    }

    if (n < 0) {
      printf("get_actual_sock_status: select failed\n");
      perror("select:");
      exit(1);
    } else if (n > 1) {
      printf("get_actual_sock_status: incorrect call to select\n");
      exit(1);
    }
    result = n;

#ifdef HAVE_EPOLL
  }
#endif /* HAVE_EPOLL */
  return(result);

} /* get_actual_sock_status */


#ifdef OLDWAY
int
is_sock_writable(int sd, long timeout)
{
  struct timeval tv;
  fd_set fdset;
  int n = 0;
  assert(sd > 2);

  if (sd < 0) {
    return 0;
  }

  FD_ZERO(&fdset);
  FD_SET(sd, &fdset);
  if (timeout < 0) {
    tv.tv_sec = 0;
    tv.tv_usec = -timeout;
  } else {
    tv.tv_sec = timeout;
    tv.tv_usec = 0;
  }

  /*
  return select(getdtablesize(), NULL, &fdset, NULL, &tv) > 0 ? 1 : 0;
  */
  n = select(sd+1, NULL, &fdset, NULL, &tv);

  if (n < 0) {
    printf("is_sock_readable: select failed\n");
    perror("select:");
    exit(1);
  } else if (n > 1) {
    printf("is_sock_readable: incorrect call to select\n");
    exit(1);
  }

  return(n);
}
#endif /* OLDWAY */


/* like asctime() but using the RFC 1123 format */
const char *
http_asctime(const struct tm *tm)
{
  static char buf[REENTRANCY_THRESHOLD][64];
  static int bufind = -1;

  bufind = (bufind + 1) % REENTRANCY_THRESHOLD;

  if (http_strftime(buf[bufind], sizeof(buf[bufind]), tm) == 0) {
    buf[bufind][0] = '\0';
  }

  return buf[bufind];
}


/* like strftime() but using the RFC 1123 format */
size_t
http_strftime(char *buf, size_t maxsize, const struct tm *tm)
{
  return strftime(buf, maxsize, "%a, %d %b %Y %T GMT", tm);
}


/* like strptime() but using any of several predefined formats */
char *
http_strptime(const char *buf, struct tm *tm)
{
  char *ptr;

  ptr = strptime(buf, "%a, %d %b %Y %T GMT", tm);	/* RFC 1123 format */
  if (ptr != NULL && *ptr == '\0') {
    return ptr;
  }
  ptr = strptime(buf, "%A, %d-%b-%y %T GMT", tm);	/* RFC 850 format */
  if (ptr != NULL && *ptr == '\0') {
    return ptr;
  }
  ptr = strptime(buf, "%a %b %d %T %Y", tm);		/* asctime() format */
  if (ptr != NULL && *ptr == '\0') {
    return ptr;
  }

  return NULL;
}

/* convert a GMT time string to a local time_t value */
time_t
http_str_to_time(const char *buf, time_t *clock)
{
  struct tm tm;
  time_t t;

  if (http_strptime(buf, &tm) == NULL) {
    if (clock != NULL) {
      *clock = (time_t) -1;
    }
    return 0;
  }

#if defined(DARWIN) || defined(FreeBSD)
  t = timegm(&tm);
#else
#if defined(__CYGWIN__)
  t = mktime(&tm);
  /* This supposedly converts t into local time */
  localtime(&t);
#else
  t = mktime(&tm) - timezone;
#endif /* __CYGWIN__ */
#endif /* DARWIN || FreeBSD */
  if (clock != NULL) {
    *clock = t;
  }
  return t;
}


/* return a GMT time string equivalent to a given local time_t value */
const char *
http_time_to_str(time_t clock)
{
  return http_asctime(gmtime(&clock));
}


void
print_req(struct req *r)
{
  printf("      state     = %d\n", r->state);
  printf("      method    = %d\n", r->method);
  printf("      close     = %d\n", r->close);
  printf("      uri       = [%s]\n", nice_str(r->uri));
  printf("      tmp       = [%s]\n", nice_str(r->tmp));
  printf("      tmp_len   = %d\n", (int) r->tmp_len);
}


void
print_rep(struct rep *r)
{
  printf("      total_len = %ld\n", (unsigned long) r->total_len);
  printf("      nwritten  = %ld\n", (unsigned long) r->nwritten);
  printf("      fd        = %d\n", r->fd);

  if (r->cur != 0) {
    printf("      cur       = [%s]\n", nice_char(*r->cur));
  }

  if (r->end != 0) {
    printf("      end       = [%s]\n", nice_char(*r->end));
  }
}


void
do_event_loop_end(struct timeval *lru_now)
{
  if (cur_conns > num_max_cur_conns) {
    num_max_cur_conns = cur_conns;
  }
  num_cur_conn_total += cur_conns;
  num_cur_conn_samples++;

  print_running_stats(lru_now);
}

/* WARNING: THIS NEEDS TO BE FIXED BEFORE IT GETS USED */
/* IT ASSUMES a 32-bits arch */

#if 0
void *
start_of_this_page(void *addr)
{
  char *tmp = (char *) addr;

  /* TODO: FIX this it's not 64-bit clean and does work for different page sizes */
  while (((unsigned long) tmp & 0x00000fff) != 0) {
    tmp--;
  }
  
  printf("start_of_this_page: addr = %p start = %p\n", addr, tmp);
  return(tmp);
}
#endif

void
print_buf(char *str, int sd, void *start, int len)
{
  int i;
  char *buf = (char *) start;

  if (len < 0) {
    printf("print_buf: len = %d is < 0\n", len);
  }

  printf("print_buf: from %s sd = %d len = %d addr = %p\n", 
      str, sd, len, start);
  printf("print_buf: contents: --start--\n");
  for (i=0; i<len; i++) {
    printf("%c", buf[i]); 
  }
  printf("\n---end---\n");
}

void
print_sendfile_buf(char *str, int sd, int file_fd, off_t offset, int len)
{
  struct stat statbuf;
  int i;
  char *buf = 0;
  if (len < 0) {
    printf("print_sendfile_buf: len = %d is < 0\n", len);
  }

  printf("print_sendfile_buf: from %s sd = %d file_fd = %d len = %d\n",
          str, sd, file_fd, len);

  if (fstat(file_fd, &statbuf) < 0) {
    perror("fstat failed");
    printf("print_sendfile_buf: fstat failed\n");
    exit(1);
  }
  
  printf("print_sendfile_buf: file size = %lu\n", (unsigned long) statbuf.st_size);

  printf("print_sendfile_buf: len = %d file_fd = %d offset = %lu\n", 
          len, file_fd, (unsigned long) offset);
  buf = (char *) mmap(0, len+offset, PROT_READ, MAP_PRIVATE, file_fd, 0);
  if (buf == MAP_FAILED) {
    perror("mmap");
    printf("print_sendfile_buf: unable to mmap file\n");
    exit(1);
  }

  printf("print_sendfile_buf: contents: --start--\n");
  for (i=offset; i<len+offset; i++) {
    printf("%c", buf[i]); 
  }
  printf("\n---end---\n");

  if (munmap(buf, len) < 0) {
    perror("munmap");
    printf("print_sendfile_buf: munmap failed\n");
    exit(1);
  }
}

#ifdef HAVE_CPUSET

void
print_cpuset(char *str, cpu_set_t *set)
{
	fprint_cpuset(stdout, str, set);
#ifdef OLDWAY
  int i;
  printf("%s", str);
  for (i=0; i<CPU_SETSIZE; i++) {
    if ((i % 80 == 0) && (i != 0)) {
      printf("\n");
    }
    if (CPU_ISSET(i, set)) {
      printf("1");
    } else {
      printf("0");
    }
  }
#endif /* OLDWAY */
}


void
fprint_cpuset(FILE *f, char *str, cpu_set_t *set)
{
  int i;
	if (str != NULL) {
	  fprintf(f, "%s", str);
	}
  for (i=0; i<CPU_SETSIZE; i++) {
    if ((i % 80 == 0) && (i != 0)) {
      fprintf(f, "\n");
    }
    if (CPU_ISSET(i, set)) {
      fprintf(f, "1");
    } else {
      fprintf(f, "0");
    }
  }
}

#ifdef OLDWAY
void
print_cpuset_values(char *str, cpu_set_t *set)
{
  int i;
	int prev = INT_MIN;
	int need_end = 0;
  printf("%s", str);

  for (i=0; i<CPU_SETSIZE; i++) {
    if (CPU_ISSET(i, set)) {
			if (prev == (i-1)) {
				if (need_end == 0) {
           printf("-");
				   need_end = 1;
				}
			} else {
				if (need_end) {
          printf("%d, ", prev);
					need_end = 0;
				} else {
          printf("%d", i);
				}
			}
			prev = i;
    } 
  }
	if (need_end) {
		printf("%d", i);
	}
}

#else
void
print_cpuset_values(char *str, cpu_set_t *set)
{
  int i;
	int start = INT_MIN;
	int end = INT_MIN;
  printf("%s", str);

  for (i=0; i<CPU_SETSIZE; i++) {
    if (CPU_ISSET(i, set)) {
			/* collect the range */
			if (start == INT_MIN) {
				start = i;
			}
			end = i;
    } else {
			/* print the range */
			print_range_if_needed(start, end);
			start = INT_MIN;
		  end = INT_MIN;
		}
  }
	print_range_if_needed(start, end);
}

static void
print_range_if_needed(int start, int end)
{
	if (start != INT_MIN) {
		if (start == end) {
			printf("%d, ", start);
		} else {
			printf("%d-%d, ", start, end);
		}
	} 
}
#endif /* OLDWAY */

#endif /* HAVE_CPUSET */


