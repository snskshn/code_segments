
/*
    userver -- (pronounced you-server or micro-server).
    This file is part of the userver, a high-performance web server designed for
    performance experiments.
          
    Copyright (C) 2010 Tim Brecht
    Original Copyright (C) 2008  Mark Grove

    Authors: Mark Groves <mjgroves@uwaterloo.ca>
             Tim Brecht <brecht@cs.uwaterloo.ca>
    See AUTHORS file for list of contributors to the project.
  
    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of the
    License, or (at your option) any later version.
  
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.
  
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
    02111-1307 USA
*/


#ifdef HAVE_SSL
#include <openssl/err.h>
#include "u_ssl_utils.h"
#include <unistd.h>
#include <errno.h>
#include "myassert.h"
#include "common.h"

#include "debug.h"

SSL_CTX * initialize_ssl(int port) {
	//set up the ssl context and everything for the server
	SSL_library_init();
	SSL_load_error_strings ();
	//SSLeay_add_ssl_algorithms ();
	SSL_CTX *ctx;
	char port_str[6];
	sprintf(&port_str[0], "%d", port);
	port_str[5] = '\0';

	printf("open context on port %d", port);

	//init_OpenSSL();
	ctx = SSL_CTX_new(SSLv3_server_method());	
	
	//load up the certificates, etc
	//MAY NOT NEED ALL OF THESE, particularly the client certs
	//if (SSL_CTX_load_verify_locations(ctx, CA_FILE, CA_DIR) != 1) 
	//	printf("couldn't load verify locs\n");
	//if (SSL_CTX_set_default_verify_paths(ctx) != 1)
	//	printf("coultn'd set default ver paths\n");
	if (SSL_CTX_use_certificate_chain_file(ctx, SERVER_CERT) != 1)
		printf("couldn't load client cert\n");
	if (SSL_CTX_use_PrivateKey_file(ctx, SERVER_PKEY, SSL_FILETYPE_PEM) != 1)
		printf("errr loading private key\n");
	//SSL_CTX_set_verify(ctx, 0, verify_callback);
	SSL_CTX_set_verify_depth(ctx, 1);		//hard-code depth for now
	
/*
	//set up the BIO
	acc = BIO_new_accept(&port_str[0]);
	if (!acc)
		printf("ERROR creating ssl socket\n");
*/

	printf("done\n");

	return ctx;
}


SSL*  do_ssl_accept(int sock) {
	SSL *ssl = 0;
        int rc = 0;
	int res;
	int err,sys_err;
	//printf("accepting a packet on the ssl port using context %x\n", *server_ctx);

	if (!(ssl = SSL_new (server_ctx))) {
	   printf("ERROR creating ssl ctx for client con\n");
           return 0;
         }
	//printf("set accept state oon %x\n",ssl);	
	//SSL_set_accept_state(ssl);
	//SSL_set_bio(ssl, client, client);
	rc = SSL_set_fd(ssl,sock);

        SSL_set_mode(ssl, SSL_MODE_ENABLE_PARTIAL_WRITE | SSL_MODE_ACCEPT_MOVING_WRITE_BUFFER);


        PRINT_TIME(NOFD, &tnow, &tprev, "do_ssl_accept: doing SSL_accept sock = %d\n", sock);
	if  ((res = SSL_accept(ssl)) <= 0) {
		sys_err = errno;
		err = SSL_get_error(ssl, res);
                ERR_print_errors_fp(stdout);
		if (err  == SSL_ERROR_NONE)  {
			printf("can ignore the accept error\n");
                } else if (err == SSL_ERROR_WANT_READ) {
			printf("hmmm...a 'want read' error during accept()\n");
                } else if (err == SSL_ERROR_SYSCALL) {
			printf("syscall error....%d\n", res);
                } else if (sys_err == EAGAIN || sys_err == EWOULDBLOCK) {
			printf("would block err = %d\n", sys_err);
                }
		printf("res = %d, sys error = %d\n", res, sys_err);
		printf("ERROR accepting conn on SSL context \n");
		perror("SSL_accept");
	} else {
          PRINT_TIME(NOFD, &tnow, &tprev, "do_ssl_accept: SSL_accept is successful ssl = %p sock = %d\n", ssl, sock);
           
        }

	return ssl;
}	

void shutdown_ssl(SSL_CTX *ctx) {
	//close the ssl context and free up everything
	SSL_CTX_free(ctx);
}

int is_ssl_listener(int listener_sd) {
        return (listener_sd == ssl_listen_sd);
}


void do_ssl_close(SSL *ssl, int sock) {
	SSL_shutdown(ssl);
	SSL_free(ssl);
	close(sock);
}


void do_ssl_reneg(SSL *ssl) {
	//do the renegotiation here
}

void do_ssl_read(struct info *client_data, int amount_to_read) {
	int num_bytes;
	int socket_flags;
	int error_no;
	num_bytes = SSL_read(client_data->ssl, client_data->req.buf, amount_to_read);
	error_no=errno;
	socket_flags = SSL_get_error(client_data->ssl, num_bytes);

	switch(socket_flags) {
	case SSL_ERROR_NONE:
		//client_data->offset += num_bytes;
		break;
	
	case SSL_ERROR_WANT_READ:
		//should be a non-blocking case
		//for now, just print an error message, see if this case actually happens
		printf("error want read\n");
		assert(0);
		break;

	case SSL_ERROR_WANT_WRITE:
		//should be a renegotiation case
		//for now, ignore it.
		printf("error want write (doing read)\n");
		break;

	case SSL_ERROR_ZERO_RETURN:
		//te session has been closed
		printf("erro, ssl session closed\n");
		do_close(client_data, REASON_CLIENT_CLOSED);
		break;

	case SSL_ERROR_SYSCALL:
		//something happened to the socket underneath
		if (error_no == EWOULDBLOCK) {
			//would block, do nothing
			//should handle epoll edge trigger case, and so on
			break;
		}
		else {
			printf("something bad happened to the socket\n");
		}
		break;
			

	case SSL_ERROR_SSL:
		//general error
		printf("general error\n");
		break;
	default:
		printf("YIKES, I didn't plan for this ssl error\n");
		assert(0);
		break;
	}

#ifdef NO_BLOCK
        socket_flags = fcntl(sock, F_GETFL, 0);
        if (socket_flags < )0
                socket_flags = 0;
        //socket_flags |= O_NONBLOCK;
        fcntl(sock, F_SETFL, socket_flags | O_NONBLOCK);

#endif

	//printf("read the following %s\n", req_buf);
}


void do_ssl_write(struct info *client_data, int amount_to_write) {
	long res = 0;
	int socket_flags;
	int my_error;

	while (client_data->rep.offset < client_data->rep.total_len) {

		res = SSL_write(client_data->ssl, &(client_data->rep.buf[client_data->rep.offset]), \
				client_data->rep.total_len - client_data->rep.offset);
		my_error = errno;

		//printf("sent total of %d  + %d of %d bytes\n", client_data->rep.offset, res, client_data->rep.total_len);
		
		socket_flags= SSL_get_error(client_data->ssl,res);
		
		switch(socket_flags) {
		case SSL_ERROR_NONE:
			client_data->rep.offset += res;
			break;
	
		case SSL_ERROR_WANT_READ:
			//should be a non-blocking case
			//for now, just print an error message, see if this case actually happens
			printf("error want read\n");
			assert(0);
			break;
	
		case SSL_ERROR_WANT_WRITE:
			//should be a renegotiation case
			//for now, ignore it.
			printf("error want write (doing read)\n");
			break;
	
		case SSL_ERROR_ZERO_RETURN:
			//te session has been closed
			printf("erro, ssl session closed\n");
			break;
	
		case SSL_ERROR_SYSCALL:
			//something happened to the socket underneath
			if (my_error == EWOULDBLOCK) {
				//would block, do nothing
				//should handle epoll edge trigger case, and so on
				break;
			}
			else {
				printf("something bad happened to the socket\n");
			}
			break;
				
		case SSL_ERROR_SSL:
			//general error
			printf("general error\n");
			break;
		default:
			printf("YIKES, I didn't plan for this ssl error\n");
			assert(0);
			break;
		}
	}

	if (client_data->rep.offset == client_data->rep.total_len) {
		//what should I be doing here??
#ifdef NO_BLOCK
        socket_flags = fcntl(client_data->socket_fd, F_GETFL, 0);
        if (socket_flags < 0)
                socket_flags = 0;
        fcntl(client_data->socket_fd, F_SETFL, socket_flags | (~O_NONBLOCK));
#endif


	}
}


/*---------------------------------------------------------------------*/
int post_sock_ssl_accept(SSL* client_ssl, struct info *ip) {
                                                                                                                                                             
	//set all the variable in the info struct
	// ip->is_ssl=1;
	ip->ssl = client_ssl;

	return 0;                                                                                                                                                             
}
//worry about reuse??
#endif /* HAVE_SSL */
