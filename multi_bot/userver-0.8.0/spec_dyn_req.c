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

#include <time.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/sendfile.h>
#include <errno.h>
#include "common.h"
#include "spec_dyn_req.h"

#undef BUG
#define BUG()							\
do {								\
	printf("SPEC DYN REQ BUG at %d:%s!\n", __LINE__, __FILE__);	\
	exit(EXIT_FAILURE);						\
} while (0)

/* use only one of the following 2 options at a time 
 * workaround to make socket I/O blocking for the duration
 * of the dynamic sec requests 
 */

 /* keep polling if write would block- NOTE: This has not been intensively tested */
/*#define DYN_SOC_POLL_HACK  */ 

/* use fcntl to clear the non-blocking flag */
#define DYN_SOC_CLEAR_NONBLOCK  


#define POSTLOG "postlog"
int postlog_file = -1;

int this_pid;

static inline int send_reply_head (user_req_t *ip, size_t body_size);
static inline int send_reply_tail (user_req_t *ip);
static time_t last_mtime;


/* Returns >0 if request was processed correctly */
int spec_dyn_req (user_req_t *ip) {
   int ret = 0;

#ifdef DYN_SOC_CLEAR_NONBLOCK
   /*clear nonblocking flag on socket - Code adapted from APUE- Stevens*/
   int val=0;
   if ( (val=fcntl(ip->sd,F_GETFL,0)) < 0)
      return 0;
   val &= ~ O_NONBLOCK;
   if (fcntl(ip->sd,F_SETFL,val) < 0)
      return 0;
#endif
   this_pid = getpid();

   for(;;) {
      /* Note that the way the request states are used by the spec dynamic
       * handler might not correspond to the states they originally intended
       * to represent. Instead of adding another enum, I have decided to use
       * these existing states - AS
       */
      switch (ip->req.state) {
      case PARSE_DONE:
         ret = CAD_query (ip);
         break;

      case SPECWEB99_REQUEST_DONE:
         ret = CAD_send (ip);
         break;

      case SPECWEB99_RESPONSE_DONE:
         ret = CAD_finish_send (ip);
         break;

      case SPECWEB99_SOCKET_CLOSED:
         ret = 1;
         goto out;

      default:
         ret = error(ip, "invalid request state \n");
         break;
      }/*end switch*/
      if(ret<0)
         goto out;
   }/*end for*/

out:
   if(ip->priv) {
      free(ip->priv);
      ip->priv = NULL;
   }
   /* reset request method */
   ip->req.state = PARSE_METHOD;

#ifdef DYN_SOC_CLEAR_NONBLOCK
   /* reset socket to nonblocking */
   val=0;
   if ( (val=fcntl(ip->sd,F_GETFL,0)) < 0)
      return 0;
   val |=  O_NONBLOCK;
   if (fcntl(ip->sd,F_SETFL,val) < 0)
      return 0;
#endif

   return ret;
}


/*
 * Return SPECweb99 error message.
 */
static int error (user_req_t *ip, char *message) {
   CAD_t *CADp = (CAD_t *)ip->priv;
   int len = strlen(message);
   int bytes, ret;

   fprintf(stderr,"query=%s\ncookie=%s\n", ip->req.query_string,ip->req.cookie);
   if (ip->req.method == HTTP_POST) {
      fprintf(stderr, "content_len=%d  buf=%s\n",
	  ip->req.content_len, nice_strn(ip->req.cur, ip->req.content_len));
   }
   fflush(stderr);

   fprintf(stderr,"---=> error(): %s\n<=---\n", message);

   /*
    * Return a -1 Ad_id in the reply cookie.
    */
   if (CADp)
      CADp->reply_cookies_len = sprintf(CADp->reply_cookies,
                                        "found_cookie=Ad_id=-1&Ad_weight=0&Expired=1");

   bytes = send_reply_head(ip, len);
   ret = sock_write_more(ip->sd, message, len);

   if (ret != len) {
      fprintf(stderr,"---=> error(): %s\n<=---\n", message);
   }

   bytes += len;
   bytes += send_reply_tail(ip);
   fflush(stderr);
   ip->rep.nwritten = bytes;
   return -1;
}


static int CAD_query (user_req_t *ip) {
   int ret = 0;
   CAD_t *CADp;
   if (ip->priv)
      goto CADp_error;

   CADp = parse_GET_cookies(ip);

   if (ip->req.method == HTTP_POST) {
      ip->req.state = SPECWEB99_SOCKET_CLOSED;
      if (!CADp)
         goto CADp_error;
      ret = do_POST(ip);
      if (ret ) {
         return ret;
      }
      CADp = (CAD_t *)ip->priv;
      strcpy(ip->rep.objectname, TOPDIR);
      strcat(ip->rep.objectname, CADp->ad_filename);
   } else {
      char *tmp = ip->req.query_string;
      if (ip->req.method != HTTP_GET)
         goto url_error;
      if (TOKEN_EQUAL(ip->req.query_string, COMMAND_STRING)) {
         ip->req.state = SPECWEB99_SOCKET_CLOSED;
         tmp += sizeof(COMMAND_STRING)-1;
         return do_command(ip, tmp);
      }
      strcpy(ip->rep.objectname,TOPDIR);
      strcat(ip->rep.objectname, ip->req.query_string);
      if (CADp && (CADp->user_id != -1)) {
         ret = custom_ad_rotate(ip, CADp);
         if (ret)
            goto CAD_error;
      }
   }

   ip->req.state = SPECWEB99_REQUEST_DONE;
   ret = populate_object(ip);
   if (ret < 0)
      goto get_object_error;
   return ret;

url_error:
   return error(ip, "error while parsing CAD request!\n");
get_object_error:
   return error(ip, "error while trying to fetch URL object!\n");
CADp_error:
   return error(ip, "CADp error!\n");
CAD_error:
   return error(ip, "CAD-rotate error!\n");
}


static CAD_t * parse_GET_cookies (user_req_t *ip) {
   int uid, last_ad;
   CAD_t *CADp;
   char *tmp;

   if (!strlen(ip->req.cookie))
      return NULL;

   if (ip->priv)
      error(ip, "error, priv structure already exists!\n");

   CADp = (CAD_t *) malloc(sizeof(*CADp));
   ip->priv = CADp;
   CADp->reply_cookies_len = 0;

   tmp = ip->req.cookie + sizeof("my_cookie=user_id=")-1;
   uid = strtoul(tmp, &tmp, 10) - 10000;

   tmp += sizeof("&last_ad=")-1;
   last_ad = strtoul(tmp, &tmp, 10);

   CADp->user_id = uid;
   CADp->last_ad = last_ad;

   return CADp;
}

static int do_POST (user_req_t *ip) {
   char buf[400], count_buf[10], urlroot[100], *tmp, *curr;
   int dir = -1, post_class = -1, num = -1, client = -1;
   CAD_t *CADp = (CAD_t *) ip->priv;
   int size, ret;

   /* Hack to ensure we send the right response to the spec perl manager script
    * Does not affect any other post requests 
    */
   if (ip->req.content_len == 0) {
     goto parse_error;
   }
   *ip->req.end = '\0';
   curr = ip->req.cur;

#define POST_URLROOT "urlroot="
#define POST_CLASS "class="
#define POST_CLIENT "client="
#define POST_DIR "dir="
#define POST_NUM "num="

   for (;;) {
      switch (*curr) {
      case 'u':
         if (PARSE_STRING( POST_URLROOT, curr, urlroot))
            continue;
         goto parse_error;
      case 'c':
         if (PARSE_UINT( POST_CLASS, curr, post_class))
            continue;
         if (PARSE_UINT( POST_CLIENT, curr, client))
            continue;
         goto parse_error;
      case 'd':
         if (PARSE_UINT( POST_DIR, curr, dir))
            continue;
         goto parse_error;
      case 'n':
         if (PARSE_UINT( POST_NUM, curr, num))
            continue;
         goto parse_error;
      case '&':
         curr++;
         continue;
      case 0:
         goto out;
      default:
         goto parse_error;
      }
      goto parse_error;
   }
out:
   if (!CADp)
      goto parse_error;
   tmp = CADp->ad_filename;
   tmp += sprintf(tmp, "%sdir%05d/class%d_%d", urlroot, dir, post_class, num);

   /*  FIXME: If we are to have multiple processes to handle spec requests,
    *	   we need a synchronization mechanism of our own, to ensure that 
	 *    writes to the postlog are atomic
    */
   if ((postlog_file) == -1)
      open_postlog_file(ip);
   if (postlog_file == -1)
      goto file_error;

   /* Go to the end of file, and add a POST record */
   size = lseek (postlog_file, 0, SEEK_END);
   if (size == -1)
      goto file_error;
   sprintf(count_buf, "%10d", size/139 + 1);

   tmp = buf;
   tmp += sprintf(tmp, "%10d %10ld %10d %5d %2d %2d %10d %-60.60s %10d %10d\n", atoi(count_buf), time(NULL), this_pid, dir, post_class, num, client, CADp->ad_filename, this_pid, CADp->user_id + 10000);
   //printf("before write, plogfile in %d is %d\n",this_pid,postlog_file);fflush(stdout);
   /* FIXME: File i/o can block. */
   ret = write(postlog_file, buf, tmp-buf);
   //printf("1st ret=%d\n",ret);
   //if(ret==-1) perror("1st ret failed");
   lseek(postlog_file, 0, SEEK_SET);
   ret += write(postlog_file, count_buf, 10);
   /*printf("2nd ret=%d\n",t);
   if(t==-1) perror("2nd ret failed");
   ret+=t;*/
   if (ret != tmp-buf+10){
      //printf("Expected to write %d bytes, wrote only %d\n",(tmp-buf+10),ret);
      //fflush(stdout);
      //if(ret==-1) perror("sock_write_failed");
      goto sock_write_error;
   }
   CADp->reply_cookies_len = sprintf(CADp->reply_cookies,"my_cookie=%u", 10000 + CADp->user_id);
   return 0;

parse_error:
   return error(ip,"error while parsing POST request!\n");

file_error:
   return error(ip, "POST-log file error!\n");

sock_write_error:
   return error(ip, "POST-log sock_write error!\n");
}



static int my_send (int sock, char *buf, int total_len, int flags) {
   int len = total_len, ret;

   /*FIXME: Uses blocking socket i/o*/
repeat:
   ret = send(sock, buf, len, flags);
   if (ret == -EAGAIN)
      goto repeat;
   if (ret <= 0)
      return ret;
   len -= ret;
   buf += ret;
   if (len)
      goto repeat;
   return total_len;
}


/*
 * Determine whether the URL is class1 or 2;
 */
static inline int is_class12 (char *str) {
   char *tmp;

   tmp = strstr(str, "class");
   if (tmp) {
      tmp += sizeof("class")-1;
      if ((tmp[0] != '1') && (tmp[0] != '2'))
         return 0;
      return 1;
   }
   return 0;
}


typedef struct user_dem_s {
   unsigned int dem;
}
user_dem_t;

static int max_userid;
static user_dem_t *user_dem;

#define USER_PERS_FILE "User.Personality"
#define USER_PERS_RECLEN 15

typedef struct ad_s {
   unsigned int dem;
   unsigned int gender_weight;
   unsigned int age_weight;
   unsigned int region_weight;
   unsigned int interest_1_weight;
   unsigned int interest_2_weight;
   unsigned int min_match;
   unsigned int expires;
}
ad_t;

static int max_adid;
static ad_t *ad;

#define AD_FILE "Custom.Ads"
#define AD_RECLEN 39

static int read_custom_ads (user_req_t *ip) {
   int file;
   int ret, len, err = -2;
   char *buf = NULL, *tmp;
   unsigned int adid, i, dem, min_match, weight, expires;
   struct stat sbuf;
   char tmpstr[256];

   strcpy(tmpstr, TOPDIR);
   strcat(tmpstr, AD_FILE);
   file = open(tmpstr, O_RDONLY);
   if (file == -1)
      goto error;

   if (fstat(file, &sbuf))
      goto error;
   len = sbuf.st_size;

   if (!len)
      goto error;
   if (ad) {
      free(ad);
      ad = NULL;
   }
   max_adid = len/AD_RECLEN + 1;
   ad = (ad_t *) malloc((max_adid) * sizeof(ad_t));
   buf = (char *) malloc(len);
   if (!ad || !buf)
      goto error;

   ret = read(file, buf, len);
   close(file);
   if (ret != len)
      goto error;
   /*
    * Sample ad record:
   	"   54 24808100    97F61  75  952393980\n"
    */

   tmp = buf;
   i = 0;
   adid = 0;
   for (tmp = buf; tmp < buf+len; tmp++) {

      while (*tmp == ' ')
         tmp++;
      adid = strtoul(tmp, &tmp, 10);
      if (adid != i)
         goto error;
      if ((int) adid >= max_adid)
         goto error;
      i++;
      if (*tmp != ' ')
         goto error;
      tmp++;
      while (*tmp == ' ')
         tmp++;
      dem = strtoul(tmp, &tmp, 16);
      tmp++;
      while (*tmp == ' ')
         tmp++;
      weight = strtoul(tmp, &tmp, 16);
      while (*tmp == ' ')
         tmp++;
      min_match = strtoul(tmp, &tmp, 10);
      while (*tmp == ' ')
         tmp++;
      expires = strtoul(tmp, &tmp, 10);
      if (*tmp != '\n')
         goto error;
      ad[adid].dem = dem;

      ad[adid].gender_weight		= (weight & 0x000f0000) >> 16;
      ad[adid].age_weight		= (weight & 0x0000f000) >> 12;
      ad[adid].region_weight		= (weight & 0x00000f00) >> 8;
      ad[adid].interest_1_weight	= (weight & 0x000000f0) >> 4;
      ad[adid].interest_2_weight	= (weight & 0x0000000f);

      ad[adid].min_match = min_match;
      ad[adid].expires = expires;

   }
   err = 0;
error:
   if (buf)
      free(buf);
   if (err)
      return error(ip, "error while parsing the ad file.\n");
   return err;
}

static int read_user_personality (user_req_t *ip) {
   int file;
   int ret, len, err = -2;
   char *buf = NULL, *tmp;
   int uid, i;
   unsigned int dem;
   struct stat sbuf;
   char tmpstr[256];

   strcpy(tmpstr, TOPDIR);
   strcat(tmpstr, USER_PERS_FILE);
   file = open(tmpstr, O_RDONLY);
   if (file == -1)
      goto error;
   if (fstat(file, &sbuf))
      goto error;
   len = sbuf.st_size;

   if (!len)
      goto error;
   if (user_dem) {
      free(user_dem);
      user_dem = NULL;
   }
   max_userid = len/USER_PERS_RECLEN + 1;
   user_dem = (user_dem_t *) malloc((max_userid) * sizeof(user_dem_t));
   buf = (char *) malloc(len);
   if (!user_dem || !buf)
      goto error;

   ret = read(file, buf, len);
   close(file);
   if (ret <= 0)
      goto error;

   tmp = buf;
   i = 0;
   for (tmp = buf; tmp != buf+len; tmp++) {
      if (*tmp == ' ')
         continue;
      uid = strtoul(tmp, &tmp, 10);
      if (uid != i)
         goto error;
      if (uid >= max_userid)
         goto error;
      i++;
      if (*tmp != ' ')
         goto error;
      while (*tmp == ' ')
         tmp++;
      dem = strtoul(tmp, &tmp, 16);
      if (*tmp != '\n')
         goto error;
      (user_dem)[uid].dem = dem;
   }
   err = 0;
error:
   if (buf)
      free(buf);
   if (err)
      return error(ip, "error while parsing the user file.\n");
   return err;
}

#define MAX_CUSTOM_ADS 360

static inline int find_ad (int user_id, int last_ad, int *weight_p) {
   int adid, dem;
   unsigned int weight = 0;

   if ((user_id < 0) || (user_id > max_userid))
      return -1;
   for (adid = last_ad + 1; adid != last_ad; adid++) {
      if (adid >= MAX_CUSTOM_ADS) {
         adid = 0;
         if (last_ad == 0)
            break;
      }

      dem = (user_dem)[user_id].dem & ad[adid].dem;
      weight = 0;

      if (dem & 0x30000000)
         weight += ad[adid].gender_weight;
      if (dem & 0x0f000000)
         weight += ad[adid].age_weight;
      if (dem & 0x00f00000)
         weight += ad[adid].region_weight;
      if (dem & 0x000ffc00)
         weight += ad[adid].interest_1_weight;
      if (dem & 0x000003ff)
         weight += ad[adid].interest_2_weight;
      if (weight >= ad[adid].min_match)
         break;
   }

   *weight_p = weight;
   return adid;
}

static inline int reread_files (user_req_t *ip) {
   int ret = -2;
   struct stat buf;
   char tmpstr[256];

   strcpy(tmpstr, TOPDIR);
   strcat(tmpstr, USER_PERS_FILE);
   stat(tmpstr, &buf);

   if (buf.st_mtime != last_mtime) {
      void *tmp = user_dem;

      user_dem = NULL;
      free(tmp);
      if (read_user_personality(ip))
         goto error;
      if (read_custom_ads(ip))
         goto error;
      last_mtime = buf.st_mtime;
   }
   ret = 0;

error:
   return ret;
}


static void open_postlog_file (user_req_t *ip) {
   //printf("Opening postlog_file in %d\n",this_pid);fflush(stdout);
   if (postlog_file != -1)
      error(ip, "POST-log already exists!\n");
   postlog_file = open(POSTLOG, O_RDWR);
   if ((postlog_file) == -1)
      error(ip, "could not open POST-log!\n");
}

static int init_postlog_file (user_req_t *ip) {
   char count_buf[11];
   //printf("Init postlog file in %d\n",this_pid);fflush(stdout);
   if (postlog_file != -1)
      close(postlog_file);
   postlog_file = open(POSTLOG, O_CREAT|O_TRUNC|O_RDWR, S_IRWXU);
   if ((postlog_file) == -1)
      return error(ip, "could not open POST-log!\n");
   sprintf(count_buf, "%10d\n", 0);
   /*FIXME: this can block*/
   if (write(postlog_file, count_buf, 11) != 11)
      return error(ip, "could not sock_write POST-log!\n");
   return 0;
}



static int do_reset (user_req_t *ip, char *query) {
   char maxload [20], pttime[20], maxthread[20],
   exp1[20], exp2[20], urlroot [100];
   char tmpstr1[256], tmpstr2[256];

   sleep(1);
   if (!PARSE_STRING("&maxload=", query, maxload))
      return error(ip,"invalid &maxload field!\n");
   if (!PARSE_STRING("&pttime=", query, pttime))
      return error(ip,"invalid &pttime field!\n");
   if (!PARSE_STRING("&maxthread=", query, maxthread))
      return error(ip,"invalid &maxthread field!\n");
   if (!PARSE_STRING("&exp=", query, exp1))
      return error(ip,"invalid &exp1 field!\n");
   if (!PARSE_STRING(",", query, exp2))
      return error(ip,"invalid &exp2 field!\n");
   if (!PARSE_STRING("&urlroot=", query, urlroot))
      return error(ip,"invalid &urlroot field!\n");

   strcpy(tmpstr1, TOPDIR);
   strcat(tmpstr1, "/upfgen99");
   strcpy(tmpstr2, TOPDIR);
   strcat(tmpstr2, "/cadgen99");

#define UPFGEN tmpstr1
#define CADGEN tmpstr2

   {
      int pid = 0;
      char *argv_upfgen[] = { UPFGEN, "-C", TOPDIR, "-n", maxload,
                              "-t", maxthread, NULL};
      char *argv_cadgen[] = { CADGEN, "-C", TOPDIR, "-e", pttime,
                              "-t", maxthread, exp1, exp2, NULL};
      char * envp[] = { "HOME=/", "TERM=linux",
                        "PATH=/sbin:/usr/sbin:/bin:/usr/bin", NULL };

      pid = fork();
      if (!pid)
         if (execve(UPFGEN, argv_upfgen, envp) < 0)
            return error(ip,"could not execute UPFGEN!\n");
      waitpid(pid, NULL, 0);

      pid = fork();
      if (!pid)
         if (execve(CADGEN, argv_cadgen, envp) < 0)
            return error(ip,"could not execute CADGEN!\n");
      waitpid(pid, NULL, 0);
   }
   /*
    * Clear post log
    */
   init_postlog_file(ip);


   /* mtime has a 1 second resolution, sleep 1 second so that
    * the check for modified User.Personality and Custom.Ads
    * files notices multiple resets correctly.
    */
   sleep(1);
   last_mtime = 0;
   reread_files(ip);
   
   ip->rep.nwritten = send_reply_head(ip, 0);
   ip->rep.nwritten += send_reply_tail(ip);

   return 0;
}

#define BLOCKLEN 8192

static int send_postlog (user_req_t *ip) {
   int file;
   int delay=0;
   int ret, len, bytes, bytes_sent, bytes_read;
   struct stat sbuf;
   char buf[BLOCKLEN], count_buf[10];

   file = open(POSTLOG, O_RDWR);
   if (file == -1)
      return error(ip, "no POST-log file!\n");

   if (fstat(file, &sbuf))
      return error(ip, "fstat failed!\n");
   len = sbuf.st_size;

   sprintf(count_buf, "%10d", len / 139);
   lseek(file, 0, SEEK_SET);
   /*FIXME: Blocking File I/O */
   if (write(file, count_buf, 10) != 10)
      return error(ip, "write postlog count failed!\n");
   lseek(file, 0, SEEK_SET);


   bytes = send_reply_head(ip, len);

   for (bytes_sent = 0, delay = 0; bytes_sent < len;
         bytes_sent+= bytes_read, delay += bytes_read) {

      /*FIXME: Blocking File I/O */
      bytes_read = read(file, buf, BLOCKLEN);
      if (bytes_read <= 0)
         return error(ip, "postlog read failed!\n");
      if (delay > 5000000) {
         delay = 0;
         sleep(1);
      }
      ret = write(ip->sd, buf, bytes_read);
      if (ret != bytes_read)
         return error(ip, "postlog write failed!\n");
   }

   bytes += bytes_sent;

   close(file);
   bytes += send_reply_tail(ip);
   ip->rep.nwritten = bytes;
   sleep(2);
   return 0;
}

static int do_command (user_req_t *ip, char *query) {
   if (TOKEN_EQUAL(query, COMMAND_RESET))
      return do_reset(ip, query + sizeof(COMMAND_RESET)-1);
   if (TOKEN_EQUAL(query, COMMAND_FETCH)) {
      int ret;
      ret = send_postlog(ip);
      return ret;
   }
   return error(ip,"got invalid command!\n");
}



static inline int custom_ad_rotate (user_req_t *ip, CAD_t *CADp) {
   int adid, weight, expired;
   int user_id, last_ad, err;
   time_t now;

   user_id = CADp->user_id;
   last_ad = CADp->last_ad;

   err = reread_files(ip);
   if (err)
      return err;

   /*
    * Any error in either reading or parsing of the files results
    * in a returned -1 adid.
    */
   adid = -1;
   expired = 1;
   weight = 0;

   adid = find_ad(user_id, last_ad, &weight);
   if (adid < 0)
      goto error;
   now = time(NULL);
   if ((unsigned int) now <= ad[adid].expires)
      expired = 0;

error:
   CADp->reply_cookies_len = sprintf(CADp->reply_cookies,
                                     "found_cookie=Ad_id=%d&Ad_weight=%d&Expired=%d",
                                     adid, weight, expired);

   sprintf(CADp->ad_filename, "dir%05d/class%d_%d",
           adid / 36, ((adid % 36) / 9), adid % 9);
   return 0;
}

/*
 * This function reads the object, scans the temporary buffer for
 * the SPECweb99 tag string, does CAD replacement and sends the
 * result out to the client.
 */
static int scan_send_file (user_req_t *ip) {
   char *tmpbuf=NULL, *target, *last_target;
   unsigned int size, scan_size, ret, len;
   int left;
   CAD_t *CADp = (CAD_t *) ip->priv;
   off_t offset = 0;
   int fd = -1;
   if (!ip->rep.objectname[0])
      BUG();
   if (!ip->rep.objectlen)
      BUG();

   /* NOTE: The fd and the objectlen is already populated by
    * the populate_object function
    */
   fd = ip->rep.fd;

   tmpbuf = (char *) malloc(ip->rep.objectlen);
   read(fd, tmpbuf,ip->rep.objectlen);
   if (!tmpbuf)
      BUG();

   size = ip->rep.objectlen;
   scan_size = size - CAD_TAG_LEN + 1;

   target = tmpbuf;
   last_target = tmpbuf;

   for (;;) {
      left = scan_size - (target - tmpbuf);
      if (left < 0) {
         fprintf(stderr,"HUH?? %p (left: %d)\n", target, left);
         BUG();
      }
      if (!left)
         break;
      target = (char *) memchr(target, CAD_TAG[0], left);
      if (!target)
         break; // no such character left

      if (memcmp(target, CAD_TAG, CAD_TAG_LEN)) {
         target++; // skip past the '<'
         continue;
      }
      target += CAD_TAG_BODY_POS;
      len = target-last_target;
repeat_sendfile:
      ret = sendfile(ip->sd, fd, &offset, len);
      if (ret != len) {
         fprintf(stderr,"sendfile #1 (sock %d) returned: %d, != %d.\n", ip->sd, ret, len);
         if (ret > 0) {
            len -= ret;
            goto repeat_sendfile;
         }
#ifdef DYN_SOC_POLL_HACK
         if (errno == EAGAIN)
            goto  repeat_sendfile;
#endif

         BUG();
      }
      len = CAD_TAG_BODY_LEN;
      ret = sock_write_more(ip->sd, CADp->ad_filename, len);
      if (ret != len) {
         BUG();
      }
      offset += len;
      last_target = target + CAD_TAG_BODY_LEN;
      target += CAD_TAG_BODY_LEN + CAD_TAG_TAIL_LEN;
   }

   len = size-(last_target-tmpbuf);
repeat_sendfile2:
   ret = sendfile(ip->sd, fd, &offset, len);
   if (ret != len) {
      fprintf(stderr,"sendfile #2 returned: %d, != %d.\n", ret, len);
      if (ret > 0) {
         len -= ret;
         goto repeat_sendfile2;
      }
#ifdef DYN_SOC_POLL_HACK
      if (errno == EAGAIN)
         goto  repeat_sendfile2;
#endif

      BUG();
   }
   free(tmpbuf);
   close(fd);
   return size;
}


#define REPLY_HEAD_HEAD \
	"HTTP/1.1 200 OK\r\n" \
	"Content-Type: text/html\r\n" \
	"Connection: Keep-Alive\r\n" \
	"Content-Length: %d\r\n\r\n"

#define REPLY_HEAD_HEAD_COOKIE \
	"HTTP/1.1 200 OK\r\n" \
	"Content-Type: text/html\r\n" \
	"Connection: Keep-Alive\r\n" \
	"Content-Length: %d\r\n" \
	"Set-Cookie: %s\r\n" \
	"\r\n"

#define REPLY_HEAD_TAIL \
	"<html>\n" \
	"<head><title>SPECweb99 Dynamic GET & POST Test</title></head>\n"\
	"<body>\n" \
	"<p>SERVER_SOFTWARE = %s\n" \
	"<p>REMOTE_ADDR = %d.%d.%d.%d\n" \
	"<p>SCRIPT_NAME = %s\n" \
	"<p>QUERY_STRING = %s\n" \
	"<pre>\n"



#define REPLY_TAIL \
	"\n</pre>\n" \
	"</body></html>\n"

static int inline send_reply_head (user_req_t *ip, size_t body_size) {
   int ret;
   char buf [1000];
   char *tmp, *head, *tail;
   CAD_t *CADp = (CAD_t *)ip->priv;
   unsigned int head_len, tail_len, total_len;


   tmp = tail = buf;
   /* FIXME:
   	  Replace IP(.) to get the remote client IP address 
   	  Replace modulename to get the name of script/module invoked
		  e.g.: http://server:port/module.usapi?getkey1=val1 etc.
		        parse "module.usapi" in userver
    */ 
#define  PARSED_MODULE_NAME "spec_dyn_req"

   tmp += sprintf(tmp, REPLY_HEAD_TAIL, server_ident,
                  /*IP(0), IP(1), IP(2), IP(3),*/
                  0,0,0,0,
                  PARSED_MODULE_NAME , ip->req.query_string);


   tail_len = tmp-buf;

   total_len = tail_len + sizeof(REPLY_TAIL)-1 + body_size;

   tmp = (char *)(((unsigned int)tmp + sizeof(int)-1)&~(sizeof(int)-1));
   head = tmp;
   if (CADp && CADp->reply_cookies_len)
      tmp += sprintf(tmp, REPLY_HEAD_HEAD_COOKIE,
                     total_len, CADp->reply_cookies);
   else
      tmp += sprintf(tmp, REPLY_HEAD_HEAD, total_len);

   head_len = tmp-head;

   ret = sock_write_more(ip->sd, head, head_len);

   if (ret != (int) head_len)
      goto sock_write1_err;

   ret = sock_write_more(ip->sd, tail, tail_len);

   if (ret != (int) tail_len)
      goto sock_write2_err;

   return tail_len;

sock_write1_err:
   fprintf(stderr,"error1 %d while trying to send reply head!\n", ret);
   return 0;

sock_write2_err:
   fprintf(stderr,"error2 %d while trying to send reply head!\n", ret);
   return 0;
}

static inline int send_reply_tail (user_req_t *ip) {
   int len = sizeof(REPLY_TAIL)-1, ret;

   ret = sock_write_push(ip->sd, REPLY_TAIL, len);
   if (ret != len) {
      printf("error %d while trying to send reply tail!\n", ret);
      return 0;
   }
   return len;
}

/*
 * Send a dynamicly generated buffer. (this is the typical
 * CAD case) Every reply is generated dynamically based on
 * the template and cookie values. The template is scanned
 * for every send.
 */
static int CAD_send (user_req_t *ip) {
   int bytes;
   CAD_t *CADp = (CAD_t *) ip->priv;

   bytes = send_reply_head(ip, ip->rep.objectlen);

   if ((ip->req.method != HTTP_GET) || !CADp ||
         !ip->req.query_string[0] || !is_class12(ip->req.query_string)) {
      int ret=0;
      ip->req.state = SPECWEB99_RESPONSE_DONE;
      ret = direct_send_file(ip);
      return ret;
   } else {
      bytes += scan_send_file(ip);
   }
   bytes += send_reply_tail(ip);
   ip->rep.nwritten += bytes;
   ip->req.state = SPECWEB99_SOCKET_CLOSED;
   return 0;
}

static int CAD_finish_send (user_req_t *ip) {
   ip->rep.nwritten += send_reply_tail(ip);
   ip->req.state = SPECWEB99_SOCKET_CLOSED;
   return 0;
}

static int populate_object(user_req_t *ip) {
   int rc =0;
   struct stat stat;
   rc = open(ip->rep.objectname, O_RDONLY);
   ip->rep.fd = rc;
   if (ip->rep.fd < 0) {
      fprintf(stderr,"Failed to open file %s in populate_object\n",ip->rep.objectname);
      return -1;
   }
   rc = fstat (ip->rep.fd, &stat);

   if (rc < 0) {
      fprintf(stderr,"Failed to stat file %s in populate_object\n",ip->rep.objectname);
      close(ip->rep.fd);
      return  -1;
   }

   /* Check that the request is for a file and not for a directory */
   if (S_ISDIR(stat.st_mode)) {
      fprintf(stderr,"File requested %s in populate_object is a directory\n",ip->rep.objectname);
      return -1;
   }
   ip->rep.objectlen = stat.st_size;
   return 1;
}

static int direct_send_file(user_req_t *ip) {
   int ret = 0;
   off_t offset = 0;
   size_t len = ip->rep.objectlen;
repeat_sendfile:
   ret = (int) sendfile(ip->sd, ip->rep.fd, &offset, len);
   if (ret != (int) len) {
      /* hack: since the sockets are in non-blocking mode, if we would block.
       * just keep retrying. FIXME: Change this, infact try to use userver do_sendfile
       * function
       */
      printf("sendfile (sock %d) returned: %d, != %d.\n", ip->sd, ret, len);
      if (ret > 0) {
         len -= ret;
         goto repeat_sendfile;
      }
#ifdef DYN_SOC_POLL_HACK
      /* see hack defined above */
      if(errno == EAGAIN)
         goto repeat_sendfile;
#endif

      perror("direct_sendfile_failed");
      BUG();
   }
   return close (ip->rep.fd);
}

#endif /* MOD_SPECWEB99 */
