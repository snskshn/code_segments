/*
    userver -- (pronounced you-server or micro-server).
    This file is part of the userver, a high-performance web server designed for
    performance experiments.

    Copyright (C) 2005-2010 Tim Brecht

    Contributions by Tim Brecht <brecht@cs.uwaterloo.ca>
                     David Pariag <db2paria@cs.uwaterloo.ca>
                     Amol Shukla <ashukla@real.com>
                         (DARWIN mods)
                     Brian Lynn <brian.lynn@hp.com> 
                         (cache warming code)

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

#ifndef CACHE_MAPPED_NEW_H
#define CACHE_MAPPED_NEW_H

#ifdef DARWIN
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#else
#include <sys/uio.h>
#endif /* DARWIN */

 
/* NOTE: should choose these so that they are multiples of the word
 * size or the cache line size
 */
#define CACHED_HDR_SIZE (128)

/* Note that this has been adjusted so the entire cacheinfo
 * structure is hopefully a multiple of the estimated cache line size
 */
#define CACHE_NAME_MAX  (64)
 
/* NOTE: should try to ensure that each cacheinfo entry 
 * and probably the header and name
 * are aligned to at least word boundaries - if not cache line boundaries
 */
typedef struct cacheinfo {
  char *header;               /* rep.iovec[0] will point to this */
  int  reference;             /* Reference counter to be used by
                                 GCLOCK cache replacement strategy */
  int  reply_count;           /* number of times a full reply gets sent */
  int  use_count;             /* number of times it's been used */
  int  buf_len;               /* Number of bytes in the buffer */
  char *buf;                  /* Buffer to hold the file */
  int  header_len;            /* Length of the reply header */
  int  fd;                    /* fd of underlying file - used with sendfile */
  int ref_count;
  int next;                   /* next element in chain or empty list */
  int prev;                   /* prev element in chain or empty list */
  int lru_next;               /* index for LRU purposes */
  int lru_prev;
  int hash_to;
  time_t last_modified;       /* When was file last modified? */
  time_t last_stat;           /* When was file last fstat'd */

#ifdef HAVE_ENCRYPTION
  int encryptfd;                /* encyrption buf by fd for sendfile */
  char *encryptbuf;             /* encryption buffer address */
  char *encryptmalloc;          /* buffer address from malloc */
  char encryptfname[32];        /* file name if using send file */
#endif /* HAVE_ENCRYPTION */

  void *file_reg_handle;        /* file region handle mem reg/deregister */
  void *header_reg_handle;      /* header region handle mem reg/deregister */
  void *encrypt_buf_handle;     /* handle for encryption buffer */
  char name[CACHE_NAME_MAX];    /* Name/Path of the file being cached */

} cacheinfo;

typedef struct fileCache {
  cacheinfo * hashTable;   /* A hash table to hold cached files */
  int clockHand;           /* Circular pointer used in the CLOCK  */
                           /*    cache replacement algorithm */
  int numEntries;          /* Number of entries currently cached in table */
  int tableSize;           /* Number of buckets in the hash table */
  int totalSize;           /* Number of buckets for chaining */
  double numBytes;         /* Number of bytes currently used for caching */
  double maxBytes;         /* Max number of bytes that can be cached */
  double maxFileSize;      /* Size of largest file that should be cached */
  double maxLoadFactor;    /* The maximum load factor (as a percentage) */
  int lockPages;           /* Lock pages in memory if this flag is non-zero */
  int initDone;            /* Has the table been initialized. */
  int firstFree;           /* First element in the list of free buckets */
  int lruFirst;            /* First element in the list of LRU buckets */
  int lruLast;             /* Last element in the list of LRU buckets */
  int *indexTable;
#ifdef HAVE_AIO_LAYER
  void *headers_handle;
#endif
  void *headers;
  int headers_size;
} fileCache;


typedef struct cacheStats {
  double cacheRequests;     /* Number of cache requests processed */
  double numHits;           /* Number of cache (file) hits */
  double bytesRequested;    /* Number of bytes requested */
  double bytesServed;       /* Number of bytes served from cache */
  double numCollisions;     /* Number of hash table collisions */
  double numEvictions;      /* Number of cache replacements/evictions that 
			       were made */
  double numProbes;         /* Number of probes done during lookups */
  double maxProbes;         /* Max number of probes done during a lookup */
  double workingSetMax;     /* Maximum working set size */
  double workingSetTotal;   /* Total working set size */
  double workingSetCur;     /* Currrent working set size */
  double workingSetCount;   /* Num sample points for working set size */
  double workingSetAvg;     /* Average working set size */
} cacheStats;


int initCache(int tableSize, double maxBytes, double maxFileSize, 
              double maxLoadFactor, int lockPages);

cacheinfo * cache_add(char * uri, int http_version);
int cache_lookup(char * uri, int *index, int *hash_to);
int check_if_modified_since( cacheinfo * cur, time_t since );
void cacheClear();
void printCacheStats();
void cache_print_stats();
void cache_clear_stats();
void cache_cleanup();
void cache_inc_ref_count(struct cacheinfo *c);
void cache_dec_ref_count(struct cacheinfo *c);
void cache_check_ref_counts(void);

#endif /* CACHE_MAPPED_NEW_H */
