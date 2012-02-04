/*
    gen-coax : generate log file used for cache coaxing experiments

    userver -- (pronounced you-server or micro-server).
    This file is part of the userver, a high-performance web server designed for
    performance experiments.

    This file is Copyright (C) 2010  Tim Brecht

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

#if 0

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

#endif /* if 0 */


#ifdef SMALL_FILE_SET
#define BIG_FILES         (130)
#define SMALL_FILES       (800)
#else
#define BIG_FILES         (0)
#define SMALL_FILES       (500)
#endif

#define BIG_SIZE          (125 * 1024 * 1024)
#define SMALL_SIZE        ((125 * 1024 * 1024) / 10)

#define MAX_FILE_CHARS    (200)
#define NUM_CLIENTS       (8)

#define CALL_STATS_FILE "call-stats-files.in"

int
main(int argc, char *argv[])
{
  int i;
  int num = 0;
  int smallfile = 0;
  int bigfile = 0;
  int total_requests = 0;
  char destfile[MAX_FILE_CHARS];
  char logfile[MAX_FILE_CHARS];
  FILE *fp = 0;
  FILE *cfp = 0;

  printf("Generating %d client log files\n", (int) NUM_CLIENTS);
  printf("Generating call stats size file %s\n", CALL_STATS_FILE);

  for (num = 1; num <= NUM_CLIENTS; num++) {
    total_requests = 0;
    sprintf(logfile, "coax-%d.log", num);
    printf("logfile = %s\n", logfile);

    fp = fopen(logfile, "w");
    if (fp == NULL) {
      perror("fopen failed ");
      exit(1);
    }

    bigfile = 0;
    for (smallfile = 0; smallfile < SMALL_FILES/NUM_CLIENTS; smallfile++) {
#ifdef SMALL_FILE_SET
        if ((smallfile % 10) == 0) {
	  fprintf(fp, "# Session %d\n", total_requests + 1);
	  sprintf(destfile, "%s/125M-%03d.txt", "file_set_125M", bigfile + ((num-1) * 10)); 
	  fprintf(fp, "%s\n\n", destfile);
	  fflush(fp);
	  total_requests++;
          bigfile++;
        }
#endif

	fprintf(fp, "# Session %d\n", total_requests + 1);
#ifdef SMALL_FILE_SET
	sprintf(destfile, "%s/100Mb-%04d.txt", "file_set_10M", smallfile + ((num-1) * 100)); 
#else
	sprintf(destfile, "%s/100Mb-%05d.txt", "file_set_10M", 
                ((num-1) * (SMALL_FILES/NUM_CLIENTS) + smallfile));
#endif
	fprintf(fp, "%s\n\n", destfile);
        fflush(fp);
	total_requests++;
    }
  } /* for */

  cfp = fopen(CALL_STATS_FILE, "w");
  if (cfp == NULL) {
    perror("fopen failed ");
    exit(1);
  }

  /* First print out how many entries are in the file */
  fprintf(cfp, "%d\n", SMALL_FILES + BIG_FILES);

  for (i=0; i<SMALL_FILES; i++) {
#ifdef SMALL_FILE_SET
    fprintf(cfp, "%s/100Mb-%04d.txt %d\n", "file_set_10M", i, SMALL_SIZE);
#else
    fprintf(cfp, "%s/100Mb-%05d.txt %d\n", "file_set_10M", i, SMALL_SIZE);
#endif
  }

  for (i=0; i<BIG_FILES; i++) {
    fprintf(cfp, "%s/125M-%03d.txt %d\n", "file_set_125M", i, BIG_SIZE);
  }

  fclose(fp);
  fclose(cfp);

  exit(0);
}
