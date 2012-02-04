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


#ifndef USERVER_SSL_UTILS
#define USERVER_SSL_UTILS

#include "options.h"
#include "openssl/ssl.h"
#include "info.h"


#define SERVER_CERT "mycert"
#define SERVER_PKEY "mypkey"
#define CLIENT_CERT "client_cert.pem"
#define CA_FILE "mycert"
#define CA_DIR NULL

SSL_CTX * initialize_ssl(int port);
void shutdown_ssl(SSL_CTX *ctx);
int is_ssl_listener(int listener_sd);
SSL* do_ssl_accept(int sock);
int post_sock_ssl_accept(SSL *client_ssl, struct info *ip);

void do_ssl_close(SSL *ssl, int sock);
void do_ssl_reneg(SSL *ssl);

void do_ssl_read(struct info *client_data, int amount_to_read);
void do_ssl_write(struct info *client_data, int amount_to_read);

#endif
