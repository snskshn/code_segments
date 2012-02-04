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

#ifndef MYASSERT_H
#define MYASSERT_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

extern void print_all();
extern void info_print_status();
extern int dump_core_on_assert;

/* I've had problems with assert in some cases so  */
/* if there is a problem we can use myassert */
#define ASSERT_BUG

/* Define this for a version of exit that gives more info */
/* #define USE_MY_EXIT */

#define STRING(param) #param

#ifdef ASSERT_BUG

#ifndef ASSERT_ON
#define assert(x) \
  do { \
	  ; \
  } while (0);
#else
#define assert myassert
#endif /* ASSERT_ON */

#else
/* No assert bug */
/* #define ASSERT assert */
#endif /* ASSERT_BUG */

#ifdef USE_MY_EXIT
#define exit(value) \
  do { \
    printf("Calling exit value = %d in file %s on line %d\n", \
	(value), __FILE__, __LINE__); \
    fprintf(stderr, "Calling exit value = %d in file %s " \
	"on line %d\n", (value), __FILE__, __LINE__); \
    fflush(stdout); \
    fflush(stderr); \
    if (value != 0) { \
      print_all(); \
      stop_pgm(); \
    } else { \
      fflush(stdout); \
      fflush(stderr); \
      print_all(); \
      _exit(0); \
    } \
  } while (0);
#endif /* USE_MY_EXIT */

#define myassert(expr) \
  do { \
    if (!(expr)) { \
      printf("My Assertion failed: %s in file %s on line %d\n", \
	  STRING(expr), __FILE__, __LINE__); \
      fprintf(stderr, "My Assertion failed: %s in file %s on line %d\n", \
	  STRING(expr), __FILE__, __LINE__); \
      fflush(stdout); \
      fflush(stderr); \
      should_i_core_dump(); \
      info_print_status(); \
      print_all(); \
      stop_pgm(); \
      exit(100); \
    } \
  } while (0);

#define should_i_core_dump() \
  do { \
    if (dump_core_on_assert) { \
      printf("Trying to dump core\n"); \
      fprintf(stderr, "Trying to dump core\n"); \
      dump_core_sigabort() \
    } \
  } while (0);

#define dump_core_sigabort() \
  do { \
    kill(getpid(), SIGABRT); \
  } while (0);

#define dump_core_segv() \
  do { \
    char x; \
    char *y = 0; \
    printf("dump_core_segv\n"); \
    fflush(stdout); \
    x = (char) *y; \
    printf("char = %c\n", x); \
    exit(200); \
  } while (0);

#define dump_core_div() \
  do { \
    double x = 0.0; \
    double y = 2.5; \
    printf("dump_core_div\n"); \
    fflush(stdout); \
    x = y / x; \
    printf("x = %f\n", x); \
    exit(300); \
  } while (0);

/* Could do raise(SIGKILL);  */
/* printf("stop_pgm: calling raise(SIGKILL)\n"); */

#define stop_pgm() \
  do { \
    printf("stop_pgm: calling exit\n"); \
    fflush(stdout); \
    exit(400); \
  } while (0);


#endif /* MYASSERT_H */
