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
#include "gen.h"

int
main(int argc, char *argv[])
{
  int i;
  int rc;
  char srcfile[MAX_FILE_CHARS];
  char destfile[MAX_FILE_CHARS];
  char cmd[MAX_FILE_CHARS * 2];

  /* first create the test files */
  sprintf(cmd, "mkdir %s", DESTDIR);
  rc = system(cmd);

  sprintf(srcfile, "%s/%s", SRCDIR, ONEKSRC);
  for (i=0; i<ONEKFILES; i++) {
    sprintf(destfile, "%s/1K-%06d", DESTDIR, i);
    sprintf(cmd, "cp %s %s", srcfile, destfile);
    rc = system(cmd);
  } /* for */

  sprintf(srcfile, "%s/%s", SRCDIR, TENKSRC);
  for (i=0; i<TENKFILES; i++) {
    sprintf(destfile, "%s/10K-%06d", DESTDIR, i);
    sprintf(cmd, "cp %s %s", srcfile, destfile);
    rc = system(cmd);
  } /* for */

  sprintf(srcfile, "%s/%s", SRCDIR, ONEHUNDREDKSRC);
  for (i=0; i<ONEHUNDREDKFILES; i++) {
    sprintf(destfile, "%s/100K-%06d", DESTDIR, i);
    sprintf(cmd, "cp %s %s", srcfile, destfile);
    rc = system(cmd);
  } /* for */

  exit(0);
}
