/*
    userver -- (pronounced you-server or micro-server).
    This file is part of the userver, a high-performance web server designed for
    performance experiments.

    Copyright (C) 2005-2010 Tim Brecht
    Based on the file originally Copyright (C) 2004  Hewlett-Packard Company

    Authors: Tim Brecht <brecht@cs.uwaterloo.ca>
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


#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "debug.h"
#include "common.h"
#include "options.h"
#include "util.h"

// #define LOCAL_DEBUG
#include "local_debug.h"


/* ---------------------------------------------------------------------- */
req_type_t
analyze_req(struct req *req)
{
  app_t *app;

  assert(req != NULL);
  req->type = REQ_INVALID;
  req->app = NULL;

  /* if there's a question mark in the URI, what follows is a query string */
  if ((req->query_string = strchr(req->uri, '?')) != NULL) {
    *(req->query_string++) = '\0';
  } 

  /* Just hard code how to handle php scripts for now */
  if (strstr(req->uri, ".php") != NULL)  {
    ldbg("analyze_req: is a php script req->uri = %s\n", req->uri);
    app = find_app("php");
    if (app != NULL) {
      ldbg("analyze_req: found php app, app->uri = %s and req_type = %d\n", app->uri, app->req_type);
      req->type = app->req_type;
      req->app = app;
    } else {
      ldbg("analyze_req: did not find php app\n");
    }
    ldbg("analyze_req: found php app and returning\n");
    return req->type;
  }

  if (req->content_len > 0 && req->method != HTTP_POST) {
    /* only POST requests may have content */
    ldbg("analyze_req: looks like a non HTTP_POST but has content, may not be handled properly\n");
  } else if (req->method == HTTP_POST || req->query_string != NULL) {
    /* looks like a dynamic request; find an application to handle it */
    ldbg("analyze_req: looking up app\n");
    app = find_app(req->uri + options.doc_root_len);
    if (app != NULL) {
      ldbg("analyze_req: found app, app->uri = %s and req_type = %d\n", app->uri, app->req_type);
      req->type = app->req_type;
      req->app = app;
    } else {
      ldbg("analyze_req: did not find app\n");
    }
  } else {
    ldbg("analyze_req: determined that request is static\n");
    req->type = REQ_STATIC;
  }

  return req->type;
}


/* ---------------------------------------------------------------------- */
int
parse_bytes(struct req *req)
{
  char *cur, *end;
  int position;
  int rc = -1;  // assume failure

  assert(req->state != PARSE_DONE);

  cur = req->cur;
  end = req->end;

  while (cur < end && req->state != PARSE_DONE) {
    if (req->skip_lws)
    {
      if (*cur == ' ' || *cur == '\t') {
	ldbg("parse_bytes: skipping whitespace\n");
	cur++;
	continue;
      } else {
	req->skip_lws = 0;
      }
    }
    ldbg("parse_bytes: *cur = [%s] req->state = %d\n",
	nice_char(*cur), req->state);
    ldbg("parse_bytes: req->tmp_len = %d buf = [%s] req->tmp = [%s]\n",
	req->tmp_len,
	nice_strn(cur, end - cur),
	nice_strn(req->tmp, req->tmp_len));

    switch (req->state) {
      case PARSE_HEADER_NAME:
	while (cur < end && *cur != ':' && !isspace((int) *cur)) {
	  req->tmp[req->tmp_len++] = *cur++;
	  if (req->tmp_len >= (int) sizeof(req->tmp)) {
	    ldbg("parse_bytes: req->tmp_len too large\n");
	    goto parse_bytes_done;
	  }
	}
	if (cur == end) {
	  ldbg("parse_bytes: end of buf reached\n");
	  break;
	}

	req->tmp[req->tmp_len] = '\0';
	req->tmp_len = 0;
	ldbg("parse_bytes: req->tmp = [%s]\n", nice_str(req->tmp));

	if (*cur == '\r' || *cur == '\n') {
	  if (req->tmp_len == 0) {
	    /* special case: an empty line marks end of headers */
	    req->state = PARSE_CR_2;
	  } else {
	    req->state = PARSE_CR_1;
	  }
	} else {
	  req->state = PARSE_COLON;
	  req->skip_lws = 1;
	}
	break;

      case PARSE_COLON:
	if (*cur == ':') {
	  ldbg("parse_bytes: PARSE_COLON got COLON\n");
	  /* header names are NOT case sensitive -- use stricmp() */
	  if (stricmp(req->tmp, "Content-Length") == 0) {
	    req->state = PARSE_CONTENT_LENGTH;
	    req->skip_lws = 1;
	  } else if (stricmp(req->tmp, "If-Modified-Since") == 0) {
	    req->state = PARSE_IF_MODIFIED_SINCE;
	    req->skip_lws = 1;
#ifdef CALL_STATS
	  } else if (stricmp(req->tmp, "Client-Id") == 0) {
	    req->state = PARSE_CLIENT_ID;
	    req->skip_lws = 1;
#endif /* CALL STATS */
	  } else if (stricmp(req->tmp, "Cookie") == 0) {
	    if (req->cookie_len > 0) {
	      /* append the new cookie to the end of existing cookie */
	      if (req->cookie_len + 2 >= (int) sizeof(req->cookie)) {
		ldbg("parse_bytes: req->cookie_len too large\n");
		goto parse_bytes_done;
	      }
	      req->cookie[req->cookie_len++] = ';';
	      req->cookie[req->cookie_len++] = ' ';
	    }
	    req->state = PARSE_COOKIE;
	    req->skip_lws = 1;
	  } else {
	    ldbg("parse_bytes: unrecognized header [%s]\n",
		    nice_str(req->tmp));
	    req->state = PARSE_SKIP;
	  }
	  cur++;
	} else {
	  ldbg("parse_bytes: expected colon\n");
	  req->state = PARSE_SKIP;
	}
	break;

      case PARSE_COOKIE:
        while (cur < end) {
	  ldbg("parse_bytes: PARSE_COOKIE *cur = [%s]\n", nice_char(*cur));
          if (*cur == '\r' || *cur == '\n') {
	    ldbg("parse_bytes: PARSE_COOKIE done\n");
            req->state = PARSE_CR_1;
	    break;
          } else {
	    req->cookie[req->cookie_len++] = *cur++;
	    if (req->cookie_len >= (int) sizeof(req->cookie)) {
	      ldbg("parse_bytes: req->cookie_len too large\n");
	      goto parse_bytes_done;
	    }
          }
        }
	break;

      case PARSE_IF_MODIFIED_SINCE:
        while (cur < end) {
	  ldbg("parse_bytes: PARSE_IF_MODIFIED_SINCE *cur = [%s]\n", nice_char(*cur));
          if (*cur == '\r' || *cur == '\n') {
	    ldbg("parse_bytes: PARSE_IF_MODIFIED_SINCE done\n");
	    req->tmp[req->tmp_len] = '\0';
	    req->tmp_len = 0;
	    if (!http_str_to_time(req->tmp, &req->ifmodsince)
		&& req->ifmodsince) {
	      ldbg("parse_bytes: date conversion failed [%s]\n", req->tmp);
	      goto parse_bytes_done;
	    }
            req->state = PARSE_CR_1;
	    break;
          } else {
	    req->tmp[req->tmp_len++] = *cur++;
	    if (req->tmp_len >= (int) sizeof(req->tmp)) {
	      ldbg("parse_bytes: req->tmp_len too large\n");
	      goto parse_bytes_done;
	    }
          }
        }
	break;

#ifdef CALL_STATS
      case PARSE_CLIENT_ID:
	while (cur < end && !isspace((int) *cur)) {
	  if (isdigit((int) *cur)) {
	    req->client_id = (req->client_id * 10) + (*cur - '0');
	    if (req->client_id > MAX_CLIENT_ID) {
	      ldbg("parse_bytes: client id too large\n");
	      goto parse_bytes_done;
	    }
	    cur++;
	  } else {
	    ldbg("parse_bytes: client_id invalid char [%s] in Client Id\n",
		nice_char(*cur));
	    goto parse_bytes_done;
	  }
	}

        /* skip over spaces to get to the next number */
        while (isspace((int) *cur)) {
	    ldbg("parse_bytes: client_id skipping space\n");
            cur++;
        }

	while (cur < end && !isspace((int) *cur)) {
	  if (isdigit((int) *cur)) {
	    req->call_id = (req->call_id * 10) + (*cur - '0');
	    if (req->call_id > MAX_CLIENT_ID) {
	      ldbg("parse_bytes: client id too large\n");
	      goto parse_bytes_done;
	    }
	    cur++;
	  } else {
	    ldbg("parse_bytes: client_id invalid char [%s] in Client Id\n",
		nice_char(*cur));
	    goto parse_bytes_done;
	  }
	}


	if (cur == end) {
	  ldbg("parse_bytes: client_id end of buf reached\n");
	  break;
	}

	ldbg("parse_bytes: Client Id is %d call id is %d\n", req->client_id, req->call_id);
	req->state = PARSE_HEADER_NAME;
        break;
#endif /* CALL_STATS */

      case PARSE_CONTENT_LENGTH:
	while (cur < end && !isspace((int) *cur)) {
	  if (isdigit((int) *cur)) {
	    req->content_len = (req->content_len * 10) + (*cur - '0');
	    if (req->content_len > MAX_CONTENT_LEN) {
	      ldbg("parse_bytes: content-length too large\n");
	      goto parse_bytes_done;
	    }
	    cur++;
	  } else {
	    ldbg("parse_bytes: invalid char [%s] in content-length\n",
		nice_char(*cur));
	    goto parse_bytes_done;
	  }
	}
	if (cur == end) {
	  ldbg("parse_bytes: end of buf reached\n");
	  break;
	}

	ldbg("parse_bytes: content length is %d\n", req->content_len);
	req->state = PARSE_SKIP;
	break;

      case PARSE_SKIP:
        /* skip over any characters until we get to CR or LF */
        while (cur < end) {
	  ldbg("parse_bytes: PARSE_SKIP *cur = [%s]\n", nice_char(*cur));
          if (*cur == '\r' || *cur == '\n') {
	    ldbg("parse_bytes: PARSE_SKIP got rid of characters\n");
            req->state = PARSE_CR_1;
	    break;
          } else {
            cur++;
          }
        }
	break;

      case PARSE_CR_1:
      case PARSE_CR_2:
	ldbg("parse_bytes: PARSE_CR *cur = [%s]\n", nice_char(*cur));
	if (*cur == '\r') {
	  if (req->state == PARSE_CR_1) {
	    req->state = PARSE_LF_1;
	  } else {
	    req->state = PARSE_LF_2;
	  }
	  ldbg("parse_bytes: PARSE_CR got CR\n");
	} else {
	  req->state = PARSE_SKIP;
	}
	cur++;
        break;

      case PARSE_LF_1:
      case PARSE_LF_2:
	ldbg("parse_bytes: PARSE_LF *cur = [%s]\n", nice_char(*cur));
	if (*cur == '\n') {
	  ldbg("parse_bytes: PARSE_LF got LF\n");
	  switch (req->method) {
	    case HTTP_NONE:
	      /*
	       * We got a blank line where we were expecting a method.
	       * Uncomment one of the two following lines to either allow
	       * it or return an error.
	       */
	      req->state = PARSE_METHOD;	// allow it
	      //goto parse_bytes_done;		// return an error
	      break;
	    case HTTP_SIMPLE:
	      /* the request is equivalent to "GET uri HTTP/1.0" */
	      req->method = HTTP_GET;
	      /* explicitly forbid headers */
	      req->state = PARSE_DONE;
	      break;
	    default:
	      if (req->state == PARSE_LF_1) {
		req->state = PARSE_HEADER_NAME;
	      } else {
		req->state = PARSE_DONE;
	      }
	      break;
	  }
	} else {
	  req->state = PARSE_SKIP;
	}
	cur++;
	break;

      case PARSE_METHOD:
	while (cur < end && !isspace((int) *cur)) {
	  req->tmp[req->tmp_len++] = *cur++;
	  if (req->tmp_len >= (int) sizeof(req->tmp)) {
	    ldbg("parse_bytes: req->tmp_len too large\n");
	    goto parse_bytes_done;
	  }
	}
	if (cur == end) {
	  ldbg("parse_bytes: end of buf reached\n");
	  break;
	}

	req->tmp[req->tmp_len] = '\0';
	req->tmp_len = 0;
	ldbg("parse_bytes: req->tmp = [%s]\n", nice_str(req->tmp));

	/* methods ARE case sensitive -- use strcmp() */
	if (strcmp(req->tmp, "GET") == 0) {
	  req->method = HTTP_GET;
	  ldbg("parse_bytes: GET\n");
	} else if (strcmp(req->tmp, "HEAD") == 0) {
	  req->method = HTTP_HEAD;
	  ldbg("parse_bytes: HEAD\n");
	} else if (strcmp(req->tmp, "POST") == 0) {
	  req->method = HTTP_POST;
	  ldbg("parse_bytes: POST\n");
	} else if (req->tmp[0] == '\0') {
	  /*
	   * We got a blank line (possibly with whitespace) where we were
	   * expecting a method.  We'll handle this condition later, once
	   * we've processed the linefeed.
	   */
	  req->state = PARSE_CR_1;
	  break;
	} else {
	  ldbg("parse_bytes: unrecognized method [%s]\n",
		  nice_str(req->tmp));

	  req->method = HTTP_NOT_IMPL;
	}
	req->skip_lws = 1;
	req->state = PARSE_URI;
	break;

      case PARSE_URI:
	/* ignore leading slash */
	if (*cur == '/') {
	  ldbg("parse_bytes: parsing uri *cur = [%s]\n", nice_char(*cur));
	  cur++;
	}
	while (cur < end && !isspace((int) *cur)) {
	  ldbg("parse_bytes: parsing uri *cur = [%s]\n", nice_char(*cur));
	  position = options.doc_root_len + req->tmp_len;
	  req->uri[position] = *cur++;
	  req->tmp_len++;

	  if (options.doc_root_len + req->tmp_len >= (int) sizeof(req->uri)) {
	    ldbg("parse_bytes: req->tmp_len too large\n");
	    goto parse_bytes_done;
	  }
	}
	if (cur == end) {
	  ldbg("parse_bytes: end of buf reached\n");
	  break;
	}

	req->uri[options.doc_root_len + req->tmp_len] = '\0';
	req->tmp_len = 0;
	req->skip_lws = 1;
	req->state = PARSE_VERSION;
	ldbg("parse_bytes: parsing uri state now %d\n", req->state);
	break;

      case PARSE_VERSION:
	while (cur < end && !isspace((int) *cur)) {
	  req->tmp[req->tmp_len++] = *cur++;
	  if (req->tmp_len >= (int) sizeof(req->tmp)) {
	    ldbg("parse_bytes: req->tmp_len too large\n");
	    goto parse_bytes_done;
	  }
	}
	if (cur == end) {
	  ldbg("parse_bytes: end of buf reached\n");
	  break;
	}

	req->tmp[req->tmp_len] = '\0';
	req->tmp_len = 0;
	ldbg("parse_bytes: req->tmp = [%s]\n", nice_str(req->tmp));

	if (strcmp(req->tmp, "HTTP/1.1") == 0) {
	  ldbg("parse_bytes: HTTP/1.1\n");
	  req->close = 0;
	} else if (strcmp(req->tmp, "HTTP/1.0") == 0) {
	  ldbg("parse_bytes: HTTP/1.0\n");
	  req->close = 1;
	} else if (req->tmp[0] == '\0') {
	  /*
	   * HTTP/1.0 and earlier allow "simple" requests that omit the
	   * version field.  These requests are supposed to be limited to
	   * the GET method.
	   */
	  if (req->method != HTTP_GET) {
	    ldbg("parse_bytes: simple requests only support GET method\n");
	    goto parse_bytes_done;
	  }
	  ldbg("parse_bytes: simple HTTP/1.0\n");
	  /*
	   * We store the method as HTTP_SIMPLE for now, as a hint to our
	   * subsequent parsing logic (we need to know to forbid headers).
	   * After parsing, the method will revert to HTTP_GET.
	   */
	  req->method = HTTP_SIMPLE;
	  req->close = 1;
	} else {
	  ldbg("parse_bytes: HTTP version [%s] unknown\n",
	      nice_str(req->tmp));
	  goto parse_bytes_done;
	}
	req->state = PARSE_SKIP;
	ldbg("parse_bytes: state now %d\n", req->state);
	break;

      default:
	fprintf(stderr, "Request state %d is invalid\n", req->state);
	exit(1);
    } /* switch */
  } /* while */

  if (req->state == PARSE_DONE) {
    ldbg("parse_bytes: parse done\n");
    req->cookie[req->cookie_len] = '\0';
    analyze_req(req);
    PRINT_TIME(NOFD, &tnow, &tprev, "req->type = %d", req->type);
    if (req->type == REQ_INVALID) {
      printf("parse_bytes: unable to determine type of request\n");
      assert(0);
      req->type = REQ_STATIC;
      goto parse_bytes_done;
    }
  }

  rc = 0;  // everything's OK

parse_bytes_done:
  ldbg("parse_bytes: returning 0 state = %d\n", req->state);
  ldbg("parse_bytes: loop req->tmp [%s]\n", nice_strn(req->tmp, req->tmp_len));
  /*
   * For convenience, and speed, we've been using 'cur' instead of 'req->cur'
   * throughout this function.  Now we must update req->cur before returning.
   */
  if (cur == end) {
    req->end = req->cur = req->buf;
  } else {
    req->cur = cur;
  }
  return rc;
}
