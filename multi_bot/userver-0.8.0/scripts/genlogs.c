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
#include "gen.h"

#define DEBUG_ON

#ifdef DEBUG_ON
#define DEBG(...) \
  do {\
    printf(__VA_ARGS__); \
    fflush(stdout); \
    fflush(stderr); \
  } while (0);
#else
#define DEBG(x, __VA_ARGS__...)
#endif


struct test {
  char *file_prefix;       /* e.g., 1K- 10K- 100K- */
  int start_num;
  int end_num;
  int iterations;
};

struct test test0 [] = {
  {"1K-",   0,  ONEKFILES,        1},
  {"10K-",  0,  TENKFILES,        1},
  {"1K-",   0,  ONEKFILES,        1},
  {"100K-", 0,  ONEHUNDREDKFILES, 1},
  {"1K-",   0,  ONEKFILES,        1},
  {"",      0,  0,                0},
};

struct test test1 [] = {
  {"1K-",   0,  ONEKFILES,        2},
  {"10K-",  0,  TENKFILES,        2},
  {"100K-", 0,  ONEHUNDREDKFILES, 2},
  {"",      0,  0,                0},
};

struct test test2 [] = {
  {"100K-", 0,  ONEHUNDREDKFILES, 2},
  {"1K-",   0,  ONEKFILES,        10},
  {"10K-",  0,  TENKFILES,        2},
  {"1K-",   0,  ONEKFILES,        10},
  {"",      0,  0,                0},
};

struct test test3 [] = {
  {"100K-", 0,  ONEHUNDREDKFILES, 100},
  {"1K-",   0,  TENKFILES,        1},
  {"",      0,  0,                0},
};

struct test *alltests [] = {
  test0, test1, test2, test3
};

#define NUMTESTS       ((sizeof(alltests)) / (sizeof(struct test *)))

int
main(int argc, char *argv[])
{
  int i,j,k;
  int num = 0;
  int total_requests = 0;
  char destfile[MAX_FILE_CHARS];
  char logfile[MAX_FILE_CHARS];
  char requests[MAX_FILE_CHARS];
  struct test *tst = 0;
  FILE *fp;

  printf("Doing %d tests\n", (int) NUMTESTS);

  for (num = 0; num < NUMTESTS; num++) {
    total_requests = 0;
    sprintf(logfile, "logfile-%d", num+1);
    printf("logfile = %s\n", logfile);

    fp = fopen(logfile, "w");
    if (fp == NULL) {
      perror("fopen failed ");
      exit(1);
    }

    i = 0;
    tst = &alltests[num][i];

    while (strcmp(tst->file_prefix, "") != 0) {
      for (j = 0; j < tst->iterations; j++) {
	for (k = tst->start_num; k < tst->end_num; k++) {
	  sprintf(destfile, "%s/%s%06d", REQUESTDIR, tst->file_prefix, k);
	  fprintf(fp, "%s%c", destfile, '\0');
	  total_requests++;
	} /* for */
        DEBG("num %5d prefix %8s start %4d end %5d iter %5d files %10d\n", 
	      num, tst->file_prefix, tst->start_num, 
	      tst->end_num, j, total_requests);
      } /* for */

      i++;
      tst = &alltests[num][i];
    } /* while */

    fclose(fp);

    sprintf(requests, "requests-%d", num+1);
    printf("requests = %s\n", requests);
    fp = fopen(requests, "w");
    if (fp == NULL) {
      perror("fopen failed ");
      exit(1);
    }
    fprintf(fp, "%d\n", total_requests);

    fclose(fp);

  } /* for */

  exit(0);
}
