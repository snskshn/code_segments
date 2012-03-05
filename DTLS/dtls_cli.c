#include "ssl.h" 
#include "cyassl_test.h"
#include <unistd.h>

int main(int argc, char **argv)
{
    SOCKET_T	sockfd	= 0;
    SSL_METHOD*	method	= 0;
    SSL_CTX*	ctx	= 0;
    SSL*	ssl	= 0;
    int sendSz;
    int ret;
    int tries = 0;	// XXX

    char send[1024];
    //char reply[1024];
    //int sid;

    if(argc != 2) {
	printf("usage: %s servIP\n", argv[0]);
	return 1;
    }

    // retry connection to server
    while(1) {
	// init
	tcp_connect(&sockfd, argv[1], yasslPort);
	method = DTLSv1_client_method();
	ctx = SSL_CTX_new(method);

	// XXX: prevent block in SSL_connect
	struct timeval timeout;
	timeout.tv_sec = 1;  
	timeout.tv_usec = 0;  
	int optlen = sizeof(timeout);
	if(setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO,
		    (void*)&timeout, (socklen_t)optlen) < 0){ 
	    //debug(DEBUG_EVENT,"[ERROR] set server timeout error\n");
	    close(sockfd);
	    SSL_CTX_free(ctx);
	    return -1; 
	}    

	caCert = "ca-cert.pem";
	if(SSL_CTX_load_verify_locations(ctx, caCert, 0) != SSL_SUCCESS) {
	    err_sys("can't load ca file");
	}

	ssl = SSL_new(ctx);
	SSL_set_fd(ssl, sockfd);
#ifdef WIN32
	Sleep(100);
#endif

	puts("###################################");
	if((ret = SSL_connect(ssl)) != SSL_SUCCESS) {
	    printf("%d\n", SSL_get_error(ssl, ret));
	    puts("SSL_connect failed");

	    SSL_free(ssl);
	    SSL_CTX_free(ctx);
	    CloseSocket(sockfd);

	    sleep(1);
	    continue;
	    //err_sys("SSL_connect failed");
	}

#if 0
	// XXX: send sensorID
	sid = atoi(argv[2]);
	if(SSL_write(ssl, &sid, 1) != 1) {
	    err_sys("SSL_write failed");
	    goto finish;
	}
#endif

	// get my IP addr
	struct sockaddr_in me;
	unsigned socklen = sizeof(struct sockaddr_in);
	getsockname(sockfd, (struct sockaddr *)&me, &socklen);

	// send events
	//while(fgets(send, sizeof(send), stdin)) {
	while(1) {
	    //sendSz = (int)strlen(send) + 1;
	    sendSz = snprintf(send, sizeof(send), "[%s] %d\n",
		    inet_ntoa(me.sin_addr), tries++) + 1;

	    if(SSL_write(ssl, send, sendSz) != sendSz) {
		puts("SSL_write failed");
		sleep(5);
		break;
		//err_sys("SSL_write failed");
	    }
	    usleep(500000);
	    //sleep(5);

	    /*
	       while(sendSz) {
	       int got;
	       if((got = SSL_read(ssl, reply, sizeof(reply))) > 0) {
	       fputs(reply, stdout);
	       sendSz -= got;
	       }
	       else {
	       break;
	       }
	       }
	     */

	    if(strncmp(send, "break", 5) == 0) {
		break;
	    }
	}
//finish:

	/* must tell server done (SSL_write().. something)*/
	SSL_free(ssl);
	SSL_CTX_free(ctx);
	fflush(stdout);
	CloseSocket(sockfd);
    }

    return 0;
}
