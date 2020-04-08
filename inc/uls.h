#pragma once

#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <dirent.h>
#include <errno.h>
#include <grp.h>
#include <limits.h>
#include <pwd.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/cdefs.h>
#include <sys/param.h>
#include <stddef.h>
#include <time.h>
#include <ctype.h>
#include <signal.h>
#include <sys/xattr.h>
#include <sys/acl.h>

#include "../libmx/inc/libmx.h"

#define MX_FL_USAGE "@ABCFGHLPRSTUabcdfhiklmnpqrstuwx1"

#define MX_IOC_OUT (__uint32_t)0x40000000
#define MX_IOC(i, g, n, l) (i | ((l & 0x1fff) << 16) | ((g) << 8) | (n))
#define MX_IOR(g, n, t) MX_IOC(MX_IOC_OUT,    (g), (n), sizeof(t))
#define MX_TIOCGWINSZ MX_IOR('t', 104, struct winsize)

#define MX_AT(x, o, y) ((x)->st_atimespec.tv_nsec o (y)->st_atimespec.tv_nsec)
#define MX_CT(x, o, y) ((x)->st_ctimespec.tv_nsec o (y)->st_ctimespec.tv_nsec)
#define MX_MT(x, o, y) ((x)->st_mtimespec.tv_nsec o (y)->st_mtimespec.tv_nsec)
#define MX_BT(x, o, y) ((x)->st_btimespec.tv_nsec o (y)->st_btimespec.tv_nsec)

#define MX_S_ISBLK(m)      (((m) & S_IFMT) == S_IFBLK)
#define MX_S_ISCHR(m)      (((m) & S_IFMT) == S_IFCHR)
#define MX_S_ISDIR(m)      (((m) & S_IFMT) == S_IFDIR)
#define MX_S_ISFIFO(m)     (((m) & S_IFMT) == S_IFIFO)
#define MX_S_ISREG(m)      (((m) & S_IFMT) == S_IFREG)
#define MX_S_ISLNK(m)      (((m) & S_IFMT) == S_IFLNK)
#define MX_S_ISSOCK(m)     (((m) & S_IFMT) == S_IFSOCK
#define MX_S_ISWHT(m)      (((m) & S_IFMT) == S_IFWHT)

#define MX_MAJOR(x) ((int32_t)(((u_int32_t)(x) >> 24) & 0xff))
#define MX_MINOR(x) ((int32_t)((x) & 0xffffff))

#define MX_COMFOLLOW    0x001        /* follow command line symlinks */
#define MX_LOGICAL      0x002        /* logical walk */
#define MX_NORECUR      0x004        /* non-recursive */
#define MX_NOSTAT       0x008        /* don't get stat info */
#define MX_PHYSICAL     0x010        /* physical walk */
#define MX_SEEDOT       0x020        /* return dot and dot-dot */
#define MX_NAMEONLY     0x100        /* (private) child names only */
#define MX_ENOENT       0x200        /* to signal rval */

#define MX_ROOTLEV      0

#define MX_D            1        /* preorder directory */
#define MX_DC           2        /* directory that causes cycles */
#define MX_DEFAULT      3        /* none of the above */
#define MX_LNF          4        /* link that points to non-dir */
#define MX_ERR          7        /* error; errno is set */
#define MX_SLNONE      13        /* symbolic link without target */

#define FTS_ROOTLEVEL         0

#define MX_MAXUSRNAME         32
#define MX_MAXGRPNAME         32

#define MX_IS_NOPRINT(p)    ((p)->number == MX_NO_PRINT)

#define MX_ROUND(n) n % 10 >= 5 ? (((n / 10) % 10) + 1) : ((n / 10) % 10)

#define MX_ABS(x) ((x) < 0 ? -(x) : (x))

#define MX_HOWMANY(x, y) ((((x) % (y)) == 0) ? ((x) / (y)) : (((x) / (y)) + 1))

#define MX_ISDIR(m)     (((m) & S_IFMT) == S_IFDIR)
#define MX_ISBLK(m)     (((m) & S_IFMT) == S_IFBLK)
#define MX_ISCHR(m)     (((m) & S_IFMT) == S_IFCHR)
#define MX_ISLNK(m)     (((m) & S_IFMT) == S_IFLNK)
#define MX_ISREG(m)     (((m) & S_IFMT) == S_IFREG)
#define MX_ISFIFO(m)    (((m) & S_IFMT) == S_IFIFO)
#define MX_ISSOCK(m)    (((m) & S_IFMT) == S_IFSOCK)
#define MX_ISWHT(m)     (((m) & S_IFMT) == S_IFWHT)

/* time formats */
#define MX_TDAYFIRST 00001
#define MX_TSEC 00002
#define MX_TNOSEC 00004

/*
 * Upward approximation of the maximum number of characters needed to
 * represent a value of integral type t as a string, excluding the
 * NUL terminator, with provision for a sign.
 */
#define MX_STRBUF_SIZEOF(t) (1 + CHAR_BIT * sizeof(t) / 3 + 1)

#define MX_PR_AL(b, n, w, r) mx_print_align(mx_printbuf(b, mx_llitoa(n)), w, r)

#define MX_NO_PRINT 1

#define MX_SIXMONTHS ((365 / 2) * 86400)

typedef enum {
    NONE, KILO, MEGA, GIGA, TERA, PETA, UNIT_MAX
} unit_t;

typedef struct s_ftsent {
    u_short info;                   /* flags for t_file structure */
    char accpath[PATH_MAX + 1];     /* access path */
    u_short accpathlen;             /* strlen(fts_accpath) */
    char par_path[PATH_MAX + 1];    /* root path */
    u_short par_pathlen;            /* strlen(fts_path) */
    char name[NAME_MAX + 1];        /* file name */
    u_short namelen;                /* strlen(fts_name) */
    short level;                    /* depth (-1 to N) */
    int f_errno;                    /* file errno */
    long number;                    /* local numeric value */
    void *pointer;                  /* local address value */
    struct s_ftsent *parent;        /* parent directory */
    struct s_ftsent *link;          /* next file structure */
    struct s_ftsent *cycledir;      /* cycle structure */
    struct stat *statp;             /* stat(2) information */
    unsigned short fts_instr;       /* fts_set() instructions */
    __uint8_t  d_type;              /* file type, see below */
}                t_file;

typedef struct st_dirlist {
    struct dirent *data;
    struct st_dirlist *next;
}              t_dirlist;

typedef struct st_ftslist {
    t_file *data;
    struct st_ftslist *next;
}              t_ftslist;

typedef struct s_fts {
    t_ftslist *head;
    t_ftslist *cur;
    bool read_launched;
    bool (*cmp)(void *, void *);
    int fts_options;
    char **argv;
    bool enoent; // true if there is an ENOENT entry
}                t_fts;

/* ctime string time elements indexs */
enum e_timeind {
    MON = 4,
    DAY = 8,
    HOUR = 11,
    MIN = 14,
    SEC = 17,
    YEAR = 20
};

#define MX_DEF_COLORS "ExFxCxDxBxegedabagacad"

/* Most of these are taken from <sys/stat.h> */
enum e_colors {
    C_DIR,              /* directory */
    C_LNK,              /* symbolic link */
    C_SOCK,             /* socket */
    C_FIFO,             /* pipe */
    C_EXEC,             /* executable */
    C_BLK,              /* block special */
    C_CHR,              /* character special */
    C_SUID,             /* setuid executable */
    C_SGID,             /* setgid executable */
    C_WSDIR,            /* directory writeble to others, with sticky bit */
    C_WDIR,             /* directory writeble to others, without st. bit */
    C_NUMCOLORS         /* just a place-holder */
};

typedef struct {
    int num[2];
    bool bold;
} t_colors;

#define MX_C_FBLACK   "\x1b[30m"
#define MX_C_FRED     "\x1b[31m"
#define MX_C_FGREEN   "\x1b[32m"
#define MX_C_FYELLOW  "\x1b[33m"
#define MX_C_FBLUE    "\x1b[34m"
#define MX_C_FMAGENTA "\x1b[35m"
#define MX_C_FCYAN    "\x1b[36m"
#define MX_C_FWHYTE   "\x1b[37m"

#define MX_C_BBLACK   "\x1b[40m"
#define MX_C_BRED     "\x1b[41m"
#define MX_C_BGREEN   "\x1b[42m"
#define MX_C_BYELLOW  "\x1b[43m"
#define MX_C_BBLUE    "\x1b[44m"
#define MX_C_BMAGENTA "\x1b[45m"
#define MX_C_BCYAN    "\x1b[46m"
#define MX_C_BWHYTE   "\x1b[47m"

#define MX_C_BOLD     "\x1b[1m"
#define MX_C_RESET    "\x1b[0m"

typedef struct {
    bool accesstime;       /* use time of last access */
    bool statustime;       /* use time of last mode change */
    bool birthtime;        /* use time of birth */
    bool humanval;         /* show human-readable file sizes */
    bool inode;            /* print inode */
    bool longform;         /* long listing format */
    bool octal;            /* print unprintables in octal */
    bool octal_escape;     /* like octal but use C escapes if possible */
    bool nonprint;         /* show unprintables as ? */
    bool sectime;          /* print the real time for all files */
    bool size;             /* list size in short listing */
    bool slash;            /* append a '/' if the file is a directory */
    bool sortacross;       /* sort across rows, not down columns */
    bool notabs;           /* don't use tab-separated multi-col output */
    bool type;             /* add type character for non-regular files */
    bool color;            /* add type in color for non-regular files */
    bool kblocks;          /* print size in kilobytes */
    bool listdir;          /* list actual directory, not contents */
    bool listdot;          /* list files beginning with . */
    bool nosort;           /* don't sort output */
    bool numericonly;      /* don't convert uid/gid to name */
    bool recursive;        /* ls subdirectories also */
    bool reversesort;      /* reverse whatever sort is used */
    bool singlecol;        /* use single column output */
    bool timesort;         /* sort by time vice name */
    bool sizesort;         /* sort by size */
    bool whiteout;         /* show whiteout entries */
    bool lomac;            /* show LOMAC attributes */
    bool stream;           /* stream the output, separate with commas */
    bool comfollow;        /* follow symbolic links on the com. line */
    bool lfollow;          /* follow symbolic links */
    bool acl;
    bool xattr;
    int options;           /* fts options */
    t_colors colors[C_NUMCOLORS];
    char *fg[8];           /* foreground colors */
    char *bg[8];           /* background colors */
    long blocksize;        /* block size units */
    int termwidth;         /* terminal width */
    int rval;              /* return value */
    int output;            /* if anything output */
    t_fts *ftsp;           /* files hierarchy pointer */
    char *dotav[2];        /* argv for "." */
} t_flags;

struct s_opt {
    char c;
    int optind;
};

struct s_display_max {
    off_t size;
    u_int64_t block;
    u_long inode;
    u_long len;
    u_long nlink;
    gid_t group;
    uid_t user;
};

struct s_display {
    t_file *list;
    u_int64_t btotal;
    int bcfile;
    int entries;
    int maxlen;
    u_int s_block;
    u_int s_group;
    u_int s_xattr;
    u_int s_inode;
    u_int s_nlink;
    u_int s_size;
    u_int s_user;
    char *xattr_text;    /* -@ */
};

struct s_disp_buf {
    struct stat *sp;
    t_file *cur;
    u_int64_t btotal;
    int bcfile;
    size_t flen;
    size_t ulen;
    size_t glen;
    int entries;
    int needstats;
    char buf[MX_STRBUF_SIZEOF(u_quad_t) + 1];
    char ngroup[MX_MAXGRPNAME + 1];
    char nuser[MX_MAXUSRNAME + 1];
    t_file *parent;
    t_file *ch_list;
};

struct s_names {
    ssize_t xattr_size;
    char *user;
    char *group;
    char data[1];
};

typedef struct s_col_param {
    int base;
    int chcnt;
    int cnt;
    int col;
    int colwidth;
    int endcol;
    int num;
    int numcols;
    int numrows;
    int row;
    int tabwidth;
    int *lastentries;
    char *buf;
}                t_col_param;

/* FTS */
void mx_paste_node(t_list **list, void *data);
bool mx_is_dot(char *name);
bool mx_namecmp(void *a, void *b);
bool mx_revnamecmp(void *a, void *b);
bool mx_modcmp(void *a, void *b);
bool mx_revmodcmp(void *a, void *b);
bool mx_acccmp(void *a, void *b);
bool mx_revacccmp(void *a, void *b);
bool mx_statcmp(void *a, void *b);
bool mx_birthcmp(void *a, void *b);
bool mx_revbirthcmp(void *a, void *b);
bool mx_revstatcmp(void *a, void *b);
bool mx_sizecmp(void *a, void *b);
bool mx_revsizecmp(void *a, void *b);
int mx_fts_chlist_size(t_file *list);
t_file *mx_sort_fts_chlist(t_file *lst, bool (*cmp)(void *, void *));
short mx_type_stat(char *path);
int mx_link_info(char *name);
bool mx_is_cycle_dir(t_file *p);
bool mx_islink_stat(char *path);
bool mx_islink_dirent(struct dirent *file);
bool mx_file_exist(char *name);
t_file *mx_enoent(char *name);
short mx_fts_root_info(char *name, int options);
short mx_fts_dirent_info(struct dirent *ptr, char *path, int options);
t_file *mx_root_file(char *name, int options);
t_file *mx_dirent_to_ftsent(t_file *p, struct dirent *stdp, int options);
void mx_follow_link(char *name, char *buf, int size);
void mx_fts_child_push(t_file **last_child, t_file *child,
                       t_file **ch_list_start);
t_file *mx_fts_children_1(t_ftslist *list, int options,
                          bool (*cmp)(void *, void *));
void mx_children_clear(t_file *p);
short mx_fts_paste_chlist(t_ftslist *parent, int options,
                          bool (*cmp)(void *, void *));
void mx_push_ftsent(t_ftslist **list, t_file *ptr);
short mx_fts_push_name(t_ftslist **list, char *name, int options,
                       t_list **errlist);
bool mx_str_cmp(void *a, void *b);
bool mx_print_errlist_names(t_list *errlist, bool sort);
t_ftslist *mx_push_roots(t_ftslist **list, char **argv, int *options,
                         bool (*cmp)(void *, void *));
t_file *mx_argv_chlist(char **argv, int options,
                       bool (*cmp)(void *, void *));
t_fts *mx_fts_open(char **path_argv, int options,
                   bool (*cmp)(void *, void *));
t_file *mx_fts_read(t_fts *ftsp);
t_file *mx_fts_children(t_fts *ftsp, int options);
void mx_fts_skip_ch(t_fts *ftsp);

/* UTILS */
void *mx_printbuf(void *buf, char *s);
void mx_print_spaces(int count);
int mx_print_align(char *s, int len, bool right);
int mx_print_align_c(char *s, char c, int len, bool right);
void mx_print_mode(t_flags *f, t_file *file);
void mx_printerr_errnum(char *s, int errnum);
short mx_round(int n);

/* FOR MAIN */
char mx_getopt(int argc, char **argv, char *optstring, int *optind);
char *mx_usrname(uid_t st_uid, char *user, t_flags *f);
char *mx_grpname(gid_t st_gid, char *group, t_flags *f);
void mx_parse_flag(char c, t_flags *f, int *fts_options);
void mx_bBcCflqtTuUvwx1(char c, int *fts_options, t_flags *f);
void mx_display_if_need_stat(t_flags *f, struct s_names **np,
                             struct s_display_max *max, struct s_disp_buf *db);
void mx_display_process_name_and_stat(t_flags *f, struct s_names **np,
    struct s_display_max *max, struct s_disp_buf *db);
void mx_display(t_file *p, t_file *list, t_flags *f);
t_fts *mx_traverse(int argc, char *argv[], int options, t_flags *f);

/* PRINT */
ssize_t mx_xattr_size(t_flags *f, t_file *file);
void mx_print_xattr(t_file *file, t_flags *f);
void mx_printcolor(enum e_colors c, t_flags *f);
int mx_colortype(mode_t mode, t_flags *f);
int mx_prfname(char *name, t_flags *f);
int mx_printname(t_file *p, u_long inodefield, u_long sizefield, t_flags *f);
bool mx_notexec_type(u_int mode);
int mx_printtype(u_int mode, t_flags *f);
void mx_printtime(time_t ftime, t_flags *f);
void mx_printlink(t_file *p, t_flags *f);
void mx_printsize(size_t width, off_t bytes, t_flags *f);
void mx_printscol(struct s_display *, t_flags *);
void mx_printlong(struct s_display *, t_flags *);
void mx_printcol(struct s_display *, t_flags *);
void mx_printstream(struct s_display *, t_flags *);
void mx_usage(void);
size_t mx_len_octal(const char *, int);
int mx_prn_octal(const char *, t_flags *);
int mx_prn_printable(const char *);
void mx_parsecolors(const char *cs, t_flags *f);
