CC = gcc
CFLAGS = -g -Wall `xml2-config --cflags`
LDFLAGS = `xml2-config --libs`
TARGETS = $(patsubst %.c, %, $(wildcard *.c))

all: $(TARGETS)

% : %.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

.PHONY: .tags
tags:
	find . -name "*.c" > cscope.files
	cscope -b -I/usr/include/libxml2

clean:
	rm -rf $(TARGETS)
