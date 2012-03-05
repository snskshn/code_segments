#ifndef WIN32
# include <pthread.h>
#else
# include <process.h>
#endif

#include "ssl.h"
#include "cyassl_test.h"
#include "platform.h"

/*
void cleanup(void *arg)
{
    SSL *ssl = (SSL *)arg;

    if(SSL_get_session(ssl)) {
	SSL_free(ssl);
	printf("free ssl: %d\n", SSL_get_fd(ssl));
    }
}
*/

void *handle_client(void *arg)
{
    SSL *ssl = (SSL *)arg;
    SOCKET_T clientfd = SSL_get_fd(ssl);
    int len;
    char command[2048];

    //pthread_cleanup_push(cleanup, ssl);
    if(SSL_accept(ssl) != SSL_SUCCESS) {
	printf("SSL_accept failed (%d)\n", clientfd);
	CloseSocket(SSL_get_fd(ssl));	// XXX: useful?
	return NULL;
    }
    showPeer(ssl);
    printf("SSL_accept success! (%d)\n", clientfd);

    while((len = SSL_read(ssl, command, sizeof(command))) > 0) {
	if(len == SOCKET_ERROR) {
	    return NULL;
	}
	command[len] = 0;
    }

    /* never reached */
    //pthread_cleanup_pop(0);
    //cleanup(ssl);
    return NULL;
}

int main()
{
#ifndef WIN32
    pthread_attr_t attr;
    pthread_t pid[256] = {0, };
#else
    HANDLE hThread[256];
#endif
    SOCKET_T sockfd = 0;
    SSL_METHOD *method = 0;
    SSL *ssl[256] = {0, };
    SSL_CTX *ctx;
    //SOCKET_T fd[256] = {0, };
    int sid = 0;

    Sleep(2000);	// flush UDP data packet: prevent blocked SSL_accept()

    // init
    tcp_listen(&sockfd);
    method = DTLSv1_server_method();
    ctx = SSL_CTX_new(method);
#ifndef WIN32
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
#endif

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

    while(1) {
	SOCKADDR_IN peer;
	unsigned socklen = sizeof(SOCKADDR_IN);
	SOCKET_T clientfd;

	clientfd = udp_read_connect(sockfd);
	if(clientfd == -1) {
	    err_sys("udp read failed");
	}
	getpeername(clientfd, (struct sockaddr *)&peer, &socklen);

	// determine sensor ID
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
	printf("connection try from %s(%d)\n",
		inet_ntoa(peer.sin_addr), clientfd);

	// reconnected sensor?
	if(ssl[sid] != NULL && SSL_get_session(ssl[sid])) {
#ifndef WIN32
	    pthread_cancel(pid[sid]);
	    CloseSocket(SSL_get_fd(ssl[sid]));
#else
	    CloseSocket(SSL_get_fd(ssl[sid]));	// thread will exit
	    CloseHandle(hThread[sid]);
#endif
	    printf("free ssl: %d\n", SSL_get_fd(ssl[sid]));
	    SSL_free(ssl[sid]);
	    ssl[sid] = NULL;
	}
	/*
	if(pid[sid] != (pthread_t)NULL) {
	    pthread_cancel(pid[sid]);
	    CloseSocket(fd[sid]);
	    pid[sid] = (pthread_t)NULL;
	    printf("# cancel thread %d\n", sid);
	}
	*/
	if(ssl[sid] != NULL && SSL_get_fd(ssl[sid]) != 0) {
	    CloseSocket(SSL_get_fd(ssl[sid]));
	}

	// make SSL object
	ssl[sid] = SSL_new(ctx);
	if(ssl[sid] == NULL) {
	    err_sys("SSL_new failed");
	}
	SSL_set_fd(ssl[sid], clientfd);
	SetDH(ssl[sid]);
	//usleep(100000);

#ifndef WIN32
	pthread_create(&pid[sid], &attr, handle_client, ssl[sid]);
#else
	hThread[sid] = (HANDLE)_beginthreadex(NULL, 0, handle_client,
		(void *)&ssl[sid], 0, &threadID[sid]);
#endif

	printf("# create thread %d\n", sid);
	//fd[sid] = clientfd;
	tcp_listen(&sockfd);
    }

    CloseSocket(sockfd);
    SSL_CTX_free(ctx);

    return 0;
}
