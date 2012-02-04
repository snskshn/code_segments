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

#define MAX_FILE_CHARS (200)
#define REQUESTDIR   "specweb99/file_set"

struct test {
  char *file_prefix;
  int start_num;
  int end_num;
  int iterations;
};

struct test test0 [] = {
  {"dir",   0,  1,                1},
  {"",      0,  0,                0},
};

struct test test1 [] = {
  {"dir",   0,  1,                2},
  {"",      0,  0,                0},
};

struct test test2 [] = {
  {"dir",   0,  3,                1},
  {"",      0,  0,                0},
};

struct test test3 [] = {
  {"dir",   0,  3,                5},
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
  int file = 0;
  int class = 0;
  int total_requests = 0;
  char destfile[MAX_FILE_CHARS];
  char logfile[MAX_FILE_CHARS];
  char requests[MAX_FILE_CHARS];
  struct test *tst = 0;
  FILE *fp;

  printf("Doing %d tests\n", (int) NUMTESTS);

  for (num = 0; num < NUMTESTS; num++) {
    total_requests = 0;
    sprintf(logfile, "speclogfile-%d", num+1);
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
	  for (class=0; class<4; class++) {
	    for (file=0; file<9; file++) {
	      sprintf(destfile, "%s/%s%05d/class%1d_%1d", 
	        REQUESTDIR, tst->file_prefix, k, class, file);
	      fprintf(fp, "%s%c", destfile, '\0');
	      total_requests++;
	    }
	  }
	} /* for */
        DEBG("num %5d prefix %8s start %4d end %5d iter %5d files %10d\n", 
	      num, tst->file_prefix, tst->start_num, 
	      tst->end_num, j, total_requests);
      } /* for */

      i++;
      tst = &alltests[num][i];
    } /* while */

    fclose(fp);

    sprintf(requests, "spec-requests-%d", num+1);
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
