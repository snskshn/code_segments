#include <pthread.h>
#include <signal.h>
#include <unistd.h>

#include "ssl.h" 
#include "cyassl_int.h"
#include "cyassl_test.h"

static int pkt_count[256];
static int on[256] = {0, };
//SSL *ssl;

typedef struct arg
{
    int sid;
    int clientfd;
    SSL *ssl;
    SSL_CTX *ctx;
} ARG;

void sigint_handler(int signum)
{
    int i;

    if(signum == SIGINT) {
	for(i = 0; i < 256; i++) {
	    if(pkt_count[i] == 0) {
		continue;
	    }
	    printf("from sensor %d, count=%d\n", i, pkt_count[i]);
	}
    }

    
}

void cleanup(void *arg) {
    SSL *ssl = (SSL *)arg;
    int clientfd = SSL_get_fd(ssl);

    SSL_free(ssl);
    CloseSocket(clientfd);
    ssl = 0;
    printf("free end\n");
}

void *handle_client(void *arg)
{
    ARG argument = *(ARG *)arg;
    SSL *ssl;
    SSL_CTX *ctx = argument.ctx;
    int clientfd = argument.clientfd;
    char command[1024];
    int echoSz;

    //sleep(1);
    //assert(ssl != NULL);

    // make TLS connection
    ssl = SSL_new(ctx);
    if(ssl == NULL) {
	err_sys("SSL_new failed");
    }
    SSL_set_fd(ssl, clientfd);
    SetDH(ssl);
    if(SSL_accept(ssl) != SSL_SUCCESS) {
	printf("SSL_accept failed\n");
	printf("                         critical error!: %d\n", ssl->options.connectState);
	cleanup(ssl);
	on[argument.sid] = 0;
	return NULL;
    }
    showPeer(ssl);

    pthread_cleanup_push(cleanup, ssl);

    // echo UDP packets
    while((echoSz = SSL_read(ssl, command, sizeof(command))) > 0) {
	command[echoSz] = 0;
	fputs(command, stdout);
	//pkt_count[argument.sensor_id]++;

	// close client
	if(strncmp(command, "break", 5) == 0) {
	    printf("close client\n");
	    break;
	}

	/*
	   if(SSL_write(ssl, command, echoSz) != echoSz) {
	   err_sys("SSL_write failed");
	   }
	 */
    }

    pthread_cleanup_pop(0);

    // end
    cleanup(ssl);

    return NULL;
}


int main()
{
    SOCKET_T	sockfd = 0;
    SSL_METHOD* method = 0;
    SSL_CTX*	ctx    = 0;
    pthread_t tid[256];
    pthread_attr_t attr;

    //signal(SIGINT, sigint_handler);
    sleep(3);	// flush UDP data packet sent before

    // init
    tcp_listen(&sockfd);
    method = DTLSv1_server_method();
    ctx = SSL_CTX_new(method);
    //CyaSSL_Debugging_ON();
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    // load files
    svrCert = "server-cert.pem";
    svrKey = "server-key.pem";
    if(SSL_CTX_use_certificate_file(ctx, svrCert, SSL_FILETYPE_PEM)
	    != SSL_SUCCESS) {
	err_sys("can't load server cert file");
    }
    if(SSL_CTX_use_PrivateKey_file(ctx, svrKey, SSL_FILETYPE_PEM)
	    != SSL_SUCCESS) {
	err_sys("can't load server key file");
    }

    // many clients
    while(1) {
	//SSL* ssl = 0;
	int clientfd;
	char sid;
	struct sockaddr_in peer;
	unsigned socklen = sizeof(struct sockaddr_in);
	static ARG arg;

	// BLOCK! get HELO (?)
	clientfd = udp_read_connect(sockfd);
	if(clientfd == -1) {
	    err_sys("udp read failed");
	}
	printf("## got HELO\n");

	getpeername(clientfd, (struct sockaddr *)&peer, &socklen);
	printf("getpeername(): %s\n", inet_ntoa(peer.sin_addr));

#if 1
	// XXX: sensor_id matching by IP address
	if(strncmp("127.0.0.1", inet_ntoa(peer.sin_addr), 15) == 0) {
	    sid = 1;
	}
	else if(strncmp("10.51.51.2", inet_ntoa(peer.sin_addr), 15) == 0) {
	    sid = 1;
	}
	else if(strncmp("10.25.1.44", inet_ntoa(peer.sin_addr), 15) == 0) {
	    sid = 2;
	}
	else if(strncmp("10.25.1.49", inet_ntoa(peer.sin_addr), 15) == 0) {
	    sid = 3;
	}
	else if(strncmp("10.25.1.50", inet_ntoa(peer.sin_addr), 15) == 0) {
	    sid = 4;
	}
	else {
	    printf("!! unregistered sensor\n");
	    CloseSocket(clientfd);
	    continue;
	}
#endif

	// reconnected sensor? (by IP)
	if(on[(int)sid]) {
	    printf("cancel thread %d\n", sid);
	    pthread_cancel(tid[(int)sid]);
	    on[(int)sid] = 0;
	}

#if 0
	// make TLS connection
	ssl = SSL_new(ctx);
	if(ssl == NULL) {
	    err_sys("SSL_new failed");
	}
	SSL_set_fd(ssl, clientfd);
	SetDH(ssl);
	if(SSL_accept(ssl) != SSL_SUCCESS) {
	    printf("SSL_accept failed\n");
	    SSL_free(ssl);
	    CloseSocket(clientfd);
	    tcp_listen(&sockfd);
	    continue;
	}
	showPeer(ssl);
#endif

#if 0
	SSL_read(ssl, &sid, 1);

	// reconnected sensor?
	if(on[(int)sid]) {
	    pthread_cancel(tid[(int)sid]);
	    on[(int)sid] = 0;
	}
	printf("## [%d] connection established, clientfd:%d, %d\n",
		sid, clientfd, SSL_get_fd(ssl));
#endif

	//arg.sensor_id = sid;	// XXX: not used
	arg.clientfd = clientfd;
	//arg.ssl = ssl;
	arg.ctx = ctx;
	arg.sid = sid;
	printf("make thread :%d\n", sid);
	pthread_create(&tid[(int)sid], &attr, handle_client, &arg);
	on[(int)sid] = 1;

	tcp_listen(&sockfd);
    }

    /* Never reached */
    CloseSocket(sockfd);
    SSL_CTX_free(ctx);

    return 0;
}


