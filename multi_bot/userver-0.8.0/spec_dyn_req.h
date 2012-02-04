/*
    userver -- (pronounced you-server or micro-server).
    This file is part of the userver, a high-performance web server designed for
    performance experiments.
          
    This file is Copyright (C) 2004-2010  Tim Brecht

    Authors: Amol Shukla <ashukla@real.com>
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

   Adapted from: 
  	TUX - Integrated HTTP layer and Object Cache
 	Copyright (C) 2000, Ingo Molnar <mingo@redhat.com>
 	CAD_u.c: Implementation of the SPECweb99 dynamic web application
	         using TUXAPI.
	FIXME:
	1. The following code probably contains many goto statements that
	   can be rewritten, I have tried to keep it as similar to
		the Tux module code (which contains the same number of gotos) as possible.
	2. Many portability issues need to be resolved. This code has been
	   tested only on Linux x86 systems. Among other things, assumes (I think)
		that sizeof(int) == sizeof(long)
	3. File I/O is blocking. In fact, socket I/O is blocking too since 
	   all responses are without using any of the existing write 
		functionality in userver. This needs to be fixed ASAP.
	   Blocking socket writes occur in :	
		my_send(), send_postlog(), scan_send_file(), direct_send_file()
		and any functions calling sock_write_more() or sock_write_push()
		(e.g., send_reply_head(), send_reply_tail())
	-AS
 
*/


#ifdef MOD_SPECWEB99

#ifndef SPEC_DYN_REQ_H
#define SPEC_DYN_REQ_H

#include "common.h"
#include "options.h"

/* userver document root */
#define TOPDIR (options.doc_root_len ? options.doc_root : ".")

typedef struct info user_req_t;
typedef struct CAD_struct {
	int user_id;
	int last_ad;
	char ad_filename [100];
	int reply_cookies_len;
	char reply_cookies [MAX_COOKIE_LEN];
} CAD_t;

int spec_dyn_req(user_req_t *req);
static int error(user_req_t *req, char *message);
static int my_send(int sock, char *buf, int total_len, int flags);
static int read_custom_ads(user_req_t *req);
static int read_user_personality(user_req_t *req);
static void open_postlog_file(user_req_t *req);
static int init_postlog_file(user_req_t *req);
static int do_reset(user_req_t *req, char *query);
static int send_postlog(user_req_t *req);
static int do_command(user_req_t *req, char *query);
static CAD_t *parse_GET_cookies(user_req_t *req);
static int scan_send_file(user_req_t *req);
static int do_POST(user_req_t *req);
static inline int custom_ad_rotate(user_req_t*, CAD_t*);

static int CAD_query(user_req_t *req);
static int CAD_send(user_req_t *req);
static int CAD_finish_send(user_req_t *req);

static int populate_object(user_req_t *req);
static int direct_send_file(user_req_t *req);

#define CAD_TAG_HEAD	"<!WEB99CAD><IMG SRC=\"/file_set/"
#define CAD_TAG_BODY	"dirNNNNN/classX_Y"
#define CAD_TAG_TAIL	"\"><!/WEB99CAD>"
#define sock_write_push(s,b,l) my_send(s, b, l, 0x0000)
#define sock_write_more(s,b,l) my_send(s, b, l, 0x8000)

#define CAD_TAG CAD_TAG_HEAD CAD_TAG_BODY CAD_TAG_TAIL

#define CAD_TAG_BODY_POS (sizeof(CAD_TAG_HEAD)-1)
#define CAD_TAG_BODY_LEN (sizeof(CAD_TAG_BODY)-1)
#define CAD_TAG_TAIL_LEN (sizeof(CAD_TAG_TAIL)-1)
#define CAD_TAG_LEN (sizeof(CAD_TAG)-1)

#define COMMAND_STRING "command/"
#define COMMAND_RESET "Reset"
#define COMMAND_FETCH "Fetch"

#define TOKEN_EQUAL(input,token) \
		(!memcmp(input, token, sizeof(token)-1))
#define PARSE_STRING(token,input,output)				\
	({								\
		int __ret = 0;						\
		if (TOKEN_EQUAL(input, token)) {			\
			char *tmp;					\
									\
			input += sizeof(token)-1;			\
			tmp = output;					\
			while (*input && *input != '&' &&		\
						*input != ',')		\
				*tmp++ = *input++;			\
			*tmp = 0;					\
			__ret = 1;					\
		}							\
		__ret;							\
	})

#define PARSE_UINT(token,input,output)					\
	({								\
		int __ret = 0;						\
		if (TOKEN_EQUAL(input, token)) {			\
									\
			input += sizeof(token)-1;			\
			output = strtoul(input, &input, 10);	\
			__ret = 1;					\
		}							\
		__ret;							\
	})


#endif /* SPEC_DYN_REQ_H */

#endif /* MOD_SPECWEB99 */
