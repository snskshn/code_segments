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


#ifndef STATE_H
#define STATE_H

#include "info.h"

extern char *fsm_string[];

extern void set_fsm_state(struct info *ip, enum info_fsm_state state);
extern int get_fsm_state(int sd);
char *get_fsm_state_string(int sd);
extern int fsm_state_writing(int sd);
extern int fsm_state_reading(int sd);
extern char *fsm_state_string(int sd);
extern char *fsm_state_to_string(enum info_fsm_state state);
#ifdef FIXHUP
void convert_to_hupped(struct info *client);
void unblock_hupped_client(struct info *client_ip);
#endif /* FIXHUP */

extern void sd_state_not_writable(struct info *ip);
extern void sd_state_not_readable(struct info *ip);
extern void sd_state_update(struct info *ip, int events);

#endif /* STATE_H */
