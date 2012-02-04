/*
    userver -- (pronounced you-server or micro-server).
    This file is part of the userver, a high-performance web server designed for
    performance experiments.
          
    This file is Copyright (C) 2004-2010  Tim Brecht

    Author: Tim Brecht <brecht@cs.uwaterloo.ca>.

    This code has evolved from code for servers by: 
       David Mosberger <david.mosberger@hp.com> and
       Abhishek Chandra <abhishek@cs.umass.edu>

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
#include "common.h"
#include "stats.h"
#include "options.h"
#include "aio_layer.h"
#include "debug.h"

/* ---------------------------------------------------------------------- */
int max_sd;
int max_opensd;
int num_idle;     /* number of fds/sds that aren't used */
int num_select;
int cur_conns;    /* current number of connections */
                  /* this should often == maxconns - num_idle 
		   * except when aio is being used */

int saved_mode = 0;
int in_cs = 0;
int in_handler = 0;
int sigio_blocked = 0;
int max_sock_sd = 0;
fd_set openfds;

int pagesize;
int cache_line_size;
