/*
    userver -- (pronounced you-server or micro-server).
    This file is part of the userver, a high-performance web server designed for
    performance experiments.

    Copyright (C) 2005-2010 Tim Brecht
    Based on the file originally Copyright (C) 2004  Hewlett-Packard Company

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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "primes.h"

/* #define LOCAL_DEBUG */
#include "local_debug.h"

#define    USE_HASH_ADDITIVE       (0)
#define    USE_HASH_BOB            (1)
#define    USE_HASH                (USE_HASH_BOB)
#define    USE_REHASH              (USE_HASH_BOB)

void check_hash_not_rehash();
static unsigned long int do_hash_additive(char * uri);
extern unsigned long int do_hash_bob(unsigned char *uri,
    unsigned long int length, unsigned long int initval);
unsigned long int hash(char *uri);
int find_closest_prime(int size);
int find_closest_power_of_2(int size);


/*---------------------------------------------------------------------*/
int 
hash_check_size(int size)
{
  int ret = 0;

  /* TBB: for now ensure that we adjust to the */
  /*      same size no matter what hash function is used. */
  ret = find_closest_prime(size);
    
  return ret;
}


/*---------------------------------------------------------------------*/
unsigned int 
rehash(char *uri, int extraval)
{
  unsigned long int ret = 0;

  switch (USE_REHASH) {
    case USE_HASH_ADDITIVE:
      check_hash_not_rehash();
      ret = do_hash_additive(uri);
      break;

    case USE_HASH_BOB:
      ret = do_hash_bob((unsigned char *) uri, strlen(uri), extraval);
      break;

    default:
      printf("rehash: unknown hash function defined %d\n", USE_HASH);
      exit(1);
      break;
  }
    
  ldbg("rehash: ret = %lu\n", ret);
  return ret;
}

/*---------------------------------------------------------------------*/
unsigned long int 
hash(char *uri)
{
  unsigned long int ret = 0;

  switch (USE_HASH) {
    case USE_HASH_ADDITIVE:
      ret = do_hash_additive(uri);
      break;

    case USE_HASH_BOB:
      ret = do_hash_bob((unsigned char *) uri, strlen(uri), 0);
      break;

    default:
      printf("hash: unknown hash function defined %d\n", USE_HASH);
      exit(1);
      break;
  }
    
  ldbg("hash: ret = %lu\n", ret);
  return ret;
}


/*---------------------------------------------------------------------*/
static unsigned long int 
do_hash_additive(char * uri) 
{
  int i, len;
  unsigned long int hashValue = 0;

  len = strlen(uri);
  for (i = 0; i < len; i++) {
    hashValue += ((unsigned long int) uri[i]) * (len+i);
  }

  return hashValue;
}

/*---------------------------------------------------------------------- */

int 
find_closest_prime(int size)
{
  int i;

  if (size < 0) {
    printf("Can't handle negative size = %d\n", size);
    exit(1);
  }

  if (size <= prime_list[0]) {
    return prime_list[0];
  } else if (size >= prime_list[PRIME_ENTRIES-1]) {
    printf("find_closest_prime: trying to find prime larger than %d\n", size);
    printf("find_closest_prime: max prime in list = %d\n", 
       prime_list[PRIME_ENTRIES-1]);
    printf("find_closest_prime: prime_list needs to be expanded\n");
    exit(1);
  } else {
    for (i=1; i<PRIME_ENTRIES; i++) {
      if (size > prime_list[i-1] && size <= prime_list[i]) {
        return(prime_list[i]);
      }
    }
  }
  return(-1);
}

/*---------------------------------------------------------------------*/
int
find_closest_power_of_2(int size)
{
  int cur = 0;
  int power = (1 << cur);
  int prev_power = power;

  if (size < 0) {
    printf("Can't handle negative size = %d\n", size);
    exit(1);
  }

  while (size > power) {
    /*
    printf("size = %d cur = %d power = %d prev_power = %d\n",
       size, cur, power, prev_power);
    */
    cur++;
    prev_power = power;
    power = (1 << cur);
  }

  if (size == power) {
    return power;
  } else {
    return prev_power;
  }
}


/*---------------------------------------------------------------------*/
void
check_hash_not_rehash()
{
  if (USE_HASH == USE_REHASH) {
    printf("Can't use these hash and rehash functions together\n");
    printf("USE_HASH = %d USE_REHASH = %d\n", USE_HASH, USE_REHASH);
    exit(1);
  }
}

/*---------------------------------------------------------------------*/
#ifdef TESTMAIN
int
main()
{
  int in = 0;
  int out = 0;
  char *str;
  unsigned int x = 0;
  ub4 xb = 0;

  /* int (*find_closest)(int) = find_closest_prime; */
  int (*find_closest)(int) = find_closest_power_of_2;

  in = 3;
  out = (*find_closest)(in);
  printf("in = %d, out = %d\n", in, out);

  in = 5;
  out = (*find_closest)(in);
  printf("in = %d, out = %d\n", in, out);

  in = 6;
  out = (*find_closest)(in);
  printf("in = %d, out = %d\n", in, out);

  in = 7;
  out = (*find_closest)(in);
  printf("in = %d, out = %d\n", in, out);

  in = 44987;
  out = (*find_closest)(in);
  printf("in = %d, out = %d\n", in, out);

  in = 44988;
  out = (*find_closest)(in);
  printf("in = %d, out = %d\n", in, out);

  in = 49999;
  out = (*find_closest)(in);
  printf("in = %d, out = %d\n", in, out);

  in = 50000;
  out = (*find_closest)(in);
  printf("in = %d, out = %d\n", in, out);

  in = (1 << 0);
  out = (*find_closest)(in);
  printf("in = %d, out = %d\n", in, out);

  in = (1 << 10) - 1;
  out = (*find_closest)(in);
  printf("in = %d, out = %d\n", in, out);

  in = (1 << 10);
  out = (*find_closest)(in);
  printf("in = %d, out = %d\n", in, out);

  in = (1 << 10) + 1;
  out = (*find_closest)(in);
  printf("in = %d, out = %d\n", in, out);

  in = (1 << 30);
  out = (*find_closest)(in);
  printf("in = %d, out = %d\n", in, out);

  printf("start of hash_bob\n");
  str = "specweb99/file_set/dir00000/class0_4"; 
  xb = hash_bob(str, strlen(str), 0);
  printf("str = %s hashes to %lu\n", str, xb);
  xb = hash_bob(str, strlen(str), 0);
  printf("str = %s hashes to %lu\n", str, xb);
  xb = hash_bob(str, strlen(str), 0);
  printf("str = %s hashes to %lu\n", str, xb);
  printf("end of of hash_bob\n");
  printf("\n");

  str = "specweb99/file_set/dir00000/class0_4"; 
  x = hash(str);
  printf("str = %s hashes to %u\n", str, x);
  x = hash(str);
  printf("str = %s hashes to %u\n", str, x);
  x = hash(str);
  printf("str = %s hashes to %u\n", str, x);

  printf("\n");
  str = "specweb99/file_set/dir00004/class0_0"; 
  x = hash(str);
  printf("str = %s hashes to %u\n", str, x);

  str = "specweb99/file_set/dir00040/class0_0"; 
  x = hash(str);
  printf("str = %s hashes to %u\n", str, x);

  str = "specweb99/file_set/dir00000/class4_0"; 
  x = hash(str);
  printf("str = %s hashes to %u\n", str, x);

}
#endif
