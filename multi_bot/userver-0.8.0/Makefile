#
#    userver -- (pronounced you-server or micro-server).
#    This file is part of the userver, a high-performance web server designed for
#    performance experiments.
#          
#    This file is Copyright (C) 2004-2010  Tim Brecht
#    Based on the file originally Copyright (C) 2004  Hewlett-Packard Company
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


include Makefile.base

MAKE-GPROF      = Makefile.gprof
MAKE-NOGPROF    = Makefile.nogprof
MAKE-SEND       = Makefile.send
MAKE-SEND-GPROF = Makefile.send-gprof
BIN             = $(HOME)/bin

PGMS =  userver userver-gprof userver-send

cur: docs userver
# cur: userver-send docs

all: userver docs man

fresh:
	make clean
	make depend
	make
		
remake: FORCE
	$(MAKE) clean
	$(MAKE) depend
	$(MAKE)

man: userver.txt userver.ps userver.pdf userver.html

userver.txt: userver.1
	groff -man -Tascii userver.1 > userver.txt

userver.ps: userver.1
	groff -man -Tps userver.1 > userver.ps

userver.pdf: userver.ps
	ps2pdf userver.ps userver.pdf

userver.html: userver.1
	man2html userver.1 > userver.html

tarball:
	make clean
	tar cf userver.tar fastcgi include getopt *.c *.h Makefile*
	gzip userver.tar

indent:
	indent -kr -psl -pmt -i2 -hnl -ci2 -c2 -cp8 --no-tabs -ts2 -l78 *.c *.h
# -kr     (Kernighan and Ritchie style)
# -psl    Put the type of a procedure on the line before its name.
# -pmt    Preserve access and modification times
# -i2     Indent n spaces
# -hnl    Prefer to break long lines at the position of newlines in the input.
# -ci2    Continuation indent of n spaces.
# -c2     Put comments to the right of code in column n.
# -cp8    Put comments to the right of #else and #endif statements in column n.
# --no-tabs  Use spaces instead of tabs
# -ts2    Set tab size to n spaces.
# -l78    Set maximum line length for non-comment lines to n.

params: FORCE
	echo OS = $(OS)
	echo OSALT = $(OSALT)
	echo HOSTARC = $(HOSTARCH)

userver-gprof: FORCE
	$(MAKE) -f $(MAKE-GPROF) userver-gprof

userver-send: FORCE
	$(MAKE) -f $(MAKE-SEND) userver-send

userver-send-gprof: FORCE
	$(MAKE) -f $(MAKE-SEND-GPROF) userver-send-gprof

predepend:
	touch .depend

$(FCGI_LIB): fastcgi/$(FCGI).tar.gz fastcgi/$(FCGI).patch
	cd fastcgi && rm -rf $(FCGI)
	cd fastcgi && gunzip -c $(FCGI).tar.gz | tar xf -
	cd fastcgi/$(FCGI) && patch -p1 < ../$(FCGI).patch
	cd fastcgi/$(FCGI) && ./configure && make

depend: predepend $(FCGI_LIB) userver-depend

userver-depend: predepend $(FCGI_LIB) FORCE
	$(MAKE) -f $(MAKE-NOGPROF) userver-depend

userver-send-depend: $(FCGI_LIB) FORCE
	$(MAKE) -f $(MAKE-SEND) userver-send-depend

userver: $(FCGI_LIB) FORCE
	$(MAKE) -f $(MAKE-NOGPROF) userver

regression: testall test-1.0 testspecial

test: userver test-1.0 testall testspecial testcache testencrypt
testall: test1 test2 test3 test4 test5 test6 test7 test8 test9 test10 test11

test-without-sendfile: test-1.0 testsome testspecial testcache
testsome: test1 test2 test7 test8 test9 test10 test11


OPTS = -D 0x0ffffffff

# NOTE: because the client and server are both running on
# the same machine we have to leave a bit of extra time for
# the server to start before starting the client and a bit
# of extra time for the server to finish dumping stats, events,
# etc. after it has been killed and before starting the next
# test (otherwise the server can't bind the ip addr / port combo

test1: FORCE
	@echo "Running Basic Test Subset"
	@echo "Running test 1 - should get 100 replies and 0 errors"
	$(CSH) -c "./userver $(OPTS) >& test1.out &"
	sleep 4
	$(CSH) -c "./scripts/run 100 >& test1-httperf.out"
	egrep 'Reply status:|Errors:' test1-httperf.out
	sleep 2
	@grep ' 2xx=100 ' test1-httperf.out
	@./scripts/kill-server
	sleep 4
	@echo ""

test2:  FORCE
	@echo "Running test 2 - should get 100 replies and 0 errors"
	$(CSH) -c "./userver -C $(OPTS) >& test2.out &"
	sleep 4
	$(CSH) -c "./scripts/run 100 >& test2-httperf.out"
	egrep 'Reply status:|Errors:' test2-httperf.out
	sleep 2
	@grep ' 2xx=100 ' test2-httperf.out
	@./scripts/kill-server
	sleep 4
	@echo ""

test3:  FORCE
	@echo "Running test 3 - should get 100 replies and 0 errors"
	$(CSH) -c "./userver -C --use-sendfile --use-tcp-cork $(OPTS) >& test3.out &"
	sleep 4
	$(CSH) -c "./scripts/run 100 >& test3-httperf.out"
	egrep 'Reply status:|Errors:' test3-httperf.out
	@grep ' 2xx=100 ' test3-httperf.out
	@./scripts/kill-server
	sleep 4
	@echo ""

test4:  FORCE
	@echo "Running test 4 - should get 5000 replies and 0 errors in about 10 seconds"
	$(CSH) -c "./userver -C --use-sendfile --use-tcp-cork -c 450 -f 1000 $(OPTS) >& test4.out &"
	sleep 4
	$(CSH) -c "./scripts/runrate 500 5000 >& test4-httperf.out"
	egrep 'Reply status:|Errors:' test4-httperf.out
	@grep ' 2xx=5000 ' test4-httperf.out
	@./scripts/kill-server
	sleep 4
	@echo ""

test5:  FORCE
	@echo "Running test 5 - should get 1000 replies and 0 errors in about 10 seconds"
	$(CSH) -c "./userver -C --use-sendfile --use-tcp-cork -m 0 -e 10 -c 450 -f 1000 $(OPTS) >& test5.out &"
	sleep 4
	$(CSH) -c "./scripts/runrate 100 1000 >& test5-httperf.out"
	egrep 'Reply status:|Errors:' test5-httperf.out
	@grep ' 2xx=1000 ' test5-httperf.out
	@./scripts/kill-server
	sleep 4
	@echo ""

test6:  FORCE
	@echo "Running test 6 - should get 4000 replies and 0 errors in about 10 seconds"
	$(CSH) -c "./userver -C --use-sendfile --use-tcp-cork -m 0 -c 450 -f 1000 --use-poll $(OPTS) >& test6.out &"
	sleep 4
	$(CSH) -c "./scripts/runrate 400 4000 >& test6-httperf.out"
	egrep 'Reply status:|Errors:' test6-httperf.out
	@grep ' 2xx=4000 ' test6-httperf.out
	@./scripts/kill-server
	sleep 4
	@echo ""

test7:  FORCE
	@echo "Running IP Address Subset"
	@echo "Running test 7 - should get 100 replies and 0 errors"
	$(CSH) -c "./userver --ip-addr 127.0.0.1 $(OPTS) >& test7.out &"
	sleep 8
	$(CSH) -c "./scripts/run 100 >& test7-httperf.out"
	egrep 'Reply status:|Errors:' test7-httperf.out
	@grep ' 2xx=100 ' test7-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 8
	@echo ""

test8:  FORCE
	@echo "Running test 8 - should get 100 replies and 0 errors"
	$(CSH) -c "./userver --ip-addr 0.0.0.0 $(OPTS) >& test8.out &"
	sleep 8
	$(CSH) -c "./scripts/run 100 >& test8-httperf.out"
	egrep 'Reply status:|Errors:' test8-httperf.out
	@grep ' 2xx=100 ' test8-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 10
	@echo ""

test9:  FORCE
	@echo "Running test 9 - should get 10 replies and 0 errors"
	$(CSH) -c "./userver --ip-addr 127.0.0.1:6802 $(OPTS) >& test9.out &"
	sleep 8
	$(CSH) -c "./scripts/runaddrport 127.0.0.1 6802 >& test9-httperf.out"
	egrep 'Reply status:|Errors:' test9-httperf.out
	@grep ' 2xx=10 ' test9-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 8
	@echo ""

test10:  FORCE
	@echo "Running test 10 - should get 1000 replies and 0 errors"
	$(CSH) -c "./userver $(OPTS) -C -c 10 -f 500 >& test10.out &"
	sleep 8
	$(CSH) -c "./scripts/runrate 100 1000 >& test10-httperf.out"
	egrep 'Reply status:|Errors:' test10-httperf.out
	@grep ' 2xx=1000 ' test10-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 8
	@echo ""

test11:  FORCE
	@echo "Running test 11 - should get 2000 replies and 0 errors"
	$(CSH) -c "./userver $(OPTS) -C -c 100 -f 1000 >& test11.out &"
	sleep 8
	$(CSH) -c "./scripts/runrate 200 2000 >& test11-httperf.out"
	egrep 'Reply status:|Errors:' test11-httperf.out
	@grep ' 2xx=2000 ' test11-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 8
	@echo ""

testhttperf: testhttperf1 testhttperf2 testhttperf3

testhttperf1: FORCE
	@echo "Running test 1"
	$(CSH) -c "./userver $(OPTS) --stats-interval 1 -C -m 1 -c 1 -f 10 --listenq 0 >& test1.out &"
	sleep 8
	$(CSH) -c "./scripts/runrate 1000 10000 >& test1-httperf.out"
	egrep 'Reply status:|Errors:' test1-httperf.out
	@grep ' 2xx=10000 ' test1-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 8
	@echo ""


testhttperf2: FORCE
	@echo "Running test 2"
	$(CSH) -c "./userver $(OPTS) --stats-interval 1 -C -m 1 -c 10 -f 20 --listenq 0 >& test2.out &"
	sleep 8
	$(CSH) -c "./scripts/runrate 1000 10000 >& test2-httperf.out"
	egrep 'Reply status:|Errors:' test2-httperf.out
	@grep ' 2xx=10000 ' test2-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 8
	@echo ""

testhttperf3: FORCE
	@echo "Running test 3"
	$(CSH) -c "./userver $(OPTS) --stats-interval 1 -C -m 0 -c 20 -f 40 --listenq 128 >& test3.out &"
	sleep 8
	$(CSH) -c "./scripts/runrate 3000 60000 >& test3-httperf.out"
	egrep 'Reply status:|Errors:' test3-httperf.out
	@grep ' 2xx=60000 ' test3-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 8
	@echo ""


testidle: testidle1 testidle2 testidle3 testidle4

testidle1:
	@echo "Running test 1 - should get 1 replies and 0 errors"
	$(CSH) -c "$(USERVER) -c 2 -f 30 -m 1 $(OPTS) >& test1.out &"
	sleep 4
	$(CSH) -c "deadconns $(SERVER) 6800 10 >& test1-deadconns.out"
	$(CSH) -c "./scripts/runaddr $(SERVER) 1 >& test1-httperf.out"
	@egrep 'Reply status:|Errors:' test1-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

test-1.0: testnew1 testnew2 testnew3 testnew4

testnew1:
	@echo "Running test 1 - should get 1 replies and 0 errors"
	$(CSH) -c "$(USERVER) -c 2 -f 30 -m 1 $(OPTS) >& test1.out &"
	sleep 4
	$(CSH) -c "./scripts/runaddr-1.0 $(SERVER) 1 >& test1-httperf.out"
	@egrep 'Reply status:|Errors:' test1-httperf.out
	@grep ' 2xx=1 ' test1-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testnew2:
	@echo "Running test 2 - should get 10 replies and 0 errors"
	$(CSH) -c "$(USERVER) -c 2 -f 30 -m 1 $(OPTS) >& test2.out &"
	sleep 4
	$(CSH) -c "./scripts/runaddr-1.0 $(SERVER) 10 >& test2-httperf.out"
	@egrep 'Reply status:|Errors:' test2-httperf.out
	@grep ' 2xx=10 ' test2-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testnew3:
	@echo "Running test 3 - should get 100 replies and 0 errors"
	$(CSH) -c "$(USERVER) -c 2 -f 30 -m 1 -C $(OPTS) >& test3.out &"
	sleep 4
	$(CSH) -c "./scripts/runaddr-1.0 $(SERVER) 100 >& test3-httperf.out"
	@egrep 'Reply status:|Errors:' test3-httperf.out
	@grep ' 2xx=100 ' test3-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testnew4:
	@echo "Running test 4 - should get 1000 replies and 0 errors"
	$(CSH) -c "$(USERVER) -c 2 -f 30 -m 1 -C $(OPTS) >& test4.out &"
	sleep 4
	$(CSH) -c "./scripts/runaddr-1.0 $(SERVER) 1000 >& test4-httperf.out"
	@egrep 'Reply status:|Errors:' test4-httperf.out
	@grep ' 2xx=1000 ' test4-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

SPECIAL_OPTS = -D 0x0

testspecial: testspecial1 testspecial2 testspecial3 testspecial4 \
             testspecial5 testspecial6 testspecial7 testspecial8 \
             testspecial9 testspecial10

testspecial1:
	@echo "Running test 1 - should get 1 Not found (4xx=1) and 0 errors"
	$(CSH) -c "$(USERVER) -c 2 -f 30 -m 1 -C $(SPECIAL_OPTS) >& test1.out &"
	sleep 4
	$(CSH) -c "./scripts/runaddruri $(SERVER) BOGUS.txt 1 >& test1-httperf.out"
	@egrep 'Reply status:|Errors:' test1-httperf.out
	@grep ' 4xx=1 ' test1-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testspecial2:
	@echo "Running test 2 - should get 1 Not found (4xx=1) and 0 errors"
	$(CSH) -c "$(USERVER) -c 2 -f 30 -m 1 -C $(SPECIAL_OPTS) >& test2.out &"
	sleep 4
	$(CSH) -c "./scripts/runaddruri-1.0 $(SERVER) BOGUS.txt 1 >& test2-httperf.out"
	@egrep 'Reply status:|Errors:' test2-httperf.out
	@grep ' 4xx=1 ' test2-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testspecial3:
	@echo "Running test 3 - should get 10 Not found (4xx=10) and 0 errors"
	$(CSH) -c "$(USERVER) -c 2 -f 30 -m 1 -C $(SPECIAL_OPTS) >& test3.out &"
	sleep 4
	$(CSH) -c "./scripts/runaddruri $(SERVER) BOGUS.txt 10 >& test3-httperf.out"
	@egrep 'Reply status:|Errors:' test3-httperf.out
	@grep ' 4xx=10 ' test3-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testspecial4:
	@echo "Running test 4 - should get 10 Not found (4xx=10) and 0 errors"
	$(CSH) -c "$(USERVER) -c 2 -f 30 -m 1 -C $(SPECIAL_OPTS) >& test4.out &"
	sleep 4
	$(CSH) -c "./scripts/runaddruri-1.0 $(SERVER) BOGUS.txt 10 >& test4-httperf.out"
	@egrep 'Reply status:|Errors:' test4-httperf.out
	@grep ' 4xx=10 ' test4-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testspecial5:
	@echo "Running test 5 - should get 1 reply and 0 errors"
	$(CSH) -c "$(USERVER) -c 2 -f 30 -m 1 -C $(SPECIAL_OPTS) >& test5.out &"
	sleep 4
	$(CSH) -c "./scripts/runaddrhead $(SERVER) 1 >& test5-httperf.out"
	@egrep 'Reply status:|Errors:' test5-httperf.out
	@grep ' 2xx=1 ' test5-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testspecial6:
	@echo "Running test 6 - should get 1 reply and 0 errors"
	$(CSH) -c "$(USERVER) -c 2 -f 30 -m 1 -C $(SPECIAL_OPTS) >& test6.out &"
	sleep 4
	$(CSH) -c "./scripts/runaddrhead-1.0 $(SERVER) 1 >& test6-httperf.out"
	@egrep 'Reply status:|Errors:' test6-httperf.out
	@grep ' 2xx=1 ' test6-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testspecial7:
	@echo "Running test 7 - should get 10 replies and 0 errors"
	$(CSH) -c "$(USERVER) -c 2 -f 30 -m 1 -C $(SPECIAL_OPTS) >& test7.out &"
	sleep 4
	$(CSH) -c "./scripts/runaddrhead $(SERVER) 10 >& test7-httperf.out"
	@egrep 'Reply status:|Errors:' test7-httperf.out
	@grep ' 2xx=10 ' test7-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testspecial8:
	@echo "Running test 8 - should get 10 replies and 0 errors"
	$(CSH) -c "$(USERVER) -c 2 -f 30 -m 1 -C $(SPECIAL_OPTS) >& test8.out &"
	sleep 4
	$(CSH) -c "./scripts/runaddrhead-1.0 $(SERVER) 10 >& test8-httperf.out"
	@egrep 'Reply status:|Errors:' test8-httperf.out
	@grep ' 2xx=10 ' test8-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testspecial9:
	@echo "Running test 9 - should get 10 replies and 0 errors"
	$(CSH) -c "$(USERVER) -c 2 -f 30 -m 1 $(SPECIAL_OPTS) >& test9.out &"
	sleep 4
	$(CSH) -c "./scripts/runaddrhead $(SERVER) 10 >& test9-httperf.out"
	@egrep 'Reply status:|Errors:' test9-httperf.out
	@grep ' 2xx=10 ' test9-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testspecial10:
	@echo "Running test 10 - should get 10 replies and 0 errors"
	$(CSH) -c "$(USERVER) -c 2 -f 30 -m 1 $(SPECIAL_OPTS) >& test10.out &"
	sleep 4
	$(CSH) -c "./scripts/runaddrhead-1.0 $(SERVER) 10 >& test10-httperf.out"
	@egrep 'Reply status:|Errors:' test10-httperf.out
	@grep ' 2xx=10 ' test10-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2



sendfile: sf1

sf1: FORCE
	@echo "Running Sendfile"
	@echo "Running test 1 - should get 1 reply and 0 errors"
	$(CSH) -c "./userver $(OPTS) --use-sendfile >& test1.out &"
	sleep 4
	$(CSH) -c "./scripts/runuri docs/100K.txt >& test1-httperf.out"
	egrep 'Reply status:|Errors:' test1-httperf.out
	sleep 2
	@grep ' 2xx=1 ' test1-httperf.out
	@grep ' content 102400.0 ' test1-httperf.out
	@./scripts/kill-server
	sleep 5
	@grep '^socket send reply failed            0' test1.out
	@echo ""


EPOLL_BASE = -D 0xffffffff --cache-table-print --stats-interval 1  --epoll-modify-listeners
# EPOLL_BASE = -D 0xffffffff --cache-table-print --stats-interval 1

testepoll: testepoll1 testepoll2 testepoll3 testepoll4 testepoll5 testepoll6 testepoll7

testepoll1:
	@echo "Running test 1 - should get 10 replies and 0 errors"
	$(CSH) -c "$(USERVER) -c 2 -f 30 -m 1 -C --use-epoll $(EPOLL_BASE) >& test1.out &"
	sleep 4
	$(CSH) -c "./scripts/runaddr $(SERVER) 10 >& test1-httperf.out"
	@egrep 'Reply status:|Errors:' test1-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testepoll2:
	@echo "Running test 2 - should get 1000 replies and 0 errors"
	$(CSH) -c "$(USERVER) -c 200 -f 300 -m 0 -C --use-epoll $(EPOLL_BASE) >& test2.out &"
	sleep 4
	$(CSH) -c "./scripts/runaddrrate $(SERVER) 100 1000 >& test2-httperf.out"
	@egrep 'Reply status:|Errors:' test2-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testepoll3:
	@echo "Running test 3 - should get 1000 replies and 0 errors"
	$(CSH) -c "$(USERVER) -c 200 -f 300 -m 0 -C --use-epoll2 $(EPOLL_BASE) >& test3.out &"
	sleep 4
	$(CSH) -c "./scripts/runaddrrate $(SERVER) 100 1000 >& test3-httperf.out"
	@egrep 'Reply status:|Errors:' test3-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testepoll4:
	@echo "Running test 4 - should get 15000 replies and 0 errors"
	$(CSH) -c "$(USERVER) -c 500 -f 1000 -m 0 -C --use-epoll2 $(EPOLL_BASE) >& test4.out &"
	sleep 4
	$(CSH) -c "./scripts/runaddrrate $(SERVER) 500 15000 >& test4-httperf.out"
	@egrep 'Reply status:|Errors:' test4-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testepoll5:
	@echo "Running test 5 - should get 25000 replies and 0 errors"
	$(CSH) -c "$(USERVER) -c 500 -f 1000 -m 0 -C --use-sendfile --use-tcp-cork --use-epoll $(EPOLL_BASE) >& test5.out &"
	sleep 4
	$(CSH) -c "./scripts/runaddrrate $(SERVER) 500 25000 >& test5-httperf.out"
	@egrep 'Reply status:|Errors:' test5-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testepoll6:
	@echo "Running test 6 - should get 25000 replies and 0 errors"
	$(CSH) -c "$(USERVER) -c 500 -f 1000 -m 0 -C --use-sendfile --use-tcp-cork --use-epoll2 $(EPOLL_BASE) >& test6.out &"
	sleep 4
	$(CSH) -c "./scripts/runaddrrate $(SERVER) 500 25000 >& test6-httperf.out"
	@egrep 'Reply status:|Errors:' test6-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testepoll7:
	@echo "Running test 7 - should get 25000 replies and 0 errors"
	$(CSH) -c "$(USERVER) -p 4 -c 100 -f 1000 -m 0 -C --use-sendfile --use-tcp-cork --use-epoll2 $(EPOLL_BASE) >& test7.out &"
	sleep 4
	$(CSH) -c "./scripts/runaddrrate $(SERVER) 500 25000 >& test7-httperf.out"
	@egrep 'Reply status:|Errors:' test7-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testepoll8:
	@echo "Running test 8 - should get 250 replies and 0 errors"
	$(CSH) -c "$(USERVER) -p 2 -c 100 -f 1000 -m 0 -C --use-sendfile --use-tcp-cork --use-epoll2 $(EPOLL_BASE) >& test8.out &"
	sleep 4
	$(CSH) -c "./scripts/runaddrrate $(SERVER) 100 10000 >& test8-httperf.out"
	@egrep 'Reply status:|Errors:' test8-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testepollctlv: testepollctlv1 testepollctlv2 testepollctlv3

testepollctlv1:
	@echo "Running test 1 - should get 15000 replies and 0 errors"
	$(CSH) -c "$(USERVER) -c 500 -f 1000 -m 0 -C --use-epoll2 --use-epoll-ctlv $(EPOLL_BASE) >& test1.out &"
	sleep 4
	$(CSH) -c "./scripts/runaddrrate $(SERVER) 500 15000 >& test1-httperf.out"
	@egrep 'Reply status:|Errors:' test1-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testepollctlv2:
	@echo "Running test 2 - should get 35000 replies and 0 errors"
	$(CSH) -c "$(USERVER) -c 500 -f 1000 -m 0 -C --use-sendfile --use-tcp-cork --use-epoll2 --use-epoll-ctlv $(EPOLL_BASE) >& test2.out &"
	sleep 4
	$(CSH) -c "./scripts/runaddrrate $(SERVER) 750 35000 >& test2-httperf.out"
	@egrep 'Reply status:|Errors:' test2-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testepollctlv3:
	@echo "Running test 3 - should get 25000 replies and 0 errors"
	$(CSH) -c "$(USERVER) -c 500 -f 1000 -m 10 -C --use-sendfile --use-tcp-cork --use-epoll2 --use-epoll-ctlv $(EPOLL_BASE) >& test3.out &"
	sleep 4
	$(CSH) -c "./scripts/runaddrrate $(SERVER) 500 25000 >& test3-httperf.out"
	@egrep 'Reply status:|Errors:' test3-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testepollctlv4:
	@echo "Running test 4 - should get 2000 replies and 0 errors"
	$(CSH) -c "$(USERVER) -c 500 -f 1000 -m 0 -C --use-sendfile --use-tcp-cork --use-epoll2 --use-epoll-ctlv $(EPOLL_BASE) >& test4.out &"
	sleep 4
	$(CSH) -c "./scripts/runaddrrate1K $(SERVER) 100 2000 >& test4-httperf.out"
	@egrep 'Reply status:|Errors:' test4-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testet: testet1 testet2 testet3 testet4 testet5 testet6 testet7 testet8 testet9 testet10

testet1:
	@echo "Running test 1 - should get 1 reply and 0 errors"
	$(CSH) -c "$(USERVER) -c 2 -f 30 -m 1 -C --use-epoll2 --epoll-edge-triggered $(EPOLL_BASE) >& test1.out &"
	sleep 4
	$(CSH) -c "./scripts/runaddr $(SERVER) 1 >& test1-httperf.out"
	@egrep 'Reply status:|Errors:' test1-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testet2:
	@echo "Running test 2 - should get 10 replies and 0 errors"
	$(CSH) -c "$(USERVER) -c 2 -f 30 -m 1 -C --use-epoll2 --epoll-edge-triggered $(EPOLL_BASE) >& test2.out &"
	sleep 4
	$(CSH) -c "./scripts/runaddr $(SERVER) 10 >& test2-httperf.out"
	@egrep 'Reply status:|Errors:' test2-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testet3:
	@echo "Running test 3 - should get 10 replies and 0 errors"
	$(CSH) -c "$(USERVER) -c 2 -f 30 -m 1 -C --use-epoll2 --epoll-edge-triggered $(EPOLL_BASE) >& test3.out &"
	sleep 4
	$(CSH) -c "./scripts/runaddruri $(SERVER) docs/100K.txt 10 >& test3-httperf.out"
	@egrep 'Reply status:|Errors:' test3-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testet4:
	@echo "Running test 4 - should get 10 replies and 0 errors"
	$(CSH) -c "$(USERVER) -c 2 -f 30 -m 0 -C --use-epoll2 --epoll-edge-triggered $(EPOLL_BASE) >& test4.out &"
	sleep 4
	$(CSH) -c "./scripts/runaddruri $(SERVER) docs/100K.txt 10 >& test4-httperf.out"
	@egrep 'Reply status:|Errors:' test4-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testet5:
	@echo "Running test 5 - should get 10000 replies and 0 errors"
	$(CSH) -c "$(USERVER) --cache-table-size 4 -c 2 -f 15 -m 0 -C --use-sendfile --use-epoll2 --epoll-edge-triggered $(EPOLL_BASE) >& test5.out &"
	sleep 4
	$(CSH) -c "./scripts/runaddrrateuri $(SERVER) 400 10000 docs/10B.txt >& test5-httperf.out"
	@egrep 'Reply status:|Errors:' test5-httperf.out
	sleep 10
	@./scripts/kill-server
	sleep 2


testet6:
	@echo "Running test 6 - should get 1000 replies (twice) and 0 errors"
	$(CSH) -c "$(USERVER) -c 200 -f 300 -m 0 -C --use-epoll2 --epoll-edge-triggered $(EPOLL_BASE) >& test6.out &"
	sleep 4
	$(CSH) -c "./scripts/runaddrpersist $(SERVER) 1000 >& test6-httperf.out"
	$(CSH) -c "./scripts/runaddrpersist $(SERVER) 1000 >>& test6-httperf.out"
	@egrep 'Reply status:|Errors:' test6-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testet7:
	@echo "Running test 7 - should get ???? replies and ? errors"
	$(CSH) -c "$(USERVER) -c 200 -f 300 -m 0 -C --use-epoll2 --epoll-edge-triggered $(EPOLL_BASE) >& test7.out &"
	sleep 4
	$(CSH) -c "./scripts/runaddrsesslog $(SERVER) 200 2000 scripts/httpspec99-d-1.log >& test7-httperf.out"
	@egrep 'Reply status:|Errors:' test7-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testet8:
	@echo "Running test 8 - should get ???? replies and ? errors"
	$(CSH) -c "$(USERVER) -c 200 -f 300 -m 0 -C --use-sendfile --use-tcp-cork --use-epoll2 --epoll-edge-triggered $(EPOLL_BASE) >& test8.out &"
	sleep 4
	$(CSH) -c "./scripts/runaddrsesslog $(SERVER) 300 3000 scripts/httpspec99-d-1.log >& test8-httperf.out"
	@egrep 'Reply status:|Errors:' test8-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testet9:
	@echo "Running test 9 - should get ??? replies and ? errrors"
	$(CSH) -c "$(USERVER) -c 2 -f 10 -m 0 -C --use-epoll2 --epoll-edge-triggered $(EPOLL_BASE) >& test9.out &"
	sleep 4
	$(CSH) -c "./scripts/runaddrrateuri $(SERVER) 300 3000 docs/1K.txt >& test9-httperf.out"
	@egrep 'Reply status:|Errors:' test9-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testet10:
	@echo "Running test 10 - should get 10000 replies and 0 errors"
	$(CSH) -c "$(USERVER) --cache-table-size 4 -p 2 -c 1 -f 15 -m 0 -C --use-sendfile --epoll-good --cache-table-print --stats-interval 1 >& test10.out &"
	sleep 4
	$(CSH) -c "./scripts/runaddrrateuri $(SERVER) 600 10000 docs/10B.txt >& test10-httperf.out"
	@egrep 'Reply status:|Errors:' test10-httperf.out
	sleep 10
	@./scripts/kill-server
	sleep 2

DYN_BASE = -D 0xffffffff
#Assumes application launched as '$src/fastcgi/specweb99-fcgi.pl :9000" (port 9000)
#The topdir variable in that fastcgi script must be modified so that upfgen99 and cadgen99
#can be found
DYN_FCGI  = --app=/specweb99-fcgi.pl,FASTCGI,:9000,1 --read-buffer-size 1000000
DYN_MOD   = --app=/specweb99-fcgi.pl,specweb99
USE_SENDFILE = --use-sendfile --use-tcp-cork
UNIX_DOMAIN_SOCK = /tmp/fastunixdom-brecht
# USE UNIX DOMAIN SOCKET
DYN_FCGI_UDS  = --app=/specweb99-fcgi.pl,FASTCGI,$(UNIX_DOMAIN_SOCK) --read-buffer-size 1000000
#
#	@fastcgi/specweb99-fcgi.pl $(UNIX_DOMAIN_SOCK) >& test1-fcgi.out &
#
DYN_FCGI_SHARED  = -D 0xffffffff --app=/specweb99-fcgi.pl,FASTCGI-sharedmem,:9000,1 --read-buffer-size 1000000

testdyn: testdyn1 testdyn2 testdyn3 testdyn4 testdyn5

testdynlong:
	@echo "Running testdyn 1 - should get 5+1 replies and 0 errors"
	fastcgi/specweb99-fcgi.pl $(UNIX_DOMAIN_SOCK) >& test1-fcgi.out &
	@sleep 2
	$(CSH) -c "$(USERVER) -D 0xffffffff -c 500 -f 1000 -m 0 -C $(USE_SENDFILE) $(DYN_BASE) \
	     $(DYN_FCGI_UDS) >& test1.out &"
	@sleep 4
	$(CSH) -c "./scripts/runspecdynamic-long >& test1-httperf.out"
	@egrep 'Reply status:|Errors:' test1-httperf.out
	sleep 2
	@@./scripts/kill-server
	pkill specweb99
	@sleep 2
	@echo ""

testdyn1:
	@echo "Running testdyn 1 - should get 5+1 replies and 0 errors"
	fastcgi/specweb99-fcgi.pl $(UNIX_DOMAIN_SOCK) >& test1-fcgi.out &
	@sleep 2
	$(CSH) -c "$(USERVER) -c 500 -f 1000 -m 0 -C $(USE_SENDFILE) $(DYN_BASE) \
	     $(DYN_FCGI_UDS) >& test1.out &"
	@sleep 4
	$(CSH) -c "./scripts/runspecdynamic 1 >& test1-httperf.out"
	@egrep 'Reply status:|Errors:' test1-httperf.out
	sleep 2
	@@./scripts/kill-server
	pkill specweb99
	@sleep 2
	@echo ""


testdyn2:
	@echo "Running testdyn 2 - should get 5+1 replies and 0 errors"
	fastcgi/specweb99-fcgi.pl >& test2-fcgi.out &
	@sleep 2
	$(CSH) -c "$(USERVER) -c 500 -f 1000 -m 0 -C $(DYN_BASE) $(DYN_FCGI) $(USE_SENDFILE) >& test2.out &"
	@sleep 4
	$(CSH) -c "./scripts/runspecdynamic 1 >& test2-httperf.out"
	@egrep 'Reply status:|Errors:' test2-httperf.out
	sleep 2
	@@./scripts/kill-server
	pkill specweb99
	@sleep 2
	@echo ""

testdyn3:
	@echo "Running testdyn 3 - should get 25+5 replies and 0 errors"
	fastcgi/specweb99-fcgi.pl >& test3-fcgi.out &
	@sleep 2
	$(CSH) -c "$(USERVER) -c 500 -f 1000 -m 0 -C $(DYN_BASE) $(DYN_FCGI) $(USE_SENDFILE) >& test3.out &"
	sleep 4
	$(CSH) -c "./scripts/runspecdynamic 5 >& test3-httperf.out"
	@egrep 'Reply status:|Errors:' test3-httperf.out
	sleep 2
	@./scripts/kill-server
	pkill specweb99
	sleep 2
	@echo ""

testdyn4:
	@echo "Running testdyn 4 - should get 5+1 replies and 0 errors"
	fastcgi/specweb99-fcgi.pl $(UNIX_DOMAIN_SOCK) >& test4-fcgi.out &
	@sleep 2
	$(CSH) -c "$(USERVER) -c 500 -f 1000 -m 0 -C $(DYN_BASE) $(DYN_MOD) $(USE_SENDFILE) >& test4.out &"
	sleep 4
	$(CSH) -c "./scripts/runspecdynamic 1 >& test4-httperf.out"
	@egrep 'Reply status:|Errors:' test4-httperf.out
	sleep 2
	@./scripts/kill-server
	pkill specweb99
	sleep 2

testdyn5:
	@echo "Running testdyn 5 - should get 25+5 replies and 0 errors"
	fastcgi/specweb99-fcgi.pl $(UNIX_DOMAIN_SOCK) >& test5-fcgi.out &
	@sleep 2
	$(CSH) -c "$(USERVER) -c 500 -f 1000 -m 0 -C $(DYN_BASE) $(DYN_MOD) $(USE_SENDFILE) >& test5.out &"
	sleep 4
	$(CSH) -c "./scripts/runspecdynamic 5 >& test5-httperf.out"
	@egrep 'Reply status:|Errors:' test5-httperf.out
	sleep 2
	@./scripts/kill-server
	pkill specweb99
	sleep 2

testdyn6:
	@echo "Running testdyn 6 - should get 25+5 replies and 0 errors"
	fastcgi/specweb99-fcgi.pl $(UNIX_DOMAIN_SOCK) >& test6-fcgi.out &
	@sleep 2
	$(CSH) -c "$(USERVER) -c 1 -f 1000 -m 0 -C $(DYN_BASE) $(DYN_MOD) $(USE_SENDFILE) --use-epoll >& test6.out &"
	sleep 4
	$(CSH) -c "./scripts/runspecdynamic 5 >& test6-httperf.out"
	@egrep 'Reply status:|Errors:' test6-httperf.out
	sleep 2
	@./scripts/kill-server
	pkill specweb99-fcgi.pl
	sleep 2

testdynshared1:
	@echo "Running testdyn 1 - should get 5+1 replies and 0 errors"
	fastcgi/specweb99-fcgi.pl >& test1-fcgi.out &
	@sleep 5
	$(CSH) -c "$(USERVER) -c 500 -f 1000 -m 0 -C $(USE_SENDFILE) \
	     $(DYN_FCGI_SHARED) >& test1.out &"
	@sleep 4
	$(CSH) -c "./scripts/runspecdynamic 1 >& test1-httperf.out"
	@egrep 'Reply status:|Errors:' test1-httperf.out
	sleep 2
	@@./scripts/kill-server
	pkill specweb99
	@sleep 2


testdyn1c:
	@echo "Running testdyn 1 - should get 5+1 replies and 0 errors"
	fastcgi/specweb99-fcgi $(UNIX_DOMAIN_SOCK) >& test1-fcgi.out &
	@sleep 2
	$(CSH) -c "$(USERVER) -c 500 -f 1000 -m 0 -C $(DYN_BASE) \
	     $(DYN_FCGI_UDS) >& test1.out &"
	@sleep 4
	$(CSH) -c "./scripts/runspecdynamic 1 >& test1-httperf.out"
	@egrep 'Reply status:|Errors:' test1-httperf.out
	sleep 2
	@@./scripts/kill-server
	pkill specweb99
	@sleep 2
	@echo ""

testdyntimeout1:
	@echo "Running testdyn timeout 1 - should get 0 replies and 1 timeout error"
	# $(HOME)/specweb99-fcgi.pl >& test1-fcgi.out &
	./fastcgi/specweb99-fcgi.pl >& test1-fcgi.out &
	@sleep 2
	$(CSH) -c "$(USERVER) -c 500 -f 1000 -m 0 --use-epoll -C $(DYN_BASE) $(DYN_FCGI) >& test1.out &"
	@sleep 4
	$(CSH) -c "./scripts/runspecdynamic-timeout 1 >& test1-httperf.out"
	@egrep 'Reply status:|Errors:' test1-httperf.out
	sleep 15
	$(CSH) -c "./scripts/runspecdynamic-timeout 1 >& test2-httperf.out"
	@egrep 'Reply status:|Errors:' test2-httperf.out
	sleep 3
	@@./scripts/kill-server
	pkill specweb99
	@sleep 2

testdynbufs:
	@echo "Running testdynbufs check if we can allocate lots of dyn buffers"
	@echo "This test probably only works on 64-bit machines"
	./fastcgi/specweb99-fcgi.pl >& test1-fcgi.out &
	@sleep 2
	$(CSH) -c "$(USERVER) -c 500 -f 1000 -m 0 --use-epoll -C $(DYN_BASE) $(DYN_FCGI) --num-dyn-buffers 8000 >& test1.out &"
	@sleep 4
	$(CSH) -c "./scripts/runspecdynamic-timeout 1 >& test1-httperf.out"
	@egrep 'Reply status:|Errors:' test1-httperf.out
	sleep 3
	@@./scripts/kill-server
	pkill specweb99
	@sleep 2


testdynconf: testdynconf1 testdynconf2
testdynconf1:
	@echo "Running testdynconf 1 - should get 5+1 replies and 0 errors"
	fastcgi/specweb99-fcgi.pl $(UNIX_DOMAIN_SOCK) >& test1-fcgi.out &
	@sleep 2
	$(CSH) -c "$(USERVER) -c 500 -f 1000 -m 0 -C $(DYN_BASE) \
	     $(DYN_FCGI_UDS) >& test1.out &"
	@sleep 4
	$(CSH) -c "./scripts/runspecdynamic 1 >& test1-httperf.out"
	@egrep 'Reply status:|Errors:' test1-httperf.out
	sleep 2
	@@./scripts/kill-server
	pkill specweb99
	@sleep 2
	@echo ""

testdynconf2:
	@echo "Running testdyn 2 - should get 5+1 replies and 0 errors"
	fastcgi/specweb99-fcgi $(UNIX_DOMAIN_SOCK) >& test2-fcgi.out &
	@sleep 2
	$(CSH) -c "$(USERVER) -c 500 -f 1000 -m 0 -C $(DYN_BASE) \
	     $(DYN_FCGI_UDS) >& test2.out &"
	@sleep 4
	$(CSH) -c "./scripts/runspecdynamic 1 >& test2-httperf.out"
	@egrep 'Reply status:|Errors:' test2-httperf.out
	sleep 2
	@@./scripts/kill-server
	pkill specweb99
	@sleep 2
	@echo ""



# DYN_FCGITEST  = --app=/test-fcgi.pl,FASTCGI,:8000,1 --read-buffer-size 1000
# testfcgi:
# 	@echo "Running testfcgi - should get 0 errors and should contain the strings 'param1=val1' and 'param2=val2'"
# 	fastcgi/specweb99-fcgi.pl :8000 >& test-fcgi.out &
# 	$(CSH) -c "$(USERVER) -c 500 -f 1000 -m 0 -C $(DYN_BASE) $(DYN_FCGITEST) $(USE_SENDFILE) >& testfcgi.out &"
# 	sleep 4
# 	$(CSH) -c "./scripts/runfcgitest >& testfcgi-httperf.out"
# 	@egrep 'Reply status:|Errors:'  testfcgi-httperf.out
# 	@egrep  'param1=val1$$' testfcgi-httperf.out
# 	@egrep  '[^&]param2=val2$$' testfcgi-httperf.out
# 	sleep 2
# 	@./scripts/kill-server
# 	pkill specweb99
# 	sleep 2




# Note: requires the hacked version of the FastCGI library (a copy is installed on suzuka)
# Start the fastcgi app as ./fastcgi/test-fcgi.pl /tmp/fastunixdom

DYN_FCGITEST_UNIXDOM = --app=/test-fcgi.pl,FASTCGI,/tmp/fastunixdom --read-buffer-size 1000
testfcgiunixdom:
	@echo "Running testfcgiunixdom - should get 0 errors and should contain the strings 'param1=val1' and 'param2=val2'"
	$(CSH) -c "$(USERVER) -c 500 -f 1000 -m 0 -C $(SPEC_BASE) $(DYN_FCGITEST_UNIXDOM) $(USE_SENDFILE) >& testfcgi_unixdom.out &"
	sleep 4
	$(CSH) -c "./scripts/runfcgitest >& testfcgi_unixdom-httperf.out"
	@egrep 'Reply status:|Errors:'  testfcgi_unixdom-httperf.out
	@egrep  'param1=val1$$' testfcgi_unixdom-httperf.out
	@egrep  '[^&]param2=val2$$' testfcgi_unixdom-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2


testspec: testspec1 testspec2 testspec3

testspec1:
	@echo "Running test 1 - should get about 7200 or 7300 replies and 0 errors"
	$(CSH) -c "$(USERVER) -c 500 -f 1000 -m 0 -C --use-sendfile --use-tcp-cork $(SPEC_BASE) >& test1.out &"
	sleep 4
	$(CSH) -c "./scripts/runaddrsesslog $(SERVER) 200 20000 scripts/httpspec99-d-1.log >& test1-httperf.out"
	@egrep 'Reply status:|Errors:' test1-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testspec2:
	@echo "Running test 2 - should get about 7200 or 7300 replies and 0 errors"
	$(CSH) -c "$(USERVER) -c 500 -f 1000 -m 0 -C --use-epoll2 --use-sendfile --use-tcp-cork $(SPEC_BASE) >& test2.out &"
	sleep 4
	$(CSH) -c "./scripts/runaddrsesslog $(SERVER) 200 2000 scripts/httpspec99-d-1.log >& test2-httperf.out"
	@egrep 'Reply status:|Errors:' test2-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testspec3:
	@echo "Running test 3 - should get about 7200 or 7300 replies and 0 errors"
	$(CSH) -c "$(USERVER) -c 500 -f 1000 -m 0 -C --use-epoll2 --use-epoll-ctlv --use-sendfile --use-tcp-cork $(SPEC_BASE) >& test3.out &"
	sleep 4
	$(CSH) -c "./scripts/runaddrsesslog $(SERVER) 200 2000 scripts/httpspec99-d-1.log >& test3-httperf.out"
	@egrep 'Reply status:|Errors:' test3-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testspec4:
	@echo "Running test 4 - should get about 7200 or 7300 replies and 0 errors"
	$(CSH) -c "$(USERVER) -c 500 -f 1000 -m 0 -C --use-epoll --use-epoll-ctlv --use-sendfile --use-tcp-cork $(SPEC_BASE) >& test4.out &"
	sleep 4
	$(CSH) -c "./scripts/runaddrsesslog $(SERVER) 200 2000 scripts/httpspec99-d-1.log >& test4-httperf.out"
	@egrep 'Reply status:|Errors:' test4-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

# Note: we currently skip testcache1 
#      (default httperf versions have a session limit of 1000)
testcache: testcache2 testcache3 testcache4

testcache1:
	@echo "Running test cache 1 - should get 7336 replies and 0 errors"
	$(CSH) -c "$(USERVER) -c 1 -m 1 -C --cache-max-file-size 1000000 --cache-max-bytes 2500000000 --cache-table-size 150 -f 480 >& test1.out&"
	sleep 4
	$(CSH) -c "./scripts/runaddrsesslog $(SERVER) 50 1000 scripts/httpspec99-d-1.log  >& test1-httperf.out"
	@egrep 'Reply status:|Errors:' test1-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 4
	@grep ' 2xx=7336 ' test1-httperf.out
	@grep '^cache requests .* 7336' test1.out
	@grep '^cache hits .* 7300' test1.out
	sleep 2

testcache2:
	@echo "Running test cache 2 - should get 360 replies and 0 errors"
	$(CSH) -c "$(USERVER) -c 1 -m 1 -C --cache-max-file-size 1000000 --cache-max-bytes 2500000000 --cache-table-size 150 -f 480 >& test2.out&"
	sleep 4
	$(CSH) -c "./scripts/runaddrsesslog $(SERVER) 1 10 scripts/httpspec99-test-cache.log  >& test2-httperf.out"
	@egrep 'Reply status:|Errors:' test2-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 4
	@grep ' 2xx=360 ' test2-httperf.out
	@grep '^cache requests .* 360' test2.out
	@grep '^cache hits .* 324' test2.out
	sleep 2

testcache3:
	@echo "Running test cache 3 - should get 360 replies and 0 errors"
	$(CSH) -c "$(USERVER) -c 1 -m 1 -C --cache-max-file-size 900000 --cache-max-bytes 2500000000 --cache-table-size 150 -f 480 >& test3.out&"
	sleep 4
	$(CSH) -c "./scripts/runaddrsesslog $(SERVER) 1 10 scripts/httpspec99-test-cache.log  >& test3-httperf.out"
	@egrep 'Reply status:|Errors:' test3-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 4
	@grep ' 2xx=360 ' test3-httperf.out
	@grep '^cache requests .* 360' test3.out
	@grep '^cache hits .* 315' test3.out
	sleep 2

testcache4:
	@echo "Running test cache 4 - should get 360 replies and 0 errors"
	$(CSH) -c "$(USERVER) -c 1 -m 1 -C --cache-max-file-size 200 --cache-max-bytes 2500000000 --cache-table-size 150 -f 480 >& test4.out&"
	sleep 4
	$(CSH) -c "./scripts/runaddrsesslog $(SERVER) 1 10 scripts/httpspec99-test-cache.log  >& test4-httperf.out"
	@egrep 'Reply status:|Errors:' test4-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 4
	@grep ' 2xx=360 ' test4-httperf.out
	@grep '^cache requests .* 360' test4.out
	@grep '^cache hits .* 9' test4.out
	sleep 2

#----------------------------------------------------------------------------------
testencrypt: testencrypt1 testencrypt2 testencrypt3

testencrypt1:
	@echo "Running test encrypt 1 - should get 360 replies and 0 errors"
	$(CSH) -c "$(USERVER) -c 1 -m 1 -C --cache-max-file-size 10000000 --cache-max-bytes 2500000000 --cache-table-size 150 -f 480 --encrypt-data --encrypt-percent 100 --encrypt-max-file-size 1000000 >& test1.out&"
	sleep 4
	$(CSH) -c "./scripts/runaddrsesslog $(SERVER) 1 10 scripts/httpspec99-test-cache.log  >& test1-httperf.out"
	@egrep 'Reply status:|Errors:' test1-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 4
	@grep ' 2xx=360 ' test1-httperf.out
	@grep '^cache requests .* 360' test1.out
	@grep '^cache hits .* 324' test1.out
	@grep '^total candidates .* 360' test1.out
	@grep '^candidates not too large .* 360' test1.out
	@grep '^requests encrypted .* 360' test1.out
	@grep '^.* 360' test1.out
	sleep 2

testencrypt2:
	@echo "Running test encrypt 2 - should get 360 replies and 0 errors"
	$(CSH) -c "$(USERVER) -c 1 -m 1 -C --cache-max-file-size 10000000 --cache-max-bytes 2500000000 --cache-table-size 150 -f 480 --encrypt-data --encrypt-percent 100 --encrypt-max-file-size 200 >& test2.out&"
	sleep 4
	$(CSH) -c "./scripts/runaddrsesslog $(SERVER) 1 10 scripts/httpspec99-test-cache.log  >& test2-httperf.out"
	@egrep 'Reply status:|Errors:' test2-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 4
	@grep ' 2xx=360 ' test2-httperf.out
	@grep '^cache requests .* 360' test2.out
# @grep '^cache hits .* 324' test2.out
	@grep '^total candidates .* 360' test2.out
	@grep '^candidates not too large .* 10' test2.out
	@grep '^requests encrypted .* 10' test2.out
	@grep '^percentage encrypted .* 2.78' test2.out
	@grep '^percentage of size ok .* 100.00' test2.out
	sleep 2

testencrypt3:
	@echo "Running test encrypt 3 - should get 360 replies and 0 errors"
	$(CSH) -c "$(USERVER) -c 1 -m 1 -C --cache-max-file-size 10000000 --cache-max-bytes 2500000000 --cache-table-size 150 -f 480 --encrypt-data --encrypt-percent 50 --encrypt-max-file-size 0 >& test3.out&"
	sleep 4
	$(CSH) -c "./scripts/runaddrsesslog $(SERVER) 1 10 scripts/httpspec99-test-cache.log  >& test3-httperf.out"
	@egrep 'Reply status:|Errors:' test3-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 4
	@grep ' 2xx=360 ' test3-httperf.out
	@grep '^cache requests .* 360' test3.out
# @grep '^cache hits .* 324' test3.out
	@grep '^total candidates .* 360' test3.out
	@grep '^candidates not too large .* 360' test3.out
	@grep '^requests encrypted ' test3.out | egrep ' 17[0-9]|18[0-9]'
	@grep '^percentage encrypted ' test3.out | egrep ' 4[5-9]|5[0-5]'
	@grep '^percentage of size ok ' test3.out | egrep ' 4[5-9]|5[0-5]'
	sleep 2

testashif:
	@grec '^.* 360' test2.out
	@echo "Running testasfhif"
	$(CSH) -c "$(USERVER) -c 1 -m 1 -C --cache-max-bytes 2500000000 --cache-table-size 15000 -f 48000 >& test1.out&"
	sleep 4
	$(CSH) -c "./scripts/runaddrsesslog $(SERVER) 700 20000 scripts/httpspec99-d-1.log  >& test1-httperf.out"
	sleep 2
	@./scripts/kill-server
	sleep 2
	@egrep 'Reply status:|Errors:' test1-httperf.out
	sleep 2

WARM_OPTS=-C --cache-table-print --cache-table-size=40 --cache-max-load-factor=1.0 -D 0x1

testwarm: testwarm1 testwarm2 testwarm3 testwarm4

testwarm1: FORCE
	@echo "Running cache warming test"
	@echo "Running test 1 - should get 100 replies and 0 errors"
	$(CSH) -c "./userver --cache-warm=warm-list-1 $(WARM_OPTS) >& test1.out &"
	sleep 4
	$(CSH) -c "./scripts/run 100 >& test1-httperf.out"
	@egrep 'Reply status:|Errors:' test1-httperf.out
	@grep ' 2xx=100 ' test1-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 5
	@grep '^cache hits .* 100' test1.out
	@echo ""

testwarm2: FORCE
	@echo "Running cache warming test"
	@echo "Running test 2 - should get 100 replies and 0 errors"
	$(CSH) -c "./userver --cache-warm=warm-list-1 --use-sendfile --use-tcp-cork \
	       $(WARM_OPTS) >& test2.out &"
	sleep 4
	$(CSH) -c "./scripts/run 100 >& test2-httperf.out"
	@egrep 'Reply status:|Errors:' test2-httperf.out
	@grep ' 2xx=100 ' test2-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 5
	@grep '^cache hits .* 100' test2.out
	@echo ""

testwarm3: FORCE
	@echo "Running cache warming test"
	@echo "Running test 3 - should get 100 replies and 0 errors"
	$(CSH) -c "./userver --cache-warm=warm-list-2 --use-sendfile --use-tcp-cork \
	       $(WARM_OPTS) >& test3.out &"
	sleep 4
	$(CSH) -c "./scripts/run 100 >& test3-httperf.out"
	@egrep 'Reply status:|Errors:' test3-httperf.out
	@grep ' 2xx=100 ' test3-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 5
	@grep '^cache hits .* 100' test3.out
	@echo ""

testwarm4: FORCE
	@echo "Running cache warming test"
	@echo "Running test 4 - should get 100 replies and 0 errors"
	$(CSH) -c "./userver --cache-warm=warm-list-3 --use-sendfile --use-tcp-cork \
	       $(WARM_OPTS) >& test4.out &"
	sleep 4
	$(CSH) -c "./scripts/run 100 >& test4-httperf.out"
	@egrep 'Reply status:|Errors:' test4-httperf.out
	@grep ' 2xx=100 ' test4-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 5
	@grep '^cache hits .* 100' test4.out
	@echo ""

testwarm5:  FORCE
	@echo "Running IP Address Subset"
	@echo "Running test 5 - should get 100 replies and 0 errors"
	$(CSH) -c "./userver --ip-addr 127.0.0.1 --ip-addr 192.168.0.5 --ip-addr 192.168.0.8 --cache-warm warm-list-3 $(WARM_OPTS) --use-sendfile --use-tcp-cork >& test5.out &"
	sleep 8
	$(CSH) -c "./scripts/run 100 >& test5-httperf.out"
	egrep 'Reply status:|Errors:' test5-httperf.out
	@grep ' 2xx=100 ' test5-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 5
	@grep '^cache hits .* 100' test5.out
	@echo ""

testwarmaio: testwarmaio1

testwarmaio1: FORCE
	@echo "Running cache warming test"
	@echo "Running test 1 - should get 100 replies and 0 errors"
	$(CSH) -c "./userver --cache-warm=warm-list-1 $(AIO_OPTS) \
	       $(WARM_OPTS) >& test1.out &"
	sleep 4
	$(CSH) -c "./scripts/run 100 >& test1-httperf.out"
	@egrep 'Reply status:|Errors:' test1-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2
	@echo ""


testsend: testsend1 testsend2 testsend3 testsend4 testsend5


testsend1: FORCE
	@echo "Running SEND Test Subset"
	@echo "Running test 1 - should get 100 replies and 0 errors"
	$(CSH) -c "./userver-send -B -E -Y -U -N $(OPTS) >& test1.out &"
	sleep 4
	$(CSH) -c "./scripts/run 100 >& test1-httperf.out"
	@egrep 'Reply status:|Errors:' test1-httperf.out
	sleep 2
	@./scripts/kill-server-send
	sleep 2
	@echo ""


testsend2: FORCE
	@echo "Running test 2 - should get 100 replies and 0 errors"
	$(CSH) -c "./userver-send -C -B -E -Y -U -N $(OPTS) >& test2.out &"
	sleep 4
	$(CSH) -c "./scripts/run 100 >& test2-httperf.out"
	@egrep 'Reply status:|Errors:' test2-httperf.out
	sleep 2
	@./scripts/kill-server-send
	sleep 2
	@echo ""

testsend3: FORCE
	@echo "Running test 3 - should get 100 replies and 0 errors"
	$(CSH) -c "./userver-send -C --use-sendfile --use-tcp-cork -B -E -Y -U -N $(OPTS) >& test3.out &"
	sleep 4
	$(CSH) -c "./scripts/run 100 >& test3-httperf.out"
	@egrep 'Reply status:|Errors:' test3-httperf.out
	sleep 2
	@./scripts/kill-server-send
	sleep 2
	@echo ""

testsend4: FORCE
	@echo "Running test 4 - should get 5000 replies and 0 errors in about 10 seconds"
	$(CSH) -c "./userver-send -C --use-sendfile --use-tcp-cork -c 450 -f 1000 -B -E -Y -U -N $(OPTS) >& test4.out &"
	sleep 4
	$(CSH) -c "./scripts/runrate 500 5000 >& test4-httperf.out"
	@egrep 'Reply status:|Errors:' test4-httperf.out
	sleep 2
	@./scripts/kill-server-send
	sleep 2
	@echo ""

testsend5: FORCE
	@echo "Running test 5 - should get 4000 replies and 0 errors in about 10 seconds"
	$(CSH) -c "./userver-send -C --use-sendfile --use-tcp-cork -m 0 -c 450 -f 1000 -B -E -Y -U -N $(OPTS) >& test5.out &"
	sleep 4
	$(CSH) -c "./scripts/runrate 400 4000 >& test5-httperf.out"
	@egrep 'Reply status:|Errors:' test5-httperf.out
	sleep 2
	@./scripts/kill-server-send
	sleep 2
	@echo ""


testaperture: testaperture1 testaperture2

testaperture1: FORCE
	@echo "Running test 1 - should get 10 replies and 0 errors"
	$(CSH) -c "./userver-send -C -B -E -Y -U -N $(OPTS) --auto-accept-aperture=1 >& test1.out &"
	sleep 4
	$(CSH) -c "./scripts/run 10 |& egrep 'Reply status:|Errors:'"
	sleep 2
	@./scripts/kill-server-send
	sleep 2
	@echo ""

testaperture2: FORCE
	@echo "Running test 2 - should get 4000 replies and 0 errors in about 10 seconds"
	$(CSH) -c "./userver-send -C --use-sendfile --use-tcp-cork -m 0 -c 450 -f 1000 -B -E -Y -U -N $(OPTS) --auto-accept-aperture=10 >& test2.out &"
	sleep 4
	$(CSH) -c "./scripts/runrate 400 4000 |& egrep 'Reply status:|Errors:'"
	sleep 2
	@./scripts/kill-server-send
	sleep 2
	@echo ""


# AIO_OPTS = --ignore-fd-setsize --use-socket-aio --aio-accept-thold=10 -D 0x0 # --use-aio-wait
AIO_OPTS = --ignore-fd-setsize --aio-read-before-write --aio-read-before-accept --use-socket-aio --aio-accept-thold=10 -D 0x0 --aio-complq-count=1 # --use-aio-wait
# AIO_OPTS =  --use-socket-aio --aio-accept-thold=10 -D 0x0

# Which client to use for remote AIO tests
# CLIENT = 15.0.119.22
CLIENT = 127.0.0.1
# Which server the client will send requests to
# SERVER = 15.0.117.3
SERVER = 127.0.0.1
# Where to find the userver directory
SHOME = .
# Temporary directory to store scripts on client host.
CHOME = /tmp/testaio
# CHOME = ./scripts
# No need to change this if SHOME is set.
USERVER = $(SHOME)/userver

# Test aio with headers stored in the file.
testaio_w_header: aio_w_header1 aio_w_header2 aio_w_header3 aio_w_header4

DO_CLIENT=
# DO_CLIENT=ssh $(CLIENT)

aio_w_header1:
	@echo "Running test 1 - should get 1 replies and 0 errors"
	$(CSH) -c "$(USERVER) -c 2 -f 30 -m 1 -C $(AIO_OPTS) >& test1.out &"
	sleep 4
	$(CSH) -c "ssh $(CLIENT) '$(CHOME)/runaddruri $(SERVER) docs/10B-w-header.txt 1' >& test1-httperf.out"
	@egrep 'Reply status:|Errors:' test1-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

aio_w_header2:
	@echo "Running test 2 - should get 100 replies and 0 errors"
	$(CSH) -c "$(USERVER) -c 2 -f 30 -m 1 -C $(AIO_OPTS) >& test2.out &"
	sleep 4
	$(CSH) -c "ssh $(CLIENT) '$(CHOME)/runaddruri $(SERVER) docs/10B-w-header.txt 100' >& test2-httperf.out"
	@egrep 'Reply status:|Errors:' test2-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

aio_w_header3:
	@echo "Running test 3 - should get 1 replies and 0 errors"
	$(CSH) -c "$(USERVER) -c 2 -f 30 -m 1 $(AIO_OPTS) >& test3.out &"
	sleep 4
	$(CSH) -c "ssh $(CLIENT) '$(CHOME)/runaddruri $(SERVER) docs/10B-w-header.txt 1' >& test3-httperf.out"
	@egrep 'Reply status:|Errors:' test3-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

aio_w_header4:
	@echo "Running test 4 - should get 100 replies and 0 errors"
	$(CSH) -c "$(USERVER) -c 2 -f 30 -m 1 $(AIO_OPTS) >& test4.out &"
	sleep 4
	$(CSH) -c "ssh $(CLIENT) '$(CHOME)/runaddruri $(SERVER) docs/10B-w-header.txt 100' >& test4-httperf.out"
	@egrep 'Reply status:|Errors:' test4-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtaio: rtaio1 rtaio2 rtaio3 rtaio4 rtaio5 rtaio6 rtaio7 rtaio8 rtaio9 rtaio10 \
       rtaio11 rtaio12 rtaio13 rtaio14 rtaio15

rtsetup:
	@ssh $(CLIENT) '/bin/rm -rf $(CHOME)'
	@ssh $(CLIENT) 'mkdir $(CHOME)'
	scp scripts/runaddr $(CLIENT):$(CHOME)
	scp scripts/runaddrrate $(CLIENT):$(CHOME)
	scp scripts/runaddrpersist $(CLIENT):$(CHOME)
	scp scripts/runaddrport $(CLIENT):$(CHOME)
	scp scripts/runaddruri $(CLIENT):$(CHOME)
	scp scripts/runaddrsesslog $(CLIENT):$(CHOME)
	scp scripts/runaddrsesslogtimeout $(CLIENT):$(CHOME)
	scp scripts/httpspec99-d-1.log $(CLIENT):$(CHOME)
	ssh $(CLIENT) 'chmod -R 777 $(CHOME)'

rtaio0:
	@echo "Running test 0 - should get 1000 replies and 0 errors"
	$(CSH) -c "$(USERVER) -c 40000 -f 50000 -m 1 -C --ignore-fd-setsize $(AIO_OPTS) >& test1.out &"
	sleep 4
	$(CSH) -c "ssh $(CLIENT) '$(CHOME)/runaddr $(SERVER) 1000' >& test1-httperf.out"
	@egrep 'Reply status:|Errors:' test1-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtaio1:
	@echo "****** WARNING: MAKE SURE YOU'VE RUN make rtsetup first **********"
	@echo "Running test 1 - should get 1 replies and 0 errors"
	$(CSH) -c "$(USERVER) -c 2 -f 30 -m 1 -C $(AIO_OPTS) >& test1.out &"
	sleep 4
	$(CSH) -c "ssh $(CLIENT) '$(CHOME)/runaddr $(SERVER) 1' >& test1-httperf.out"
	@egrep 'Reply status:|Errors:' test1-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtaio2:
	@echo "Running test 2 - should get 2 replies and 0 errors"
	$(CSH) -c "./userver -c 10 -f 40 -m 1 -C $(AIO_OPTS) >& test2.out &"
	sleep 4
	$(CSH) -c "ssh $(CLIENT) '$(CHOME)/runaddr $(SERVER) 2' >& test2-httperf.out"
	@egrep 'Reply status:|Errors:' test2-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtaio3:
	@echo "Running test 3 - should get 10 replies and 0 errors"
	$(CSH) -c "./userver -c 20 -f 60 -m 1 $(AIO_OPTS) >& test3.out &"
	sleep 4
	$(CSH) -c "ssh $(CLIENT) '$(CHOME)/runaddr $(SERVER) 10' >& test3-httperf.out"
	@egrep 'Reply status:|Errors:' test3-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtaio4:
	@echo "Running test 4 - should get 100 replies and 0 errors"
	$(CSH) -c "./userver -c 2 -f 30 -m 1 -C $(AIO_OPTS) >& test4.out &"
	sleep 4
	$(CSH) -c "ssh $(CLIENT) '$(CHOME)/runaddr $(SERVER) 100' >& test4-httperf.out"
	@egrep 'Reply status:|Errors:' test4-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtaio5:
	@echo "Running test 5 - should get 500 replies and 0 errors"
	$(CSH) -c "./userver -c 300 -f 500 -m 1 -C $(AIO_OPTS) >& test5.out &"
	sleep 4
	$(CSH) -c "ssh $(CLIENT) '$(CHOME)/runaddrrate $(SERVER) 50 500' >& test5-httperf.out"
	@egrep 'Reply status:|Errors:' test5-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtaio6:
	@echo "Running test 6 - should get 1000 replies and 0 errors"
	$(CSH) -c "./userver -c 300 -f 500 -m 1 -C $(AIO_OPTS) >& test6.out &"
	sleep 4
	$(CSH) -c "ssh $(CLIENT) '$(CHOME)/runaddrpersist $(SERVER) 1000' >& test6-httperf.out"
	@egrep 'Reply status:|Errors:' test6-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtaio7:
	@echo "Running test 7 - should get 1000 replies and 0 errors"
	$(CSH) -c "./userver -c 300 -f 500 -m 1 -C $(AIO_OPTS) >& test7.out &"
	sleep 4
	$(CSH) -c "ssh $(CLIENT) '$(CHOME)/runaddr $(SERVER) 100' >& test7-httperf.out"
	@egrep 'Reply status:|Errors:' test7-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtaio8:
	@echo "Running test 8 - should get 1000 replies and 0 errors"
	$(CSH) -c "./userver -c 300 -f 500 -m 1 -C $(AIO_OPTS) >& test8.out &"
	sleep 4
	$(CSH) -c "ssh $(CLIENT) '$(CHOME)/runaddrrate $(SERVER) 100 1000' >& test8-httperf.out"
	@egrep 'Reply status:|Errors:' test8-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtaio9:
	@echo "Running test 9 - should get 10000 replies and 0 errors"
	$(CSH) -c "./userver -c 300 -f 500 -m 1 -C $(AIO_OPTS) >& test9.out &"
	sleep 4
	$(CSH) -c "ssh $(CLIENT) '$(CHOME)/runaddrrate $(SERVER) 100 10000' >& test9-httperf.out"
	@egrep 'Reply status:|Errors:' test9-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2


rtaio10:
	@echo "Running test 10 - should get 1000 replies and 0 errors"
	$(CSH) -c "./userver -c 300 -f 500 -m 1 $(AIO_OPTS) >& test10.out &"
	sleep 4
	$(CSH) -c "ssh $(CLIENT) '$(CHOME)/runaddrrate $(SERVER) 100 1000' >& test10-httperf.out"
	@egrep 'Reply status:|Errors:' test10-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtaio11:
	@echo "Running test 11 - should get 8000 replies and 0 errors"
	$(CSH) -c "./userver -c 500 -f 1024 -m 10 -C $(AIO_OPTS) >& test11.out &"
	sleep 4
	$(CSH) -c "ssh $(CLIENT) '$(CHOME)/runaddrrate $(SERVER) 200 8000' >& test11-httperf.out"
	@egrep 'Reply status:|Errors:' test11-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtaio12:
	@echo "Running test 12 - should get 1000 replies and 0 errors"
	$(CSH) -c "./userver -c 15000 -f 32000 -m 1 -C $(AIO_OPTS) >& test12.out &"
	sleep 4
	$(CSH) -c "ssh $(CLIENT) '$(CHOME)/runaddrrate $(SERVER) 100 1000' >& test12-httperf.out"
	@egrep 'Reply status:|Errors:' test12-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtaio13:
	@echo "Running test 13 - should get 9000 replies and 0 errors"
	$(CSH) -c "./userver -c 15000 -f 32000 -m 1 -C $(AIO_OPTS) >& test13.out &"
	sleep 4
	$(CSH) -c "ssh $(CLIENT) '$(CHOME)/runaddrrate $(SERVER) 1000 9000' >& test13-httperf.out"
	@egrep 'Reply status:|Errors:' test13-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtaio14:
	@echo "Running test 14 - should get 20000 replies and 0 errors"
	$(CSH) -c "./userver -c 15000 -f 32000 -m 1 -C $(AIO_OPTS) >& test14.out &"
	sleep 4
	$(CSH) -c "ssh $(CLIENT) '$(CHOME)/runaddrrate $(SERVER) 5000 50000' >& test14-httperf.out"
	@egrep 'Reply status:|Errors:' test14-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

NEW_AIO_OPTS= -C -c 1500 -f 3200 --ignore-fd-setsize \
	      --accept-count=0 --use-socket-aio \
	      --aio-read-before-write --aio-read-before-accept \
	      --aio-accept-thold=0 -D 0x0 \
	      --stats-interval=1

rtaio15:
	@echo "Running test 15 - should get 40000 replies and 0 errors"
	$(CSH) -c "./userver $(NEW_AIO_OPTS) >& test15.out &"
	sleep 4
	$(CSH) -c "ssh $(CLIENT) '$(CHOME)/runaddrrate $(SERVER) 2000 40000' >& test15-httperf.out"
	@egrep 'Reply status:|Errors:' test15-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtaionew: rtaionew1 rtaionew2 rtaionew3 rtaionew4 rtaionew5 rtaionew6

rtaionew1:
	@echo "****** WARNING: MAKE SURE YOU'VE RUN make rtsetup first **********"
	@echo "Running test 1 - should get 1 replies and 0 errors"
	$(CSH) -c "$(USERVER) -c 2 -f 30 -m 1 $(AIO_OPTS) >& test1.out &"
	sleep 4
	$(CSH) -c "./scripts/runaddr-1.0 $(SERVER) 1 >& test1-httperf.out"
	@egrep 'Reply status:|Errors:' test1-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtaionew2:
	@echo "Running test 2 - should get 10 replies and 0 errors"
	$(CSH) -c "$(USERVER) -c 2 -f 30 -m 1 $(AIO_OPTS) >& test2.out &"
	sleep 4
	$(CSH) -c "./scripts/runaddr-1.0 $(SERVER) 10 >& test2-httperf.out"
	@egrep 'Reply status:|Errors:' test2-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtaionew3:
	@echo "Running test 3 - should get 1 replies and 0 errors"
	$(CSH) -c "$(USERVER) -c 2 -f 30 -m 1 -C $(AIO_OPTS) >& test3.out &"
	sleep 4
	$(CSH) -c "./scripts/runaddr-1.0 $(SERVER) 1 >& test3-httperf.out"
	@egrep 'Reply status:|Errors:' test3-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtaionew4:
	@echo "Running test 4 - should get 10 replies and 0 errors"
	$(CSH) -c "$(USERVER) -c 2 -f 30 -m 1 -C $(AIO_OPTS) >& test4.out &"
	sleep 4
	$(CSH) -c "./scripts/runaddr-1.0 $(SERVER) 10 >& test4-httperf.out"
	@egrep 'Reply status:|Errors:' test4-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtaionew5:
	@echo "Running test 5 - should get 500 replies and 0 errors"
	$(CSH) -c "./userver -c 300 -f 500 -m 1 $(AIO_OPTS) >& test5.out &"
	sleep 4
	$(CSH) -c "./scripts/runaddrrate-1.0 $(SERVER) 50 500 >& test5-httperf.out"
	@egrep 'Reply status:|Errors:' test5-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtaionew6:
	@echo "Running test 6 - should get 500 replies and 0 errors"
	$(CSH) -c "./userver -c 300 -f 500 -m 1 -C $(AIO_OPTS) >& test6.out &"
	sleep 4
	$(CSH) -c "./scripts/runaddrrate-1.0 $(SERVER) 50 500 >& test6-httperf.out"
	@egrep 'Reply status:|Errors:' test6-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2


AIO_SPEC_ORIG = --max-conns 7000 --max-fds 15000 --accept-count 0 \
          --caching-on --cache-table-size=300 --cache-max-load-factor=1.5 \
          --cache-max-file-size=1048576 --cache-max-bytes=10000000 \
          --cache-table-print \
          --ignore-fd-setsize --use-socket-aio \
          --aio-read-before-write --aio-read-before-accept \
          --aio-accept-thold=0 -D 0x0  \
          --cache-warm=cachewarmfile \
          --stats-interval=1

AIO_SPEC = $(AIO_SPEC_ORIG) --accept-on-close --aio-completion-order raw

# Other AIO options that one might like to explore
#
#           --aio-read-events-limit 10 \
#           --aio-write-events-limit 10 \
#           --aio-accept-events-limit 10 \
#           --aio-dont-wait \
#           --skip-header


rtaiospec: rtaiospec1 timewait rtaiospec2 timewait \
           rtaiospec3 timewait rtaiospec4

timewait:
	sleep 90

rtaioserver:
	@echo "Running test 1 - should get 0 errors"
	$(CSH) -c "$(USERVER) $(AIO_SPEC) >& OUTPUT &"


rtaiospec1:
	@echo "Running test 1 - should get 0 errors"
	$(CSH) -c "$(USERVER) $(AIO_SPEC) >& test1.out &"
	sleep 4
	$(CSH) -c "ssh $(CLIENT) '$(CHOME)/runaddrsesslog $(SERVER) 200 20000 $(CHOME)/httpspec99-d-1.log' >& test1-httperf.out"
	@egrep 'Reply status:|Errors:' test1-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtaiospec2:
	@echo "Running test 2 - should get 0 errors"
	$(CSH) -c "$(USERVER) $(AIO_SPEC) >& test2.out &"
	sleep 4
	$(CSH) -c "ssh $(CLIENT) '$(CHOME)/runaddrsesslog $(SERVER) 400 40000 $(CHOME)/httpspec99-d-1.log' >& test2-httperf.out"
	@egrep 'Reply status:|Errors:' test2-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtaiospec3:
	@echo "Running test 3 - should get 0 errors"
	$(CSH) -c "$(USERVER) $(AIO_SPEC) >& test3.out &"
	sleep 4
	$(CSH) -c "ssh $(CLIENT) '$(CHOME)/runaddrsesslog $(SERVER) 800 40000 $(CHOME)/httpspec99-d-1.log' >& test3-httperf.out"
	@egrep 'Reply status:|Errors:' test3-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtaiospec4:
	@echo "Running test 4 - should get 0 errors"
	$(CSH) -c "$(USERVER) $(AIO_SPEC) >& test4.out &"
	sleep 4
	$(CSH) -c "ssh $(CLIENT) '$(CHOME)/runaddrsesslog $(SERVER) 1000 40000 $(CHOME)/httpspec99-d-1.log' >& test4-httperf.out"
	@egrep 'Reply status:|Errors:' test4-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtaiospec4test:
	@echo "Running test 4 - should get 0 errors"
	$(CSH) -c "$(USERVER) $(AIO_SPEC) --accept-on-close --aio-completion-order rwra >& test4-test.out &"
	sleep 4
	$(CSH) -c "ssh $(CLIENT) '$(CHOME)/runaddrsesslog $(SERVER) 1000 40000 $(CHOME)/httpspec99-d-1.log' >& test4-test-httperf.out"
	@egrep 'Reply status:|Errors:' test4-test-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtaiospec4b:
	@echo "Running test 4b - should get 0 errors"
	$(CSH) -c "$(USERVER) $(AIO_SPEC) >& test4b.out &"
	sleep 4
	$(CSH) -c "ssh $(CLIENT) '$(CHOME)/runaddrsesslog $(SERVER) 1000 300000 $(CHOME)/httpspec99-d-1.log' >& test4b-httperf.out"
	@egrep 'Reply status:|Errors:' test4b-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtaiospec4c:
	@echo "Running test 4c - should get 0 errors"
	$(CSH) -c "$(USERVER) $(AIO_SPEC) >& test4c.out &"
	sleep 4
	$(CSH) -c "ssh $(CLIENT) '$(CHOME)/runaddrsesslog $(SERVER) 1020 40000 $(CHOME)/httpspec99-d-1.log' >& test4c-httperf.out"
	@egrep 'Reply status:|Errors:' test4c-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtaiospec4d:
	@echo "Running test 4d - should get 0 errors"
	$(CSH) -c "$(USERVER) $(AIO_SPEC) >& test4d.out &"
	sleep 4
	$(CSH) -c "ssh $(CLIENT) '$(CHOME)/runaddrsesslog $(SERVER) 1040 40000 $(CHOME)/httpspec99-d-1.log' >& test4d-httperf.out"
	@egrep 'Reply status:|Errors:' test4d-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtaiospec4e:
	@echo "Running test 4e"
	$(CSH) -c "$(USERVER) $(AIO_SPEC) >& test4e.out &"
	sleep 4
	$(CSH) -c "ssh $(CLIENT) '$(CHOME)/runaddrsesslog $(SERVER) 1060 40000 $(CHOME)/httpspec99-d-1.log' >& test4e-httperf.out"
	@egrep 'Reply status:|Errors:' test4e-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtaiospec4f:
	@echo "Running test 4f"
	$(CSH) -c "$(USERVER) $(AIO_SPEC) >& test4f.out &"
	sleep 4
	$(CSH) -c "ssh $(CLIENT) '$(CHOME)/runaddrsesslog $(SERVER) 1080 40000 $(CHOME)/httpspec99-d-1.log' >& test4f-httperf.out"
	@egrep 'Reply status:|Errors:' test4f-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2


rtaiospec5:
	@echo "Running test 5"
	$(CSH) -c "$(USERVER) $(AIO_SPEC) >& test5.out &"
	sleep 4
	$(CSH) -c "ssh $(CLIENT) '$(CHOME)/runaddrsesslog $(SERVER) 1100 40000 $(CHOME)/httpspec99-d-1.log' >& test5-httperf.out"
	@egrep 'Reply status:|Errors:' test5-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

# Talks to the server on the same subnet/IP as CLIENT.
CLIENT2=15.0.119.21

rtaiospec6:
	@echo "Running test 6 - should get lots of errors"
	$(CSH) -c "$(USERVER) $(AIO_SPEC) >& test6.out &"
	sleep 4
	$(CSH) -c "ssh $(CLIENT) '$(CHOME)/runaddrsesslog $(SERVER) 1000 40000 $(CHOME)/httpspec99-d-1.log' >& test6a-httperf.out&" 
	$(CSH) -c "ssh $(CLIENT2) '$(CHOME)/runaddrsesslog $(SERVER) 1000 40000 $(CHOME)/httpspec99-d-1.log' >& test6b-httperf.out"
	sleep 10
	@egrep 'Reply status:|Errors:' test6a-httperf.out
	@egrep 'Reply status:|Errors:' test6b-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

# Talks to the server on different subnet/IP from CLIENT.
CLIENT3=15.0.119.24
CLIENT4=15.0.119.26
CLIENT5=15.0.119.28
IP1=15.0.117.3
IP2=15.0.117.19
IP3=15.0.117.35
IP4=15.0.117.51

rtaiospec7:
	@echo "Running test 7 - should get 0 errors"
	$(CSH) -c "$(USERVER) $(AIO_SPEC) --ip-addr $(IP1) --ip-addr $(IP2) >& test7.out &"
	sleep 4
	$(CSH) -c "ssh $(CLIENT) '$(CHOME)/runaddrsesslog $(IP1) 500 10000 $(CHOME)/httpspec99-d-1.log' >& test7a-httperf.out&" 
	$(CSH) -c "ssh $(CLIENT3) '$(CHOME)/runaddrsesslog $(IP2) 500 10000 $(CHOME)/httpspec99-d-1.log' >& test7b-httperf.out"
	sleep 10
	@egrep 'Reply status:|Errors:' test7a-httperf.out
	@egrep 'Reply status:|Errors:' test7b-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtaiospec8:
	@echo "Running test 8 - should get 0 errors"
	$(CSH) -c "$(USERVER) $(AIO_SPEC) --ip-addr $(IP1) --ip-addr $(IP2) >& test8.out &"
	sleep 4
	$(CSH) -c "ssh $(CLIENT) '$(CHOME)/runaddrsesslog $(IP1) 1000 40000 $(CHOME)/httpspec99-d-1.log' >& test8a-httperf.out&" 
	$(CSH) -c "ssh $(CLIENT3) '$(CHOME)/runaddrsesslog $(IP2) 1000 40000 $(CHOME)/httpspec99-d-1.log' >& test8b-httperf.out"
	sleep 10
	@egrep 'Reply status:|Errors:' test8a-httperf.out
	@egrep 'Reply status:|Errors:' test8b-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtaiospec9:
	@echo "Running test 9 - should get 0 errors"
	$(CSH) -c "$(USERVER) $(AIO_SPEC) \
            --ip-addr $(IP1) --ip-addr $(IP2) --ip-addr $(IP3) >& test9.out &"
	sleep 4
	$(CSH) -c "ssh $(CLIENT)  '$(CHOME)/runaddrsesslog $(IP1) 500 10000 $(CHOME)/httpspec99-d-1.log' >& test9a-httperf.out&" 
	$(CSH) -c "ssh $(CLIENT3) '$(CHOME)/runaddrsesslog $(IP2) 500 10000 $(CHOME)/httpspec99-d-1.log' >& test9b-httperf.out&"
	$(CSH) -c "ssh $(CLIENT4) '$(CHOME)/runaddrsesslog $(IP3) 500 10000 $(CHOME)/httpspec99-d-1.log' >& test9c-httperf.out"
	sleep 15
	@egrep 'Reply status:|Errors:' test9a-httperf.out
	@egrep 'Reply status:|Errors:' test9b-httperf.out
	@egrep 'Reply status:|Errors:' test9c-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtaiospec10:
	@echo "Running test 10 - should get 0 errors"
	$(CSH) -c "$(USERVER) $(AIO_SPEC) \
           --ip-addr $(IP1) --ip-addr $(IP2) --ip-addr $(IP3) >& test10.out &"
	sleep 4
	$(CSH) -c "ssh $(CLIENT)  '$(CHOME)/runaddrsesslog $(IP1) 750 10000 $(CHOME)/httpspec99-d-1.log' >& test10a-httperf.out&" 
	$(CSH) -c "ssh $(CLIENT3) '$(CHOME)/runaddrsesslog $(IP2) 750 10000 $(CHOME)/httpspec99-d-1.log' >& test10b-httperf.out&"
	$(CSH) -c "ssh $(CLIENT4) '$(CHOME)/runaddrsesslog $(IP3) 750 10000 $(CHOME)/httpspec99-d-1.log' >& test10c-httperf.out"
	sleep 15
	@egrep 'Reply status:|Errors:' test10a-httperf.out
	@egrep 'Reply status:|Errors:' test10b-httperf.out
	@egrep 'Reply status:|Errors:' test10c-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtaiospec11:
	@echo "Running test 11 - should get 0 errors"
	$(CSH) -c "$(USERVER) $(AIO_SPEC) -D 0x0 \
            --ip-addr $(IP4) --ip-addr $(IP3) --ip-addr $(IP2) --ip-addr $(IP1) >& test11.out &"
	sleep 4
	$(CSH) -c "ssh $(CLIENT)  '$(CHOME)/runaddrsesslog $(IP1) 500 10000 $(CHOME)/httpspec99-d-1.log' >& test11a-httperf.out&" 
	$(CSH) -c "ssh $(CLIENT3) '$(CHOME)/runaddrsesslog $(IP2) 500 10000 $(CHOME)/httpspec99-d-1.log' >& test11b-httperf.out&"
	$(CSH) -c "ssh $(CLIENT4) '$(CHOME)/runaddrsesslog $(IP3) 500 10000 $(CHOME)/httpspec99-d-1.log' >& test11c-httperf.out&"
	$(CSH) -c "ssh $(CLIENT5) '$(CHOME)/runaddrsesslog $(IP4) 500 10000 $(CHOME)/httpspec99-d-1.log' >& test11d-httperf.out"
	sleep 15
	@egrep 'Reply status:|Errors:' test11a-httperf.out
	@egrep 'Reply status:|Errors:' test11b-httperf.out
	@egrep 'Reply status:|Errors:' test11c-httperf.out
	@egrep 'Reply status:|Errors:' test11d-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtaiospec12:
	@echo "Running test 12 - should get 0 errors"
	$(CSH) -c "$(USERVER) $(AIO_SPEC) -D 0x0 \
            --ip-addr $(IP1) --ip-addr $(IP2) >& test12a.out &"
	sleep 10
	$(CSH) -c "$(USERVER) $(AIO_SPEC) -D 0x0 \
            --ip-addr $(IP3) --ip-addr $(IP4) >& test12b.out &"
	sleep 10
	$(CSH) -c "ssh $(CLIENT)  '$(CHOME)/runaddrsesslog $(IP1) 150 10000 $(CHOME)/httpspec99-d-1.log' >& test12a-httperf.out&" 
	$(CSH) -c "ssh $(CLIENT3) '$(CHOME)/runaddrsesslog $(IP2) 150 10000 $(CHOME)/httpspec99-d-1.log' >& test12b-httperf.out&"
	$(CSH) -c "ssh $(CLIENT4) '$(CHOME)/runaddrsesslog $(IP3) 150 10000 $(CHOME)/httpspec99-d-1.log' >& test12c-httperf.out&"
	$(CSH) -c "ssh $(CLIENT5) '$(CHOME)/runaddrsesslog $(IP4) 150 10000 $(CHOME)/httpspec99-d-1.log' >& test12d-httperf.out"
	sleep 15
	@egrep 'Reply status:|Errors:' test12a-httperf.out
	@egrep 'Reply status:|Errors:' test12b-httperf.out
	@egrep 'Reply status:|Errors:' test12c-httperf.out
	@egrep 'Reply status:|Errors:' test12d-httperf.out
	sleep 2
	pkill userver
	sleep 2

# ---------
# lsnp2 non eta ip address
CLIENT1TMP = 15.0.119.22
# lsnp4 non eta ip address
CLIENT2TMP = 15.0.119.24
# lsnp6 non eta ip address
CLIENT3TMP = 15.0.119.26
# IP addr that lsnp2 talks to lsnpsvr2 on
IP1TMP = 15.0.117.3
# IP addr that lsnp4 talks to lsnpsvr2 on
IP2TMP = 15.0.117.19
# IP addr that lsnp6 talks to lsnpsvr2 on
IP3TMP = 15.0.117.35

rtaiosetuptmp: rtaiosetuptmp1 rtaiosetuptmp2 rtaiosetuptmp3

rtaiosetuptmp1:
	@ssh $(CLIENT1TMP) '/bin/rm -rf $(CHOME)'
	@ssh $(CLIENT1TMP) 'mkdir $(CHOME)'
	scp scripts/runaddr $(CLIENT1TMP):$(CHOME)
	scp scripts/runaddrrate $(CLIENT1TMP):$(CHOME)
	scp scripts/runaddrpersist $(CLIENT1TMP):$(CHOME)
	scp scripts/runaddrport $(CLIENT1TMP):$(CHOME)
	scp scripts/runaddruri $(CLIENT1TMP):$(CHOME)
	scp scripts/runaddrsesslog $(CLIENT1TMP):$(CHOME)
	scp scripts/runaddrsesslogtimeout $(CLIENT1TMP):$(CHOME)
	scp scripts/httpspec99-d-1.log $(CLIENT1TMP):$(CHOME)
	scp scripts/httpspec99-d-1-longsess.log $(CLIENT1TMP):$(CHOME)
	ssh $(CLIENT1TMP) 'chmod -R 777 $(CHOME)'

rtaiosetuptmp2:
	@ssh $(CLIENT2TMP) '/bin/rm -rf $(CHOME)'
	@ssh $(CLIENT2TMP) 'mkdir $(CHOME)'
	scp scripts/runaddr $(CLIENT2TMP):$(CHOME)
	scp scripts/runaddrrate $(CLIENT2TMP):$(CHOME)
	scp scripts/runaddrpersist $(CLIENT2TMP):$(CHOME)
	scp scripts/runaddrport $(CLIENT2TMP):$(CHOME)
	scp scripts/runaddruri $(CLIENT2TMP):$(CHOME)
	scp scripts/runaddrsesslog $(CLIENT2TMP):$(CHOME)
	scp scripts/runaddrsesslogtimeout $(CLIENT2TMP):$(CHOME)
	scp scripts/httpspec99-d-1.log $(CLIENT2TMP):$(CHOME)
	scp scripts/httpspec99-d-1-longsess.log $(CLIENT2TMP):$(CHOME)
	ssh $(CLIENT1TMP) 'chmod -R 777 $(CHOME)'

rtaiosetuptmp3:
	@ssh $(CLIENT3TMP) '/bin/rm -rf $(CHOME)'
	@ssh $(CLIENT3TMP) 'mkdir $(CHOME)'
	scp scripts/runaddr $(CLIENT3TMP):$(CHOME)
	scp scripts/runaddrrate $(CLIENT3TMP):$(CHOME)
	scp scripts/runaddrpersist $(CLIENT3TMP):$(CHOME)
	scp scripts/runaddrport $(CLIENT3TMP):$(CHOME)
	scp scripts/runaddruri $(CLIENT3TMP):$(CHOME)
	scp scripts/runaddrsesslog $(CLIENT3TMP):$(CHOME)
	scp scripts/runaddrsesslogtimeout $(CLIENT3TMP):$(CHOME)
	scp scripts/httpspec99-d-1.log $(CLIENT3TMP):$(CHOME)
	scp scripts/httpspec99-d-1-longsess.log $(CLIENT3TMP):$(CHOME)
	ssh $(CLIENT1TMP) 'chmod -R 777 $(CHOME)'

rtaiospec-tmp1:
	@echo "Running test - tmp1"
	$(CSH) -c "$(USERVER) $(AIO_SPEC) >& test-tmp1.out &"
	sleep 4
	$(CSH) -c "ssh $(CLIENT1TMP) '$(CHOME)/runaddrsesslogtimeout $(IP1TMP) 12.3 400 $(CHOME)/httpspec99-d-1-longsess.log 3' >& test-tmp1-httperf.out"
	sleep 2
	@egrep 'Reply status:|Errors:' test-tmp1-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtaiospec-tmp2:
	@echo "Running test - tmp2"
	$(CSH) -c "$(USERVER) $(AIO_SPEC) --ip-addr $(IP1TMP) --ip-addr $(IP2TMP) >& test-tmp2.out &"
	sleep 4
	$(CSH) -c "ssh $(CLIENT1TMP) '$(CHOME)/runaddrsesslogtimeout $(IP1TMP) 12.3 400 $(CHOME)/httpspec99-d-1-longsess.log 3' >& test-tmp2a-httperf.out&"
	$(CSH) -c "ssh $(CLIENT2TMP) '$(CHOME)/runaddrsesslogtimeout $(IP2TMP) 12.3 400 $(CHOME)/httpspec99-d-1-longsess.log 3' >& test-tmp2b-httperf.out"
	sleep 2
	@egrep 'Reply status:|Errors:' test-tmp2a-httperf.out
	@egrep 'Reply status:|Errors:' test-tmp2b-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtaiospec-tmp3:
	@echo "Running test - tmp3"
	$(CSH) -c "$(USERVER) $(AIO_SPEC) --ip-addr $(IP1TMP) --ip-addr $(IP2TMP) --ip-addr $(IP3TMP) >& test-tmp3.out &"
	sleep 4
	$(CSH) -c "ssh $(CLIENT1TMP) '$(CHOME)/runaddrsesslogtimeout $(IP1TMP) 9 400 $(CHOME)/httpspec99-d-1-longsess.log 3' >& test-tmp3a-httperf.out&"
	$(CSH) -c "ssh $(CLIENT2TMP) '$(CHOME)/runaddrsesslogtimeout $(IP2TMP) 9 400 $(CHOME)/httpspec99-d-1-longsess.log 3' >& test-tmp3b-httperf.out&"
	$(CSH) -c "ssh $(CLIENT3TMP) '$(CHOME)/runaddrsesslogtimeout $(IP3TMP) 9 400 $(CHOME)/httpspec99-d-1-longsess.log 3' >& test-tmp3c-httperf.out"
	sleep 2
	@egrep 'Reply status:|Errors:' test-tmp3a-httperf.out
	@egrep 'Reply status:|Errors:' test-tmp3b-httperf.out
	@egrep 'Reply status:|Errors:' test-tmp3c-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

LSNP1_STD = 15.0.119.21
LSNP2_STD = 15.0.119.22
LSNP3_STD = 15.0.119.23
LSNP4_STD = 15.0.119.24
LSNP5_STD = 15.0.119.25
LSNP6_STD = 15.0.119.26
LSNP7_STD = 15.0.119.27
LSNP8_STD = 15.0.119.28

LSNP1_TO_SVR = 15.0.117.3
LSNP2_TO_SVR = 15.0.117.3
LSNP3_TO_SVR = 15.0.117.19
LSNP4_TO_SVR = 15.0.117.19
LSNP5_TO_SVR = 15.0.117.35
LSNP6_TO_SVR = 15.0.117.35
LSNP7_TO_SVR = 15.0.117.51
LSNP8_TO_SVR = 15.0.117.51

rtaioip: rtaioip1 rtaioip2 rtaioip3 rtaioip4 \
         rtaioip5 rtaioip6 rtaioip7 rtaioip8

rtaioip1:
	@echo "Running test 1 - should get 0 errors"
	$(CSH) -c "$(USERVER) $(AIO_SPEC) -D 0x1 \
            --ip-addr $(LSNP1_TO_SVR) >& test1.out &"
	sleep 4
	$(CSH) -c "ssh $(LSNP1_STD) '$(CHOME)/runaddrsesslog $(LSNP1_TO_SVR) 5 10 $(CHOME)/httpspec99-d-1.log' >& test1-httperf.out" 
	@egrep 'Reply status:|Errors:' test1-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtaioip2:
	@echo "Running test 2 - should get 0 errors"
	$(CSH) -c "$(USERVER) $(AIO_SPEC) -D 0x1 \
            --ip-addr $(LSNP2_TO_SVR) >& test2.out &"
	sleep 4
	$(CSH) -c "ssh $(LSNP2_STD)  '$(CHOME)/runaddrsesslog $(LSNP2_TO_SVR) 5 10 $(CHOME)/httpspec99-d-1.log' >& test2-httperf.out" 
	@egrep 'Reply status:|Errors:' test2-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtaioip3:
	@echo "Running test 3 - should get 0 errors"
	$(CSH) -c "$(USERVER) $(AIO_SPEC) -D 0x1 \
            --ip-addr $(LSNP3_TO_SVR) >& test3.out &"
	sleep 4
	$(CSH) -c "ssh $(LSNP3_STD)  '$(CHOME)/runaddrsesslog $(LSNP3_TO_SVR) 5 10 $(CHOME)/httpspec99-d-1.log' >& test3-httperf.out" 
	@egrep 'Reply status:|Errors:' test3-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtaioip4:
	@echo "Running test 4 - should get 0 errors"
	$(CSH) -c "$(USERVER) $(AIO_SPEC) -D 0x1 \
            --ip-addr $(LSNP4_TO_SVR) >& test4.out &"
	sleep 4
	$(CSH) -c "ssh $(LSNP4_STD)  '$(CHOME)/runaddrsesslog $(LSNP4_TO_SVR) 5 10 $(CHOME)/httpspec99-d-1.log' >& test4-httperf.out" 
	@egrep 'Reply status:|Errors:' test4-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtaioip5:
	@echo "Running test 5 - should get 0 errors"
	$(CSH) -c "$(USERVER) $(AIO_SPEC) -D 0x1 \
            --ip-addr $(LSNP5_TO_SVR) >& test5.out &"
	sleep 4
	$(CSH) -c "ssh $(LSNP5_STD)  '$(CHOME)/runaddrsesslog $(LSNP5_TO_SVR) 5 10 $(CHOME)/httpspec99-d-1.log' >& test5-httperf.out" 
	@egrep 'Reply status:|Errors:' test5-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtaioip6:
	@echo "Running test 6 - should get 0 errors"
	$(CSH) -c "$(USERVER) $(AIO_SPEC) -D 0x1 \
            --ip-addr $(LSNP6_TO_SVR) >& test6.out &"
	sleep 4
	$(CSH) -c "ssh $(LSNP6_STD)  '$(CHOME)/runaddrsesslog $(LSNP6_TO_SVR) 5 10 $(CHOME)/httpspec99-d-1.log' >& test6-httperf.out" 
	@egrep 'Reply status:|Errors:' test6-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtaioip7:
	@echo "Running test 7 - should get 0 errors"
	$(CSH) -c "$(USERVER) $(AIO_SPEC) -D 0x1 \
            --ip-addr $(LSNP7_TO_SVR) >& test7.out &"
	sleep 4
	$(CSH) -c "ssh $(LSNP7_STD)  '$(CHOME)/runaddrsesslog $(LSNP7_TO_SVR) 5 10 $(CHOME)/httpspec99-d-1.log' >& test7-httperf.out" 
	@egrep 'Reply status:|Errors:' test7-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtaioip8:
	@echo "Running test 8 - should get 0 errors"
	$(CSH) -c "$(USERVER) $(AIO_SPEC) -D 0x1 \
            --ip-addr $(LSNP8_TO_SVR) >& test8.out &"
	sleep 4
	$(CSH) -c "ssh $(LSNP8_STD)  '$(CHOME)/runaddrsesslog $(LSNP8_TO_SVR) 5 10 $(CHOME)/httpspec99-d-1.log' >& test8-httperf.out" 
	@egrep 'Reply status:|Errors:' test8-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testaio: rtaionew testaiolist

testaiolist: testaio1 testaio2 testaio3 testaio4 testaio5 testaio6 testaio7 \
         testaio8 testaio9 testaio10 testaio11 testaio12 testaio13 testaio14

testaio1:
	@echo "Running AIO Test Subset"
	@echo "Running test 1 - should get 1 replies and 0 errors"
	$(CSH) -c "./userver -c 2 -f 30 -m 1 $(AIO_OPTS) >& test1.out &"
	sleep 4
	$(CSH) -c "./scripts/run 1 >& test1-httperf.out"
	@egrep 'Reply status:|Errors:' test1-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testaio2:
	@echo "Running test 2 - should get 2 replies and 0 errors"
	$(CSH) -c "./userver -c 2 -f 30 -m 1 $(AIO_OPTS) >& test2.out &"
	sleep 4
	$(CSH) -c "./scripts/run 2 >& test2-httperf.out"
	@egrep 'Reply status:|Errors:' test2-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testaio3:
	@echo "Running test 3 - should get 10 replies and 0 errors"
	$(CSH) -c "./userver -c 2 -f 30 -m 1 $(AIO_OPTS) >& test3.out &"
	sleep 4
	$(CSH) -c "./scripts/run 10 >& test3-httperf.out"
	@egrep 'Reply status:|Errors:' test3-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testaio4:
	@echo "Running test 4 - should get 100 replies and 0 errors"
	$(CSH) -c "./userver -c 2 -f 30 -m 1 $(AIO_OPTS) >& test4.out &"
	sleep 4
	$(CSH) -c "./scripts/run 100 >& test4-httperf.out"
	@egrep 'Reply status:|Errors:' test4-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testaio5:
	@echo "Running test 5 - should get 500 replies and 0 errors"
	$(CSH) -c "./userver -c 300 -f 500 -m 1 $(AIO_OPTS) >& test5.out &"
	sleep 4
	$(CSH) -c "./scripts/runrate 50 500 >& test5-httperf.out"
	@egrep 'Reply status:|Errors:' test5-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testaio6:
	@echo "Running test 6 - should get 1000 replies and 0 errors"
	$(CSH) -c "./userver -c 300 -f 500 -m 1 $(AIO_OPTS) >& test6.out &"
	sleep 4
	$(CSH) -c "./scripts/runpersist 1000 >& test6-httperf.out"
	@egrep 'Reply status:|Errors:' test6-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testaio7:
	@echo "Running test 7 - should get 1000 replies and 0 errors"
	$(CSH) -c "./userver -c 300 -f 500 -m 1 $(AIO_OPTS) >& test7.out &"
	sleep 4
	$(CSH) -c "./scripts/run 1000 >& test7-httperf.out"
	@egrep 'Reply status:|Errors:' test7-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testaio8:
	@echo "Running test 8 - should get 1000 replies and 0 errors"
	$(CSH) -c "./userver -c 300 -f 500 -m 1 $(AIO_OPTS) >& test8.out &"
	sleep 4
	$(CSH) -c "./scripts/runrate 100 1000 >& test8-httperf.out"
	@egrep 'Reply status:|Errors:' test8-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testaio9:
	@echo "Running test 9 - should get 10000 replies and 0 errors"
	$(CSH) -c "./userver -c 300 -f 500 -m 1 $(AIO_OPTS) >& test9.out &"
	sleep 4
	$(CSH) -c "./scripts/runrate 100 10000 >& test9-httperf.out"
	@egrep 'Reply status:|Errors:' test9-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2


testaio10:
	@echo "Running test 10 - should get 1000 replies and 0 errors"
	$(CSH) -c "./userver -c 300 -f 500 -m 1 $(AIO_OPTS) -C >& test10.out &"
	sleep 4
	$(CSH) -c "./scripts/runrate 100 1000 >& test10-httperf.out"
	@egrep 'Reply status:|Errors:' test10-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testaio11:
	@echo "Running test 11 - should get 8000 replies and 0 errors"
	$(CSH) -c "./userver -c 500 -f 1024 -m 10 $(AIO_OPTS) >& test11.out &"
	sleep 4
	$(CSH) -c "./scripts/runrate 200 8000 >& test11-httperf.out"
	@egrep 'Reply status:|Errors:' test11-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testaio12:
	@echo "Running test 12 - should get 1000 replies and 0 errors"
	$(CSH) -c "./userver -c 15000 -f 32000 -m 1 $(AIO_OPTS) -C >& test12.out &"
	sleep 4
	$(CSH) -c "./scripts/runrate 100 1000 >& test12-httperf.out"
	@egrep 'Reply status:|Errors:' test12-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testaio13:
	@echo "Running test 13 - should get 9000 replies and 0 errors"
	$(CSH) -c "./userver -c 15000 -f 32000 -m 1 $(AIO_OPTS) -C >& test13.out &"
	sleep 4
	$(CSH) -c "./scripts/runrate 1000 9000 >& test13-httperf.out"
	@egrep 'Reply status:|Errors:' test13-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testaio14:
	@echo "Running test 14 - should get 20000 replies and 0 errors"
	$(CSH) -c "./userver -c 15000 -f 32000 -m 1 $(AIO_OPTS) -C >& test14.out &"
	sleep 4
	$(CSH) -c "./scripts/runrate 5000 50000 >& test14-httperf.out"
	@egrep 'Reply status:|Errors:' test14-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

testaionew: testaionew1

AIO_NEW_OPTS = --use-socket-aio --aio-read-before-accept --aio-accept-thold=1 -D 0x0

testaionew1:
	$(CSH) -c "./userver -c 1 -f 20 $(AIO_NEW_OPTS) >& test1.out"

INTER_AIO_OPTS = --ignore-fd-setsize \
		 --aio-read-before-write \
		 --aio-read-before-accept \
		 --use-socket-aio
startaio1:
	$(CSH) -c "./userver -c 100 -f 210 $(INTER_AIO_OPTS) --stats-interval=1 -D 0x0"

startaio2:
	$(CSH) -c "./userver -c 10 -f 30 $(INTER_AIO_OPTS) --doc-root=/home/brecht -D 0x1"

startaio3:
	$(CSH) -c "./userver -c 11 -f 30 $(INTER_AIO_OPTS) --ip-addr 127.0.0.1 --ip-addr 192.168.0.5 -D 0x1 --aio-accept-thold=3"

start3:
	$(CSH) -c "./userver -c 11 -f 30 --ip-addr 127.0.0.1 --ip-addr 192.168.0.5 -D 0x1"

SERVERIP1 = 127.0.0.1
SERVERIP2 = 192.168.0.5

testip: ip1 ip2 ip3 ip4 ip5

ip1:
	@echo "Running test 1 - should get 200 replies and 0 errors"
	$(CSH) -c "./userver -c 11 -f 30 -C \
	    --ip-addr $(SERVERIP1) --ip-addr $(SERVERIP2) \
	    -D 0x1 >& test1.out &"
	sleep 5
	$(CSH) -c "./scripts/runaddr $(SERVERIP1) 100 >& test1a-httperf.out"
	$(CSH) -c "./scripts/runaddr $(SERVERIP2) 100 >& test1b-httperf.out"
	@egrep 'Reply status:|Errors:' test1a-httperf.out
	@egrep 'Reply status:|Errors:' test1b-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

ip2:
	@echo "Running test 2 - should get 200 replies and 0 errors"
	$(CSH) -c "./userver -c 11 -f 30 -C \
	    --ip-addr $(SERVERIP1) --ip-addr $(SERVERIP2) \
	    $(INTER_AIO_OPTS) -D 0x1 --aio-accept-thold=3 >& test2.out &"
	sleep 5
	$(CSH) -c "./scripts/runaddr $(SERVERIP1) 100 >& test2a-httperf.out"
	$(CSH) -c "./scripts/runaddr $(SERVERIP2) 100 >& test2b-httperf.out"
	@egrep 'Reply status:|Errors:' test2a-httperf.out
	@egrep 'Reply status:|Errors:' test2b-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

ip3:
	@echo "Running test 3 - should get 2000 replies and 0 errors"
	$(CSH) -c "./userver -c 11 -f 30 -C \
	    --ip-addr $(SERVERIP1) --ip-addr $(SERVERIP2) \
	    $(INTER_AIO_OPTS) -D 0x1 --aio-accept-thold=3 >& test3.out &"
	sleep 5
	$(CSH) -c "./scripts/runaddrrate $(SERVERIP1) 200 1000 >& test3a-httperf.out &"
	$(CSH) -c "./scripts/runaddrrate $(SERVERIP2) 200 1000 >& test3b-httperf.out"
	sleep 5
	@egrep 'Reply status:|Errors:' test3a-httperf.out
	@egrep 'Reply status:|Errors:' test3b-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2


ip4:
	@echo "Running test 4 - should get 8000 replies and 0 errors"
	$(CSH) -c "./userver -c 800 -f 900 -C \
	    --ip-addr $(SERVERIP1) --ip-addr $(SERVERIP2) \
	    --stats-interval=1 \
	    $(INTER_AIO_OPTS) -D 0x0 --aio-accept-thold=0 >& test4.out &"
	sleep 5
	$(CSH) -c "./scripts/runaddrrate $(SERVERIP1) 400 4000 >& test4a-httperf.out &"
	$(CSH) -c "./scripts/runaddrrate $(SERVERIP2) 400 4000 >& test4b-httperf.out"
	sleep 10
	@egrep 'Reply status:|Errors:' test4a-httperf.out
	@egrep 'Reply status:|Errors:' test4b-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

ip5:
	@echo "Running test 5 - should get 8000 replies and 0 errors"
	$(CSH) -c "./userver -c 800 -f 900 -C \
	    --ip-addr $(SERVERIP1) --ip-addr $(SERVERIP2) \
	    $(INTER_AIO_OPTS) -D 0x0 --aio-accept-thold=0 >& test5.out &"
	sleep 5
	$(CSH) -c "./scripts/runaddrrate $(SERVERIP1) 400 4000 >& test5a-httperf.out &"
	$(CSH) -c "./scripts/runaddrrate $(SERVERIP2) 400 4000 >& test5b-httperf.out"
	sleep 10
	@egrep 'Reply status:|Errors:' test5a-httperf.out
	@egrep 'Reply status:|Errors:' test5b-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

#-----------------------------------------------------------------------
# Which client1 to use for remote tests
BASECLIENT1 = lsnp1
BASECLIENT2 = lsnp3
BASECLIENT3 = lsnp5
BASECLIENT4 = lsnp7

# Which IP addr on the server (svr1) each client will send requests to
# lsnp1 talks to the server on IP1, lsnp2 on IP2, etc.
BASEIP1 = 15.0.117.2
BASEIP2 = 15.0.117.18
BASEIP3 = 15.0.117.34
BASEIP4 = 15.0.117.50

# BASECLIENT1 = s01
# BASECLIENT2 = s02
# BASECLIENT3 = s03
# BASECLIENT4 = s04

# s01 talks to the server on IP1, s02 on IP2, etc.
# BASEIP1 = 192.168.41.103
# BASEIP2 = 192.168.61.103
# BASEIP3 = 192.168.81.103
# BASEIP4 = 192.168.101.103

# Where to find the userver directory
SHOME = .
# Temporary directory to store scripts on client host.
CHOME = /tmp/testaio

# No need to change this if SHOME is set.
USERVER = $(SHOME)/userver


SPECOPTS = --max-conns 7000 --max-fds 15000 --accept-count 0 \
          --use-sendfile --use-tcp-cork \
          --caching-on --cache-table-size=300 --cache-max-load-factor=1.5 \
          --cache-max-file-size=1048576 --cache-max-bytes=10000000 \
          --cache-table-print \
          --cache-warm=cachewarmfile \
          --stats-interval=1 \
          --listenq=128 \
          -D 0x0


rtspec: rtspec4 rtspec4c rtspec4d rtspec4e rtspec4f

rtsetupnew: rtsetup1 rtsetup2 rtsetup3 rtsetup4

rtsetup1:
	@ssh $(BASECLIENT1) '/bin/rm -rf $(CHOME)'
	@ssh $(BASECLIENT1) 'mkdir $(CHOME)'
	scp scripts/runaddr $(BASECLIENT1):$(CHOME)
	scp scripts/runaddrrate $(BASECLIENT1):$(CHOME)
	scp scripts/runaddrpersist $(BASECLIENT1):$(CHOME)
	scp scripts/runaddrport $(BASECLIENT1):$(CHOME)
	scp scripts/runaddruri $(BASECLIENT1):$(CHOME)
	scp scripts/runaddrsesslog $(BASECLIENT1):$(CHOME)
	scp scripts/runaddrsesslogtimeout $(BASECLIENT1):$(CHOME)
	scp scripts/httpspec99-d-1.log $(BASECLIENT1):$(CHOME)
	scp scripts/httpspec99-d-1-longsess.log $(BASECLIENT1):$(CHOME)
	ssh $(BASECLIENT1) 'chmod -R 777 $(CHOME)'

rtsetup2:
	@ssh $(BASECLIENT2) '/bin/rm -rf $(CHOME)'
	@ssh $(BASECLIENT2) 'mkdir $(CHOME)'
	scp scripts/runaddr $(BASECLIENT2):$(CHOME)
	scp scripts/runaddrrate $(BASECLIENT2):$(CHOME)
	scp scripts/runaddrpersist $(BASECLIENT2):$(CHOME)
	scp scripts/runaddrport $(BASECLIENT2):$(CHOME)
	scp scripts/runaddruri $(BASECLIENT2):$(CHOME)
	scp scripts/runaddrsesslog $(BASECLIENT2):$(CHOME)
	scp scripts/runaddrsesslogtimeout $(BASECLIENT2):$(CHOME)
	scp scripts/httpspec99-d-1.log $(BASECLIENT2):$(CHOME)
	scp scripts/httpspec99-d-1-longsess.log $(BASECLIENT2):$(CHOME)
	ssh $(BASECLIENT2) 'chmod -R 777 $(CHOME)'

rtsetup3:
	@ssh $(BASECLIENT3) '/bin/rm -rf $(CHOME)'
	@ssh $(BASECLIENT3) 'mkdir $(CHOME)'
	scp scripts/runaddr $(BASECLIENT3):$(CHOME)
	scp scripts/runaddrrate $(BASECLIENT3):$(CHOME)
	scp scripts/runaddrpersist $(BASECLIENT3):$(CHOME)
	scp scripts/runaddrport $(BASECLIENT3):$(CHOME)
	scp scripts/runaddruri $(BASECLIENT3):$(CHOME)
	scp scripts/runaddrsesslog $(BASECLIENT3):$(CHOME)
	scp scripts/runaddrsesslogtimeout $(BASECLIENT3):$(CHOME)
	scp scripts/httpspec99-d-1.log $(BASECLIENT3):$(CHOME)
	scp scripts/httpspec99-d-1-longsess.log $(BASECLIENT3):$(CHOME)
	ssh $(BASECLIENT3) 'chmod -R 777 $(CHOME)'

rtsetup4:
	@ssh $(BASECLIENT4) '/bin/rm -rf $(CHOME)'
	@ssh $(BASECLIENT4) 'mkdir $(CHOME)'
	scp scripts/runaddr $(BASECLIENT4):$(CHOME)
	scp scripts/runaddrrate $(BASECLIENT4):$(CHOME)
	scp scripts/runaddrpersist $(BASECLIENT4):$(CHOME)
	scp scripts/runaddrport $(BASECLIENT4):$(CHOME)
	scp scripts/runaddruri $(BASECLIENT4):$(CHOME)
	scp scripts/runaddrsesslog $(BASECLIENT4):$(CHOME)
	scp scripts/runaddrsesslogtimeout $(BASECLIENT4):$(CHOME)
	scp scripts/httpspec99-d-1.log $(BASECLIENT4):$(CHOME)
	scp scripts/httpspec99-d-1-longsess.log $(BASECLIENT4):$(CHOME)
	ssh $(BASECLIENT4) 'chmod -R 777 $(CHOME)'

BASECLIENT1TMP = $(BASECLIENT1)
BASEIP1TMP     = $(BASEIP1)

rtsetuptmp:
	@ssh $(BASECLIENT1TMP) '/bin/rm -rf $(CHOME)'
	@ssh $(BASECLIENT1TMP) 'mkdir $(CHOME)'
	scp scripts/runaddr $(BASECLIENT1TMP):$(CHOME)
	scp scripts/runaddrrate $(BASECLIENT1TMP):$(CHOME)
	scp scripts/runaddrpersist $(BASECLIENT1TMP):$(CHOME)
	scp scripts/runaddrport $(BASECLIENT1TMP):$(CHOME)
	scp scripts/runaddruri $(BASECLIENT1TMP):$(CHOME)
	scp scripts/runaddrsesslog $(BASECLIENT1TMP):$(CHOME)
	scp scripts/runaddrsesslogtimeout $(BASECLIENT1TMP):$(CHOME)
	scp scripts/httpspec99-d-1.log $(BASECLIENT1TMP):$(CHOME)
	scp scripts/httpspec99-d-1-longsess.log $(BASECLIENT1TMP):$(CHOME)
	ssh $(BASECLIENT1TMP) 'chmod -R 777 $(CHOME)'

rtspec-tmp1:
	@echo "Running test - tmp1"
	$(CSH) -c "$(USERVER) $(SPECOPTS) >& test-tmp1.out &"
	sleep 4
	$(CSH) -c "ssh $(BASECLIENT1TMP) '$(CHOME)/runaddrsesslogtimeout $(BASEIP1TMP) 11 400 $(CHOME)/httpspec99-d-1-longsess.log 3' >& test-tmp1-httperf.out"
	sleep 2
	@egrep 'Reply status:|Errors:' test-tmp1-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtspec-tmp2:
	@echo "Running test - tmp2"
	$(CSH) -c "$(USERVER) $(SPECOPTS) --ip-addr $(BASEIP1) --ip-addr $(BASEIP2) >& test-tmp2.out &"
	sleep 4
	$(CSH) -c "ssh $(BASECLIENT1) '$(CHOME)/runaddrsesslogtimeout $(BASEIP1) 11 400 $(CHOME)/httpspec99-d-1-longsess.log 3' >& test-tmp2a-httperf.out&"
	$(CSH) -c "ssh $(BASECLIENT2) '$(CHOME)/runaddrsesslogtimeout $(BASEIP2) 11 400 $(CHOME)/httpspec99-d-1-longsess.log 3' >& test-tmp2b-httperf.out"
	sleep 2
	@egrep 'Reply status:|Errors:' test-tmp2a-httperf.out
	@egrep 'Reply status:|Errors:' test-tmp2b-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtspec-tmp3:
	@echo "Running test - tmp3"
	$(CSH) -c "$(USERVER) $(SPECOPTS) >& test-tmp3.out &"
	sleep 4
	$(CSH) -c "ssh $(BASECLIENT1TMP) '$(CHOME)/runaddrsesslogtimeout $(BASEIP1TMP) 18 400 $(CHOME)/httpspec99-d-1-longsess.log 3' >& test-tmp3-httperf.out"
	sleep 2
	@egrep 'Reply status:|Errors:' test-tmp3-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtspec4:
	@echo "Running test 4"
	$(CSH) -c "$(USERVER) $(SPECOPTS) >& test4.out &"
	sleep 4
	$(CSH) -c "ssh $(BASECLIENT1) '$(CHOME)/runaddrsesslog $(BASEIP1) 1000 40000 $(CHOME)/httpspec99-d-1.log' >& test4-httperf.out"
	@egrep 'Reply status:|Errors:' test4-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtspec4b:
	@echo "Running test 4b"
	$(CSH) -c "$(USERVER) $(SPECOPTS) >& test4b.out &"
	sleep 4
	$(CSH) -c "ssh $(BASECLIENT1) '$(CHOME)/runaddrsesslog $(BASEIP1) 1000 300000 $(CHOME)/httpspec99-d-1.log' >& test4b-httperf.out"
	@egrep 'Reply status:|Errors:' test4b-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtspec4c:
	@echo "Running test 4c"
	$(CSH) -c "$(USERVER) $(SPECOPTS) >& test4c.out &"
	sleep 4
	$(CSH) -c "ssh $(BASECLIENT1) '$(CHOME)/runaddrsesslog $(BASEIP1) 1020 40000 $(CHOME)/httpspec99-d-1.log' >& test4c-httperf.out"
	@egrep 'Reply status:|Errors:' test4c-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtspec4d:
	@echo "Running test 4d"
	$(CSH) -c "$(USERVER) $(SPECOPTS) >& test4d.out &"
	sleep 4
	$(CSH) -c "ssh $(BASECLIENT1) '$(CHOME)/runaddrsesslog $(BASEIP1) 1040 40000 $(CHOME)/httpspec99-d-1.log' >& test4d-httperf.out"
	@egrep 'Reply status:|Errors:' test4d-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtspec4e:
	@echo "Running test 4e"
	$(CSH) -c "$(USERVER) $(SPECOPTS) >& test4e.out &"
	sleep 4
	$(CSH) -c "ssh $(BASECLIENT1) '$(CHOME)/runaddrsesslog $(BASEIP1) 1060 40000 $(CHOME)/httpspec99-d-1.log' >& test4e-httperf.out"
	@egrep 'Reply status:|Errors:' test4e-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtspec4f:
	@echo "Running test 4f"
	$(CSH) -c "$(USERVER) $(SPECOPTS) >& test4f.out &"
	sleep 4
	$(CSH) -c "ssh $(BASECLIENT1) '$(CHOME)/runaddrsesslog $(BASEIP1) 1080 40000 $(CHOME)/httpspec99-d-1.log' >& test4f-httperf.out"
	@egrep 'Reply status:|Errors:' test4f-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

rtspec8:
	@echo "Running test 8"
	$(CSH) -c "$(USERVER) $(SPECOPTS) --ip-addr $(BASEIP1) --ip-addr $(BASEIP2) >& test8.out &"
	sleep 4
	$(CSH) -c "ssh $(BASECLIENT1) '$(CHOME)/runaddrsesslog $(BASEIP1) 1000 40000 $(CHOME)/httpspec99-d-1.log' >& test8a-httperf.out&" 
	$(CSH) -c "ssh $(BASECLIENT2) '$(CHOME)/runaddrsesslog $(BASEIP2) 1000 40000 $(CHOME)/httpspec99-d-1.log' >& test8b-httperf.out"
	sleep 10
	@egrep 'Reply status:|Errors:' test8a-httperf.out
	@egrep 'Reply status:|Errors:' test8b-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2


rtspec8-new:
	@echo "Running test 8-new"
	$(CSH) -c "$(USERVER) $(SPECOPTS) --ip-addr $(BASEIP1) --ip-addr $(BASEIP2) >& test8-new.out &"
	sleep 4
	$(CSH) -c "ssh $(BASECLIENT1) '$(CHOME)/runaddrsesslog $(BASEIP1) 1040 40000 $(CHOME)/httpspec99-d-1.log' >& test8-new-a-httperf.out&" 
	$(CSH) -c "ssh $(BASECLIENT2) '$(CHOME)/runaddrsesslog $(BASEIP2) 1040 40000 $(CHOME)/httpspec99-d-1.log' >& test8-new-b-httperf.out"
	sleep 10
	@egrep 'Reply status:|Errors:' test8-new-a-httperf.out
	@egrep 'Reply status:|Errors:' test8-new-b-httperf.out
	sleep 2
	@./scripts/kill-server
	sleep 2

#-----------------------------------------------------------------------
# Test working set size 
#

WSOPTS = -C --use-sendfile --use-tcp-cork --cache-table-size 5 --stats-interval=1
testws1: FORCE
	@echo "Running Working Set Test"
	@echo "Running test 1"
	$(CSH) -c "./userver $(WSOPTS) --max-conns 1 -f 10 >& test1.out &"
	sleep 4
	$(CSH) -c "./scripts/run 100 >& test1-httperf.out"
	egrep 'Reply status:|Errors:' test1-httperf.out
	@./scripts/kill-server
	sleep 4
	@echo ""

testws2: FORCE
	@echo "Running Working Set Test"
	@echo "Running test 2"
	$(CSH) -c "./userver $(WSOPTS) -c 5 -f 20 >& test2.out &"
	sleep 4
	$(CSH) -c "./scripts/runrate 500 5000 >& test2-httperf.out"
	egrep 'Reply status:|Errors:' test2-httperf.out
	@./scripts/kill-server
	sleep 4
	@echo ""

testws3: FORCE
	@echo "Running Working Set Test"
	@echo "Running test 3"
	$(CSH) -c "./userver $(WSOPTS) -c 10 -f 25 >& test3.out &"
	sleep 4
	$(CSH) -c "./scripts/runrate 2000 20000 >& test3-httperf.out"
	egrep 'Reply status:|Errors:' test3-httperf.out
	@./scripts/kill-server
	sleep 4
	@echo ""

#-----------------------------------------------------------------------
# Test extra accepts

testextra1:  FORCE
	@echo "Running test 1 - should get 1000 replies and 0 errors in about 10 seconds"
	$(CSH) -c "./userver -C --use-sendfile --use-tcp-cork -m 0 -e 1 -c 450 -f 1000 $(OPTS) >& test1.out &"
	sleep 4
	$(CSH) -c "./scripts/runrate 100 1000 >& test1-httperf.out"
	egrep 'Reply status:|Errors:' test1-httperf.out
	@./scripts/kill-server
	sleep 4
	@echo ""

testextra2:  FORCE
	@echo "Running test 2 - should get 1000 replies and 0 errors in about 10 seconds"
	$(CSH) -c "./userver -C --use-sendfile --use-tcp-cork -m 0 --extra-accepts 1 -c 450 -f 1000 $(OPTS) >& test2.out &"
	sleep 4
	$(CSH) -c "./scripts/runrate 100 1000 >& test2-httperf.out"
	egrep 'Reply status:|Errors:' test2-httperf.out
	@./scripts/kill-server
	sleep 4
	@echo ""

#-----------------------------------------------------------------------
# Trying to create test to trigger the problem with EPOLLHUPs
# not getting handled properly. 
# Often happens when a EPOLLHUP arrives on the client while
# it is waiting for a response from the appserver.
# The socket ends up stuck in
# a mode where we keep getting EPOLLHUPs.

testhup: testhup0 testhup1 testhup2 testhup3 testhup4 testhup5 testhup6 testhup7 \
         testhup8 testhup9 testhup10 testhup11 testhup12 testhup13 testhup14 testhup15 \
         testhup16 testhup17 testhup18 testhup19 testhup20
#
# in micro seconds (2 seconds)
# Note that in this case we don't want to kill the userver
# until after the delay goes off and it processes the request.
HUPDELAY = --delay 2000000 --state-delay 17

testhup0:
	@echo "Running test hup 0 - should get 1 replies and 0 errors"
	fastcgi/specweb99-fcgi.pl $(UNIX_DOMAIN_SOCK) > test0-fcgi.out &
	@sleep 4
	$(CSH) -c "$(USERVER) --use-epoll --epoll-modify-listeners -c 5 -f 10 -m 0 -C --num-dyn-buffers 10 --dyn-buffer-size 1000000 $(DYN_BASE) \
	     $(DYN_FCGI_UDS) > test0.out &"
	@sleep 4
	$(CSH) -c "./scripts/run-hup-reset1 2 > test0-httperf.out"
	@@./scripts/kill-server
	@grep ' 2xx=2 ' test0-httperf.out && egrep 'Errors:' test0-httperf.out
	@grep 'Errors: total 0 ' test0-httperf.out
	pkill specweb99
	@sleep 2
	@echo ""

# This test doesn't actually generate a HUP as it uses select
# it is included to ensure that a solution to the EPOLLHUP 
# doesn't mess up the select case.
testhup1:
	@echo "Running test hup 1 - should get 0 replies and 1 errors"
	fastcgi/specweb99-fcgi-hup.pl $(UNIX_DOMAIN_SOCK) > test1-fcgi.out &
	@sleep 4
	$(CSH) -c "$(USERVER) -c 5 -f 100 -m 0 -C --num-dyn-buffers 10 --dyn-buffer-size 1000000 $(DYN_BASE) \
	     $(DYN_FCGI_UDS) > test1.out &"
	@sleep 4
	$(CSH) -c "./scripts/run-hup-reset1 1 > test1-httperf.out"
	@grep ' 2xx=0 ' test1-httperf.out && egrep 'Errors:' test1-httperf.out
	@grep 'client-timo 1 ' test1-httperf.out
	sleep 10
	@@./scripts/kill-server
	pkill specweb99
	@sleep 2
	@echo ""

testhup2:
	@echo "Running test hup 2 - should get 0 replies and 1 errors"
	fastcgi/specweb99-fcgi-hup.pl $(UNIX_DOMAIN_SOCK) > test2-fcgi.out &
	@sleep 4
	$(CSH) -c "$(USERVER) --use-epoll --epoll-modify-listeners -c 5 -f 100 -m 0 -C --num-dyn-buffers 10 --dyn-buffer-size 1000000 $(DYN_BASE) \
	     $(DYN_FCGI_UDS) > test2.out &"
	@sleep 4
	$(CSH) -c "./scripts/run-hup-reset1 1 > test2-httperf.out"
	sleep 10
	@@./scripts/kill-server
	@grep ' 2xx=0 ' test2-httperf.out && egrep 'Errors:' test2-httperf.out
	@grep 'client-timo 1 ' test2-httperf.out
	pkill specweb99
	@sleep 2
	@echo ""

testhup3:
	@echo "Running test hup 3 - should get 0 replies and 2 errors"
	fastcgi/specweb99-fcgi-hup.pl $(UNIX_DOMAIN_SOCK) > test3-fcgi.out &
	@sleep 4
	$(CSH) -c "$(USERVER) --use-epoll --epoll-modify-listeners -c 5 -f 100 -m 0 -C --num-dyn-buffers 10 --dyn-buffer-size 1000000 $(DYN_BASE) \
	     $(DYN_FCGI_UDS) > test3.out &"
	@sleep 4
	$(CSH) -c "./scripts/run-hup-reset1 2 > test3-httperf.out"
	sleep 15
	@@./scripts/kill-server
	@grep ' 2xx=0 ' test3-httperf.out && egrep 'Errors:' test3-httperf.out
	@grep 'client-timo 2 ' test3-httperf.out
	pkill specweb99
	@sleep 2
	@echo ""

testhup4:
	@echo "Running test hup 4 - should get 0 replies and 1 errors"
	@echo " and 1 reply and 0 errors"
	fastcgi/specweb99-fcgi-hup.pl $(UNIX_DOMAIN_SOCK) > test4-fcgi.out &
	@sleep 4
	$(CSH) -c "$(USERVER) --use-epoll --epoll-modify-listeners -c 20 -f 100 \
             -m 0 -C --num-dyn-buffers 10 --dyn-buffer-size 1000000 $(DYN_BASE) \
	     $(DYN_FCGI_UDS) > test4.out &"
	@sleep 4
	$(CSH) -c "./scripts/run-hup-reset1 1 > test4-httperf-a.out &" 
	# @sleep 1
	$(CSH) -c "./scripts/runrate 1 10 > test4-httperf-b.out &" 
	sleep 15
	@@./scripts/kill-server
	@grep ' 2xx=0 ' test4-httperf-a.out && egrep 'Errors:' test4-httperf-a.out
	@grep 'client-timo 1 ' test4-httperf-a.out
	@grep ' 2xx=10 ' test4-httperf-b.out && egrep 'Errors:' test4-httperf-b.out
	pkill specweb99
	@sleep 2
	@echo ""


testhup5:
	@echo "Running test hup 5 - should get 0 replies and 1 errors"
	@echo " and 1 reply and 0 errors"
	fastcgi/specweb99-fcgi-hup.pl  > test5-fcgi.out &
	@sleep 4
	$(CSH) -c "$(USERVER) --use-epoll --epoll-modify-listeners -c 5 -f 100 \
             -m 0 -C --num-dyn-buffers 10 --dyn-buffer-size 1000000 $(DYN_BASE) \
	     $(DYN_FCGI) > test5.out &"
	@sleep 4
	$(CSH) -c "./scripts/run-hup-reset1 1 > test5-httperf-a.out &" 
	$(CSH) -c "./scripts/runrate 100 1000 > test5-httperf-b.out &" 
	sleep 15
	@@./scripts/kill-server
	@grep ' 2xx=0 ' test5-httperf-a.out && egrep 'Errors:' test5-httperf-a.out
	@grep 'client-timo 1 ' test5-httperf-a.out
	@grep ' 2xx=1000 ' test5-httperf-b.out && egrep 'Errors:' test5-httperf-b.out
	pkill specweb99
	@sleep 2
	@echo ""


testhup6:
	@echo "Running test hup 6 - should get 0 replies and 1 errors"
	@echo " and 1 reply and 0 errors"
	fastcgi/specweb99-fcgi-hup.pl  > test6-fcgi.out &
	@sleep 4
	$(CSH) -c "$(USERVER) --use-epoll --epoll-modify-listeners -c 5 -f 100 \
             -m 0 -C --num-dyn-buffers 10 --dyn-buffer-size 1000000 $(DYN_BASE) \
	     $(DYN_FCGI) > test6.out &"
	@sleep 4
	$(CSH) -c "./scripts/run-hup-reset1 1 > test6-httperf-a.out &" 
	$(CSH) -c "./scripts/runrate 1 10 > test6-httperf-b.out &" 
	sleep 15
	@@./scripts/kill-server
	@grep ' 2xx=0 ' test6-httperf-a.out && egrep 'Errors:' test6-httperf-a.out
	@grep 'client-timo 1 ' test6-httperf-a.out
	@grep ' 2xx=10 ' test6-httperf-b.out && egrep 'Errors:' test6-httperf-b.out
	pkill specweb99
	@sleep 2
	@echo ""


testhup7:
	@echo "Running test hup 7 - should get 0 replies and 1 errors"
	@echo " and 1 reply and 0 errors"
	fastcgi/specweb99-fcgi-hup.pl  > test7-fcgi.out &
	@sleep 4
	$(CSH) -c "$(USERVER) --use-epoll --epoll-modify-listeners -c 5 -f 100 \
             -m 0 -C --num-dyn-buffers 10 --dyn-buffer-size 1000000 $(DYN_BASE) \
	     $(DYN_FCGI) > test7.out &"
	@sleep 4
	$(CSH) -c "./scripts/run-hup-reset1 1 > test7-httperf-a.out &" 
	$(CSH) -c "./scripts/runrate 1 10 > test7-httperf-b.out &"  
	sleep 1
  # Do another test on a version of the appserver to see if it
  # sends the request to the same appserver properly
	$(CSH) -c "./scripts/run-hup-reset1 1 > test7-httperf-c.out &" 
	sleep 15
	@@./scripts/kill-server
	@grep ' 2xx=0 ' test7-httperf-a.out && egrep 'Errors:' test7-httperf-a.out
	@grep 'client-timo 1 ' test7-httperf-a.out
	@grep ' 2xx=10 ' test7-httperf-b.out && egrep 'Errors:' test7-httperf-b.out
	pkill specweb99
	@sleep 2
	@echo ""

testhup8:
	@echo "Running test hup 8 - should get 0 replies and 5 errors"
	fastcgi/specweb99-fcgi-hup.pl $(UNIX_DOMAIN_SOCK) > test8-fcgi.out &
	@sleep 4
	$(CSH) -c "$(USERVER) --use-epoll --epoll-modify-listeners -c 10 -f 100 -m 0 -C --num-dyn-buffers 10 --dyn-buffer-size 1000000 $(DYN_BASE) \
	     $(DYN_FCGI_UDS) > test8.out &"
	@sleep 4
	$(CSH) -c "./scripts/run-hup-reset1 1 > test8a-httperf.out &"
	$(CSH) -c "./scripts/run-hup-reset1 1 > test8b-httperf.out &"
	$(CSH) -c "./scripts/run-hup-reset1 1 > test8c-httperf.out &"
	$(CSH) -c "./scripts/run-hup-reset1 1 > test8d-httperf.out &"
	$(CSH) -c "./scripts/run-hup-reset1 1 > test8e-httperf.out &"
	sleep 15
	@@./scripts/kill-server
	@grep ' 2xx=0 ' test8a-httperf.out
	@grep 'client-timo 1 ' test8a-httperf.out
	@grep ' 2xx=0 ' test8b-httperf.out
	@grep 'client-timo 1 ' test8b-httperf.out
	@grep ' 2xx=0 ' test8c-httperf.out
	@grep 'client-timo 1 ' test8c-httperf.out
	@grep ' 2xx=0 ' test8d-httperf.out
	@grep 'client-timo 1 ' test8d-httperf.out
	@grep ' 2xx=0 ' test8e-httperf.out
	@grep 'client-timo 1 ' test8e-httperf.out
	pkill specweb99
	@sleep 2
	@echo ""


testhup9:
	@echo "Running test hup 9 - should get 0 replies and 5 errors"
	fastcgi/specweb99-fcgi-hup.pl $(UNIX_DOMAIN_SOCK) > test9-fcgi.out &
	@sleep 4
	$(CSH) -c "$(USERVER) --use-epoll --epoll-modify-listeners -c 10 -f 100 -m 0 -C --num-dyn-buffers 10 --dyn-buffer-size 1000000 $(DYN_BASE) \
	     $(DYN_FCGI_UDS) > test9.out &"
	@sleep 4
	$(CSH) -c "./scripts/run-hup-reset1 5 > test9-httperf.out"
	sleep 15
	@@./scripts/kill-server
	@grep ' 2xx=0 ' test9-httperf.out && egrep 'Errors:' test9-httperf.out
	@grep 'client-timo 5 ' test9-httperf.out
	pkill specweb99
	@sleep 2
	@echo ""

testhup10:
	@echo "Running test hup 10 - should get 2 replies and 2 errors"
	fastcgi/specweb99-fcgi-hup.pl $(UNIX_DOMAIN_SOCK) > test10-fcgi.out &
	@sleep 4
	$(CSH) -c "$(USERVER) --use-epoll --epoll-modify-listeners -c 10 -f 100 -m 0 -C --num-dyn-buffers 10 --dyn-buffer-size 1000000 $(DYN_BASE) \
	     $(DYN_FCGI_UDS) > test10.out &"
	@sleep 4
	$(CSH) -c "./scripts/run-hup1 2 > test10-httperf-a.out &"
	$(CSH) -c "./scripts/run-hup10 2 > test10-httperf-b.out &"
	sleep 15
	@@./scripts/kill-server
	@grep ' 2xx=0 ' test10-httperf-a.out
	@grep 'client-timo 2 ' test10-httperf-a.out
	@grep ' 2xx=2 ' test10-httperf-b.out
	@grep 'client-timo 0 ' test10-httperf-b.out
	pkill specweb99
	@sleep 2
	@echo ""

testhup11:
	@echo "Running test hup 11"
	# specweb99-fcgi-hup-some.pl sleeps for 2 seconds on every 2nd request
	# this allows us to ensure that requests fail and others work.
	fastcgi/specweb99-fcgi-hup-some.pl $(UNIX_DOMAIN_SOCK) > test11-fcgi.out &
	@sleep 4
	$(CSH) -c "$(USERVER) --use-epoll --epoll-modify-listeners -c 10 -f 100 -m 0 -C --num-dyn-buffers 10 --dyn-buffer-size 1000000 $(DYN_BASE) \
	     $(DYN_FCGI_UDS) > test11.out &"
	@sleep 4
	$(CSH) -c "./scripts/run-hup-reset1 1 > test11-httperf-a.out"
	$(CSH) -c "./scripts/run-hup-reset1 1 > test11-httperf-b.out"
	$(CSH) -c "./scripts/run-hup-reset1 1 > test11-httperf-c.out"
	$(CSH) -c "./scripts/run-hup-reset1 1 > test11-httperf-d.out"
	$(CSH) -c "./scripts/run-hup-reset1 1 > test11-httperf-e.out"
	sleep 5
	@@./scripts/kill-server
	pkill specweb99
	@grep ' 2xx=0 ' test11-httperf-a.out
	@grep 'client-timo 1 ' test11-httperf-a.out
	@grep ' 2xx=1 ' test11-httperf-b.out
	@grep 'client-timo 0 ' test11-httperf-b.out
	@grep ' 2xx=0 ' test11-httperf-c.out
	@grep 'client-timo 1 ' test11-httperf-c.out
	@grep ' 2xx=1 ' test11-httperf-d.out
	@grep 'client-timo 0 ' test11-httperf-d.out
	@grep ' 2xx=0 ' test11-httperf-e.out
	@grep 'client-timo 1 ' test11-httperf-e.out
	@sleep 2
	@echo ""

testhup12:
	@echo "Running test hup 12"
	# specweb99-fcgi-hup-some.pl sleeps for 2 seconds on every 2nd request
	# this allows us to ensure that requests fail and others work.
	fastcgi/specweb99-fcgi-hup-some.pl $(UNIX_DOMAIN_SOCK) > test12-fcgi.out &
	@sleep 4
	$(CSH) -c "$(USERVER) --use-epoll --epoll-modify-listeners -c 10 -f 100 -m 0 -C --num-dyn-buffers 10 --dyn-buffer-size 1000000 $(DYN_BASE) \
	     $(DYN_FCGI_UDS) > test12.out &"
	@sleep 4
	$(CSH) -c "./scripts/run-hup1 1 > test12-httperf-a.out"
	$(CSH) -c "./scripts/run-hup1 1 > test12-httperf-b.out"
	$(CSH) -c "./scripts/run-hup1 1 > test12-httperf-c.out"
	$(CSH) -c "./scripts/run-hup1 1 > test12-httperf-d.out"
	$(CSH) -c "./scripts/run-hup1 1 > test12-httperf-e.out"
	sleep 5
	@@./scripts/kill-server
	pkill specweb99
	@grep ' 2xx=0 ' test12-httperf-a.out
	@grep 'client-timo 1 ' test12-httperf-a.out
	@grep ' 2xx=1 ' test12-httperf-b.out
	@grep 'client-timo 0 ' test12-httperf-b.out
	@grep ' 2xx=0 ' test12-httperf-c.out
	@grep 'client-timo 1 ' test12-httperf-c.out
	@grep ' 2xx=1 ' test12-httperf-d.out
	@grep 'client-timo 0 ' test12-httperf-d.out
	@grep ' 2xx=0 ' test12-httperf-e.out
	@grep 'client-timo 1 ' test12-httperf-e.out
	@sleep 2
	@echo ""

testhup13:
	@echo "Running test hup 13 -- just make sure that things don't crash"
	# specweb99-fcgi-hup-some.pl sleeps for 2 seconds on every 2nd request
	# this allows us to ensure that requests fail and others work.
	fastcgi/specweb99-fcgi-hup-some.pl $(UNIX_DOMAIN_SOCK) > test13-fcgi.out &
	@sleep 4
	$(CSH) -c "$(USERVER) --use-epoll --epoll-modify-listeners -c 50 -f 1000 -m 0 -C --num-dyn-buffers 10 --dyn-buffer-size 1000000 $(DYN_BASE) \
	     $(DYN_FCGI_UDS) > test13.out &"
	@sleep 4
	$(CSH) -c "./scripts/runspecdynamic-rate-hup1 100 100 > test13-httperf-a.out &"
	$(CSH) -c "./scripts/runspecdynamic-rate-hup10 100 100 > test13-httperf-b.out"
	@sleep 20
	@@./scripts/kill-server
	pkill specweb99
	@grep ' 2xx=' test13-httperf-a.out
	@grep 'client-timo' test13-httperf-a.out
	@grep ' 2xx=' test13-httperf-b.out
	@grep 'client-timo' test13-httperf-b.out
	@sleep 2
	@echo ""


testhup14:
	@echo "Running test hup 14 -- just make sure that things don't crash"
	# specweb99-fcgi-hup-some.pl sleeps for 2 seconds on every 2nd request
	# this allows us to ensure that requests fail and others work.
	fastcgi/specweb99-fcgi-hup-some.pl $(UNIX_DOMAIN_SOCK) > test14-fcgi.out &
	@sleep 20
	$(CSH) -c "$(USERVER) --use-epoll --epoll-modify-listeners -c 50 -f 1000 -m 0 -C --num-dyn-buffers 10 --dyn-buffer-size 1000000 $(DYN_BASE) \
	     $(DYN_FCGI_UDS) > test14.out &"
	@sleep 4
	$(CSH) -c "./scripts/runspecdynamic-rate-hup10 100 100 > test14-httperf-a.out"
	$(CSH) -c "./scripts/runspecdynamic-rate-hup1 100 100 > test14-httperf-b.out"
	$(CSH) -c "./scripts/runspecdynamic-rate-hup10 100 100 > test14-httperf-c.out"
	@sleep 4
	@@./scripts/kill-server
	pkill specweb99
	@grep ' 2xx=' test14-httperf-a.out
	@grep 'client-timo' test14-httperf-a.out
	@grep ' 2xx=' test14-httperf-b.out
	@grep 'client-timo' test14-httperf-b.out
	@grep ' 2xx=' test14-httperf-c.out
	@grep 'client-timo' test14-httperf-c.out
	@sleep 2
	@echo ""

testhup15:
	@echo "Running test hup 15 - should get 0 replies and 1 errors"
	@echo " and 1 reply and 0 errors"
	fastcgi/specweb99-fcgi-hup.pl $(UNIX_DOMAIN_SOCK) > test15-fcgi.out &
	@sleep 4
	$(CSH) -c "$(USERVER) --use-epoll --epoll-modify-listeners -c 5 -f 100 \
             -m 0 -C --num-dyn-buffers 10 --dyn-buffer-size 1000000 $(DYN_BASE) \
	     $(DYN_FCGI_UDS) > test15.out &"
	@sleep 4
	$(CSH) -c "./scripts/run-hup-reset1 1 > test15-httperf-a.out &" 
	# @sleep 1
	$(CSH) -c "./scripts/runrate 1 10 > test15-httperf-b.out &" 
	sleep 15
	@@./scripts/kill-server
	@grep ' 2xx=0 ' test15-httperf-a.out && egrep 'Errors:' test15-httperf-a.out
	@grep 'client-timo 1 ' test15-httperf-a.out
	@grep ' 2xx=10 ' test15-httperf-b.out && egrep 'Errors:' test15-httperf-b.out
	pkill specweb99
	@sleep 2
	@echo ""

testhup16:
	@echo "Running test hup 16 -- just make sure that things don't crash"
	# specweb99-fcgi-hup-some.pl sleeps for 2 seconds on every 2nd request
	# this allows us to ensure that requests fail and others work.
	fastcgi/specweb99-fcgi-hup-some.pl $(UNIX_DOMAIN_SOCK) > test16-fcgi.out &
	@sleep 4
	$(CSH) -c "$(USERVER) --use-epoll --epoll-modify-listeners --no-exit-on-no-buffers -c 50 -f 1000 -m 0 -C --num-dyn-buffers 1 --dyn-buffer-size 1000000 $(DYN_BASE) \
	     $(DYN_FCGI_UDS) > test16.out &"
	@sleep 4
	$(CSH) -c "./scripts/runspecdynamic-rate-hup10 10 10000 > test16-httperf-a.out"
	$(CSH) -c "./scripts/runspecdynamic-rate-hup1 10 10000 > test16-httperf-b.out"
	$(CSH) -c "./scripts/runspecdynamic-rate-hup10 10 10000 > test16-httperf-c.out"
	sleep 15
	@@./scripts/kill-server
	pkill specweb99
	@grep ' 2xx=' test16-httperf-a.out
	@grep 'client-timo' test16-httperf-a.out
	@grep ' 2xx=' test16-httperf-b.out
	@grep 'client-timo' test16-httperf-b.out
	@grep ' 2xx=' test16-httperf-c.out
	@grep 'client-timo' test16-httperf-c.out
	@sleep 2
	@echo ""

testhup17:
	@echo "Running test hup 17 -- just make sure that things don't crash"
	# specweb99-fcgi-hup-some.pl sleeps for 2 seconds on every 2nd request
	# this allows us to ensure that requests fail and others work.
	fastcgi/specweb99-fcgi-hup-some.pl $(UNIX_DOMAIN_SOCK) > test17-fcgi.out &
	@sleep 4
	$(CSH) -c "$(USERVER) --use-epoll --epoll-modify-listeners --no-exit-on-no-buffers -c 50 -f 1000 -m 0 -C --num-dyn-buffers 1 --dyn-buffer-size 1000000 $(DYN_BASE) \
	     $(DYN_FCGI_UDS) > test17.out &"
	@sleep 4
	$(CSH) -c "./scripts/runspecdynamic-rate-hup10 5 10000 > test17-httperf-a.out &"
	$(CSH) -c "./scripts/runspecdynamic-rate-hup1 5 10000 > test17-httperf-b.out &"
	$(CSH) -c "./scripts/runspecdynamic-rate-hup10 5 10000 > test17-httperf-c.out &"
	@sleep 20
	@@./scripts/kill-server
	pkill specweb99
	@grep ' 2xx=' test17-httperf-a.out
	@grep 'client-timo' test17-httperf-a.out
	@grep ' 2xx=' test17-httperf-b.out
	@grep 'client-timo' test17-httperf-b.out
	@grep ' 2xx=' test17-httperf-c.out
	@grep 'client-timo' test17-httperf-c.out
	@sleep 2
	@echo ""


testhup18:
	@echo "Running test hup 18 -- just make sure that things don't crash"
	fastcgi/specweb99-fcgi-hup-some.pl $(UNIX_DOMAIN_SOCK) > test18-fcgi.out &
	@sleep 4
	$(CSH) -c "$(USERVER) --no-exit-on-no-buffers --use-epoll --epoll-modify-listeners -c 2 -f 1000 -m 0 -C --num-dyn-buffers 1 --dyn-buffer-size 1000000 $(DYN_BASE) \
	     $(DYN_FCGI_UDS) > test18.out &"
	@sleep 4
	$(CSH) -c "./scripts/runspecdynamic-rate-hup1 4 100 > test18-httperf-a.out &"
	$(CSH) -c "./scripts/runspecdynamic-rate-hup10 4 100 > test18-httperf-b.out"
	@sleep 4
	@@./scripts/kill-server
	pkill specweb99
	@grep ' 2xx=0' test18-httperf-a.out
	@grep 'client-timo 4' test18-httperf-a.out
	@grep ' 2xx=4' test18-httperf-b.out
	@grep 'client-timo 0' test18-httperf-b.out
	@grep 'iffy_sd_count =          0' test18.out
	@sleep 2
	@echo ""

testhup19:
	@echo "Running test hup 19 -- just make sure that things don't crash"
	fastcgi/specweb99-fcgi-hup-some.pl $(UNIX_DOMAIN_SOCK) > test19-fcgi.out &
	@sleep 4
	$(CSH) -c "$(USERVER) --no-exit-on-no-buffers --use-epoll --epoll-modify-listeners -c 5 -f 1000 -m 0 -C --num-dyn-buffers 10 --dyn-buffer-size 1000000 $(DYN_BASE) \
	     $(DYN_FCGI_UDS) > test19.out &"
	@sleep 4
	$(CSH) -c "./scripts/runspecdynamic-rate-hup1 10 100 > test19-httperf-a.out &"
	$(CSH) -c "./scripts/runspecdynamic-rate-hup10 10 100 > test19-httperf-b.out"
	@sleep 15
	@@./scripts/kill-server
	pkill specweb99
	@grep ' 2xx=' test19-httperf-a.out
	@grep 'client-timo' test19-httperf-a.out
	@grep ' 2xx=' test19-httperf-b.out
	@grep 'client-timo' test19-httperf-b.out
	grep 'iffy_sd_count =          0' test19.out
	@sleep 2
	@echo ""


testhup20:
	@echo "Running test hup 20 -- just make sure that things don't crash"
	# specweb99-fcgi-hup-some.pl sleeps for 2 seconds on every 2nd request
	# this allows us to ensure that requests fail and others work.
	fastcgi/specweb99-fcgi-hup-some.pl $(UNIX_DOMAIN_SOCK) > test20-fcgi.out &
	@sleep 4
	$(CSH) -c "$(USERVER) --use-epoll --epoll-modify-listeners -c 50 -f 1000 -m 0 -C --num-dyn-buffers 10 --dyn-buffer-size 1000000 $(DYN_BASE) \
	     $(DYN_FCGI_UDS) > test20.out &"
	@sleep 4
	$(CSH) -c "./scripts/runspecdynamic-rate-hup10 100 100 > test20-httperf-b.out &"
	$(CSH) -c "./scripts/runspecdynamic-rate-hup1 100 100 > test20-httperf-a.out "
	@sleep 60
	@@./scripts/kill-server
	pkill specweb99
	@grep ' 2xx=' test20-httperf-a.out
	@grep 'client-timo' test20-httperf-a.out
	@grep ' 2xx=' test20-httperf-b.out
	@grep 'client-timo' test20-httperf-b.out
	@sleep 2
	@echo ""

#-----------------------------------------------------------------------
newtesthup: newtesthup0 newtesthup1 newtesthup2 newtesthup3 newtesthup4 \
            newtesthup5 newtesthup6 newtesthup7 newtesthup8 \
            newtesthup9 newtesthup10 newtesthup11 newtesthup12 newtesthup13 \
            newtesthup14

newtesthup0:
	@echo "Running test hup 0 - should get 1 replies and 0 errors"
	fastcgi/specweb99-fcgi.pl $(UNIX_DOMAIN_SOCK) > test0-fcgi.out &
	@sleep 4
	$(CSH) -c "$(USERVER) --use-epoll --epoll-modify-listeners -c 5 -f 10 -m 0 -C --num-dyn-buffers 10 --dyn-buffer-size 1000000 $(DYN_BASE) \
	     $(DYN_FCGI_UDS) > test0.out &"
	@sleep 4
	$(CSH) -c "./scripts/run-hup1 2 > test0-httperf.out"
	@@./scripts/kill-server
	@grep ' 2xx=2 ' test0-httperf.out && egrep 'Errors:' test0-httperf.out
	@grep 'Errors: total 0 ' test0-httperf.out
	pkill specweb99
	@sleep 2
	@echo ""

# This test doesn't actually generate a HUP as it uses select
# it is included to ensure that a solution to the EPOLLHUP 
# doesn't mess up the select case.
newtesthup1:
	@echo "Running test hup 1 - should get 0 replies and 1 errors"
	fastcgi/specweb99-fcgi-hup.pl $(UNIX_DOMAIN_SOCK) > test1-fcgi.out &
	@sleep 4
	$(CSH) -c "$(USERVER) -c 5 -f 100 -m 0 -C --num-dyn-buffers 10 --dyn-buffer-size 1000000 $(DYN_BASE) \
	     $(DYN_FCGI_UDS) > test1.out &"
	@sleep 4
	$(CSH) -c "./scripts/run-hup1 1 > test1-httperf.out"
	@grep ' 2xx=0 ' test1-httperf.out && egrep 'Errors:' test1-httperf.out
	@grep 'client-timo 1 ' test1-httperf.out
	sleep 10
	@@./scripts/kill-server
	pkill specweb99
	@sleep 2
	@echo ""

newtesthup2:
	@echo "Running test hup 2 - should get 0 replies and 1 errors"
	fastcgi/specweb99-fcgi-hup.pl $(UNIX_DOMAIN_SOCK) > test2-fcgi.out &
	@sleep 4
	$(CSH) -c "$(USERVER) --use-epoll --epoll-modify-listeners -c 5 -f 100 -m 0 -C --num-dyn-buffers 10 --dyn-buffer-size 1000000 $(DYN_BASE) \
	     $(DYN_FCGI_UDS) > test2.out &"
	@sleep 4
	$(CSH) -c "./scripts/run-hup1 1 > test2-httperf.out"
	sleep 10
	@@./scripts/kill-server
	@grep ' 2xx=0 ' test2-httperf.out && egrep 'Errors:' test2-httperf.out
	@grep 'client-timo 1 ' test2-httperf.out
	pkill specweb99
	@sleep 2
	@echo ""

newtesthup3:
	@echo "Running test hup 3 - should get 0 replies and 2 errors"
	fastcgi/specweb99-fcgi-hup.pl $(UNIX_DOMAIN_SOCK) > test3-fcgi.out &
	@sleep 4
	$(CSH) -c "$(USERVER) --use-epoll --epoll-modify-listeners -c 5 -f 100 -m 0 -C --num-dyn-buffers 10 --dyn-buffer-size 1000000 $(DYN_BASE) \
	     $(DYN_FCGI_UDS) > test3.out &"
	@sleep 4
	$(CSH) -c "./scripts/run-hup1 2 > test3-httperf.out"
	sleep 15
	@@./scripts/kill-server
	@grep ' 2xx=0 ' test3-httperf.out && egrep 'Errors:' test3-httperf.out
	@grep 'client-timo 2 ' test3-httperf.out
	pkill specweb99
	@sleep 2
	@echo ""

newtesthup4:
	@echo "Running test hup 4 - should get 0 replies and 1 errors"
	@echo " and 1 reply and 0 errors"
	fastcgi/specweb99-fcgi-hup.pl $(UNIX_DOMAIN_SOCK) > test4-fcgi.out &
	@sleep 4
	$(CSH) -c "$(USERVER) --use-epoll --epoll-modify-listeners -c 5 -f 100 \
             -m 0 -C --num-dyn-buffers 10 --dyn-buffer-size 1000000 $(DYN_BASE) \
	     $(DYN_FCGI_UDS) > test4.out &"
	@sleep 4
	$(CSH) -c "./scripts/run-hup1 1 > test4-httperf-a.out &" 
	# @sleep 1
	$(CSH) -c "./scripts/runrate 1 10 > test4-httperf-b.out &" 
	sleep 15
	@@./scripts/kill-server
	@grep ' 2xx=0 ' test4-httperf-a.out && egrep 'Errors:' test4-httperf-a.out
	@grep 'client-timo 1 ' test4-httperf-a.out
	@grep ' 2xx=10 ' test4-httperf-b.out && egrep 'Errors:' test4-httperf-b.out
	pkill specweb99
	@sleep 2
	@echo ""


newtesthup5:
	@echo "Running test hup 5 - should get 0 replies and 1 errors"
	@echo " and 1 reply and 0 errors"
	fastcgi/specweb99-fcgi-hup.pl  > test5-fcgi.out &
	@sleep 4
	$(CSH) -c "$(USERVER) --use-epoll --epoll-modify-listeners -c 5 -f 100 \
             -m 0 -C --num-dyn-buffers 10 --dyn-buffer-size 1000000 $(DYN_BASE) \
	     $(DYN_FCGI) > test5.out &"
	@sleep 4
	$(CSH) -c "./scripts/run-hup1 1 > test5-httperf-a.out &" 
	$(CSH) -c "./scripts/runrate 100 1000 > test5-httperf-b.out &" 
	sleep 15
	@@./scripts/kill-server
	@grep ' 2xx=0 ' test5-httperf-a.out && egrep 'Errors:' test5-httperf-a.out
	@grep 'client-timo 1 ' test5-httperf-a.out
	@grep ' 2xx=1000 ' test5-httperf-b.out && egrep 'Errors:' test5-httperf-b.out
	pkill specweb99
	@sleep 2
	@echo ""


newtesthup6:
	@echo "Running test hup 6 - should get 0 replies and 1 errors"
	@echo " and 1 reply and 0 errors"
	fastcgi/specweb99-fcgi-hup.pl  > test6-fcgi.out &
	@sleep 4
	$(CSH) -c "$(USERVER) --use-epoll --epoll-modify-listeners -c 5 -f 100 \
             -m 0 -C --num-dyn-buffers 10 --dyn-buffer-size 1000000 $(DYN_BASE) \
	     $(DYN_FCGI) > test6.out &"
	@sleep 4
	$(CSH) -c "./scripts/run-hup1 1 > test6-httperf-a.out &" 
	$(CSH) -c "./scripts/runrate 1 10 > test6-httperf-b.out &" 
	sleep 15
	@@./scripts/kill-server
	@grep ' 2xx=0 ' test6-httperf-a.out && egrep 'Errors:' test6-httperf-a.out
	@grep 'client-timo 1 ' test6-httperf-a.out
	@grep ' 2xx=10 ' test6-httperf-b.out && egrep 'Errors:' test6-httperf-b.out
	pkill specweb99
	@sleep 2
	@echo ""


newtesthup7:
	@echo "Running test hup 7 - should get 0 replies and 1 errors"
	@echo " and 1 reply and 0 errors"
	fastcgi/specweb99-fcgi-hup.pl  > test7-fcgi.out &
	@sleep 4
	$(CSH) -c "$(USERVER) --use-epoll --epoll-modify-listeners -c 5 -f 100 \
             -m 0 -C --num-dyn-buffers 10 --dyn-buffer-size 1000000 $(DYN_BASE) \
	     $(DYN_FCGI) > test7.out &"
	@sleep 4
	$(CSH) -c "./scripts/run-hup1 1 > test7-httperf-a.out &" 
	$(CSH) -c "./scripts/runrate 1 10 > test7-httperf-b.out &"  
	sleep 1
  # Do another test on a version of the appserver to see if it
  # sends the request to the same appserver properly
	$(CSH) -c "./scripts/run-hup1 1 > test7-httperf-c.out &" 
	sleep 15
	@@./scripts/kill-server
	@grep ' 2xx=0 ' test7-httperf-a.out && egrep 'Errors:' test7-httperf-a.out
	@grep 'client-timo 1 ' test7-httperf-a.out
	@grep ' 2xx=10 ' test7-httperf-b.out && egrep 'Errors:' test7-httperf-b.out
	pkill specweb99
	@sleep 2
	@echo ""

newtesthup8:
	@echo "Running test hup 8 - should get 0 replies and 5 errors"
	fastcgi/specweb99-fcgi-hup.pl $(UNIX_DOMAIN_SOCK) > test8-fcgi.out &
	@sleep 4
	$(CSH) -c "$(USERVER) --use-epoll --epoll-modify-listeners -c 10 -f 100 -m 0 -C --num-dyn-buffers 10 --dyn-buffer-size 1000000 $(DYN_BASE) \
	     $(DYN_FCGI_UDS) > test8.out &"
	@sleep 4
	$(CSH) -c "./scripts/run-hup1 1 > test8a-httperf.out &"
	$(CSH) -c "./scripts/run-hup1 1 > test8b-httperf.out &"
	$(CSH) -c "./scripts/run-hup1 1 > test8c-httperf.out &"
	$(CSH) -c "./scripts/run-hup1 1 > test8d-httperf.out &"
	$(CSH) -c "./scripts/run-hup1 1 > test8e-httperf.out &"
	sleep 15
	@@./scripts/kill-server
	@grep ' 2xx=0 ' test8a-httperf.out
	@grep 'client-timo 1 ' test8a-httperf.out
	@grep ' 2xx=0 ' test8b-httperf.out
	@grep 'client-timo 1 ' test8b-httperf.out
	@grep ' 2xx=0 ' test8c-httperf.out
	@grep 'client-timo 1 ' test8c-httperf.out
	@grep ' 2xx=0 ' test8d-httperf.out
	@grep 'client-timo 1 ' test8d-httperf.out
	@grep ' 2xx=0 ' test8e-httperf.out
	@grep 'client-timo 1 ' test8e-httperf.out
	pkill specweb99
	@sleep 2
	@echo ""


newtesthup9:
	@echo "Running test hup 9 - should get 0 replies and 5 errors"
	fastcgi/specweb99-fcgi-hup.pl $(UNIX_DOMAIN_SOCK) > test9-fcgi.out &
	@sleep 4
	$(CSH) -c "$(USERVER) --use-epoll --epoll-modify-listeners -c 10 -f 100 -m 0 -C --num-dyn-buffers 10 --dyn-buffer-size 1000000 $(DYN_BASE) \
	     $(DYN_FCGI_UDS) > test9.out &"
	@sleep 4
	$(CSH) -c "./scripts/run-hup1 5 > test9-httperf.out"
	sleep 15
	@@./scripts/kill-server
	@grep ' 2xx=0 ' test9-httperf.out && egrep 'Errors:' test9-httperf.out
	@grep 'client-timo 5 ' test9-httperf.out
	pkill specweb99
	@sleep 2
	@echo ""

newtesthup10:
	@echo "Running test hup 10 - should get 2 replies and 2 errors"
	fastcgi/specweb99-fcgi-hup.pl $(UNIX_DOMAIN_SOCK) > test10-fcgi.out &
	@sleep 4
	$(CSH) -c "$(USERVER) --use-epoll --epoll-modify-listeners -c 10 -f 100 -m 0 -C --num-dyn-buffers 10 --dyn-buffer-size 1000000 $(DYN_BASE) \
	     $(DYN_FCGI_UDS) > test10.out &"
	@sleep 4
	$(CSH) -c "./scripts/run-hup1 2 > test10-httperf-a.out &"
	$(CSH) -c "./scripts/run-hup10 2 > test10-httperf-b.out &"
	sleep 15
	@@./scripts/kill-server
	@grep ' 2xx=0 ' test10-httperf-a.out
	@grep 'client-timo 2 ' test10-httperf-a.out
	@grep ' 2xx=2 ' test10-httperf-b.out
	@grep 'client-timo 0 ' test10-httperf-b.out
	pkill specweb99
	@sleep 2
	@echo ""

newtesthup11:
	@echo "Running test hup 11"
	# specweb99-fcgi-hup-some.pl sleeps for 2 seconds on every 2nd request
	# this allows us to ensure that requests fail and others work.
	fastcgi/specweb99-fcgi-hup-some.pl $(UNIX_DOMAIN_SOCK) > test11-fcgi.out &
	@sleep 4
	$(CSH) -c "$(USERVER) --use-epoll --epoll-modify-listeners -c 10 -f 100 -m 0 -C --num-dyn-buffers 10 --dyn-buffer-size 1000000 $(DYN_BASE) \
	     $(DYN_FCGI_UDS) > test11.out &"
	@sleep 4
	$(CSH) -c "./scripts/run-hup1 1 > test11-httperf-a.out"
	$(CSH) -c "./scripts/run-hup1 1 > test11-httperf-b.out"
	$(CSH) -c "./scripts/run-hup1 1 > test11-httperf-c.out"
	$(CSH) -c "./scripts/run-hup1 1 > test11-httperf-d.out"
	$(CSH) -c "./scripts/run-hup1 1 > test11-httperf-e.out"
	sleep 5
	@@./scripts/kill-server
	pkill specweb99
	@grep ' 2xx=0 ' test11-httperf-a.out
	@grep 'client-timo 1 ' test11-httperf-a.out
	@grep ' 2xx=1 ' test11-httperf-b.out
	@grep 'client-timo 0 ' test11-httperf-b.out
	@grep ' 2xx=0 ' test11-httperf-c.out
	@grep 'client-timo 1 ' test11-httperf-c.out
	@grep ' 2xx=1 ' test11-httperf-d.out
	@grep 'client-timo 0 ' test11-httperf-d.out
	@grep ' 2xx=0 ' test11-httperf-e.out
	@grep 'client-timo 1 ' test11-httperf-e.out
	@sleep 2
	@echo ""

newtesthup12:
	@echo "Running test hup 12"
	# specweb99-fcgi-hup-some.pl sleeps for 2 seconds on every 2nd request
	# this allows us to ensure that requests fail and others work.
	fastcgi/specweb99-fcgi-hup-some.pl $(UNIX_DOMAIN_SOCK) > test12-fcgi.out &
	@sleep 4
	$(CSH) -c "$(USERVER) --use-epoll --epoll-modify-listeners -c 10 -f 100 -m 0 -C --num-dyn-buffers 10 --dyn-buffer-size 1000000 $(DYN_BASE) \
	     $(DYN_FCGI_UDS) > test12.out &"
	@sleep 4
	$(CSH) -c "./scripts/run-hup1 1 > test12-httperf-a.out"
	$(CSH) -c "./scripts/run-hup1 1 > test12-httperf-b.out"
	$(CSH) -c "./scripts/run-hup1 1 > test12-httperf-c.out"
	$(CSH) -c "./scripts/run-hup1 1 > test12-httperf-d.out"
	$(CSH) -c "./scripts/run-hup1 1 > test12-httperf-e.out"
	sleep 5
	@@./scripts/kill-server
	pkill specweb99
	@grep ' 2xx=0 ' test12-httperf-a.out
	@grep 'client-timo 1 ' test12-httperf-a.out
	@grep ' 2xx=1 ' test12-httperf-b.out
	@grep 'client-timo 0 ' test12-httperf-b.out
	@grep ' 2xx=0 ' test12-httperf-c.out
	@grep 'client-timo 1 ' test12-httperf-c.out
	@grep ' 2xx=1 ' test12-httperf-d.out
	@grep 'client-timo 0 ' test12-httperf-d.out
	@grep ' 2xx=0 ' test12-httperf-e.out
	@grep 'client-timo 1 ' test12-httperf-e.out
	@sleep 2
	@echo ""

newtesthup13:
	@echo "Running test hup 13 -- just make sure that things don't crash"
	# specweb99-fcgi-hup-some.pl sleeps for 2 seconds on every 2nd request
	# this allows us to ensure that requests fail and others work.
	fastcgi/specweb99-fcgi-hup-some.pl $(UNIX_DOMAIN_SOCK) > test13-fcgi.out &
	@sleep 4
	$(CSH) -c "$(USERVER) --use-epoll --epoll-modify-listeners -c 50 -f 1000 -m 0 -C --num-dyn-buffers 10 --dyn-buffer-size 1000000 $(DYN_BASE) \
	     $(DYN_FCGI_UDS) > test13.out &"
	@sleep 4
	$(CSH) -c "./scripts/runspecdynamic-rate-hup1 100 100 > test13-httperf-a.out &"
	$(CSH) -c "./scripts/runspecdynamic-rate-hup10 100 100 > test13-httperf-b.out"
	@sleep 4
	@@./scripts/kill-server
	pkill specweb99
	@grep ' 2xx=' test13-httperf-a.out
	@grep 'client-timo' test13-httperf-a.out
	@grep ' 2xx=' test13-httperf-b.out
	@grep 'client-timo' test13-httperf-b.out
	@sleep 2
	@echo ""


newtesthup14:
	@echo "Running test hup 14 -- just make sure that things don't crash"
	# specweb99-fcgi-hup-some.pl sleeps for 2 seconds on every 2nd request
	# this allows us to ensure that requests fail and others work.
	fastcgi/specweb99-fcgi-hup-some.pl $(UNIX_DOMAIN_SOCK) > test14-fcgi.out &
	@sleep 4
	$(CSH) -c "$(USERVER) --use-epoll --epoll-modify-listeners -c 50 -f 1000 -m 0 -C --num-dyn-buffers 10 --dyn-buffer-size 1000000 $(DYN_BASE) \
	     $(DYN_FCGI_UDS) > test14.out &"
	@sleep 4
	$(CSH) -c "./scripts/runspecdynamic-rate-hup10 100 100 > test14-httperf-a.out"
	$(CSH) -c "./scripts/runspecdynamic-rate-hup1 100 100 > test14-httperf-b.out"
	$(CSH) -c "./scripts/runspecdynamic-rate-hup10 100 100 > test14-httperf-c.out"
	@sleep 4
	@@./scripts/kill-server
	pkill specweb99
	@grep ' 2xx=' test14-httperf-a.out
	@grep 'client-timo' test14-httperf-a.out
	@grep ' 2xx=' test14-httperf-b.out
	@grep 'client-timo' test14-httperf-b.out
	@grep ' 2xx=' test14-httperf-c.out
	@grep 'client-timo' test14-httperf-c.out
	@sleep 2
	@echo ""

#-----------------------------------------------------------------------
gary1:
	@echo "Running test gary 1 - should get 0 replies and 1 errors"
	fastcgi/specweb99-fcgi-hup.pl $(UNIX_DOMAIN_SOCK) > test1-fcgi.out &
	@sleep 4
	$(CSH) -c "$(USERVER) --state-delay 10 --delay 2000000 --use-epoll --epoll-modify-listeners -c 5 -f 100 -m 0 -C --num-dyn-buffers 10 --dyn-buffer-size 1000000 $(DYN_BASE) \
	     $(DYN_FCGI_UDS) > test1.out &"
	@sleep 4
	$(CSH) -c "./scripts/rundyn 1 > test1-httperf.out"
	sleep 10
	@@./scripts/kill-server
	@grep ' 2xx=0 ' test1-httperf.out && egrep 'Errors:' test1-httperf.out
	@grep 'client-timo 1 ' test1-httperf.out
	pkill specweb99
	@sleep 2
	@echo ""

#-----------------------------------------------------------------------
testskip: testskip1 testskip2 testskip3 \
          testskipm1 testskipm2

# Ensure that there are skips for a file that is supposed to be skipped.
testskip1: FORCE
	@echo "Running Skip Test"
	@echo "Running test 1 - should get 1 replies and 0 errors"
	$(CSH) -c "./userver -c 10 -f 15 --use-sendfile -D 0xffffffff --victim docs/100K.txt --victim-skip 4 >& test1.out &"
	sleep 4
	$(CSH) -c "./scripts/runuri docs/100K.txt >& test1-httperf.out"
	sleep 4
	@@./scripts/kill-server
	@grep ' 2xx=1 ' test1-httperf.out && egrep 'Errors:' test1-httperf.out
	@echo ""

# Ensure that there aren't any skips for a file that isn't supposed to be skipped.
testskip2: FORCE
	@echo "Running Skip Test"
	@echo "Running test 2 - should get 1 replies and 0 errors"
	$(CSH) -c "./userver -c 10 -f 15 --use-sendfile -D 0xffffffff --victim docs/NOFILE.txt --victim-skip 4 >& test2.out &"
	sleep 4
	$(CSH) -c "./scripts/runuri docs/100K.txt >& test2-httperf.out"
	sleep 4
	@@./scripts/kill-server
	@grep ' 2xx=1 ' test2-httperf.out
	sleep 3
	@grep 'Count of total skips                          0' test2.out
	@echo ""

# Skip anything containing "doc"
testskip3: FORCE
	@echo "Running Skip Test"
	@echo "Running test 3 - should get 1 replies and 0 errors"
	$(CSH) -c "./userver -c 10 -f 15 --use-sendfile -D 0xffffffff --victim doc --victim-skip 4 >& test3.out &"
	sleep 4
	$(CSH) -c "./scripts/runuri docs/100K.txt >& test3-httperf.out"
	sleep 4
	@@./scripts/kill-server
	@grep ' 2xx=1 ' test3-httperf.out

#-----------------------------------------------------------------------
testskipm1: FORCE
	@echo "Running Skip Test with madvise"
	@echo "Running test 1 - should get 1 replies and 0 errors"
	$(CSH) -c "./userver -C -c 10 -f 15 --use-sendfile -D 0xffffffff --victim docs/1M.txt --victim-skip -1 >& test1.out &"
	sleep 4
	$(CSH) -c "./scripts/runuri docs/1M.txt >& test1-httperf.out"
	sleep 4
	@@./scripts/kill-server
	@grep ' 2xx=1 ' test1-httperf.out && egrep 'Errors:' test1-httperf.out
	@echo ""

testskipm2: FORCE
	@echo "Running Skip Test with madvise"
	@echo "Running test 2 - should get 1 replies and 0 errors"
	$(CSH) -c "./userver -C -c 10 -f 15 --use-sendfile -D 0xffffffff --victim docs/1M.txt --victim-skip -1 >& test2.out &"
	sleep 4
	$(CSH) -c "./scripts/runuri docs/100K.txt >& test2-httperf.out"
	sleep 4
	@@./scripts/kill-server
	@grep ' 2xx=1 ' test2-httperf.out && egrep 'Errors:' test2-httperf.out
	@echo ""


#-----------------------------------------------------------------------
# Testing for call stats and Client Id.

callstats: callstats1 callstats2

callstats1: FORCE
	@echo "Running call stats test"
	@echo "Running test 1 - should get 10 replies and 0 errors"
	$(CSH) -c "./userver -C -c 10 -f 15 --call-stats 10 -D 0xffffffff >& test1.out &"
	sleep 4
	$(CSH) -c "./scripts/runclientid 10 >& test1-httperf.out"
	sleep 4
	@@./scripts/kill-server
	@grep ' 2xx=10 ' test1-httperf.out
	@echo ""


callstats2: FORCE
	@echo "Running call stats test"
	@echo "Running test 2 - should get 10 replies and 0 errors"
	$(CSH) -c "./userver -C -c 20 -f 40 --call-stats 5 -D 0xffffffff >& test2.out &"
	sleep 4
	$(CSH) -c "./scripts/runclientid 10 >& test2-httperf.out"
	sleep 4
	@@./scripts/kill-server
	@grep ' 2xx=10 ' test2-httperf.out
	@echo ""

callstats3: FORCE
	@echo "Running call stats test"
	# @echo "Running test 3 - should get 10 replies and 0 errors"
	$(CSH) -c "./userver -C -c 20 -f 40 --call-stats 10 -D 0xffffffff >& test3.out &"
	sleep 4
	$(CSH) -c "./scripts/runclientid-timeout 10 0.01 >& test3-httperf.out"
	sleep 4
	@@./scripts/kill-server
	# @grep ' 2xx=10 ' test3-httperf.out
	@echo ""


callstats4: FORCE
	@echo "Running call stats test"
	# @echo "Running test 4 - should get 10 replies and 0 errors"
	$(CSH) -c "./userver -C -c 20 -f 40 --call-stats 20 -D 0xffffffff >& test4.out &"
	sleep 4
	$(CSH) -c "./scripts/runclientid-timeout 10 0.001 >& test4-httperf.out"
	sleep 4
	@@./scripts/kill-server
	# @grep ' 2xx=10 ' test4-httperf.out
	@echo ""



#-----------------------------------------------------------------------
install: FORCE
	mv $(PGMS) $(BIN)

clean: FORCE
	rm -f *.o
	rm -f userver.tar.gz

vclean: FORCE
	-cd testing; make clean
	-rm -f *.o $(PGMS) makedocs core core.* .depend userver.trace \
		cscope.out userver.ps userver.pdf userver.txt userver.html \
		userver.exe userver.exe.stackdump userver.pid test*.out
	-rm -rf docs
	-cd fastcgi; make vclean
	-cd scripts; make vclean

ctags: FORCE
	ctags *.[ch]

tags: FORCE
	ctags *.[ch]

DIST_DEST = ../$(DIST_NAME)
SCRIPTS_DEST = $(DIST_DEST)/scripts
FASTCGI_DEST = $(DIST_DEST)/fastcgi
INCLUDE_DEST = $(DIST_DEST)

makedocs: makedocs.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -o makedocs makedocs.c

docs/1B.txt:
	rm -rf docs
	./makedocs

docs: makedocs docs/1B.txt

dist: docs userver.txt userver.ps userver.pdf userver.html
	rm -rf $(DIST_DEST)
	rm -f $(DIST_NAME).tar.gz
	-mkdir -p $(DIST_DEST)
	-mkdir -p $(SCRIPTS_DEST)
	-mkdir -p $(FASTCGI_DEST)
	-mkdir -p $(INCLUDE_DEST)
	cp -p $(DIST_LIST) $(DIST_DEST)
	cp -p $(SCRIPTS) $(SCRIPTS_DEST)
	cp -p $(FASTCGI) $(FASTCGI_DEST)
	cp -rp $(INCLUDE) $(INCLUDE_DEST)
	find $(DIST_DEST) -depth -type d -name CVS -exec rm -rf {} \;
	$(CSH) -c "cd .. ; pwd ; tar -czf $(DIST_NAME).tar.gz $(DIST_NAME)/"
	@echo "Don't forget to add a note about the date release to CHANGELOG"
	@echo "  this should be done before make dist"
	@echo "Don't forget to upload the distribution (make updist)"
	@echo "and tag the CVS tree (make cvstag) !!!"
	@echo "Also update the date and version number in userver.1"
	@echo "and do make upman to upload the new man pages"

updist:
	scp CHANGELOG cs.uwaterloo.ca:www/userver/src/CHANGELOG-$(DIST_VER).txt
	scp ../$(DIST_NAME).tar.gz cs.uwaterloo.ca:www/userver/src
	scp userver.pdf cs.uwaterloo.ca:www/userver/src/info/
	scp userver.ps cs.uwaterloo.ca:www/userver/src/info/
	scp userver.html cs.uwaterloo.ca:www/userver/src/info/
	ssh cs.uwaterloo.ca 'chmod a+r www/userver/src/info/*'
	ssh cs.uwaterloo.ca 'chmod a+r www/userver/src/*'
	@echo "Don't forget to tag the CVS tree!!!"

upman:
	make man
	scp userver.pdf cs.uwaterloo.ca:www/userver/src/info/
	scp userver.ps cs.uwaterloo.ca:www/userver/src/info/
	scp userver.html cs.uwaterloo.ca:www/userver/src/info/
	ssh cs.uwaterloo.ca 'chmod a+r www/userver/src/info/*'

cvstag:
	cd ..; cvs tag Ver-$(CVS_TAG_VER)

# upindex:
#  	scp index.html cs:www/userver/src

FORCE:

