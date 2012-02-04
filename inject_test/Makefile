TARGET=inject
CC=gcc
LIBS=-lpcap
CFLAGS=-Wall -g
SRC=inject.c

$(TARGET): inject.c ieee802_11.h ieee802_11_radio.h
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)

clean:
	rm -rf $(TARGET) core

new:
	make clean; make

