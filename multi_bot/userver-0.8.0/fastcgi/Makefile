#
#    userver -- (pronounced you-server or micro-server).
#    This file is part of the userver, a high-performance web server designed for
#    performance experiments.
#          
#    Copyright (C) 2010 Tim Brecht
#    Original Copyright (C) 2004  Craig Barkhouse
#
#    Authors: Craig Barkhouse <cabarkho@uwaterloo.ca>
#             Tim Brecht <brecht@cs.uwaterloo.ca>
#              
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

CC = gcc

# For Solaris
# MAKE = gmake
# EXTRA_LIBS = -lsocket -lnsl
#
EXTRA_LIBS = 

CFLAGS = -g -O2

FCGI_PERL = FCGI-0.67
FCGI_PERL_MOD = $(FCGI_PERL)/FCGI.pm

FCGI = fcgi-2.4.0
FCGI_INC = $(FCGI)/include
FCGI_LIB = $(FCGI)/libfcgi/.libs/libfcgi.a

INSTALL_DIR = /usr/local/bin

# Choose which patch to use
# PATCH = fcgi-2.4.0-sharedmem.patch
PATCH = fcgi-2.4.0-passfd.patch


all: $(FCGI_PERL_MOD) specweb99-fcgi test-fcgi

install: all
	cd $(FCGI_PERL) && make install
	cp fcgiserv $(INSTALL_DIR)
	chmod 755 $(INSTALL_DIR)/fcgiserv
	cp specweb99-fcgi $(INSTALL_DIR)
	chmod 755 $(INSTALL_DIR)/specweb99-fcgi
	cp specweb99-fcgi.pl $(INSTALL_DIR)
	chmod 755 $(INSTALL_DIR)/specweb99-fcgi.pl
	cp test-fcgi $(INSTALL_DIR)
	chmod 755 $(INSTALL_DIR)/test-fcgi
	cp test-fcgi.pl $(INSTALL_DIR)
	chmod 755 $(INSTALL_DIR)/test-fcgi.pl

clean:
	rm -rf $(FCGI_PERL)
	rm -rf $(FCGI)
	rm -f specweb99-fcgi test-fcgi

vclean: clean
	


$(FCGI_PERL_MOD): $(FCGI_PERL).tar.gz $(PATCH)
	rm -rf $(FCGI_PERL)
	gunzip -c $(FCGI_PERL).tar.gz | tar xf -
#	cp passfd.c $(FCGI_PERL)
#	cp passfd.h $(FCGI_PERL)
	cd $(FCGI_PERL) && patch -p2 < ../$(PATCH)
	cd $(FCGI_PERL) && ./configure && perl Makefile.PL && make

$(FCGI_LIB): $(FCGI).tar.gz $(PATCH)
	rm -rf $(FCGI)
	gunzip -c $(FCGI).tar.gz | tar xf -
#	cp passfd.c $(FCGI)
#	cp passfd.h $(FCGI)
	cd $(FCGI) && patch -p1 < ../$(PATCH)
	cd $(FCGI) && ./configure && make

specweb99-fcgi: specweb99-fcgi.c $(FCGI_LIB)
	$(CC) $(CFLAGS) -I$(FCGI_INC) -o $@ specweb99-fcgi.c $(FCGI_LIB) $(EXTRA_LIBS)

test-fcgi: test-fcgi.c $(FCGI_LIB)
	$(CC) $(CFLAGS) -I$(FCGI_INC) -o $@ test-fcgi.c $(FCGI_LIB) $(EXTRA_LIBS)
