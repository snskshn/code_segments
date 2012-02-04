/* Copyright (C) 1991,92,94,95,96,97,98,99 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/*
 * Never include this file directly; use <sys/types.h> instead.
 */

#ifndef	_BITS_TYPES_H
#define	_BITS_TYPES_H	1

#include <features.h>

#define __need_size_t
#include <stddef.h>

/* Convenience types.  */
typedef unsigned char __u_char;
typedef unsigned short __u_short;
typedef unsigned int __u_int;
typedef unsigned long __u_long;
#ifdef __GNUC__
__extension__ typedef unsigned long long int __u_quad_t;
__extension__ typedef long long int __quad_t;
#else
typedef struct
  {
    long int __val[2];
  } __quad_t;
typedef struct
  {
    __u_long __val[2];
  } __u_quad_t;
#endif
typedef signed char __int8_t;
typedef unsigned char __uint8_t;
typedef signed short int __int16_t;
typedef unsigned short int __uint16_t;
typedef signed int __int32_t;
typedef unsigned int __uint32_t;
#ifdef __GNUC__
__extension__ typedef signed long long int __int64_t;
__extension__ typedef unsigned long long int __uint64_t;
#endif
typedef __quad_t *__qaddr_t;

typedef __u_quad_t __dev_t;		/* Type of device numbers.  */
typedef __u_int __uid_t;		/* Type of user identifications.  */
typedef __u_int __gid_t;		/* Type of group identifications.  */
typedef __u_long __ino_t;		/* Type of file serial numbers.  */
typedef __u_int __mode_t;		/* Type of file attribute bitmasks.  */
typedef __u_int __nlink_t; 		/* Type of file link counts.  */
typedef long int __off_t;		/* Type of file sizes and offsets.  */
typedef __quad_t __loff_t;		/* Type of file sizes and offsets.  */
typedef int __pid_t;			/* Type of process identifications.  */
typedef int __ssize_t;			/* Type of a byte count, or error.  */
typedef long int __rlim_t;		/* Type of resource counts.  */
typedef __quad_t __rlim64_t;		/* Type of resource counts (LFS).  */
typedef __u_int __id_t;			/* General type for ID.  */

typedef struct
  {
    int __val[2];
  } __fsid_t;				/* Type of file system IDs.  */

/* Everythin' else.  */
typedef int __daddr_t;			/* The type of a disk address.  */
typedef char *__caddr_t;
typedef long int __time_t;
typedef long int __swblk_t;		/* Type of a swap block maybe?  */

typedef long int __clock_t;

/* One element in the file descriptor mask array.  */
typedef unsigned long int __fd_mask;

/* Number of descriptors that can fit in an `fd_set'.  */
#ifdef BIGGER_FD_SETSIZE
#define __FD_SETSIZE	64000
#else
#define __FD_SETSIZE	1024
#endif

/* It's easier to assume 8-bit bytes than to get CHAR_BIT.  */
#define __NFDBITS	(8 * sizeof (__fd_mask))
#define	__FDELT(d)	((d) / __NFDBITS)
#define	__FDMASK(d)	((__fd_mask) 1 << ((d) % __NFDBITS))

/* fd_set for select and pselect.  */
typedef struct
  {
    /* XPG4.2 requires this member name.  Otherwise avoid the name
       from the global namespace.  */
#ifdef __USE_XOPEN
    __fd_mask fds_bits[__FD_SETSIZE / __NFDBITS];
# define __FDS_BITS(set) ((set)->fds_bits)
#else
    __fd_mask __fds_bits[__FD_SETSIZE / __NFDBITS];
# define __FDS_BITS(set) ((set)->__fds_bits)
#endif
  } __fd_set;


typedef int __key_t;

/* Used in `struct shmid_ds'.  */
typedef unsigned short int __ipc_pid_t;


/* Types from the Large File Support interface.  */

/* Type to count number os disk blocks.  */
typedef long int __blkcnt_t;
typedef __quad_t __blkcnt64_t;

/* Type to count file system blocks.  */
typedef __u_long __fsblkcnt_t;
typedef __u_quad_t __fsblkcnt64_t;

/* Type to count file system inodes.  */
typedef __u_long __fsfilcnt_t;
typedef __u_quad_t __fsfilcnt64_t;

/* Type of file serial numbers.  */
typedef __u_long __ino64_t;

/* Type of file sizes and offsets.  */
typedef __loff_t __off64_t;

/* Used in XTI.  */
typedef int __t_scalar_t;
typedef unsigned int __t_uscalar_t;

/* Duplicates info from stdint.h but this is used in unistd.h.  */
typedef int __intptr_t;


/* Now add the thread types.  */
#ifdef __USE_UNIX98
# include <bits/pthreadtypes.h>
#endif

#endif /* bits/types.h */
