CC=gcc
CYASSL_ROOT=/root/work/cyassl/cyassl-2.0.0rc2
CFLAGS=-g -Wall -DUSE_ANY_ADDR -DDEBUG_CYASSL -DCYASSL_DTLS -DSESSION_CERTS -DOPENSSL_EXTRA -Wall -I$(CYASSL_ROOT)/include/openssl -I$(CYASSL_ROOT)/ctaocrypt/include
LDFLAGS=-lcyassl -lm -lpthread

all: srv cli
srv: dtls_srv.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
cli: dtls_cli.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
clean:
	rm -rf srv cli
