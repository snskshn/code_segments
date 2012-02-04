/*
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


#ifndef _CALL_STATS_H_ 
#define _CALL_STATS_H_ 

#ifdef CALL_STATS

/* Initialize the data structures used for tracking per file statistics */
void call_stats_init();

void call_stats_request(int req_num, char *uri);
void call_stats_failed(int req_num, int size, int nwritten,
  int client_id, int call_id, 
        int http_status, int reason);
void call_stats_finished(int req_num, int size, int nwritten, 
  int client_id, int call_id, int http_status);

/* Print a summary of the per file statistics. */
void call_stats_print();

#endif /* CALL_STATS */

#endif /* _CALL_STATS_H_ */
