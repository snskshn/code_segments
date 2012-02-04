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


#ifndef UTIL_H
#define UTIL_H

#if defined(FreeBSD) || defined(DARWIN)
#include <sys/time.h>
#endif
#include <time.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>

#ifdef HAVE_CPUSET
#include <sched.h>
#endif

/* the number of individual allocations we are capable of tracking */
#define MAX_ALLOCED     8192

/* data structure for tracking buffer allocations */
typedef struct alloc_track_struct {
  int in_use;             /* in-use flag */
  void *adjusted_addr;    /* this is the addr to return to the caller */
  void *real_addr;        /* this is the addr to pass to free() or munmap() */
  size_t orig_size;          /* size of bytes asked for */
  size_t size;               /* size of bytes alloced */
  int flags;              /* alloc flags */
  char mmap_file[32];     /* name of mmap backing store (ALLOC_MMAP_FILE) */
  char mmap_fd;           /* fd of opened mmap_file */
} alloc_track_t;

extern alloc_track_t alloc_track[MAX_ALLOCED];

/* alloc stats */
extern int alloc_count;
extern size_t alloced_bytes;
extern size_t freed_bytes;

/* alignment */
#define ALLOC_NOALIGN		0

/* mode flags -- use exactly one of these */
#define ALLOC_MALLOC		0x1		// use malloc()
#define ALLOC_MMAP		0x2		// use anonymous mmap()
#define ALLOC_MMAP_FILE		0x4		// use file-backed mmap()
/* optional flags -- OR these in as desired */
#define ALLOC_ZERO		0x10000		// zero the memory
#define ALLOC_MLOCK		0x20000		// use mlock()
#define ALLOC_TOUCH		0x40000		// touch each page

void *alloc_aligned_track(unsigned int alignment, size_t size, int flags,
    const char *print_str, alloc_track_t **caller_track);
void *alloc_aligned(unsigned int alignment, size_t size, int flags,
    const char *print_str);
char *strdup_aligned(const char *s);
void free_aligned(void *adjusted_start);
void free_aligned_all(void);


int max_ulong_digits(void);
int ulong_digits(unsigned long x);
void hexdump(const void *buf, int len);
int stricmp(const char *s1, const char *s2);
int strnicmp(const char *s1, const char *s2, size_t n);
const char *nice_char(char c);
const char *nice_strn(const char *s, int len);
const char *nice_str(const char *s);
const char *format_str(const char *format, ...);


typedef struct sockaddr_u {
  socklen_t len;
  union {
    struct sockaddr sa;
    struct sockaddr_un sa_un;
    struct sockaddr_in sa_in;
  } u;
} sockaddr_t;

const char *sock_addr_str(const sockaddr_t *addr);
int sock_path_to_addr(const char *path, sockaddr_t *addr, int dolookup,
    const char *defhost, int defport);
int sock_path_to_addr_un(const char *path, sockaddr_t *addr);
int sock_path_to_addr_in(const char *path, sockaddr_t *addr, int dolookup,
    const char *defhost, int defport);
int sock_addr_compare(const sockaddr_t *addr1, const sockaddr_t *addr2);
int sock_path_compare(const char *path1, const char *path2);
int is_sock_readable(int sd, long timeout);
int is_sock_writable(int sd, long timeout);
int is_sock_hupped(int sd, long timeout);


const char *http_asctime(const struct tm *tm);
size_t http_strftime(char *buf, size_t maxsize, const struct tm *tm);
char *http_strptime(const char *buf, struct tm *tm);
time_t http_str_to_time(const char *buf, time_t *clock);
const char *http_time_to_str(time_t clock);


void do_event_loop_end(struct timeval *lru_now);

void *start_of_this_page(void *addr);
void print_buf(char *str, int sd, void *start, int len);
void print_sendfile_buf(char *str, int sd, int file_fd, off_t offset, int len);
#ifdef HAVE_CPUSET
void print_cpuset(char *str, cpu_set_t *set);
void print_cpuset_values(char *str, cpu_set_t *set);
void fprint_cpuset(FILE*, char *str, cpu_set_t *set);
#endif

#endif /* UTIL_H */
