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


#ifndef LOCAL_DEBUG_H
#define LOCAL_DEBUG_H


#ifdef LOCAL_DEBUG
/* enable local debugging */
#define ldbg(...) \
  do {\
    printf(__VA_ARGS__); \
    fflush(stdout); \
    fflush(stderr); \
  } while (0)
#define lhd(...) \
  do {\
    hexdump(__VA_ARGS__); \
    fflush(stdout); \
    fflush(stderr); \
  } while (0)

#else
/* disable local debugging */
#define ldbg(...)
#define lhd(...)

#endif


#endif /* LOCAL_DEBUG_H */
