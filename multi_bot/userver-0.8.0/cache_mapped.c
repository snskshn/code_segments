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
 *---------------------------------------------------------------------
 * Author: David Pariag
 * Date: Friday January 3, 2003
 *
 * Assumptions:
 *  - The cache will not be asked to store empty (zero-length) strings.
 *  - Cache entries are evicted when the cache's memory is exhausted!
 *    Thus, an empty bucket means nothing has ever been put into that 
 *    bucket => Search stops.
 *  - If the maximum load factor is exceeded, then the server exits.
 *
 * Possible changes
 *  - Change the header interface to match the hsearch interface
 *  - The cache array may be malloc'd in the init_cache routine
 *  - Use double hashing then linear search, instead of single 
 *    hash then linear search
 *
 * TODO:
 *  - Make sure that all memory is freed and open file descriptors are closed.
 *  - Need to do straight CLOCK instead of GCLOCK
 *
 *  TBB 
 *  Sat Jan 25 00:22:05 EST 2003
 *    Integrating this with the userver.
 *    Added some stats.
 *    Changed stats to floating point (lots of bytes for some runs)
 *  Wed Mar 19 2003
 *    Added the ability to use different hash functions.
 *    Moved hash function to separate file/module.
 *    Added the ability to rehash.
 *  Thu Mar 20 09:07:56 EST 2003
 *    Changed things so that 100% load factor could be handled.
 *
 *  TODO: Check collision stats?
 *---------------------------------------------------------------------
 */

#ifdef CACHE_MAPPED

#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>

/* For testing... */

#include <stdlib.h>
#include <time.h>

/* userver headers */
#include "options.h"
#include "common.h"
#include "debug.h"
#include "cache_mapped.h"
#include "myassert.h"
#include "trace.h"
#include "reply_status.h"


/* The cache data is stored in this global variable */
static fileCache cache;

/* Statistics about the cache performance */
static cacheStats stats;

static int cache_initialized;

unsigned int hash(char *uri);
unsigned int spec_hash(char *uri);
unsigned int rehash(char *uri, int extaval);
int hash_check_size(int size);
int cache_victim(void);

/*---------------------------------------------------------------------*/
/*
 * Initialize the cache by:
 *  - Allocating memory for the hash table.
 *  - Initialize each bucket to be "empty" (i.e. holding sentinel values)
 *  - Storing the limits on memory size and number of buckets.
 * Return 1 on success, 0 on failure
 */

int 
initCache(int tableSize, int maxBytes, int maxFileSize, 
               int maxLoadFactor, int lockPages) 
{
  int i;
  int newTableSize;
  int size;

  newTableSize = hash_check_size(tableSize);
  if (newTableSize != tableSize) {
    printf("initCache: tableSize = %d not suitable for hash function\n",
        tableSize);
    tableSize = newTableSize;
    printf("initCache: changed tableSize now = %d\n", tableSize);
  }

  /* Allocate memory for the hash table */
  size = sizeof(cacheinfo) * tableSize;
  cache.hashTable = (cacheinfo*) malloc(size);
  if (cache.hashTable == 0) { return 0; }
  num_bytes_malloced += size;
  
  /* Initialize cache structure */
  cache.clockHand = 0;
  cache.tableSize = tableSize;
  cache.numEntries = 0;
  cache.maxBytes = maxBytes;
  cache.numBytes = 0;
  cache.maxFileSize = maxFileSize;
  assert(maxLoadFactor > 0 && maxLoadFactor <= 100);
  cache.maxLoadFactor = maxLoadFactor;
  cache.lockPages = lockPages;

  /* Make sure hash table entries are empty */
  for (i = 0; i < tableSize; i++) {
    cache.hashTable[i].name[0] = '\0';
    cache.hashTable[i].buf = 0;
    cache.hashTable[i].buf_len = 0;
    cache.hashTable[i].reference = 0;
    cache.hashTable[i].use_count = 0;
  }

  /* Initialize statistics variables */
  stats.cacheRequests = 0.0;
  stats.numHits = 0.0;
  stats.bytesRequested = 0.0;
  stats.bytesServed = 0.0;
  stats.numCollisions = 0.0;
  stats.numEvictions = 0.0;
  stats.numProbes = 0.0;
  stats.maxProbes = 0.0;

  cache_initialized = 1;
  return 1;
}


/*---------------------------------------------------------------------*/
void
cache_cleanup(void)
{
  cacheClear();
}

/*---------------------------------------------------------------------*/
void
cacheClear(void)
{
  int i;
  
  for (i = 0; i < cache.tableSize; i++) {
    if (cache.hashTable[i].buf_len > 0) {
      munlock(cache.hashTable[i].buf, cache.hashTable[i].buf_len);
      munmap(cache.hashTable[i].buf, cache.hashTable[i].buf_len);
      cache.hashTable[i].buf = 0;
      cache.hashTable[i].buf_len = 0;
      cache.hashTable[i].name[0] = '\0';
    }
  }
  cache.numEntries = 0;  
  cache.numBytes = 0;
  cache.clockHand = 0;
  free(cache.hashTable);
}


/*---------------------------------------------------------------------*/
/*
 * Search the cache for the given uri/filename.
 * Return 1 if the element was found in the cache, 0 otherwise.
 * Parameters:
 *   uri - The key (filename) being searched for
 *   hashValue - The integer pointed to by this variable is set as follows:
 *      If the item is found, *hashValue is the item's index in the table
 *      If the item is not found and the table has room,
 *      *  hashValue is the index the 
 *      item should be inserted at
 *      If the item is not found and the table is full, *hashValue is set to -1.
 */

int
cache_lookup(char * uri, int * hashValue) 
{
  int i;
  int index = -1;
  int goodBucket = 0; /* Set this flag to 1 once we've found  */
                      /* where the uri should go. */
  int start;
  unsigned long int tmpVal;

  if (options.cache_for_spec) {
    *hashValue = spec_hash(uri);
    if (*hashValue >  cache.tableSize) {
      printf("cache_lookup: cache table size = %d too small\n", 
          cache.tableSize);
      printf("cache_lookup: spec_hash returns entry = %d\n", *hashValue);
      exit(1);
    }
    if ((cache.hashTable[(*hashValue)].buf_len == 0) ||
        (cache.hashTable[(*hashValue)].buf_len == -1)) {
      return 0;
    } else {
      return 1;
    }
  }
  
  tmpVal = hash(uri);
  *hashValue = tmpVal % cache.tableSize;


  /* TBB I also think that this code may need to be changed
   * to account fo the fact that buf_len == 0 indicates that
   * the entry has never been used and buf_len == -1 means
   * that it has been evicted.
   * I think that there are instances where this code does extra
   * work because buf_len == -1.
   *
   * Ah - maybe I see the problem and a problem with this approach in general.
   * It could be the case that we create a number of entries that
   * collide and get added because of linear probing.
   * Assume for the sake of argument that A-F all hash to slot[i].
   * Add A ... F.
   * slot[i]      = A
   * slot[i+1]    = B
   * slot[i+2]    = C
   * slot[i+3]    = D
   * slot[i+4]    = E
   * slot[i+5]    = F
   * 
   * Now evict D
   *
   * slot[i]      = A
   * slot[i+1]    = B
   * slot[i+2]    = C
   * slot[i+3]    = <empty>
   * slot[i+4]    = E
   * slot[i+5]    = F
   * 
   * Now search for D
   * Once an empty slot is reached does that mean that D 
   * isn't in the table?
   *
   * Now search for F
   * When can one stop the search?
   */
 
  if (cache.hashTable[(*hashValue)].buf_len == 0) {
    /* Nothing was ever placed in this bucket, so the uri can't 
     * be in the table 
     */
    return 0;
  } else if (strcmp(cache.hashTable[(*hashValue)].name, uri) == 0) {
    /* Found it! */
    return 1;
  }

  tmpVal = rehash(uri, *hashValue);
  *hashValue = tmpVal % cache.tableSize;

  if (cache.hashTable[(*hashValue)].buf_len == 0) {
    /* Nothing was ever placed in this bucket, 
     * so the uri can't be in the table
     */
    return 0;
  } else if (strcmp(cache.hashTable[(*hashValue)].name, uri) == 0) {
    /* Found it! */
    return 1;
  }

  /* Didn't find it in the expected place. Start scanning subsequent buckets */
  start = *hashValue;
  for (i = 0; i < cache.tableSize; i++) {
    index = (start + i + 1) % cache.tableSize;
    if (cache.hashTable[index].buf_len == 0) {
      /* This bucket never held anything, so the uri is not in the table. */
      (*hashValue) = index;
      /* TBB: shouldn't this update numProbes? */
      stats.numProbes += i + 1;
      if (i+1 > stats.maxProbes) {
        stats.maxProbes = i + 1;
      }
      return 0;

    } else if (cache.hashTable[index].buf_len == -1 && goodBucket == 0) {
      /* Whatever the bucket held has been evicted, so the uri can go here. */
      (*hashValue) = index;
      goodBucket = 1;
      /* Keep looking...
       * TBB: Don't return here because if this slot has been evicted 
       *      we could have 
       *      previously probed ahead until we found a vacant
       *      slot - so we need to continue looking until we determine
       *      that a match isn't found.
       *      But if it isn't we do want to use this slot.
       *      The linear search must continue until either:
       *         o we find a slot that has never been occupied
       *           (this argues for ensuring a low load factor.
       *         o we find a match
       *         o we have gone through the entire table
       *
       * Q: if we have a cap on total size of the cache and
       *    a large set of files that we are requesting
       *    wouldn't it be the case that eventually every slot
       *    will have been used at sometime and later evicted.
       *    This would result in a complete linear search of the
       *    entire table in order to find out that a url
       *    is not in the cache.

       * TBB: shouldn't this update numProbes
       * OOPS - nope because i is tracking the probes and 
       * it's updated when we return
       * stats.numProbes += i + 1;
       */
    } else if (strcmp(cache.hashTable[index].name, uri) == 0) {
      (*hashValue) = index;
      stats.numProbes += i + 1;
      if (i+1 > stats.maxProbes) {
        stats.maxProbes = i + 1;
      }
      return 1;
    }
  }
  
  /* This is no longer true now that table can get 100% full */
  /* assert(goodBucket == 1); */
  
  if (goodBucket == 0) {
    *hashValue = -1;
  }
  /* TBB - since this has been modified/added we should */
  /*       update the number of probes here */
  stats.numProbes += i + 1;
  if (i+1 > stats.maxProbes) {
    stats.maxProbes = i + 1;
  }
  return 0;
}


/*---------------------------------------------------------------------*/
void
cache_print_stats(void)
{
  printf("CACHE PARAMETERS\n");
  printf("cache num buckets             %20d\n", cache.tableSize);
  printf("cache size (bytes)            %20d\n", cache.maxBytes);
  printf("cache max file size (bytes)   %20d\n", cache.maxFileSize);
  printf("cache max load factor         %20d\n", cache.maxLoadFactor);
  
  printf("\n");
  printf("CACHE STATISTICS\n");
  printf("cache bytes used              %20d\n", cache.numBytes);
  printf("cache buckets used            %20d\n", cache.numEntries);
  printf("cache current load factor     %20.1f\n",
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
}


/*---------------------------------------------------------------------*/
void
printCache(void)
{
  int i;
  printf("%7s: %40s %3s %10s %10s\n",
         "Bucket", "Filename", "Ref", "Used", "BufLen");

  for (i = 0; i < cache.tableSize; i++) {

    if (cache.hashTable[i].buf_len > 0) {
      printf("%7d: %40s %3d %10d %10d\n", 
     i, cache.hashTable[i].name, cache.hashTable[i].reference,
     cache.hashTable[i].use_count, cache.hashTable[i].buf_len);
    } else if (cache.hashTable[i].buf_len == -1) {
      printf("%7d: %40s\n", i, "Evicted");
    } else {
      printf("%7d: %40s\n", i, "Empty");
    }
  }
  printf("Number of bytes used: %d\n", cache.numBytes);
  printf("Number of buckets used: %d\n", cache.numEntries);
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
cache_add(char * uri) 
{
  int index, found;
  int fd;
  int rc = 0;
  int trace_fd = 0;
  int initial_spot = 0;
  struct stat fileStat;
  int victim = -1;
  cacheinfo *cur;

  if (!cache_initialized) {
    initCache(options.cache_table_size, options.cache_max_bytes, 
              options.cache_max_file_size, options.cache_max_load_factor, 
              options.cache_lock_pages);
  }
  
  /* Check that the max load factor has not been exceeded */
  if ((cache.numEntries * 100) / cache.tableSize > cache.maxLoadFactor) {
    printf("cache_add: maximum load factor exceeded! Exiting...\n");
    printCache();
    cache_print_stats();
    exit(-1);
  }

  stats.cacheRequests++;  
  found = cache_lookup(uri, &index);
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
      PRINT_TIME(NOFD, &tnow, &tprev, "cache_add: open failed - returning fail");
      /* printf("cache_add: file open failed\n"); */
      return 0;
    }

    /* stat the file to get its size */
    
    TRACE(EVT_FSTAT,
       trace_fd = fd;
       rc = fstat(fd, &fileStat);
    );

    if (rc < 0) {      
      PRINT_TIME(NOFD, &tnow, &tprev, "cache_add: file stat failed");
      /* printf("cache_add: file stat failed\n"); */
      return 0;
    }

    stats.bytesRequested += fileStat.st_size;
    
    /* Make sure the file isn't too big to be cached */
    if (fileStat.st_size > cache.maxFileSize || 
      fileStat.st_size > cache.maxBytes) { 
      PRINT_TIME(NOFD, &tnow, &tprev, "File too big to cache!");
      /* printf("File too big to cache!\n"); */
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
    
    if ((cache.numBytes + fileStat.st_size) >= cache.maxBytes) {
      /* Need to evict some stuff to free up space */
      while ((cache.maxBytes - cache.numBytes) < fileStat.st_size) {
         victim = cache_victim();
         PRINT_TIME(NOFD, &tnow, &tprev, "cache_add: total byte victim = %d\n",
             victim);
         cache_delete_element(victim);
         cache_free_element(victim);
      } /* outer while        */
      
      /* Find out where the file should be cached, now that there's space */
      found = cache_lookup(uri, &index);
      assert(found == 0);
      assert(index != -1);
    }

    /* We still don't have a spot in the cache for the new entry */
    /* because the table is 100% full. */
    if ((found == 0) && (index == -1)) {
      assert(cache.numEntries == cache.tableSize);
      victim = cache_victim();
      PRINT_TIME(NOFD, &tnow, &tprev, "cache_add: full table victim = %d\n", 
          victim);
      found = 1;
      index = victim;
    }
    
    /* Add the file to the cache */
    if (strlen(uri) >= CACHE_NAME_MAX) {
      printf("cache_mapped: ERROR - CACHE_NAME_MAX = %d not large enough "
       "for URL name len = %d\n", CACHE_NAME_MAX, strlen(uri));
      exit(1);
    }

    cur = &cache.hashTable[index];
    cur->use_count = 1;
    strncpy(cur->name, uri, CACHE_NAME_MAX);

    TRACE(EVT_MMAP,
        trace_fd = fd;
        cur->buf = (char*) mmap(0, fileStat.st_size, PROT_READ, 
                                MAP_PRIVATE, fd, 0);
        rc = (cur->buf != MAP_FAILED);
    );

    assert(cur->buf != MAP_FAILED);
    cur->buf_len = fileStat.st_size;
    cur->reference = 1;
    if (cache.lockPages != 0) {
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

    rc = snprintf(cur->header, CACHED_HDR_SIZE, HTTP_OK_STR_ID_LEN, 
                  server_ident, (unsigned long int) cur->buf_len);
    PRINT_TIME(NOFD, &tnow, &tprev, "cache_add: reply header = %d\n", rc);

    if (rc >= CACHED_HDR_SIZE) {
      printf("cache_mapped: ERROR - CACHED_HDR_SIZE = %d\n", CACHED_HDR_SIZE);
      printf("   not large enough for generated output header = %d\n", rc);
      exit(1);
    }

    /* New rc above now returns number of bytes written. */
    /* cur->header_len = strlen(cur->header); */
    cur->header_len = rc;

    close(fd);
    return (cur);
    
  } else {
    /* Found it in the cache */
    cur = &cache.hashTable[index];
    stats.numHits++;
    stats.bytesRequested += cur->buf_len;
    stats.bytesServed += cur->buf_len;
    cur->reference = 1;
    cur->use_count++;
    PRINT_TIME(NOFD, &tnow, &tprev, "cache_add: entry found - returning");
    return (cur);
  }
  return 0;
}

int
cache_victim(void)
{
  int victim = -1;
  cacheinfo *cur;
  int trace_fd = 0;
  int rc = 0;

  while (1) {
    cur = &cache.hashTable[cache.clockHand];
    if (cur->buf_len > 0) {
      if (cur->reference == 0) {
        /* Evict this page */
        TRACE(EVT_MUNLOCK,
          trace_fd = 0;
          rc = munlock(cur->buf, cur->buf_len);
        );

        TRACE(EVT_MUNMAP,
          trace_fd = 0;
          rc = munmap(cur->buf, cur->buf_len);
        );
        cur->buf = 0;
        cache.numBytes = cache.numBytes - cur->buf_len;
        cur->buf_len = -1;
        cur->name[0] = '\0';
        cur->use_count = 0;
        cache.numEntries--;
        stats.numEvictions++;
        victim = cache.clockHand;
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
} /* cache_victim */


/*---------------------------------------------------------------------*/
#ifdef TESTMAIN

int 
main(int argc, char *argv[]) 
{
  cacheinfo * item;
  int i, index, size;
  char * files[18] = {"/home/dpariag/tmp/cache/README",
                      "/home/dpariag/tmp/cache/index.html",
                      "/home/dpariag/tmp/cache/irix.mpi.guide.pdf",
                      "/home/dpariag/tmp/cache/xfig-3.2.3c-8.i386.rpm",
                      "/home/dpariag/tmp/cache/news.html",
                      "/home/dpariag/tmp/cache/index.htm",
                      "/home/dpariag/tmp/cache/readme.txt",
                      "/home/dpariag/tmp/cache/about.html",
                      "/home/dpariag/tmp/cache/contactus.htm",
                      "/home/dpariag/tmp/cache/features.htm",
                      "/home/dpariag/tmp/cache/lucky.xml",
                      "/home/dpariag/tmp/cache/home.xml", 
                      "/home/dpariag/tmp/cache/mynameis.xml", 
                      "/home/dpariag/tmp/cache/uedit.zip", 
                      "/home/dpariag/tmp/cache/search.xml", 
                      "/home/dpariag/tmp/cache/sitemap.html", 
                      "/home/dpariag/tmp/cache/aboutus.doc", 
                      "/home/dpariag/tmp/cache/productdescription.doc", 
  };


  srand(time(NULL));
  size = 31;
  
  initCache(size,(4096*size), 8500, 60, 1);

  for (i=0; i < 10000; i++) {
    index = (rand()) % 18;
    item = cache_add(files[index]);
  }
  
  printCacheStats();
  printCache();
  cacheClear();
  return 0;
}
#endif /* TESTMAIN */
#endif /* CACHE_MAPPED */
