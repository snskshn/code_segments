#
#    userver -- (pronounced you-server or micro-server).
#    This file is part of the userver, a high-performance web server designed for
#    performance experiments.
#          
#    This file is Copyright (C) 2010  Tim Brecht
#
#    Authors: Tim Brecht <brecht@cs.uwaterloo.ca>
#    See AUTHORS file for list of contributors to the project.
#  
#    This program is free software; you can redistribute it and/or
#    modify it under the terms of the GNU General Public License as
#    published by the Free Software Foundation; either version 2 of the
#    License, or (at your option) any later version.
#  
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#    General Public License for more details.
#  
#    You should have received a copy of the GNU General Public License
#    along with this program; if not, write to the Free Software
#    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
#    02111-1307 USA
#


# genfiles: generates a bunch of files that 
#    can be used for testing
# genlogs: generates a bunch of log files that 
#    can be used for testing (used by httperf)

CFLAGS = -Wall -Werror -O2

PGMS = genfiles genlogs genspec99sesslogs gen-coax

all: $(PGMS)

generate: 
	genfiles
	genlogs

genfiles: genfiles.c gen.h
	gcc $(CFLAGS) genfiles.c -o genfiles

genlogs: genlogs.c gen.h
	gcc $(CFLAGS) genlogs.c -o genlogs

genspec99sesslogs: genspec99sesslogs.c
	gcc $(CFLAGS) -g  genspec99sesslogs.c -o genspec99sesslogs -lm

gen-coax: gen-coax.c
	gcc $(CFLAGS) gen-coax.c -o gen-coax

clean:
	/bin/rm -rf $(PGMS)

vclean: clean
	/bin/rm -rf testdocs logfile-* requests-*
	-cd http_log_gen; make clean
