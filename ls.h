#pragma once

// #define DEBUG

// #ifndef COLORLS
// #define COLORLS
// #endif

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <dirent.h>
#include <err.h>
#include <errno.h>
#include <grp.h>
#include <limits.h>
#include <locale.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#ifdef COLORLS
#include <termcap.h>
#include <signal.h>
#endif

#include <sys/cdefs.h>
#include <sys/param.h>
#include <err.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

#include <ctype.h>
#include <termcap.h>
#include <signal.h>
#include "../libmx/inc/libmx.h"
#pragma once
// #define MY_IOC_OUT (__uint32_t)0x40000000
// #define MY_IOC(i, g, n, l) (i | ((l & 0x1fff) << 16) | ((g) << 8) | (n))
// #define MY_IOR(g, n, t) MY_IOC(MY_IOC_OUT,	(g), (n), sizeof(t))
// #define TIOCGWINSZ MY_IOR('t', 104, struct winsize)  /* get window size */
// #define MY_TIOCGSIZE MY_TIOCGWINSZ

#define MX_AT(x, o, y) ((x)->st_atimespec.tv_nsec o (y)->st_atimespec.tv_nsec)
#define MX_CT(x, o, y) ((x)->st_ctimespec.tv_nsec o (y)->st_ctimespec.tv_nsec)
#define MX_MT(x, o, y) ((x)->st_mtimespec.tv_nsec o (y)->st_mtimespec.tv_nsec)


#define	FTS_COMFOLLOW	0x001		/* follow command line symlinks */
#define	FTS_LOGICAL	    0x002		/* logical walk */
#define	FTS_NORECUR	    0x004		/* non-recursive */
#define	FTS_NOSTAT	    0x008		/* don't get stat info */
#define	FTS_PHYSICAL	0x010		/* physical walk */
#define	FTS_SEEDOT	    0x020		/* return dot and dot-dot */
// #define	FTS_XDEV	    0x040		/* don't cross devices */
// #define	FTS_WHITEOUT	0x080		/* (no longer supported) return whiteout information */
// #define	FTS_COMFOLLOWDIR 0x400		/* (non-std) follow command line symlinks for directories only */
#define	FTS_NAMEONLY	0x100		/* (private) child names only */
// #define	FTS_STOP	    0x200		/* (private) unrecoverable error */

#define	FTS_ROOTPARENTLEVEL	-1
#define	FTS_ROOTLEVEL		 0
// #define	FTS_MAXLEVEL		 0x7fffffff

#define	FTS_D		 1		/* preorder directory */
#define	FTS_DC		 2		/* directory that causes cycles */
#define	FTS_DEFAULT	 3		/* none of the above */
#define	FTS_DNR		 4		/* unreadable directory */
#define	FTS_DOT		 5		/* dot or dot-dot */
#define	FTS_DP		 6		/* postorder directory */
#define	FTS_ERR		 7		/* error; errno is set */
#define	FTS_F		 8		/* regular file */
#define	FTS_INIT	 9		/* initialized only */
#define	FTS_NS		10		/* stat(2) failed */
#define	FTS_NSOK	11		/* no stat(2) requested */
#define	FTS_SL		12		/* symbolic link */
#define	FTS_SLNONE	13		/* symbolic link without target */
#define	FTS_W		14		/* whiteout object */

// #define	FTS_DONTCHDIR	 0x01		/* don't chdir .. to the parent */
// #define	FTS_SYMFOLLOW	 0x02		/* followed a symlink to get here */
// #define	FTS_ISW 		 0x04		/* this is a whiteout object */

// #define	FTS_AGAIN	 1		/* read node again */
// #define	FTS_FOLLOW	 2		/* follow symbolic link */
// #define	FTS_NOINSTR	 3		/* no instructions */
#define	FTS_SKIP	 4		/* discard node */



typedef struct s_ftsent {
    u_short fts_info;               /* flags for FTSENT structure */
    char fts_accpath[PATH_MAX + 1]; /* access path */
    u_short fts_accpathlen;         /* strlen(fts_accpath) */
    char fts_path[PATH_MAX + 1];    /* root path */
    u_short fts_pathlen;            /* strlen(fts_path) */
    char fts_name[NAME_MAX + 1];    /* file name */
    u_short fts_namelen;            /* strlen(fts_name) */
    short fts_level;                /* depth (-1 to N) */
    int fts_errno;                  /* file errno */
    long fts_number;                /* local numeric value */
    void *fts_pointer;              /* local address value */
    struct s_ftsent *fts_parent;      /* parent directory */
    struct s_ftsent *fts_link;        /* next file structure */
    struct s_ftsent *fts_cycle;       /* cycle structure */
    struct stat *fts_statp;         /* stat(2) information */
	unsigned short fts_instr;	/* fts_set() instructions */
    __uint8_t  d_type;               /* file type, see below */    
} FTSENT;

typedef struct st_dirlist {
    struct dirent *data;
    struct st_dirlist *next;
}              t_dirlist;

typedef struct st_ftslist {
    FTSENT *data;
    struct st_ftslist *next;
}              t_ftslist;

typedef struct s_fts {
    t_ftslist *head;
    t_ftslist *cur;
    bool read_launched;
    bool (*cmp)(FTSENT *, FTSENT *);
    int fts_options;
    char **argv;
} FTS;


// #ifndef u_int
// #define u_int unsigned int
// #endif

// #ifndef u_long
// #define u_long unsigned long int
// #endif

// typedef __u_quad_t u_quad_t;

#define	IS_NOPRINT(p)	((p)->fts_number == NO_PRINT)

#define KILO_SZ(n) (n)
#define MEGA_SZ(n) ((n) * (n))
#define GIGA_SZ(n) ((n) * (n) * (n))
#define TERA_SZ(n) ((n) * (n) * (n) * (n))
#define PETA_SZ(n) ((n) * (n) * (n) * (n) * (n))

#define KILO_2_SZ (KILO_SZ(1024ULL))
#define MEGA_2_SZ (MEGA_SZ(1024ULL))
#define GIGA_2_SZ (GIGA_SZ(1024ULL))
#define TERA_2_SZ (TERA_SZ(1024ULL))
#define PETA_2_SZ (PETA_SZ(1024ULL))

/*
 * Upward approximation of the maximum number of characters needed to
 * represent a value of integral type t as a string, excluding the
 * NUL terminator, with provision for a sign.
 */
#define	STRBUF_SIZEOF(t)	(1 + CHAR_BIT * sizeof(t) / 3 + 1)

#define NO_PRINT	1

#define	SIXMONTHS	((365 / 2) * 86400)

#define	SAPPEND(s) {							\
	if (prefix != NULL)						\
		(void)strcat(string, prefix);				\
	(void)strcat(string, s);					\
	prefix = ",";							\
}

#define	TEST(a, b, f) {							\
	if (!memcmp(a, b, sizeof(b))) {					\
		if (clear) {						\
			if (clrp)					\
				*clrp |= (f);				\
		} else if (setp)					\
			*setp |= (f);					\
		break;							\
	}								\
}

#if defined(_POSIX_SOURCE) || defined(_POSIX_C_SOURCE) || \
    defined(_XOPEN_SOURCE) || defined(__NetBSD__)
#define ATIMENSEC_CMP(x, op, y) ((x)->st_atimensec op (y)->st_atimensec)
#define CTIMENSEC_CMP(x, op, y) ((x)->st_ctimensec op (y)->st_ctimensec)
#define MTIMENSEC_CMP(x, op, y) ((x)->st_mtimensec op (y)->st_mtimensec)
#else
#define ATIMENSEC_CMP(x, op, y) \
	((x)->st_atimespec.tv_nsec op (y)->st_atimespec.tv_nsec)
#define CTIMENSEC_CMP(x, op, y) \
	((x)->st_ctimespec.tv_nsec op (y)->st_ctimespec.tv_nsec)
#define MTIMENSEC_CMP(x, op, y) \
	((x)->st_mtimespec.tv_nsec op (y)->st_mtimespec.tv_nsec)
#endif

long blocksize;		/* block size units */
typedef struct s_flags {
	int accesstime;	/* use time of last access */
	int flags;		/* show flags associated with a file */
	int humanval;		/* show human-readable file sizes */
	int inode;		/* print inode */
	int longform;		/* long listing format */
	int octal;		/* print unprintables in octal */
	int octal_escape;	/* like octal but use C escapes if possible */
	int nonprint;		/* show unprintables as ? */
	int sectime;		/* print the real time for all files */
	int size;		/* list size in short listing */
	int slash;		/* append a '/' if the file is a directory */
	int sortacross;	/* sort across rows, not down columns */ 
	int statustime;	/* use time of last mode change */
	int notabs;		/* don't use tab-separated multi-col output */
	int type;		/* add type character for non-regular files */
	int color;		/* add type in color for non-regular files */
	int kblocks;		/* print size in kilobytes */
	int listdir;		/* list actual directory, not contents */
	int listdot;		/* list files beginning with . */
	int nosort;		/* don't sort output */
	int numericonly;	/* don't convert uid/gid to name */
	int recursive;		/* ls subdirectories also */
	int reversesort;	/* reverse whatever sort is used */
	int singlecol;		/* use single column output */
	int timesort;		/* sort by time vice name */
	int sizesort;		/* sort by size */
	int whiteout;		/* show whiteout entries */
	int lomac;		/* show LOMAC attributes */
	int stream;		/* stream the output, separate with commas */
} t_flags;


typedef struct {
	FTSENT *list;
	u_int64_t btotal;
	int bcfile;
	int entries;
	int maxlen;
	u_int s_block;
	u_int s_flags;
	u_int s_lattr;
	u_int s_group;
	u_int s_inode;
	u_int s_nlink;
	u_int s_size;
	u_int s_user;
} DISPLAY;

typedef struct {
	char *user;
	char *group;
	char *flags;
#ifndef __APPLE__
	char *lattr;
#endif /* __APPLE__ */
	char data[1];
} NAMES;
// int	 acccmp(const FTSENT *, const FTSENT *);
// int	 revacccmp(const FTSENT *, const FTSENT *);
// int	 modcmp(const FTSENT *, const FTSENT *);
// int	 revmodcmp(const FTSENT *, const FTSENT *);
// int	 namecmp(const FTSENT *, const FTSENT *);
// int	 revnamecmp(const FTSENT *, const FTSENT *);
// int	 statcmp(const FTSENT *, const FTSENT *);
// int	 revstatcmp(const FTSENT *, const FTSENT *);
// int	 sizecmp (const FTSENT *, const FTSENT *);
// int	 revsizecmp (const FTSENT *, const FTSENT *);

void	 printcol(DISPLAY *);
void	 printlong(DISPLAY *);
void	 printscol(DISPLAY *);
void	 printstream(DISPLAY *);
void	 usage(void);
size_t	 len_octal(const char *, int);
int	 prn_octal(const char *);
int	 prn_printable(const char *);
#ifdef COLORLS
void	 parsecolors(const char *cs);
void     colorquit(int);

extern  char    *ansi_fgcol;
extern  char    *ansi_bgcol;
extern  char    *ansi_coloff;
extern  char    *attrs_off;
extern  char    *enter_bold;
#endif

int (*statp)(const char *restrict , struct stat *restrict);
 void (*printfcn)(DISPLAY *);
 bool (*sortfcn)( FTSENT *,  FTSENT *);

t_flags f;
