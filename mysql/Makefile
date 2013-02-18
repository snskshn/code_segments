CC = gcc
CFG = `mysql_config --cflags --libs`

SRCS = $(wildcard *.c)
PROGS = $(patsubst %.c,%,$(SRCS))

all: $(PROGS)

%: %.c
	$(CC) $^ -o $@ $(CFG)
