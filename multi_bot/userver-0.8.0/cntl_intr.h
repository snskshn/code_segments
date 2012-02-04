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



#ifndef CNTL_INTR_H
#define CNTL_INTR_H

#include "cntl_conns.h"
#include "debug.h"

void intr_enable();
void intr_disable();
void intr_print_info();
void block_sigio();
void unblock_sigio();
void do_block();
void do_unblock();

#ifdef SEND
void block_send_events();
void unblock_send_events();
#endif /* SEND */

extern int entering_cs;
extern int intr_depth;


/* Use these when we aren't using asynchronous delivery */
#if 0
#define notification_off()
#define notification_on()
#endif

#define notification_off() \
  if (options.asynch_mode) { \
    entering_cs = 1; \
    PRINT_TIME(NOFD, &tnow, &tprev, "notification_off: in_handler = %d", \
	in_handler); \
    DEBG(MSG_INTR, "notification_off: file = %25s line = %6d\n", \
	__FILE__, __LINE__); \
    DEBG(MSG_INTR, "  notification_off: in_handler = %d " \
	"new_connections_on = %d intr_depth = %d\n", \
	in_handler, new_connections_on, intr_depth); \
    num_notification_off++; \
    if (!in_handler) { \
      if (intr_depth == 0) { \
        intr_disable(); \
      } \
    } \
    /* Can now nest these calls */ \
    /* assert(!in_cs); */ \
    DEBG(MSG_INTR, "  notification_off: setting in_cs = 1\n"); \
    in_cs = 1; \
    entering_cs = 0; \
    DEBG(MSG_INTR, "  notification_off: done setting in_cs = 1\n"); \
    intr_depth++; \
    DEBG(MSG_INTR, "  notification_off: intr_depth = %d\n", intr_depth); \
    DEBG(MSG_INTR, "  notification_off: done\n"); \
  }

#define notification_on() \
  if (options.asynch_mode) { \
    PRINT_TIME(NOFD, &tnow, &tprev, "notification_on: in_handler = %d", \
	in_handler); \
    DEBG(MSG_INTR, "notification_on:  file = %25s line = %6d\n", \
	__FILE__, __LINE__); \
    DEBG(MSG_INTR, "  notification_on: in_handler = %d " \
	"new_connections_on = %d intr_depth = %d\n", \
	in_handler, new_connections_on, intr_depth); \
    num_notification_on++; \
    /* Can now nest these calls */ \
    /* assert(in_cs); */ \
    DEBG(MSG_INTR, "  notification_on: setting in_cs = 0\n"); \
    in_cs = 0; \
    intr_depth--; \
    assert(intr_depth >= 0); \
    DEBG(MSG_INTR, "  notification_on: intr_depth = %d\n", intr_depth); \
    DEBG(MSG_INTR, "  notification_on: done setting in_cs = 0\n"); \
    if (!in_handler) { \
      if (intr_depth == 0) { \
        intr_enable(); \
      } \
    } \
    DEBG(MSG_INTR, "  notification_on:  done\n"); \
  }

#endif /* CNTL_INTR_H */
