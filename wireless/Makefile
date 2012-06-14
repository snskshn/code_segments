TARGETS = get_chan flooder
CFLAGS = -Wall -g
SOURCES = $(TARGETS:=.c)

all: cscope $(TARGETS)

get_chan: get_chan.c
	gcc -Wall -g -I/usr/local/include/libnl3 -L/usr/local/lib -lnl-3 -lnl-genl-3 -Wl,-rpath -Wl,/usr/local/lib $< -o $@

flooder: flooder.c
	mips64-octeon-linux-gnu-gcc -mabi=n32 -I/usr/local/WIPS_mips/include -L/usr/local/WIPS_mips/lib flooder.c -o flooder -lpcap

cscope: cscope.files
	find $(SOURCES) > cscope.files
	cscope -b -I/usr/local/include/libnl3

clean:
	rm -rf $(TARGETS)
