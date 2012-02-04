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


/* 
 *---------------------------------------------------------------------
 * Author: Tim Brecht and David Pariag
 * Date: March, 2003
 *
 * NOTE: See USE_LRU ifdef's and #define
 *
 * Assumptions:
 *  - The cache will not be asked to store empty (zero-length) strings.
 *  - Cache entries are evicted when the cache's memory is exhausted!
 *  - If the maximum load factor is exceeded, then the server exits.
 *
 *  TBB 
 *  I've modified the original cache_mapped code for this version.
 *  It does external chaining instead of linear probing (but uses
 *  a preallocated array) in order to avoid mallocs and frees.
 *
 *  I've also implemented a true LRU replacement algorithm here.
 *  Note that this isn't as good as something like Greedy-Dual Size
 *  but it's simple to implement and likely does quite well on
 *  the types of workloads we are interested in.
 *
 *  TODO: Check collision stats?
 *---------------------------------------------------------------------
 */

#ifdef CACHE_MAPPED_NEW

#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <errno.h>
#include <dirent.h>

#ifdef HAVE_ENCRYPTION
#ifdef __linux__
#include <asm/page.h>
#endif
#endif

#if !defined (FreeBSD) && !defined(DARWIN)
#include <malloc.h>
#endif

/* For testing... */

#include <stdlib.h>
#include <time.h>

/* userver headers */
#include "syscalls.h"
#include "cache_mapped_new.h"
#include "myassert.h"
#include "trace.h"
#include "options.h"
#include "common.h"
#include "debug.h"
#include "aio_layer.h"
#include "util.h"
#include "reply_status.h"
#include "stats.h"

#define  DONT_INCLUDE_INDEX    (0)
#define  INCLUDE_INDEX         (1)

/* For turning on and off debugging local to this file */
// #define LOCAL_DEBUG 
#include "local_debug.h"

#define NULL_INDEX      (-1)

#define BUF_SIZE        (8*1024)

/* The cache data is stored in this global variable */
static fileCache cache;

/* Statistics about the cache performance */
cacheStats stats;


char *http_content_type(char *uri);
int cache_lookup(char * uri, int *index, int *hash_to);
int hash(char *uri);
int spec_hash(char *uri);
int rehash(char *uri, int extaval);
int hash_check_size(int size);
int cache_victim(void);
void cache_free_element(int index);
void cache_delete_element(int index);
void cache_add_element(int index, int hash_to);
int cache_get_element(void);
void cache_lru_add(int index);
void cache_lru_access(int index);
void cache_lru_delete(int index);
void cache_clear_all_used(void);
void cache_clear_ref_count(struct cacheinfo *c);

void printBucket(int index, int which);
void printBucketHdr(int include_index);
void printLruList(void);
void printFreeList(void);

void working_set_add(int size);
void working_set_remove(int size);
/*---------------------------------------------------------------------*/
/*
 * Initialize the cache by:
 *  - Allocating memory for the hash table.
 *  - Initialize each bucket to be "empty" (i.e. holding sentinel values)
 *  - Storing the limits on memory size and number of buckets.
 * Return 1 on success, 0 on failure
 */
int 
initCache(int tableSize, double maxBytes, double maxFileSize, 
		           double maxLoadFactor, int lockPages) 
{
  int i;
  int newTableSize;
  int size = 0;
  struct cacheinfo *c = 0;
  char *tmp = 0;
#ifdef HAVE_AIO_LAYER
  int trace_fd = 0;
  int rc = 0;
#endif


  printf("(sizeof(cacheinfo) = %lu\n", (unsigned long int) sizeof(cacheinfo));
  if (sizeof(cacheinfo) % OPT_CACHE_LINE_EST != 0) {
    printf("cacheinfo structure size is not multiple of "
	"OPT_CACHE_LINE_EST = %d\n", OPT_CACHE_LINE_EST);
  }

  newTableSize = hash_check_size(tableSize);
  if (newTableSize < 0) {
    printf("Couldn't adjust hash table size to closest prime\n");
    assert(0);
    exit(1);
  }

  if (newTableSize != tableSize) {
    printf("initCache: tableSize = %d not suitable for hash function\n",
		    tableSize);
    tableSize = newTableSize;
    printf("initCache: changed tableSize now = %d\n", tableSize);
  }

  if (maxLoadFactor > 1.0) {
     cache.totalSize = (int)((double) tableSize * maxLoadFactor);
  } else {
     cache.totalSize = tableSize;
  }
  printf("initCache: tableSize = %d\n", tableSize);
  printf("initCache: total size = %d\n", cache.totalSize);

  /* Allocate memory for the hash table elements */
  size = sizeof(cacheinfo) * cache.totalSize;
  cache.hashTable = (cacheinfo*) alloc_aligned(cache_line_size, size,
      ALLOC_MALLOC, "initCache: (hashTable)");
  
  /* Allocate memory for the index table */
  size = sizeof(int) * tableSize;
  cache.indexTable = (int *) alloc_aligned(cache_line_size, size,
      ALLOC_MALLOC, "initCache: (indexTable)");

  /* Initialize cache structure */
  cache.clockHand = 0;
  cache.tableSize = tableSize;
  cache.numEntries = 0;
  cache.maxBytes = maxBytes;
  cache.numBytes = 0.0;
  cache.maxFileSize = maxFileSize;
  assert (maxLoadFactor > 0.0);
  cache.maxLoadFactor = maxLoadFactor;
  cache.lockPages = lockPages;

  cache.firstFree = NULL_INDEX;

  /* allocate space for all of the cached headers */
  size = cache.totalSize * CACHED_HDR_SIZE;
  tmp = (char *) alloc_aligned(pagesize, size, ALLOC_MMAP,
      "initCache: (cached headers)");

  printf("initCache: header entries = %d size = %d total size = %d\n", 
          cache.totalSize, CACHED_HDR_SIZE, size);

  cache.headers = tmp;
  cache.headers_size = size;

#ifdef HAVE_AIO_LAYER
  if (options.use_socket_aio) {
    /* TODO: may need to change registration because of adjustments
     * being done in alloc_aligned
     */
    TRACE(EVT_AIO_SOCK_MEM_REG,
      trace_fd = 0;
      rc = aio_sock_mem_reg(cache.headers, cache.headers_size,
	&cache.headers_handle);
    );
    stats_reg_bytes(cache.headers_size);
    PRINT_TIME(NOFD, &tnow, &tprev, "initCache: aio_sock_mem_reg: "
	"addr %p size = %d handle = %p", cache.headers, 
	 cache.headers_size, cache.headers_handle);
  }
#endif /* HAVE_AIO_LAYER */

  /* Make sure hash table entries are empty */
  for (i = 0; i < cache.totalSize; i++) {
    if (i < cache.tableSize) {
      cache.indexTable[i] = NULL_INDEX;
    }
    c = &cache.hashTable[i];
    c->name[0] = '\0';
    c->buf = 0;
    c->buf_len = 0;
    c->reference = 0;
    cache_clear_ref_count(c);
    c->use_count = 0;
    c->reply_count = 0;
    c->fd = -1;
    c->next = NULL_INDEX;
    c->prev = NULL_INDEX;
    c->lru_next = NULL_INDEX;
    c->lru_prev = NULL_INDEX;
    c->header = tmp;
    tmp += CACHED_HDR_SIZE;
    c->header_len = 0;
    if (options.use_socket_aio) {
#ifdef HAVE_AIO_LAYER
      c->header_reg_handle = cache.headers_handle;
#endif /* HAVE_AIO_LAYER */
    }
    cache_free_element(i);
  }

  cache.lruFirst = NULL_INDEX;
  cache.lruLast = NULL_INDEX;

  cache.initDone = 1;

  /* Initialize statistics variables */
  stats.cacheRequests = 0.0;
  stats.numHits = 0.0;
  stats.bytesRequested = 0.0;
  stats.bytesServed = 0.0;
  stats.numCollisions = 0.0;
  stats.numEvictions = 0.0;
  stats.numProbes = 0.0;
  stats.maxProbes = 0.0;
  stats.workingSetTotal = 0.0;
  stats.workingSetMax = 0.0;
  stats.workingSetCur = 0.0;
  stats.workingSetCount = 0.0;

  return 1;
}


/*---------------------------------------------------------------------*/
void
cache_cleanup(void)
{
#ifdef HAVE_AIO_LAYER
  int rc = 0;
  int trace_fd = 0;

  if (options.use_socket_aio) {
    PRINT_TIME(NOFD, &tnow, &tprev, "cache_cleanup: aio_sock_mem_dereg "
	"start %p size = %d handle = %p", cache.headers, 
	cache.headers_size, cache.headers_handle);
    TRACE(EVT_AIO_SOCK_MEM_DEREG,
      trace_fd = 0;
      rc = aio_sock_mem_dereg(cache.headers, cache.headers_size,
	cache.headers_handle);
    );
    PRINT_TIME(NOFD, &tnow, &tprev, "cache_cleanup: aio_sock_mem_dereg rc = %d", rc);

    if (rc < 0) {
      printf("cache_cleanup: aio_sock_mem_dereg failed rc = %d\n", rc);
      printf("cache_cleanup: start = %p size = %d handle = %p\n",
	      cache.headers, cache.headers_size, cache.headers_handle);
    } else {
      stats_dereg_bytes(cache.headers_size);
    }
  }
#endif /* HAVE_AIO_LAYER */

  cacheClear();
}

/*---------------------------------------------------------------------*/
void
cacheClear(void)
{
  int i;
  void *addr;
  int size;
  struct cacheinfo *c = 0;
#ifdef HAVE_AIO_LAYER
  int rc = 0;
  int trace_fd = 0;
#endif /* HAVE_AIO_LAYER */
  
  for (i = 0; i < cache.totalSize; i++) {
    c = &cache.hashTable[i];
    if (c->buf_len > 0) {
      addr = c->buf;
      size = c->buf_len;
      if (cache.lockPages != 0) {
        munlock(addr, size);
      }
      munmap(addr, size);

      if (options.use_socket_aio) {
#ifdef HAVE_AIO_LAYER
        PRINT_TIME(NOFD, &tnow, &tprev, "cacheClear: aio_sock_mem_dereg "
	    "start %p size = %d "
	    "handle = %p", addr, size, c->file_reg_handle);

        if (c->file_reg_handle == NULL) {
          rc = 0;
        } else {
	  TRACE(EVT_AIO_SOCK_MEM_DEREG,
	    trace_fd = 0;
	    rc = aio_sock_mem_dereg(addr, size, c->file_reg_handle);
	  );
        }

	if (rc < 0) {
	  printf("cacheClear: aio_sock_mem_dereg failed (munmap) rc = %d\n", rc);
	  printf("cacheClear: addr = %p size = %d reg_handle = %p\n",
                  addr, size, c->file_reg_handle);
	  /* exit(1); */
	} else {
          stats_dereg_bytes(size);
        }
#else
	printf("cacheClear: aio not handled\n");
	exit(1);
#endif /* HAVE_AIO_LAYER */
      }

#ifdef HAVE_ENCRYPTION
      /* If we have an encryption buffer/file, clean it up */
      if ((options.encrypt_data) &&
        ((options.encrypt_max_file_size == 0UL) ||
         ((unsigned long)size <= options.encrypt_max_file_size))) {
        if (!options.use_sendfile) {
#ifdef HAVE_AIO_LAYER
          if (options.use_socket_aio) {
            aio_sock_mem_dereg(c->encryptbuf, size+8, c->encrypt_buf_handle);
          }
#endif /* HAVE_AIO_LAYER */
          free(c->encryptmalloc);
        } else {
          munmap(c->buf, size);
          munmap(c->encryptbuf, size+8);
          close(c->encryptfd);
          unlink(c->encryptfname);
        }
      }
#endif /* HAVE_ENCRYPTION */

      c->buf = 0;
      c->buf_len = 0;
      c->name[0] = '\0';
    }
    cache_clear_ref_count(c);
  }
  cache.numEntries = 0;  
  cache.numBytes = 0.0;
  cache.clockHand = 0;
  stats.workingSetTotal = 0.0;
  stats.workingSetMax = 0.0;
  stats.workingSetCur = 0.0;
  stats.workingSetCount = 0.0;

  /* NOTE: that we free up the cache data structures elsewhere */

}


/*---------------------------------------------------------------------*/
/*
 * Search the cache for the given uri/filename.
 * Return 1 if the element was found in the cache, 0 otherwise.
 * Parameters:
 * uri - The key (filename) being searched for
 * index - The integer pointed to by this parameter is set as follows:
 *   If the item is found, *index is the item's index in the table.
 *   If the item is not found and the table has room, *index is the index
 *     of a newly allocated entry in the table.
 *   If the item is not found and the table is full, *index is -1.
 * hash_to - The integer pointed to by this parameter is set to the cache
 *   "bucket" that uri hashes to.
 */

int
cache_lookup(char * uri, int *index, int *hash_to) 
{
  unsigned long int tmp;
  int probe_count = 0;
  int retval = 0;
  int cur;

  if (options.cache_for_spec) {
    tmp = spec_hash(uri);
    *hash_to = tmp;
    PRINT_TIME(NOFD, &tnow, &tprev, "cache_lookup: hash = %ld", tmp);
    if (tmp > (unsigned long int) cache.tableSize) {
      printf("cache_lookup: cache table size = %d too small\n", 
	      cache.tableSize);
      printf("cache_lookup: spec_hash returns entry = %lu\n", tmp);
      exit(1);
    }

    if (cache.indexTable[tmp] == NULL_INDEX) {
      *index = cache_get_element();
      PRINT_TIME(NOFD, &tnow, &tprev, "cache_lookup: not found = %d", *index);
      return 0;
    } else {
      *index = cache.indexTable[tmp];
      PRINT_TIME(NOFD, &tnow, &tprev, "cache_lookup: found = %d", *index);
      PRINT_TIME(NOFD, &tnow, &tprev, "cache_lookup: name = [%s]", 
	  cache.hashTable[*index].name);
      PRINT_TIME(NOFD, &tnow, &tprev, "cache_lookup: uri = [%s]", uri);
#ifdef DEBUG_ON
      if (strcmp(cache.hashTable[*index].name, uri) != 0) {
        printf("cache_mapped_new: name = [%s] should be uri = [%s]\n",
		cache.hashTable[*index].name, uri); 
	printf("cache_mapped_new: Problem likely with using --cache-for-spec "
	       "options for non specweb99 filename\n");
        exit(1);
      }
#endif
      assert((strcmp(cache.hashTable[*index].name, uri) == 0));
      PRINT_TIME(NOFD, &tnow, &tprev, "cache_lookup: found = %d", *index);
      return 1;
    }
  }
	
  tmp = hash(uri);
  tmp = tmp % cache.tableSize;
  *hash_to = tmp;
  PRINT_TIME(NOFD, &tnow, &tprev, "cache_lookup: hash = %d", tmp);

  cur = cache.indexTable[tmp];

  /* Start scanning chained buckets. */
  while (cur != NULL_INDEX) {
    probe_count++;
    if (strcmp(cache.hashTable[cur].name, uri) == 0) {
      break;
    }
    cur = cache.hashTable[cur].next;
  }

  /* Entry is not found in the table. */
  if (cur == NULL_INDEX) {
    *index = cache_get_element();
    PRINT_TIME(NOFD, &tnow, &tprev, "cache_lookup: not found index = %d",
       	*index);
    retval = 0;
  } else {
    *index = cur;
    PRINT_TIME(NOFD, &tnow, &tprev, "cache_lookup: found = %d", *index);
    retval = 1;
  }

  stats.numProbes += probe_count;
  if (probe_count > stats.maxProbes) {
    stats.maxProbes = probe_count;
  }

  return(retval);
}


/*---------------------------------------------------------------------*/
void
cache_print_stats(void)
{
  printf("CACHE PARAMETERS\n");
  printf("cache num buckets             %20d\n", cache.tableSize);
  printf("cache size (bytes)            %20.0f\n", cache.maxBytes);
  printf("cache max file size (bytes)   %20.0f\n", cache.maxFileSize);
  printf("cache max load factor         %20.2f\n", cache.maxLoadFactor);
  
  printf("\n");
  printf("CACHE STATISTICS\n");
  printf("cache bytes used              %20.0f\n", cache.numBytes);
  printf("cache buckets used            %20d\n", cache.numEntries);
	printf("cache current load factor     %20.2f\n",
          (double) cache.numEntries / (double) cache.tableSize);

	printf("\n"); 
  printf("cache requests                %20.0f\n", stats.cacheRequests);
  printf("cache hits                    %20.0f\n", stats.numHits);
  printf("cache file hit rate           %20.2f\n",
      (stats.numHits * 100.0 / stats.cacheRequests));
  printf("cache bytes requested         %20.0f\n", stats.bytesRequested);
  printf("cache bytes hit in cache      %20.0f\n", stats.bytesServed);
  printf("cache byte hit rate           %20.2f\n",
      (stats.bytesServed * 100.0 / stats.bytesRequested));
  printf("cache collisions              %20.0f\n", stats.numCollisions);
  printf("cache collisions per lookup   %20.0f\n", 
      stats.numCollisions / stats.cacheRequests);
  printf("cache replacements            %20.0f\n", stats.numEvictions);
  printf("cache probes                  %20.0f\n", stats.numProbes);
  printf("cache max probes for a lookup %20.0f\n", stats.maxProbes);
  printf("cache probes per lookup       %20.1f\n", 
      stats.numProbes / stats.cacheRequests);

  printf("\n");
  printf("cache max working set size    %20.1f\n", 
      stats.workingSetMax);
  printf("cache cur working set size    %20.1f\n", 
      stats.workingSetCur);
  printf("cache avg working set size    %20.1f\n", 
      stats.workingSetAvg);
  printf("cache total working set size  %20.1f\n", 
      stats.workingSetTotal);
  printf("cache working set count       %20.1f\n", 
      stats.workingSetCount);
}

/*---------------------------------------------------------------------*/
void
cache_clear_stats(void)
{
  stats.cacheRequests = 0.0;
  stats.numHits = 0.0;
  stats.bytesRequested = 0.0;
  stats.bytesServed = 0.0;
  stats.numCollisions = 0.0;
  stats.numEvictions = 0.0;
  stats.numProbes = 0.0;
  stats.maxProbes = 0.0;

  cache_clear_all_used();
}

/*---------------------------------------------------------------------*/
void
cache_clear_all_used(void)
{
  int i;
  int cur;
  cacheinfo *tmp = 0;;

  for (i = 0; i < cache.tableSize; i++) {
    cur = cache.indexTable[i];
    if (cur != NULL_INDEX) {
      while (cur != NULL_INDEX) {
	tmp = &cache.hashTable[cur];
	tmp->use_count = 0;
	tmp->reply_count = 0;
	cur = cache.hashTable[cur].next;
      }
    }
  }
}
/*---------------------------------------------------------------------*/
void
cache_check_ref_counts(void)
{
  int cur;
  int non_zero = 0;
  int zero = 0;

  if (!options.caching_on) {
    return;
  }

  cur = cache.lruFirst;
  while(cur != NULL_INDEX) {
    printf("cur = %d\n", cur);
    if (cache.hashTable[cur].ref_count != 0) {
      non_zero++;
      printBucket(cur, 1);
    } else {
      zero++;
    }
    cur = cache.hashTable[cur].lru_next;
  }

  cur = cache.firstFree;
  while(cur != NULL_INDEX) {
    if (cache.hashTable[cur].ref_count != 0) {
      non_zero++;
      printBucket(cur, 0);
    } else {
      zero++;
    }
    cur = cache.hashTable[cur].next;
  }

  printf("----------------------------------------------------------------------\n");
  printf("cache_check_ref_counts\n");
  printf("number of ref counts == 0 is %d\n", zero);
  printf("number of ref counts != 0 is %d\n", non_zero);
  printf("non_zero + zero = %d\n", zero + non_zero);
  printf("cache numEntries = %d\n", cache.numEntries);

}

/*---------------------------------------------------------------------*/
void
printCache(void)
{
  int i;
  int cur;
  cacheinfo *tmp = 0;
  int first;
  int count = 0;

  printBucketHdr(INCLUDE_INDEX);

  for (i = 0; i < cache.tableSize; i++) {
    cur = cache.indexTable[i];
    if (cur == NULL_INDEX) {
      printf("%7d:%7d %40s\n", i, cur, "Empty");
    } else {
      first = cur;
      while (cur != NULL_INDEX) {
	count++;
	tmp = &cache.hashTable[cur];
	if (first == cur) {
          printf("%7d:", i);
	} else {
          printf("%7s ", "");
	}
	printBucket(cur, 0);
	cur = cache.hashTable[cur].next;
      }
    }
  }

  printf("Number of bytes used: %.0f\n", cache.numBytes);
  printf("Number of buckets used: %d\n", cache.numEntries);
  printf("Used Entries = %d\n", count);

  printLruList();
  printFreeList();
}

/*---------------------------------------------------------------------*/
void
printBucket(int index, int which)
{
  int next = NULL_INDEX;
  int prev = NULL_INDEX;

  cacheinfo *tmp = &cache.hashTable[index];
  if (which == 0) {
    next = tmp->next;
    prev = tmp->prev;
  } else {
    next = tmp->lru_next;
    prev = tmp->lru_prev;
  }
  printf("%7d %40s %3d %6d %10d %6.3lf%% %10d %6.3lf%% %10d %6d %6d %6d\n", 
	  index, tmp->name, tmp->reference, tmp->ref_count, tmp->use_count, 
	  ((double) tmp->use_count / (double) stats.cacheRequests * 100.0),
	  tmp->reply_count, (double) tmp->reply_count / (double) tmp->use_count * 100.0,
	  tmp->buf_len, next, prev, tmp->hash_to);
}

/*---------------------------------------------------------------------*/
void
printBucketHdr(int include_index)
{
  if (include_index) {
    printf("%7s:", "Index");
  }
  printf("%7s %40s %3s %6s %10s %7s %10s %7s %10s %6s %6s %6s\n",
         "Bucket", "Filename", "Ref", "RefCnt", "Used",
	 "Used%", "Replies", "Reps%", "BufLen", "next", "prev", "hashto");
}

/*---------------------------------------------------------------------*/
void
printLruList(void)
{
  int cur;
  printf("\n");
  printf("LRU List\n");
  printBucketHdr(DONT_INCLUDE_INDEX);
  cur = cache.lruFirst;
  while(cur != NULL_INDEX) {
    printBucket(cur, 1);
    cur = cache.hashTable[cur].lru_next;
  }
}

/*---------------------------------------------------------------------*/
void
printFreeList(void)
{
  int count = 0;
  int cur;
  printf("\n");
  printf("Free List\n");
  printBucketHdr(DONT_INCLUDE_INDEX);
  cur = cache.firstFree;
  while(cur != NULL_INDEX) {
    printBucket(cur, 0);
    cur = cache.hashTable[cur].next;
    count++;
  }
  printf("Free List Entries = %d\n", count);
}



/*---------------------------------------------------------------------*/
/*
 * Enforce that uris must have non-zero length?
 * The name of this function is a bit misleading. Here's what it does.
 * Check if the file associated with the given uri is in the cache. 
 * If it is, return the cacheinfo structure.
 * Otherwise (it's not in the cache), mmap the file, cache it,
 * and return the cacheinfo structure.
 * To avoid copying, this routine returns a pointer directly into the array. 
 * Saves copying, potentially dangerous.
 */

cacheinfo * 
cache_add(char * uri, int http_version) 
{
  int index, found;
  int tmp;
  int fd;
  int rc = 0;
  int trace_fd = 0;
  int initial_spot = 0;
  struct stat fileStat;
  double dbl_filesize = 0.0;
  int victim = -1;
  int hash_to;
  char *type = 0;
  cacheinfo *cur = 0;
#ifdef HAVE_ENCRYPTION
  int x, ln, encrytbuflen, pagesz;
#define FILLBUFSZ 2048
  char fillbuf[FILLBUFSZ];
#endif /* HAVE_ENCRYPTION */

  /* Check that the max load factor has not been exceeded */
  if (((double) cache.numEntries / (double) cache.tableSize) > 
			          cache.maxLoadFactor) {
    printf("cache_add: maximum load factor exceeded! Exiting...\n");
    printCache();
    cache_print_stats();
    exit(-1);
  }

  stats.cacheRequests++;  
  found = cache_lookup(uri, &index, &hash_to);
  PRINT_TIME(NOFD, &tnow, &tprev, "cache_add: cache_lookup found = %d "
			"index = %d", found, index);
  initial_spot = index;
  
  /* TBB - cache can now become 100% full. */
  /* assert(index != -1); */
  if (found == 0) {
   
    /* Not found in the cache, so open the file for reading */
    TRACE(EVT_OPEN_FILE,
       trace_fd = 0;
       fd = open(uri, O_RDONLY);
       rc = fd;
    );

    if (fd < 0) {
      PRINT_TIME(NOFD, &tnow, &tprev, 
	         "cache_add: open failed - returning fail");
      /* TBB: should this really be here */
      cache_free_element(index);
      return 0;
    }

    /* stat the file to get its size */    
    TRACE(EVT_FSTAT,
       trace_fd = fd;
       rc = fstat(fd, &fileStat);
    );

    if  (rc < 0) {      
      PRINT_TIME(NOFD, &tnow, &tprev, "cache_add: file stat failed");
      /* TBB: should this really be here */
      cache_free_element(index);
      close_file(fd);
      return 0;
    }

    /* Check that the request is for a file and not for a directory */
    if (S_ISDIR(fileStat.st_mode)) {
      PRINT_TIME(NOFD, &tnow, &tprev, "cache_add: request is for directory");
      /* TBB: should this really be here */
      cache_free_element(index);
      close_file(fd);
      return 0;
    }

    stats.bytesRequested += fileStat.st_size;
    dbl_filesize = (double) fileStat.st_size;
    
    /* Make sure the file isn't too big to be cached */
    if (dbl_filesize > cache.maxFileSize ||
	dbl_filesize > cache.maxBytes) { 
      PRINT_TIME(NOFD, &tnow, &tprev, "File too big to cache!");
      PRINT_TIME(NOFD, &tnow, &tprev, "filesize = %d maxFileSize = %.0f maxBytes = %.0f\n",
	  fileStat.st_size, cache.maxFileSize, cache.maxBytes);
      /* TBB: should this really be here */
      cache_free_element(index);
      close_file(fd);
      return 0; 
    }

    /*
     * We may need to evict for two reasons
     * 1) the maximum number of bytes would be exceeded if
     *    we added the new file
     * 2) the cache table is 100% full
     *
     * We do the cache bytes full check first as this will
     * eject at least one entry.
     */
		
    if ((cache.numBytes + dbl_filesize) >= cache.maxBytes) {
      /* Need to evict some stuff to free up space */
      while ((cache.maxBytes - cache.numBytes) < dbl_filesize) {
         victim = cache_victim();
         PRINT_TIME(NOFD, &tnow, &tprev, "cache_add: total byte victim = %d",
		    victim);
	 /* Note order is important here */
	 cache_lru_delete(victim);
	 cache_delete_element(victim);
	 cache_free_element(victim);
      } /* outer while        */
			
      /*
       * NOT needed any more because of chaining.
       * Find out where the file should be cached, now that there's space
       * found = cache_lookup(uri, &index, &hash_to);
       * assert(found == 0);
       * assert(index != -1);
       */
    }

    /* Can't happen anymore */
    /* We still don't have a spot in the cache for the new entry */
    /* because the table is 99% full. */
    if ((found == 0) && (index == -1)) {
      victim = cache_victim();
      PRINT_TIME(NOFD, &tnow, &tprev, "cache_add: full table victim = %d", 
		victim);
      found = 1;
      index = victim;

      /* Note order is important here */
      cache_lru_delete(victim);
      cache_delete_element(victim);
      cache_free_element(victim);
      tmp = cache_get_element();
      ldbg("cache_add: index = %d tmp = %d\n", index, tmp);
    }
    
    /* Add the file to the cache */
    if (strlen(uri) >= CACHE_NAME_MAX) {
      printf("cache_mapped: ERROR - CACHE_NAME_MAX = %d not large enough "
	     "for URL name len = %d\n", CACHE_NAME_MAX, (int) strlen(uri));
      exit(1);
    }

    cur = &cache.hashTable[index];
    strncpy(cur->name, uri, CACHE_NAME_MAX);

    /* Add this entry into the appropriate spot in the chain */
    /* (in this case at the front - since LRU is good) */

    ldbg("next = %d prev = %d lru_next = %d lru_prev = %d\n", 
          cur->next, cur->prev, cur->lru_next, cur->lru_prev);

    assert(cache.hashTable[index].next == NULL_INDEX);
    assert(cache.hashTable[index].prev == NULL_INDEX);
    assert(cache.hashTable[index].lru_next == NULL_INDEX);
    assert(cache.hashTable[index].lru_prev == NULL_INDEX);
    cache_add_element(index, hash_to);
    cache_lru_add(index);

    cur->use_count = 1;

   if (!options.use_sendfile || options.use_madvise || options.victim_skip == OPT_SKIP_DO_MADVISE) {
      if (fileStat.st_size > 0) {
	TRACE(EVT_MMAP,
	  trace_fd = fd;
	  cur->buf = (char*) mmap(0, fileStat.st_size, PROT_READ, 
				  MAP_SHARED, fd, 0);
	  rc = (cur->buf != MAP_FAILED);
	);
	assert(cur->buf != MAP_FAILED);
	PRINT_TIME(NOFD, &tnow, &tprev, "cache_add: mmap returns %p", cur->buf);
      }
   }

    if (!options.use_sendfile) {
      if (options.use_socket_aio) {
#ifdef HAVE_AIO_LAYER
        PRINT_TIME(NOFD, &tnow, &tprev, "cache_add: aio_sock_mem_reg start %p size = %d ",
	    cur->buf, fileStat.st_size);

        if ((options.encrypt_data) &&
            (options.encrypt_percent >= 100) &&
            ((options.encrypt_max_file_size == 0UL) ||
             ((unsigned long)fileStat.st_size <= options.encrypt_max_file_size))) {
          cur->file_reg_handle = NULL;
          rc = 0;
	} else {
	  TRACE(EVT_AIO_SOCK_MEM_REG,
		trace_fd = 0;
		rc = aio_sock_mem_reg(cur->buf, fileStat.st_size,
		  &(cur->file_reg_handle));
	  );
	}
        stats_reg_bytes(fileStat.st_size);

	if (rc < 0) {
	  printf("cache_add: aio_sock_mem_reg failed (mmapped) "
	      "addr = %p size = %lu reg_handle = %p\n", 
	      cur->buf, fileStat.st_size, cur->file_reg_handle);
	  exit(1);
	} 
        PRINT_TIME(NOFD, &tnow, &tprev, "cache_add: handle = %p ", 
           cur->file_reg_handle);
#else
      printf("cache_add: aio not handled\n");
      exit(1);
#endif /* HAVE_AIO_LAYER */
      }
    }

#ifdef HAVE_ENCRYPTION
    /* If encryption is enabled then we create a buffer to encryt the data
     * into. It is this buffer that is written, not the file buffer. So
     *   - if not aio and not using sendfile then we simply allocate an
     *     encryption buffer;
     *   - if not aio and using sendfile then we need to create and mmap
     *     a temp file to use as the encryption buffer, and it's this temp
     *     buffer that data is sent from.
     * We need to make certain the encryption buffer is large enough to do
     * the encryption, i.e. make certain there is a mutiple of 8 bytes
     * available. Also, we only do this if the file size is <= the max
     * encryption file size.
     */
    if ((options.encrypt_data) &&
      ((options.encrypt_max_file_size == 0UL) ||
       ((unsigned long)fileStat.st_size <= options.encrypt_max_file_size))) { 
      encrytbuflen = fileStat.st_size+8;
      if ((options.use_socket_aio) || (!options.use_sendfile)) {
#ifndef HAVE_AIO_LAYER
        if (options.use_socket_aio) {
          printf("cache_add: aio not handled\n");
          exit(1);
        }
#endif /* HAVE_AIO_LAYER */
        pagesz = getpagesize();
        cur->encryptmalloc = malloc(encrytbuflen+pagesz);
        assert(cur->encryptmalloc != NULL);
        cur->encryptbuf = cur->encryptmalloc + pagesz - 1;
        cur->encryptbuf = (char *) ((unsigned long) cur->encryptbuf & 
	                          ~((unsigned long)(pagesz - 1)));
        memset(cur->encryptbuf, 0, encrytbuflen);
        cur->encryptfd = -1;
#ifdef HAVE_AIO_LAYER
        if (options.use_socket_aio) {
          rc = aio_sock_mem_reg(cur->encryptbuf, encrytbuflen,
            &(cur->encrypt_buf_handle));
          if (rc < 0) {
            printf("cache_add: aio_sock_mem_reg failed (encryption buffer)\n");
            exit(1);
          }
        }
#endif /* HAVE_AIO_LAYER */
      } else {
        cur->buf = (char*) mmap(0, fileStat.st_size, PROT_READ,
                                MAP_SHARED, fd, 0);
        assert(cur->buf != MAP_FAILED);
        strcpy(cur->encryptfname, "/tmp/usrvrcache.XXXXXX");
        cur->encryptfd = mkstemp(cur->encryptfname);
        assert(cur->encryptfd >= 0);
        memset(fillbuf, 0, FILLBUFSZ);
        for (x=0; x<encrytbuflen; x+=FILLBUFSZ) {
          ln = FILLBUFSZ;
          if (ln > encrytbuflen-x)
            ln = encrytbuflen-x;
          if (write(cur->encryptfd, fillbuf, ln) != ln) {
            printf("cache_add: error writing to encryption file: %s\n",
              strerror(errno));
            exit(1);
          }
        }
        cur->encryptbuf = mmap(0, encrytbuflen, PROT_READ|PROT_WRITE,
                               MAP_SHARED, cur->encryptfd, 0);
        assert(cur->encryptbuf != MAP_FAILED);
      }
    }
#endif /* HAVE_ENCRYPTION */

    cur->buf_len = fileStat.st_size;
    cur->last_modified = fileStat.st_mtime;
    cur->last_stat = time(NULL);
    cur->fd = fd;
    cur->reference = 1;
    assert(cur->ref_count == 0);
    cache_inc_ref_count(cur);

#ifndef __CYGWIN__
    /* tell the kernel that this will be accessed sequentially
     * and it will be accessed soon.
     */
    if (options.use_madvise) {
      rc = madvise(cur->buf, cur->buf_len, MADV_SEQUENTIAL | MADV_WILLNEED);
      if (rc < 0) {
	printf("cache_add: madvise failed with rc = %d errno = %d\n", 
	    rc, errno);
	perror("madvise: ");
	exit(1);
      }
    }
#endif /* __CYGWIN__ */

    if (!options.use_sendfile && cache.lockPages != 0) {
      TRACE(EVT_MLOCK,
	trace_fd = fd;
	rc = mlock(cur->buf, cur->buf_len);
      );
      assert(rc != -1);
    }

    /* Update the cache's book-keeping stuff. */
    cache.numEntries++;
    cache.numBytes += fileStat.st_size;


    if (index != initial_spot) { 
      stats.numCollisions++; 
    }

    if (options.content_type) {
      type = http_content_type(uri);
      PRINT_TIME(NOFD, &tnow, &tprev, "cache_add: st_size = %d",
	  fileStat.st_size);
      rc = snprintf(cur->header, CACHED_HDR_SIZE, HTTP_OK_STR_ID_LEN_TYPE,
	  http_version, server_ident, 
	  (unsigned long int) fileStat.st_size, type);
      assert(rc > 0);

    } else {
      PRINT_TIME(NOFD, &tnow, &tprev, "cache_add: st_size = %d",
	  fileStat.st_size);
      rc = snprintf(cur->header, CACHED_HDR_SIZE, HTTP_OK_STR_ID_LEN,
	  http_version, server_ident, (unsigned long int) fileStat.st_size);
      assert(rc > 0);
    }
    PRINT_TIME(NOFD, &tnow, &tprev, "cache_add: reply header len = %d", rc);
    PRINT_TIME(NOFD, &tnow, &tprev, "cache_add: reply header = [%s]", 
	       cur->header);

    if (rc >= CACHED_HDR_SIZE) {
      printf("cache_mapped: ERROR - CACHED_HDR_SIZE = %d\n", CACHED_HDR_SIZE);
      printf("   not large enough for generated output header = %d\n", rc);
      exit(1);
    }

    /* New rc above now returns number of bytes written. */
    /* cur->header_len = strlen(cur->header); */
    cur->header_len = rc;


    if (!options.use_sendfile && !options.cache_warm) {
      PRINT_TIME(NOFD, &tnow, &tprev, "cache_add: closing file "
	  "because not using sendfile");
      close_file(fd);
      /* The fd is closed so we shouldn't use it anymore */
      cur->fd = -1;
    }

    return (cur);
    
  } else {
    /* Found it in the cache */
    cache_lru_access(index);
    cur = &cache.hashTable[index];
    stats.numHits++;
    stats.bytesRequested += cur->buf_len;
    stats.bytesServed += cur->buf_len;
    cur->reference = 1;
    cur->use_count++;
    cache_inc_ref_count(cur);
    PRINT_TIME(NOFD, &tnow, &tprev, 
	       "cache_add: entry found header_len = %d buf_len = %d fd = %d", 
		cur->header_len, cur->buf_len, cur->fd);
    PRINT_TIME(NOFD, &tnow, &tprev, "cache_add: returning");
    return (cur);
  }
  return 0;
}

/*---------------------------------------------------------------------*/
int
cache_victim(void)
{
#define USE_LRU
#ifdef USE_LRU
  int victim = NULL_INDEX;
  int tmp = NULL_INDEX;
  cacheinfo *cur = 0;
  int trace_fd = 0;
  int rc = 0;

  PRINT_TIME(NOFD, &tnow, &tprev, "cache_victim: entered");

  /* look for a file that isn't being used */
  tmp = cache.lruLast;
  assert(tmp != NULL_INDEX);
  while (tmp != NULL_INDEX) {
    cur = &cache.hashTable[tmp];
    PRINT_TIME(NOFD, &tnow, &tprev, "cache_victim: tmp = %d fd = %d "
	"ref_count = %d", tmp, cur->fd, cur->ref_count);
    if (cur->ref_count == 0) {
      victim = tmp;
      break;
    } else {
      tmp = cur->lru_prev;
    }
  }

  if (victim == NULL_INDEX) {
    printf("cache_victim: Can't find victim in cache\n");
    printf("cache_victim: All cache entries are currently being used\n");
    printf("cache_victim: Increase the number of cache table entries or the total cache size and retry\n");
    assert(victim != NULL_INDEX);
    exit(1);
  }

  assert(cur->ref_count == 0);

  if (!options.use_sendfile) {
    if (cache.lockPages != 0) {
      /* Evict this page */
      TRACE(EVT_MUNLOCK,
	trace_fd = 0;
	rc = munlock(cur->buf, cur->buf_len);
      );
    }

    TRACE(EVT_MUNMAP,
      trace_fd = 0;
      rc = munmap(cur->buf, cur->buf_len);
    );

    if (options.use_socket_aio) {
#ifdef HAVE_AIO_LAYER
      PRINT_TIME(NOFD, &tnow, &tprev, "cacheVictim: aio_sock_mem_dereg start %p size = %d "
	    "handle = %p", cur->buf, cur->buf_len, cur->file_reg_handle);

      rc = 0;
      if (cur->file_reg_handle != NULL) {
        TRACE(EVT_AIO_SOCK_MEM_DEREG,
          trace_fd = 0;
       	  rc = aio_sock_mem_dereg(cur->buf, cur->buf_len, 
                                  cur->file_reg_handle);
         );
      }

      if (rc < 0) {
	printf("cache_victim: Location 1 aio_sock_mem_dereg failed (munmap) "
	    "addr = %p size = %d\n", cur->buf, cur->buf_len);
	exit(1);
      } else {
        stats_dereg_bytes(cur->buf_len);
      }
#else
      printf("cache_victim: aio not handled\n");
      exit(1);
#endif /* HAVE_AIO_LAYER */
    }

  }

#ifdef HAVE_ENCRYPTION
  /* Clean up encryption buffer/file */
  if ((options.encrypt_data) &&
      ((options.encrypt_max_file_size == 0UL) ||
       ((unsigned long)cur->buf_len <= options.encrypt_max_file_size))) {
    if (!options.use_sendfile) {
#ifdef HAVE_AIO_LAYER
      if (options.use_socket_aio) {
        aio_sock_mem_dereg(cur->encryptbuf, cur->buf_len+8, cur->encrypt_buf_handle);
      }
#endif /* HAVE_AIO_LAYER */
      free(cur->encryptmalloc);
    } else {
      munmap(cur->buf, cur->buf_len);
      munmap(cur->encryptbuf, cur->buf_len+8);
      close(cur->encryptfd);
      unlink(cur->encryptfname);
    }
  }
#endif /* HAVE_ENCRYPTION */

#ifndef __CYGWIN__
  /* since this is deemed to be expendable tell the kernel */
  if (options.use_madvise) {
    rc = madvise(cur->buf, cur->buf_len, MADV_DONTNEED);
  }
#endif /* __CYGWIN__ */

  cur->buf = 0;
  cache.numBytes = cache.numBytes - cur->buf_len;

  cur->buf_len = -1;
  cur->name[0] = '\0';
  cur->use_count = 0;
  cur->reply_count = 0;
  /* cache_clear_ref_count(cur); */

  if (options.use_sendfile) {
    close_file(cur->fd);
    cur->fd = -1;
  }

  cache.numEntries--;
  stats.numEvictions++;

  return(victim);
#else
  int victim = -1;
  cacheinfo *cur = 0;
  int trace_fd = 0;
  int rc = 0;

  while (1) {
    cur = &cache.hashTable[cache.clockHand];
    if (cur->buf_len > 0) {
      if (cur->reference == 0) {

	if (!options.sendfile) {
	  if (cache.lockPages != 0) {
	    /* Evict this page */
	    TRACE(EVT_MUNLOCK,
	      trace_fd = 0;
	      rc = munlock(cur->buf, cur->buf_len);
	    );
	  }

	  TRACE(EVT_MUNMAP,
	    trace_fd = 0;
	    rc = munmap(cur->buf, cur->buf_len);
	  );

	  if (options.use_socket_aio) {
#ifdef HAVE_AIO_LAYER
            rc = 0;
            if (cur->file_reg_handle != NULL) {
              PRINT_TIME(NOFD, &tnow, &tprev, "cacheVictim: "
	        "aio_sock_mem_dereg start %p "
	        "size = %d handle = %p",
	        cur->buf, cur->buf_len, cur->file_reg_handle);
	      TRACE(EVT_AIO_SOCK_MEM_DEREG,
	        trace_fd = 0;
	        rc = aio_sock_mem_dereg(cur->buf, cur->buf_len,
		  cur->file_reg_handle);
	      );
            }

	    if (rc < 0) {
	      printf("cache_victim: Location 2 aio_sock_mem_dereg "
		  "failed (munmap) "
		  "addr = %p size = %d\n", cur->buf, cur->buf_len);
	      exit(1);
	    } else {
              stats_dereg_bytes(cur->buf_len);
            }
#else
	    printf("cache_victim: aio not handled\n");
	    exit(1);
#endif /* HAVE_AIO_LAYER */
	  }
	}

        cur->buf = 0;
        cache.numBytes = cache.numBytes - cur->buf_len;
        cur->buf_len = -1;
        cur->name[0] = '\0';
        cur->use_count = 0;
        cur->reply_count = 0;
        cache.numEntries--;
        stats.numEvictions++;
				victim = cache.clockHand;
	if (options.use_sendfile) {
	  close_file(cur->fd);
	  cur->fd = -1;
	}
        break;
      } else if (cur->reference == 1) {
        /* Decrement the reference counter */
        cur->reference = 0;
      } /* if-else */
    } /* if */
    cache.clockHand++;

    if (cache.clockHand == cache.tableSize) { 
      cache.clockHand = 0; 
    }
  } /* inner while */

	assert(victim != -1);
	return victim;
#endif
} /* cache_victim */

/*---------------------------------------------------------------------*/
void
cache_free_element(int index) 
{
  int old_first = cache.firstFree; 
  assert((index >= 0) && (index < cache.totalSize));
  cache.firstFree = index; 
  cache.hashTable[index].next = old_first;
  cache.hashTable[index].prev = NULL_INDEX;
  ldbg("cache_free_element: firstFree = %d next = %d\n",
	cache.firstFree, cache.hashTable[cache.firstFree].next);
}

/*---------------------------------------------------------------------*/
void
cache_add_element(int index, int hash_to) 
{
  cache.hashTable[index].hash_to = hash_to;
  if (cache.indexTable[hash_to] == NULL_INDEX) {
    /* first entry in this position */
    cache.hashTable[index].next = NULL_INDEX;
    cache.hashTable[index].prev = NULL_INDEX;
    cache.indexTable[hash_to] = index;
  } else {
    /* insert at the front of the list */
    cache.hashTable[index].prev = NULL_INDEX;
    cache.hashTable[index].next = cache.indexTable[hash_to];
    cache.hashTable[cache.indexTable[hash_to]].prev = index;
    cache.indexTable[hash_to] = index;
  }
}


/*---------------------------------------------------------------------*/
void
cache_delete_element(int index) 
{
  cacheinfo *cur = 0;
  int tbl_index;
  int tmp;

  assert((index >= 0) && (index < cache.totalSize));
  cur = &cache.hashTable[index];
  tbl_index = cur->hash_to;
  assert(tbl_index >= 0 && tbl_index < cache.tableSize);

  ldbg("cache_delete_element: deleting entry = %d\n", index);


  /* only one node in the q */
  if ((cur->next == NULL_INDEX) && (cur->prev == NULL_INDEX)) {
    ldbg("cache_delete_element: deleting with only entry = %d\n", index);
    cache.indexTable[tbl_index] = NULL_INDEX;
    assert(cur->next == NULL_INDEX);
    if (cur->prev != NULL_INDEX) {
      printf("cache_delete_element: index = %d prev = %d\n", index, cur->prev);
      printf("next/prev\n");
      printBucket(index, 0);
      printf("lru_next/lru_prev\n");
      printBucket(index, 1);
    }
    assert(cur->prev == NULL_INDEX);
  } else if (cache.indexTable[tbl_index] == index) {
    /* more than one node and it's the front being deleted */
    ldbg("cache_delete_element: deleting from front\n");
    assert(cur->prev == NULL_INDEX)
    assert(cur->next != NULL_INDEX)
    assert(cache.hashTable[cur->next].prev == index);
    cache.indexTable[tbl_index] = cur->next;
    cache.hashTable[cur->next].prev = NULL_INDEX;
  } else if (cur->next == NULL_INDEX) {
    /* more than one node and it's the rear being deleted */
    assert(cur->prev != NULL_INDEX)
    ldbg("cache_delete_element: deleting from rear\n");
    cache.hashTable[cur->prev].next = NULL_INDEX;
  } else {
    /* handle the case where it's in the middle 
    * at this point there must be at least 3 nodes because
    * if there are only two nodes then the one being deleted 
    * must be either the q_front or the q_rear 
    */
    ldbg("cache_delete_element: deleting from middle\n");
    ldbg("cache_delete_element: prev = %d  p->next = %d\n", 
          cur->prev, cur->next);
    
    tmp = cur->prev;
    cache.hashTable[tmp].next = cur->next;
    cache.hashTable[cur->next].prev = tmp;
  }

  cur->next = NULL_INDEX;
  cur->prev = NULL_INDEX;
}


/*---------------------------------------------------------------------*/
/* Add to the front of the list.
 * Victims come out of the end of the list.
 */

void
cache_lru_add(int index) 
{
  ldbg("cache_lru_add: index = %d\n", index);

  if (cache.lruFirst == NULL_INDEX) {
    assert(cache.lruLast == NULL_INDEX);
    cache.lruFirst = index;
    cache.lruLast = index;
    cache.hashTable[index].lru_next = NULL_INDEX;
    cache.hashTable[index].lru_prev = NULL_INDEX;
  } else {
    /* More than one element so last doesn't change */
    cache.hashTable[index].lru_prev = NULL_INDEX;
    cache.hashTable[index].lru_next = cache.lruFirst;
    assert(cache.hashTable[cache.lruFirst].lru_prev == NULL_INDEX);
    cache.hashTable[cache.lruFirst].lru_prev = index;
    cache.lruFirst = index;
  }
}

/*---------------------------------------------------------------------*/
void
cache_lru_delete(int index) 
{
  cacheinfo *cur = 0;
  int tmp;

  assert((index >= 0) && (index < cache.totalSize));
  cur = &cache.hashTable[index];

  PRINT_TIME(NOFD, &tnow, &tprev, "cache_lru_delete: deleting entry = %d\n", 
      index);
  ldbg("cache_lru_delete: deleting entry = %d\n", index);


  /* only one node in the q */
  if (cache.lruFirst == index && cache.lruLast == index) {
    ldbg("cache_lru_delete: deleting with only entry = %d\n", index);
    cache.lruFirst = NULL_INDEX;
    cache.lruLast = NULL_INDEX;
    assert((cur->lru_next == NULL_INDEX) && (cur->lru_prev == NULL_INDEX));
  } else if (cache.lruFirst == index) {
    /* more than one node and it's the front being deleted */
    ldbg("cache_lru_delete: deleting from front\n");
    assert(cur->lru_prev == NULL_INDEX);
    assert(cur->lru_next != NULL_INDEX)
    assert(cache.hashTable[cur->lru_next].lru_prev == index);
    cache.lruFirst = cur->lru_next;
    cache.hashTable[cur->lru_next].lru_prev = NULL_INDEX;
  } else if (cur->lru_next == NULL_INDEX) {
    /* more than one node and it's the rear being deleted */
    ldbg("cache_lru_delete: deleting from rear\n");
    assert(cache.lruLast == index);
    assert(cur->lru_prev != NULL_INDEX);
    cache.hashTable[cur->lru_prev].lru_next = NULL_INDEX;
    cache.lruLast = cur->lru_prev;
  } else {
    /* handle the case where it's in the middle 
    * at this point there must be at least 3 nodes because
    * if there are only two nodes then the one being deleted 
    * must be either the q_front or the q_rear 
    */
    ldbg("cache_lru_delete: deleting from middle\n");
    ldbg("cache_lru_delete: lru_prev = %d lru_next = %d\n", 
          cur->lru_prev, cur->lru_next);
    
    tmp = cur->lru_prev;
    cache.hashTable[tmp].lru_next = cur->lru_next;
    cache.hashTable[cur->lru_next].lru_prev = tmp;
  }

  cur->lru_next = NULL_INDEX;
  cur->lru_prev = NULL_INDEX;
  /* Just moving it */
  /* cache_clear_ref_count(cur); */
}
/*---------------------------------------------------------------------*/

void
cache_lru_access(int index) 
{
  assert((index >= 0) && (index < cache.totalSize));
  cache_lru_delete(index);
  cache_lru_add(index);
  ldbg("cache_lru_access: accessing = %d\n", index);
}

/*---------------------------------------------------------------------*/
int
cache_get_element(void)
{
  int ret = NULL_INDEX;

  if (cache.firstFree == NULL_INDEX) {
    ret = NULL_INDEX;
    ldbg("cache_get_element: can't find free node\n");
  } else {
    ret = cache.firstFree;
    cache.firstFree = cache.hashTable[ret].next;
    ldbg("cache_get_element: found free node at index = %d\n", ret);
    cache.hashTable[ret].next = NULL_INDEX;
    cache.hashTable[ret].prev = NULL_INDEX;
  }

  ldbg("cache_get_element: returning index = %d\n", ret);
  return(ret);
}

/*---------------------------------------------------------------------*/
void
cache_dec_ref_count(struct cacheinfo *c)
{
  assert(c);
  c->ref_count--;
  working_set_remove(c->buf_len);
  PRINT_TIME(NOFD, &tnow, &tprev, "cache_dec_ref_count: c = %p ref_count = %d",
      c, c->ref_count);
  assert(c->ref_count >= 0)
}

/*---------------------------------------------------------------------*/
void
cache_inc_ref_count(struct cacheinfo *c)
{
  assert(c);
  assert(c->ref_count >= 0)
  c->ref_count++;
  working_set_add(c->buf_len);
  PRINT_TIME(NOFD, &tnow, &tprev, "cache_inc_ref_count: c = %p ref_count = %d",
      c, c->ref_count);
}

void
cache_clear_ref_count(struct cacheinfo *c)
{
  assert(c);
  /*
  PRINT_TIME(NOFD, &tnow, &tprev, "cache_clear_ref_count: "
      "c = %p ref_count = %d", c, c->ref_count);
  */
  c->ref_count = 0;
}

/*---------------------------------------------------------------------*/
/* Causes the file to be cached, or if a directory name is passed 
 * recursively call this routine on each entry in the directory.
 */
static void
cache_it(const char *fname, const char *base)
{
#ifdef OLDWAY
  /* volatile so loop it's used in doesn't get optimized to nothing */
  static volatile char *b_temp;  
#else
  volatile char b_temp[BUF_SIZE];
#endif

  // int stride = getpagesize();
  int stride = options.cache_warm_stride;
  struct stat statinfo;
  DIR *dir;
  struct dirent *de;
  cacheinfo *info;
  char *p, uri[PATH_MAX];
  int i;
  int rc = 0;
  size_t x;
  size_t read_size;
  size_t bytes_left;

  if (stride == 0) {
    stride = getpagesize();
  } 

  if (options.cache_warm_size == 0) {
    options.cache_warm_size = getpagesize();
  }
  // printf("Using warm cache size = %d\n", options.cache_warm_size);


#if 0
  if (! (stride >= options.cache_warm_size)) {
    printf("cache_it: stride must be greater than or equal to the --cache-warm-size\n");
    exit(1);
  }
#endif

  if (options.cache_warm_size > BUF_SIZE) {
    printf("cache_it: --cache-warm-size = %d > BUF_SIZE = %d\n",
            options.cache_warm_size, BUF_SIZE);
    exit(1);
  }

  ldbg("cache_it: fname = %s\n", fname);

#ifdef OLDWAY
  b_temp = malloc(stride);
  if (b_temp == 0) {
    printf("cache_it: failed to malloc b_temp\n");
    assert(b_temp != 0);
  }
#endif
 
  if ((strcmp(fname, ".") == 0) || (strcmp(fname, "..") == 0)) {
    return;
  }

  /* Fully qualify the path */
  if (base == NULL) {
    strcpy(uri, fname);
  } else {
    strcpy(uri, base);
    x = strlen(uri);
    if ((uri[x-1] == '/') && (fname[0] == '/')) {
      uri[x-1] = '\0';
      strcat(uri, fname);
    } else if ((uri[x-1] != '/') && (fname[0] != '/')) {
      sprintf(uri, "%s/%s", base, fname);
    } else {
      strcat(uri, fname);
    }
  }

  ldbg("cache_it: fname = %s\n", fname);

  /* Directory or file? */
  if (stat(uri, &statinfo) != 0) {
    printf("cache_it: cannot stat %s, probably does not exist: ignoring\n", uri);
    return;
  }

  if (S_ISDIR(statinfo.st_mode)) {
   
    /* Directory: go through each file and directory in this directory */ 
    dir = opendir(uri);
    if (dir == NULL)
      return;
    while ((de = readdir(dir)) != NULL) {
      cache_it(de->d_name, uri);
    }
    closedir(dir);
      
  } else {

    /* File: cache it */
    info = cache_add(uri, 1);
    if (info == 0)
      return;

    assert(info->fd > 0);
   
    for (i=0; i<options.cache_warm_loops; i++) { 

      /* Touch each page to make certain it's in memory */ 
      // bytes_left = info->buf_len;
      p = info->buf;

      for (x=0; x<info->buf_len; x+=stride) {

        bytes_left = info->buf_len - x;

	if (options.cache_warm_size > bytes_left) {
	  read_size = bytes_left;
	} else {
	  read_size = options.cache_warm_size;
	}


	ldbg("cache_it: read_size = %10zu cur byte = %10zu bytes_left = %10zu\n", read_size, x, bytes_left);

#ifdef OLDWAY
	if (options.use_sendfile) {
	  rc = lseek(info->fd, x, SEEK_SET);
	  assert(rc != -1);
	  rc = read(info->fd, (void *) b_temp, read_size);
	  if (rc != read_size) {
	     printf("cache_it: rc = %d errno = %d\n", rc, errno);
	  }
	  assert(rc == read_size);
	} else {
	  memcpy((void *) b_temp, &p[x], read_size);
	}
#else
	  rc = lseek(info->fd, x, SEEK_SET);
	  ldbg("cache_it: lseek fd = %d x = %d rc = %d errno = %d\n", info->fd, x, rc, errno);
	  assert(rc != -1);
	  rc = read(info->fd, (void *) b_temp, read_size);
	  if (rc != read_size) {
	     printf("cache_it: rc = %d errno = %d\n", rc, errno);
	  }
	  assert(rc == read_size);

#endif

	// bytes_left -= read_size;
      } /* for */
    } /* for */
    if (!options.use_sendfile) {
       close(info->fd);
       /*( Now that the fd is closed it is no longer valid */
       info->fd = -1;
    }


  } /* else */
}     

/*---------------------------------------------------------------------*/
/* Called to warm the cache if the appropriate option has been passed to
* userver.  Entries to be cached are read from the given file.
*/
void
cache_warm(const char *files, const char *docroot, int printcache)
{
FILE *fh;
int ln;
char *str, buf[PATH_MAX], root[PATH_MAX];

/* Is there a user-specified document root? */
if (docroot != NULL) {
strcpy(root, docroot);
ln = strlen(root);
if (root[ln-1] != '/') {
strcat(root, "/");
}
}

ldbg("cache_warm: files = %s\n", files);

/* Open file containing the names of files to be cached */
if ((fh = fopen(files, "r")) == NULL) {
printf("cache_warm: unable to open warm cache file %s: ignoring!\n", files);
return;
}

/* For each line in the file ... */
while (fgets(buf, PATH_MAX, fh) != NULL) {

/* Skip if empty or a comment (begins with #) */
if (strlen(buf) == 0)
continue;
if (buf[0] == '#')
continue;

/* Strip leading and trailing blanks before trying to cache it */
str = strtok(buf, " \n\t");
if (str == NULL)
continue;
ldbg("cache_warm: str = %s\n", str);
cache_it(str, (docroot == NULL) ? NULL : root);
}

fclose(fh);

/* Print out the cache now if that option is set */
if (printcache)
printCache();
}

/*---------------------------------------------------------------------*/
#ifdef __CYGWIN__
int
mlock(void *addr, unsigned int len)
{
  printf("mlock: doesn't exist on CYGWIN\n");
  exit(1);
}

/*---------------------------------------------------------------------*/
int
munlock(void *addr, unsigned int len)
{
  printf("munlock: doesn't exist on CYGWIN\n");
  exit(1);
}
#endif /* __CYGWIN__ */

void
working_set_add(int size)
{
  cacheStats *s;
  s = &stats;
  s->workingSetCount++;
  s->workingSetCur += size;
  s->workingSetTotal += s->workingSetCur;
  s->workingSetAvg = s->workingSetTotal / s->workingSetCount;

  if (s->workingSetCur > s->workingSetMax) {
    s->workingSetMax = s->workingSetCur;
  }
}

void
working_set_remove(int size)
{
  cacheStats *s;
  s = &stats;
  s->workingSetCur -= size;
  s->workingSetTotal -= s->workingSetCur;
  // s->workingSetCount++;
  /* s->workingSetAvg = s->workingSetTotal / s->workingSetCount; */
}


/*-----------------------------------------------------------------------
 * Check if the given cache_entry has been modified since the time
 * specified. Return 1 if it has, 0 if it hasn't. The file may 
 * need to be stat'd again.
 ------------------------------------------------------------------------*/
int check_if_modified_since( cacheinfo * cur, time_t since ) {
  int trace_fd = 0; /* for tracing */
  struct stat fileStat;
  int rc;

  assert(cur != NULL );
  if ( cur->last_stat <= since ) {
    /* Our last stat of the file was not recent enough */
    TRACE(EVT_FSTAT,
       trace_fd = cur->fd;
       rc = fstat(cur->fd, &fileStat);
    );
    if ( rc < 0 ) {
      printf("cache_mapped_new.c: check_if_modified_since: unable to stat fd =%d\n", cur->fd);
      exit(1);
    }
    num_extra_fstat++;
    cur->last_modified = fileStat.st_mtime;
    cur->last_stat = time(NULL);
  } else {
    num_cached_fstat++;
  }
  return (cur->last_modified > since);
}

#endif /* CACHE_MAPPED_NEW */
