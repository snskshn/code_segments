/*
    userver -- (pronounced you-server or micro-server).
    This file is part of the userver, a high-performance web server designed for
    performance experiments.
          
    This file is Copyright (C) 2004-2010  Tim Brecht

    Original Author: David Pariag
    Additional Authors: Tim Brecht <brecht@cs.uwaterloo.ca>
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


/* 
 * Author: David Pariag
 * Date: Friday January 3, 2003
 */

#ifndef CACHE_MAPPED_H
#define CACHE_MAPPED_H

#include <sys/uio.h>

 
/* NOTE: should choose these so that they are multiples of the word
 * size of the cache line size
 */
#define CACHE_NAME_MAX  (64)
#define CACHED_HDR_SIZE (128)
 
/* NOTE: should try to ensure that each cacheinfo entry 
 * and probably the header and name
 * are aligned to at least word boundaries - if not cache line boundaries
 */
typedef struct cacheinfo {
  int  reference;               /* Reference counter to be used by  */
                                /*   GCLOCK cache replacement strategy */
  int  use_count;               /* Use count - number of times it's been used */
  int  buf_len;                 /* Number of bytes in the buffer */
  char *buf;                    /* Buffer to hold the file */
  int  header_len;              /* Length of the reply header */
  char header[CACHED_HDR_SIZE]; /* rep.iovec[0] will point to this */
  char name[CACHE_NAME_MAX];    /* Name/Path of the file being cached */
} cacheinfo;


typedef struct fileCache {
  cacheinfo * hashTable; /* A hash table to hold cached files */
  int clockHand;         /* Circular pointer used in the CLOCK 
			    cache replacement algorithm */
  int numEntries;        /* Number of entries currently cached in table */
  int tableSize;         /* Number of buckets in the hash table */
  int numBytes;          /* Number of bytes currently used for caching */
  int maxBytes;          /* Max number of bytes that can be cached */
  int maxFileSize;       /* Size of the largest file that should be cached (in bytes) */
  int maxLoadFactor;     /* The maximum load factor (as a percentage) */
  int lockPages;         /* Lock pages in memory if this flag is non-zero */
} fileCache;


typedef struct cacheStats {
  double cacheRequests;     /* Number of cache requests processed */
  double numHits;           /* Number of cache (file) hits */
  double bytesRequested;    /* Number of bytes requested */
  double bytesServed;       /* Number of bytes served from cache */
  double numCollisions;     /* Number of hash table collisions */
  double numEvictions;      /* Number of cache replacements/evictions 
			       that were made */
  double numProbes;         /* Number of probes done during lookups */
  double maxProbes;         /* Max number of probes done during a lookup */
} cacheStats;


int initCache(int tableSize, int maxBytes, int maxFileSize, 
              int maxLoadFactor, int lockPages);

cacheinfo *cache_add(char * uri);

void cacheClear(void);

void printCacheStats(void);
void cache_print_stats(void);
void cache_cleanup(void);
 
#endif /* CACHE_MAPPED_H */
