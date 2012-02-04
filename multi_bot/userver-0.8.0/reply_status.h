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


#ifndef REPLY_STATUS_H
#define REPLY_STATUS_H

#include "info.h"


/* Informational 1xx */
#define HTTP_CONTINUE                      (100)
#define HTTP_CONTINUE_IDENT                "Continue"
#define HTTP_SWITCHING_PROTOCOLS           (101)
#define HTTP_SWITCHING_PROTOCOLS_IDENT     "Switching Protocols"

/* Successful 2xx */
#define HTTP_OK                            (200)
#define HTTP_OK_IDENT                      "OK"
#define HTTP_CREATED                       (201)
#define HTTP_CREATED_IDENT                 "Created"
#define HTTP_ACCEPTED                      (202)
#define HTTP_ACCEPTED_IDENT                "Accepted"
#define HTTP_NON_AUTHORITATIVE_INFO        (203)
#define HTTP_NON_AUTHORITATIVE_INFO_IDENT  "Non-Authoritative Information"
#define HTTP_NO_CONTENT                    (204)
#define HTTP_NO_CONTENT_IDENT              "No Content"
#define HTTP_RESET_CONTENT                 (205)
#define HTTP_RESET_CONTENT_IDENT           "Reset Content"
#define HTTP_PARTIAL_CONTENT               (206)
#define HTTP_PARTIAL_CONTENT_IDENT         "Partial Content"

/* Redirection 3xx */
#define HTTP_MULTIPLE_CHOICES              (300)
#define HTTP_MULTIPLE_CHOICES_IDENT        "Multiple Choices"
#define HTTP_MOVED_PERMANENTLY             (301)
#define HTTP_MOVED_PERMANENTLY_IDENT       "Moved Permanently"
#define HTTP_FOUND                         (302)
#define HTTP_FOUND_IDENT                   "Found"
#define HTTP_SEE_OTHER                     (303)
#define HTTP_SEE_OTHER_IDENT               "See Other"
#define HTTP_NOT_MODIFIED                  (304)
#define HTTP_NOT_MODIFIED_IDENT            "Not Modified"
#define HTTP_USE_PROXY                     (305)
#define HTTP_USE_PROXY_IDENT               "Use Proxy"
/* 306 is unused */
#define HTTP_TEMPORARY_REDIRECT            (307)
#define HTTP_TEMPORARY_REDIRECT_IDENT      "Temporary Redirect"

/* Client Error 4xx */
#define HTTP_BAD_REQUEST                   (400)
#define HTTP_BAD_REQUEST_IDENT             "Bad Request"
#define HTTP_UNAUTHORIZED                  (401)
#define HTTP_UNAUTHORIZED_IDENT            "Unauthorized"
#define HTTP_PAYMENT_REQUIRED              (402)
#define HTTP_PAYMENT_REQUIRED_IDENT        "Payment Required"
#define HTTP_FORBIDDEN                     (403)
#define HTTP_FORBIDDEN_IDENT               "Forbidden"
#define HTTP_NOT_FOUND                     (404)
#define HTTP_NOT_FOUND_IDENT               "Not Found"
#define HTTP_METHOD_NOT_ALLOWED            (405)
#define HTTP_METHOD_NOT_ALLOWED_IDENT      "Method Not Allowed"
#define HTTP_NOT_ACCEPTABLE                (406)
#define HTTP_NOT_ACCEPTABLE_IDENT          "Not Acceptable"
#define HTTP_PROXY_AUTH_REQUIRED           (407)
#define HTTP_PROXY_AUTH_REQUIRED_IDENT     "Proxy Authentication Required"
#define HTTP_REQUEST_TIMEOUT               (408)
#define HTTP_REQUEST_TIMEOUT_IDENT         "Request Timeout"
#define HTTP_CONFLICT                      (409)
#define HTTP_CONFLICT_IDENT                "Conflict"
#define HTTP_GONE                          (410)
#define HTTP_GONE_IDENT                    "Gone"
#define HTTP_LENGTH_REQUIRED               (411)
#define HTTP_LENGTH_REQUIRED_IDENT         "Length Required"
#define HTTP_PRECONDITION_FAILED           (412)
#define HTTP_PRECONDITION_FAILED_IDENT     "Precondition Failed"
#define HTTP_REQUEST_TOO_LARGE             (413)
#define HTTP_REQUEST_TOO_LARGE_IDENT       "Request Entity Too Large"
#define HTTP_URI_TOO_LONG                  (414)
#define HTTP_URI_TOO_LONG_IDENT            "Request-URI Too Long"
#define HTTP_UNSUPPORTED_MEDIA_TYPE        (415)
#define HTTP_UNSUPPORTED_MEDIA_TYPE_IDENT  "Unsupported Media Type"
#define HTTP_RANGE_NOT_SATISFIABLE         (416)
#define HTTP_RANGE_NOT_SATISFIABLE_IDENT   "Requested Range Not Satisfiable"
#define HTTP_EXPECTATION_FAILED            (417)
#define HTTP_EXPECTATION_FAILED_IDENT      "Expectation Failed"

/* Server Error 5xx */
#define HTTP_INTERNAL_ERROR                (500)
#define HTTP_INTERNAL_ERROR_IDENT          "Internal Server Error"
#define HTTP_NOT_IMPLEMENTED               (501)
#define HTTP_NOT_IMPLEMENTED_IDENT         "Not Implemented"
#define HTTP_BAD_GATEWAY                   (502)
#define HTTP_BAD_GATEWAY_IDENT             "Bad Gateway"
#define HTTP_SERVICE_UNAVAILABLE           (503)
#define HTTP_SERVICE_UNAVAILABLE_IDENT     "Service Unavailable"
#define HTTP_GATEWAY_TIMEOUT               (504)
#define HTTP_GATEWAY_TIMEOUT_IDENT         "Gateway Timeout"
#define HTTP_VERSION_NOT_SUPPORTED         (505)
#define HTTP_VERSION_NOT_SUPPORTED_IDENT   "HTTP Version Not Supported"


#define HTTP_OK_STR_ID_LEN_TYPE_NOBLANK "HTTP/1.%d 200 OK\r\nServer: %s" \
   "\r\nContent-Length: %lu\r\nContent-Type: %s\r\n"
#define HTTP_OK_STR_ID_LEN_TYPE HTTP_OK_STR_ID_LEN_TYPE_NOBLANK "\r\n"

#define HTTP_OK_STR_ID_LEN_NOBLANK "HTTP/1.%d 200 OK\r\nServer: %s" \
   "\r\nContent-Length: %lu\r\n"
#define HTTP_OK_STR_ID_LEN HTTP_OK_STR_ID_LEN_NOBLANK "\r\n"

#define HTTP_HDR_STR_ID_LEN_TYPE_NOBLANK "HTTP/1.%d %d %s\r\nServer: %s" \
   "\r\nContent-Length: %lu\r\nContent-Type: %s\r\n"
#define HTTP_HDR_STR_ID_LEN_TYPE HTTP_HDR_STR_ID_LEN_TYPE_NOBLANK "\r\n"

#define HTTP_HDR_STR_ID_LEN_NOBLANK "HTTP/1.%d %d %s\r\nServer: %s" \
   "\r\nContent-Length: %lu\r\n"
#define HTTP_HDR_STR_ID_LEN HTTP_HDR_STR_ID_LEN_NOBLANK "\r\n"

#define HTTP_BAD_REQUEST_STR \
  "<html>\r\n400 Bad Request<br><br>\r\n" \
  "Your browser sent a request that this server could not understand.\r\n" \
  "</html>\r\n"

#define HTTP_UNAUTHORIZED_STR \
  "<html>\r\n401 Unauthorized Request<br><br>\r\n" \
  "Your browser sent a request that this server is not authorized to " \
    "respond to.\r\n" \
  "Request was for %s\r\n" \
  "</html>\r\n"

#define HTTP_FORBIDDEN_STR \
  "<html>\r\n403 Forbidden Request<br><br>\r\n" \
  "Your browser sent a request that this is forbidden on this server.\r\n" \
  "Request was for %s\r\n" \
  "</html>\r\n"

#define HTTP_NOT_FOUND_STR \
  "<html>\r\n404 Not Found<br><br>\r\n" \
  "The requested URL %s was not found on this server.\r\n" \
  "</html>\r\n"

#define HTTP_10_NOT_MODIFIED_STR \
  "HTTP/1.0 304 Not Modified\r\nDate: "

#define HTTP_11_NOT_MODIFIED_STR \
  "HTTP/1.1 304 Not Modified\r\nDate: "

#define HTTP_NOT_MODIFIED_STRLEN (33)

#define HTTP_NOT_IMPLEMENTED_STR \
  "<html>\r\n501 Service or Request Type Not Implemented<br><br>\r\n" \
  "</html>\r\n"

#define HTTP_SERVICE_UNAVAILABLE_STR \
  "<html>\r\n503 Service Unavailable<br><br>\r\n" \
  "The server is currently overloaded. Try again later.\r\n" \
  "</html>\r\n"

int reply_status(int fd, int status, char *str);
int reply_status_fill(struct info *ip, int status, char *str, int version);

#endif /* REPLY_STATUS_H */
