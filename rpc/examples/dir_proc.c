/*
 * dir_proc.c: remote readdir implementation
 */
#include <rpc/rpc.h>            /* Always needed */
#include <sys/dir.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "dir.h"                /* Created by rpcgen */
 
readdir_res *readdir_1_svc(nametype *dirname, struct svc_req *rqstp)
{
        DIR *dirp;
        struct direct *d;
        namelist nl;
        namelist *nlp;
        static readdir_res res; /* must be static! */
 
        /*
         * Open directory
         */
        dirp = opendir(*dirname);
        if (dirp == NULL) {
                res.error = errno;
                return (&res);
        }
 
        /*
         * Free previous result
         */
        xdr_free((xdrproc_t)xdr_readdir_res, (char *)&res);
 
        /*
         * Collect directory entries.
         * Memory allocated here will be freed by xdr_free
         * next time readdir_1 is called
         */
        nlp = &res.readdir_res_u.list;
        while (d = readdir(dirp)) {
                nl = *nlp = (namenode *) malloc(sizeof(namenode));
                nl->name = strdup(d->d_name);
                nlp = &nl->next;
        }
        *nlp = NULL;
 
        /*
         * Return the result
         */
        res.error = 0;
        closedir(dirp);
        return (&res);
}


