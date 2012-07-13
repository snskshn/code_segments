/*
 * dir.x: Remote directory listing protocol
 */
 
/* maximum length of a directory entry */
const MAXNAMELEN = 255;
 
/* a directory entry */
typedef string nametype<MAXNAMELEN>;
 
/* a link in the listing */
typedef struct namenode *namelist;
 
/*
 * A node in the directory listing
 */
struct namenode {
        nametype name;          /* name of directory entry */
        namelist next;          /* next entry */
};
 
/*
 * The result of a READDIR operation.
 */
union readdir_res switch (int error) {
 
case 0:
        namelist list;  /* no error: return directory listing */
 
default:
        void;           /* error occurred: nothing else to return */
};
 
/*
 * The directory program definition
 */
program DIRPROG {
        version DIRVERS {
                readdir_res
		READDIR(nametype) = 1;
        } = 1;
} = 0x20000076;


