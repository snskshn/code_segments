.SUFFIXES : .c .o 
CC = gcc
CFLAGS = -O2 -g

OBJS1 = main.o test1.o
OBJS2 = main.o test2.o 
OBJS3 = main.o test3.o 
SRCS = $(OBJS1:.o=.c) $(OBJS2:.o=.c) $(OBJS3:.o=.c) 

all : dep test1 test2 test3

test1 : $(OBJS1)
	$(CC) -o test1 $(OBJS1) 

test2 : $(OBJS2)
	$(CC) -o test2 $(OBJS2)

test3 : $(OBJS3)
	$(CC) -o test3 $(OBJS3)

dep :
	#gccmakedep $(SRCS)
	$(CC) -MM $(SRCS) > .depend

clean:
	rm -rf $(OBJS1) $(OBJS2) $(OBJS3) test1 test2 test3 .depend core

ifeq (.depend,$(wildcard .depend))
include .depend
endif 
