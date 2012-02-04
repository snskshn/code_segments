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
#include <string.h>

/* #define LOCAL_DEBUG */
#include "local_debug.h"

#define CHAR_TO_INT(c)   (c - 48)

/* Hash function that does perfect hashing on SpecWeb99 file names.
 * Names look like  dir<iiiii>/class<j>_<k>
 * There <iiiii> means that there are 5 digits that we 
 *     read into i (below), 1 digits to read into j and 1 into k.
 * The i value indicates the directory number.
 * The j value indicates the class
 * The k value indicates the file.
 *
 * Since file names in each directory go from 
 * class0_0 .. class0_8 ... class3_8
 * there are 9 names for each class and 4 classes in each directory.
 * So 36 files in each directory.
 *
 * So index = (i*36) + (j*9) + k
 *
 * The table size needs to be (36 * (max_dirnum + 1))
 */

unsigned long int
spec_hash(char *url)
{
   int i = 0;
   int j = 0;
   int k = 0;
   char *i_rev_ptr = 0;
   char *j_ptr = 0;
   char *k_ptr = 0;
   int register magnitude = 1;
   static int k_index = -1;
   unsigned long int ret = 0;

   if (k_index == -1) {
     k_index = (strlen(url) - 1);
   }

   k_ptr = &url[k_index];
   j_ptr = k_ptr - 2;
   i_rev_ptr = j_ptr - 7;
   ldbg("url = %s  i_rev_char = %c j_char = %c k_char = %c\n", 
     url, *i_rev_ptr, *j_ptr, *k_ptr);

   while (*i_rev_ptr != 'r') {
     i += CHAR_TO_INT(*i_rev_ptr) * magnitude;
     ldbg("cur = %c i = %d\n", *i_rev_ptr, i);
     magnitude *= 10;
     i_rev_ptr--;
   }

   j = CHAR_TO_INT(*j_ptr);
   k = CHAR_TO_INT(*k_ptr);

   ret = ((i*36) + (j*9) + k);
   ldbg("i = %d j = %d k = %d  ret = %d\n", i, j, k, ret);
   return(ret);
}

#ifdef TESTMAIN

char *spec_files[] = {
  "specweb99/file_set/dir00000/class0_0",
  "specweb99/file_set/dir00000/class0_1",
  "specweb99/file_set/dir00000/class0_2",
  "specweb99/file_set/dir00000/class0_3",
  "specweb99/file_set/dir00000/class0_4",
  "specweb99/file_set/dir00000/class0_5",
  "specweb99/file_set/dir00000/class0_6",
  "specweb99/file_set/dir00000/class0_7",
  "specweb99/file_set/dir00000/class0_8",
  "specweb99/file_set/dir00000/class1_0",
  "specweb99/file_set/dir00000/class1_1",
  "specweb99/file_set/dir00000/class1_2",
  "specweb99/file_set/dir00000/class1_3",
  "specweb99/file_set/dir00000/class1_4",
  "specweb99/file_set/dir00000/class1_5",
  "specweb99/file_set/dir00000/class1_6",
  "specweb99/file_set/dir00000/class1_7",
  "specweb99/file_set/dir00000/class1_8",
  "specweb99/file_set/dir00000/class2_0",
  "specweb99/file_set/dir00000/class2_1",
  "specweb99/file_set/dir00000/class2_2",
  "specweb99/file_set/dir00000/class2_3",
  "specweb99/file_set/dir00000/class2_4",
  "specweb99/file_set/dir00000/class2_5",
  "specweb99/file_set/dir00000/class2_6",
  "specweb99/file_set/dir00000/class2_7",
  "specweb99/file_set/dir00000/class2_8",
  "specweb99/file_set/dir00000/class3_0",
  "specweb99/file_set/dir00000/class3_1",
  "specweb99/file_set/dir00000/class3_2",
  "specweb99/file_set/dir00000/class3_3",
  "specweb99/file_set/dir00000/class3_4",
  "specweb99/file_set/dir00000/class3_5",
  "specweb99/file_set/dir00000/class3_6",
  "specweb99/file_set/dir00000/class3_7",
  "specweb99/file_set/dir00000/class3_8", 
  /* */
  "specweb99/file_set/dir00001/class0_0",
  "specweb99/file_set/dir00001/class0_1",
  "specweb99/file_set/dir00001/class0_2",
  "specweb99/file_set/dir00001/class0_3",
  "specweb99/file_set/dir00001/class0_4",
  "specweb99/file_set/dir00001/class0_5",
  "specweb99/file_set/dir00001/class0_6",
  "specweb99/file_set/dir00001/class0_7",
  "specweb99/file_set/dir00001/class0_8",
  "specweb99/file_set/dir00001/class1_0",
  "specweb99/file_set/dir00001/class1_1",
  "specweb99/file_set/dir00001/class1_2",
  "specweb99/file_set/dir00001/class1_3",
  "specweb99/file_set/dir00001/class1_4",
  "specweb99/file_set/dir00001/class1_5",
  "specweb99/file_set/dir00001/class1_6",
  "specweb99/file_set/dir00001/class1_7",
  "specweb99/file_set/dir00001/class1_8",
  "specweb99/file_set/dir00001/class2_0",
  "specweb99/file_set/dir00001/class2_1",
  "specweb99/file_set/dir00001/class2_2",
  "specweb99/file_set/dir00001/class2_3",
  "specweb99/file_set/dir00001/class2_4",
  "specweb99/file_set/dir00001/class2_5",
  "specweb99/file_set/dir00001/class2_6",
  "specweb99/file_set/dir00001/class2_7",
  "specweb99/file_set/dir00001/class2_8",
  "specweb99/file_set/dir00001/class3_0",
  "specweb99/file_set/dir00001/class3_1",
  "specweb99/file_set/dir00001/class3_2",
  "specweb99/file_set/dir00001/class3_3",
  "specweb99/file_set/dir00001/class3_4",
  "specweb99/file_set/dir00001/class3_5",
  "specweb99/file_set/dir00001/class3_6",
  "specweb99/file_set/dir00001/class3_7",
  "specweb99/file_set/dir00001/class3_8", 
  /* */
  "specweb99/file_set/dir00100/class0_0",
  "specweb99/file_set/dir00100/class0_1",
  "specweb99/file_set/dir00100/class0_2",
  "specweb99/file_set/dir00100/class0_3",
  "specweb99/file_set/dir00100/class0_4",
  "specweb99/file_set/dir00100/class0_5",
  "specweb99/file_set/dir00100/class0_6",
  "specweb99/file_set/dir00100/class0_7",
  "specweb99/file_set/dir00100/class0_8",
  "specweb99/file_set/dir00100/class1_0",
  "specweb99/file_set/dir00100/class1_1",
  "specweb99/file_set/dir00100/class1_2",
  "specweb99/file_set/dir00100/class1_3",
  "specweb99/file_set/dir00100/class1_4",
  "specweb99/file_set/dir00100/class1_5",
  "specweb99/file_set/dir00100/class1_6",
  "specweb99/file_set/dir00100/class1_7",
  "specweb99/file_set/dir00100/class1_8",
  "specweb99/file_set/dir00100/class2_0",
  "specweb99/file_set/dir00100/class2_1",
  "specweb99/file_set/dir00100/class2_2",
  "specweb99/file_set/dir00100/class2_3",
  "specweb99/file_set/dir00100/class2_4",
  "specweb99/file_set/dir00100/class2_5",
  "specweb99/file_set/dir00100/class2_6",
  "specweb99/file_set/dir00100/class2_7",
  "specweb99/file_set/dir00100/class2_8",
  "specweb99/file_set/dir00100/class3_0",
  "specweb99/file_set/dir00100/class3_1",
  "specweb99/file_set/dir00100/class3_2",
  "specweb99/file_set/dir00100/class3_3",
  "specweb99/file_set/dir00100/class3_4",
  "specweb99/file_set/dir00100/class3_5",
  "specweb99/file_set/dir00100/class3_6",
  "specweb99/file_set/dir00100/class3_7",
  "specweb99/file_set/dir00100/class3_8", 
  /* */
  "specweb99/file_set/dir12341/class0_0",
  "specweb99/file_set/dir12341/class0_1",
  "specweb99/file_set/dir12341/class0_2",
  "specweb99/file_set/dir12341/class0_3",
  "specweb99/file_set/dir12341/class0_4",
  "specweb99/file_set/dir12341/class0_5",
  "specweb99/file_set/dir12341/class0_6",
  "specweb99/file_set/dir12341/class0_7",
  "specweb99/file_set/dir12341/class0_8",
  "specweb99/file_set/dir12341/class1_0",
  "specweb99/file_set/dir12341/class1_1",
  "specweb99/file_set/dir12341/class1_2",
  "specweb99/file_set/dir12341/class1_3",
  "specweb99/file_set/dir12341/class1_4",
  "specweb99/file_set/dir12341/class1_5",
  "specweb99/file_set/dir12341/class1_6",
  "specweb99/file_set/dir12341/class1_7",
  "specweb99/file_set/dir12341/class1_8",
  "specweb99/file_set/dir12341/class2_0",
  "specweb99/file_set/dir12341/class2_1",
  "specweb99/file_set/dir12341/class2_2",
  "specweb99/file_set/dir12341/class2_3",
  "specweb99/file_set/dir12341/class2_4",
  "specweb99/file_set/dir12341/class2_5",
  "specweb99/file_set/dir12341/class2_6",
  "specweb99/file_set/dir12341/class2_7",
  "specweb99/file_set/dir12341/class2_8",
  "specweb99/file_set/dir12341/class3_0",
  "specweb99/file_set/dir12341/class3_1",
  "specweb99/file_set/dir12341/class3_2",
  "specweb99/file_set/dir12341/class3_3",
  "specweb99/file_set/dir12341/class3_4",
  "specweb99/file_set/dir12341/class3_5",
  "specweb99/file_set/dir12341/class3_6",
  "specweb99/file_set/dir12341/class3_7",
  "specweb99/file_set/dir12341/class3_8", 
};

int num_spec_files = (sizeof(spec_files) / sizeof(char *));

#define NUM_TESTS     (1)

main()
{
  int i, index;
  int tests;

  for (tests = 0; tests < NUM_TESTS; tests++) {
    for (i=0; i<num_spec_files; i++) {
      index = spec_hash(spec_files[i]);
      ldbg("index = %10d url = %s\n", index, spec_files[i]);
    }
  }
}

#endif /* TESTMAIN */
