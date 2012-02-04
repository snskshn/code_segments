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
#include "netstat.h"

/* #define LOCAL_DEBUG */
#include "local_debug.h"

struct netstats_struct {
  char *file;
  char *first_col;
  char *name;
  long int start_val;
  long int end_val;
};

struct netstats_struct netstats[] = {
  {"/proc/net/snmp", "Tcp:", "AttemptFails", -1, -1},
  {"/proc/net/snmp", "Tcp:", "InSegs",       -1, -1},
  {"/proc/net/snmp", "Tcp:", "OutSegs",      -1, -1},
  {"/proc/net/snmp", "Tcp:", "RetransSegs",  -1, -1},
  {"/proc/net/snmp", "Tcp:", "InErrs",       -1, -1},
  {"/proc/net/netstat", "TcpExt:", "ListenOverflows", -1, -1},
  {"/proc/net/netstat", "TcpExt:", "ListenDrops", -1, -1},
};

#define NUM_STATS    (sizeof(netstats) / sizeof(struct netstats_struct))

#define READ_BUF_SIZE   (1024)

int read_value(char *file, char *line_tag, char *looking_for);
int get_nth_value(char *buf, int n);
int get_index(char *buf, char *looking_for);
/* ------------------------------------------------------------------- */
/*
 * The idea here is to specify a file where were are looking
 * for the stats. The tag line: which specifies the first
 * column indicator in the file and then the subfield within
 * the line we are looking for

 * For example /proc/net/snmp contains the following lines:
 * Tcp: RtoAlgorithm RtoMin RtoMax MaxConn ActiveOpens PassiveOpens 
 *      AttemptFails EstabResets CurrEstab InSe gs OutSegs RetransSegs 
 *      InErrs OutRsts
 * Tcp: 0 0 0 0 0 2 0 0 2 1311 1184 0 0 0
 * So read_value("/proc/net/snmp", "Tcp:", "AttemptFails")
 *    should figure out which element of the array the AttemptFails
 *    count would be in and return that value.
 *    Here it is the 7th string in the first line
 *    so we should then return the 7th integer in the 2nd line.
 *
 */

int
get_nth_value(char *buf, int n)
{
  int index = 0;
  int ret = -1;
  int done = 0;
  int x = 0;
  char tmp[READ_BUF_SIZE];

  ldbg("get_nth_value: buf = %s n = %d\n", buf, n);    

  x = sscanf(buf, "%s:", tmp);
  ldbg("get_nth_value: start of line contains %s\n", tmp);

  while (((x = sscanf(buf, " %s", tmp)) > 0) && !done) {
    ldbg("get_nth_value: x = %d index = %d tmp = %s\n", x, index, tmp);    
    if (index == n) {
      ret = atoi(tmp);
      done = 1;
    } else {
      index++;
      buf += strlen(tmp) + 1;
      ldbg("get_nth_value: buf is now = %s", buf);
    }
  }

  ldbg("get_nth_value: returning %d\n", ret);
  return ret;
}


/* ------------------------------------------------------------------- */
int
get_index(char *buf, char *looking_for)
{
  int ret = -1;
  char tmp[READ_BUF_SIZE];
  int index = 0;
  int done = 0;
  int x = 0;

  ldbg("get_index: buf = %s", buf);
  ldbg("get_index: looking_for = %s\n", looking_for);    

  while (((x = sscanf(buf, "%s", tmp)) > 0) && !done) {
    ldbg("get_index: x = %d tmp = %s\n", x, tmp);    
    if (strcmp(tmp, looking_for) == 0) {
      ldbg("get_index: found at index = %d\n", index);    
      ret = index;
      done = 1;
    } else {
      index++;
      buf += strlen(tmp) + 1;
      ldbg("get_index: buf is now = %s", buf);
    }
  }

  ldbg("get_index: returning %d\n", ret);
  return ret;
}

/* ------------------------------------------------------------------- */
int
read_value(char *file, char *line_tag, char *looking_for) 
{
  FILE *fp = 0;
  char buf[READ_BUF_SIZE];
  char tmp[READ_BUF_SIZE];
  int done = 0;
  int index = 0;
  int ret = -1;
  int x = 0;

  ldbg("read_value: file = %s, line_tag = %s looking_for = %s\n",
      file, line_tag, looking_for);

  fp = fopen(file, "r");
  if (fp == 0) {
    ldbg("Unabled to open file = %s\n", file);
    ret = -1;
    done = 1;
  } else {
    while (fgets(buf, READ_BUF_SIZE, fp) != 0 && !done) {
      ldbg("read_value: buf = %s\n", buf);
      x = sscanf(buf, "%s", tmp);
      ldbg("read_value: sscanf returns = %d\n", x);
      ldbg("read_value: tmp = %s\n", tmp);
      if (strcmp(line_tag, tmp) == 0) {
        index = get_index(buf, looking_for);
        ldbg("read_value: index = %d\n", index);
        if (index >=0) {
          if (fgets(buf, READ_BUF_SIZE, fp) == 0) {
            ldbg("read_value: fgets second line failed\n");
            ret = -1;
            done = 1;
          } else {
            ret = get_nth_value(buf, index);
            done = 1;
          }
        } else {
          ret = -1;
          done = 1;
        }
      }
    }
    fclose(fp);
  }


  ldbg("read_value: returning %d\n", ret);
  return ret;
}


/* ------------------------------------------------------------------- */
void
netstat_init(void)
{
  unsigned int i;
  struct netstats_struct *n = 0;

  for (i = 0; i < NUM_STATS; i++) {
    n = &netstats[i];
    n->start_val = read_value(n->file, n->first_col, n->name);
  }
}

/* ------------------------------------------------------------------- */
void
netstat_print(void)
{
  unsigned int i;
  struct netstats_struct *n = 0;

  printf("-------------------------------------------------------------\n");
  printf("Netstats:\n");

  for (i = 0; i < NUM_STATS; i++) {
    n = &netstats[i];
    n->end_val = read_value(n->file, n->first_col, n->name);

    if ((n->start_val < 0) || (n->end_val < 0)) {
      printf("%s : unable to get network stats\n", n->name);
    } else {
      ldbg("%-20s end      = %ld\n", n->name, n->end_val);
      ldbg("%-20s start    = %ld\n", n->name, n->start_val);
      ldbg("%-20s diff     = %ld\n", n->name, (n->end_val - n->start_val));
      ldbg("\n"); 

      printf("%-20s        = %ld\n", n->name, (n->end_val - n->start_val));
    }
  }
}

/* ------------------------------------------------------------------- */
#ifdef TESTMAIN
int
main(int argc, char *argv[])
{
  netstat_init();
  netstat_print();
}
#endif
