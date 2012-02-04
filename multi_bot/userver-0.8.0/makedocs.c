/*
    userver -- (pronounced you-server or micro-server).
    This file is part of the userver, a high-performance web server designed for
    performance experiments.
          
    This file is Copyright (C) 2004-2010  Tim Brecht and Craig Barkhouse

    Authors: Tim Brecht <brecht@cs.uwaterloo.ca>
    Authors: Craig Barkhouse <cabarkho@student.cs.uwaterloo.ca>
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


#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>


#ifndef VERSION
#error VERSION not defined!
#endif

#include "reply_status.h"


void create_generic_doc(long size);


int create_doc(char *filename, long size, char *data, long datalen, int header)
{
  static char default_data[] =
    "0123456789ABCDEFGHIJabcdefghij0123456789ABCDEFGHIJabcdefghij012";
  FILE *fp;
  int use_default_data;
  int towrite;
  int i;

  if ((fp = fopen(filename, "wb")) == NULL) {
    printf("could not open '%s'\n", filename);
    return errno;
  }

  use_default_data = (data == NULL);
  if (use_default_data) {
    data = default_data;
  }
  if (datalen < 0) {
    datalen = strlen(data);
  }
  if (size < 0) {
    size = datalen;
  }

  if (header) {
    char header[1024];
    sprintf(header, HTTP_OK_STR_ID_LEN, 1, VERSION, size);
    if (fputs(header, fp) < 0) {
      fclose(fp);
      return errno;
    }
  }

  while (size > 0) {
    if (size >= datalen) {
      towrite = datalen;
    } else if (use_default_data) {
      towrite = size - 1;
    } else {
      towrite = size;
    }
    for (i = 0; i < towrite; i++) {
      if (fputc(data[i], fp) < 0) {
	fclose(fp);
	return errno;
      }
    }
    size -= towrite;
    if ((size > 0) && use_default_data) {
      if (fputc('\n', fp) < 0) {
	fclose(fp);
	return errno;
      }
      size--;
    }
  }

  fclose(fp);
  return 0;
}


void create_generic_doc(long size)
{
  char filename[64];
  int prefix_len;

  if ((size % 1048576) == 0) {
    prefix_len = sprintf(filename, "%ldM", size / 1048576);
  } else if ((size % 1024) == 0) {
    prefix_len = sprintf(filename, "%ldK", size / 1024);
  } else {
    prefix_len = sprintf(filename, "%ldB", size);
  }
  strcpy(filename + prefix_len, ".txt");
  create_doc(filename, size, NULL, -1, 0);
  strcpy(filename + prefix_len, "-w-header.txt");
  create_doc(filename, size, NULL, -1, 1);
}


int main(int argc, char *argv[])
{
  char *docdir = "docs";

  while (argc > 1) {
    if (strncmp(argv[1], "-d", 2) == 0) {
      if (argv[1][2] != '\0') {
	docdir = argv[1] + 2;
	argv++;
	argc--;
      } else if (argc > 2) {
	docdir = argv[2];
	argv += 2;
	argc -= 2;
      } else {
	fprintf(stderr, "The -d option requires an argument!\n");
	return 1;
      }
    } else {
      break;
    }
  }

  mkdir(docdir, 0777);
  if (chdir(docdir) < 0) {
    return -1;
  }

  create_generic_doc(0);
  create_generic_doc(1);
  create_generic_doc(2);
  create_generic_doc(10);
  create_generic_doc(960);
  create_generic_doc(1024);
  create_generic_doc(1024*2);
  create_generic_doc(1024*3);
  create_generic_doc(1024*4);
  create_generic_doc(1024*5);
  create_generic_doc(1024*10);
  create_generic_doc(14900);    /* Mean spec file size */
  create_generic_doc(1024*100);
  create_generic_doc(1024*1024);
  // create_generic_doc(1024*1024*1024);

#ifdef AFFINITY
  create_generic_doc(1024*20);
  create_generic_doc(1024*25);
  create_generic_doc(3*1024*1024);
  create_generic_doc(4*1024*1024);
  create_generic_doc(5*1024*1024);
#endif /* AFFINITY */

  create_doc("one_pkt",
      -1, 
      "     1024 file_set/dir00000/class1_0\n"\
      "f<!WEB99CAD><IMG SRC=\"/file_set/dirNNNNN/classX_Y\"><!/WEB99CAD>\n"\
      "o[|2l}Dos85m4s@`1aCHF0Pi7%p9p]Wa:Sq(R5wEM,4^|S@05bWZo'C(+3?ypsZ\n"\
      ",HM2w`)?/2Pl1$/@8ntq`{5e&G&!9v[C@(T8gZVsk&by)n;>^00>+D&0j+/&\"iF\n"\
      "AnwVVS.K>2-::zTXYcevk+y{t',w(r?F`6|8h*c)9oBSjs+EWo;Dw7Am;Je@<$c\n"\
      "|:a6\"iv*$hITR?\\vuK19DGZ4_$v!>yd<3EO5.H<20eca$BWxle13/hDkl:l*3PE\n"\
      "FrqX#9s2FXt)[6]S%Ed5QNV<:o*C\"WeGLY|Nop_8JS>&i{Xk@<}njU,$D4GEj.j\n"\
      "6dhaVZBm%s-*^(_IE{H5g|?jBR1d<?PR#95XpUEtJP{(W]P|Zw1AsO+5#<x=XIn\n"\
      "Za#3SUWH|)E&^\"SZ[Jj{?<)Q<BJXyrFSSGe&{=KxCp{#qQ\\MxGH8bNh{o2Th&y>\n"\
      "V@#\\<?)6`v3bha?5Ze\\qG-YBykudno\"G1$#JA)_#}pbgQ!y-cW}*aYL[DCA42@Z\n"\
      "CA\\laeLad<DKlDDy(zx1\\Q\\9t}W)1vbQ9?>x$i[e'!0pCSkJMdZ+57A*4w2Bmss\n"\
      "'23!5yYy}X+oz[[D+A{5U2T^EKmg;cZ?rl=*eu#bM,SGf.jpNh&$yY_?&O(>2aZ\n"\
      "&Mv-2l0s;;Fb!TLo\"4t#.MbLP1QlB2HE\\ARm-abGy()y[Sj[f^]s+?A[OoIn#n6\n"\
      "\\/gK;J-`C5f>m;)H\"f(tqD6Ls%rc(bxao_/,+9iMMRi;jqbkWg`H.us!wgc|J\\`\n"\
      ";=nDF).pT_YlIJN5$8tICi?Cb&&aM_Cez1*B972jsiX>5)P6>F_`1{#q!(TNgt3\n"\
      "c'<%=P4*E|_a4e3G#W(be#dX$kl\n",
      -1,
      0);

  return 0;
}
