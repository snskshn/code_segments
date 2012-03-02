CC=gcc
CYASSL_ROOT=/root/work/cyassl/cyassl-2.0.0rc2

CFLAGS = -g -Wall \
	-DUSE_ANY_ADDR -DDEBUG_CYASSL -DCYASSL_DTLS -DSESSION_CERTS -DOPENSSL_EXTRA \
	-I$(CYASSL_ROOT)/include/openssl -I$(CYASSL_ROOT)/ctaocrypt/include \
	-I$(CYASSL_ROOT)/include

#srv: CFLAGS+=NO_CYASSL_CLIENT
#cli: CFLAGS+=NO_CYASSL_SERVER
LDFLAGS=-lcyassl -lm -lpthread

all: srv cli
srv: dtls_srv.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
cli: dtls_cli.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
install:
	#scp cli root@10.51.51.2:~/
	#scp ca-cert.pem root@10.51.51.2:~/
	scp cli ssson@10.25.1.44:~/
	scp ca-cert.pem ssson@10.25.1.44:~/
	scp cli ssson@10.25.1.49:~/
	scp ca-cert.pem ssson@10.25.1.49:~/
	scp cli ssson@10.25.1.50:~/
	scp ca-cert.pem ssson@10.25.1.50:~/
new: clean all
clean:
	rm -rf srv cli
