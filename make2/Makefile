.SUFFIXES: .c .o

OBJS = main.o read.o write.o
SRCS = $(OBJS:.o=.c)
LIBOBJS = read.o write.o

CC = gcc
INC =
LIBS =
CFLAGS = -g -Wall
CFLAGS_DLIBS = $(CFLAGS) $(INC) -fPIC

TARGET = test

all: dep $(TARGET) subdir

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

subdir:
	$(MAKE) -C testdir

.c.o : 
	$(CC) $(CFLAGS) -c $< -o $@

libio.a: $(LIBOBJS)
	$(AR) rcv $@ $(LIBOBJS)
	ranlib $@

libio.so.1:
	$(CC) -shared -Wl,-soname,$@ -o $@ $(LIBOBJS)

clean : 
	rm -rf $(OBJS) $(TARGET) .depend core libio.a libio.so.1
	$(MAKE) clean -C testdir

dep:
	#gccmakedep $(INC) $(SRCS)
	$(CC) -MM $(INC) $(SRCS) > .depend

new:
	$(MAKE) clean
	$(MAKE)

ifeq (.depend,$(wildcard .depend))
include .depend
endif 
