#include "ls.h"

static int	printaname(FTSENT *, u_long, u_long);
static void	printlink(FTSENT *);
static void	printtime(time_t);
static int	printtype(u_int);
static void	printsize(size_t, off_t);
#ifdef COLORLS
static void	endcolor(int);
static int	colortype(mode_t);
#endif


static u_int64_t vals_base2[] = {1, KILO_2_SZ, MEGA_2_SZ, GIGA_2_SZ, TERA_2_SZ, PETA_2_SZ};

typedef enum {
	NONE, KILO, MEGA, GIGA, TERA, PETA, UNIT_MAX
} unit_t;
static unit_t unit_adjust(off_t *);

static int unitp[] = {NONE, KILO, MEGA, GIGA, TERA, PETA};

#ifdef COLORLS
/* Most of these are taken from <sys/stat.h> */
typedef enum Colors {
	C_DIR,			/* directory */
	C_LNK,			/* symbolic link */
	C_SOCK,			/* socket */
	C_FIFO,			/* pipe */
	C_EXEC,			/* executable */
	C_BLK,			/* block special */
	C_CHR,			/* character special */
	C_SUID,			/* setuid executable */
	C_SGID,			/* setgid executable */
	C_WSDIR,		/* directory writeble to others, with sticky
				 * bit */
	C_WDIR,			/* directory writeble to others, without
				 * sticky bit */
	C_NUMCOLORS		/* just a place-holder */
} Colors;

static const char *defcolors = "exfxcxdxbxegedabagacad";

/* colors for file types */
static struct {
	int	num[2];
	int	bold;
} colors[C_NUMCOLORS];
#endif

void
printscol(DISPLAY *dp)
{
	FTSENT *p;

	for (p = dp->list; p; p = p->fts_link) {
		if (IS_NOPRINT(p))
			continue;
		(void)printaname(p, dp->s_inode, dp->s_block);
		(void)putchar('\n');
	}
}

/*
 * print name in current style
 */
static int
printname(const char *name)
{
	if (f.octal || f.octal_escape)
		return prn_octal(name);
	else if (f.nonprint)
		return prn_printable(name);
	else
		return printf("%s", name);
}

void
printlong(DISPLAY *dp)
{
	struct stat *sp;
	FTSENT *p;
	NAMES *np;
	char buf[20];
#ifdef COLORLS
	int color_printed = 0;
#endif

	if (dp->list->fts_level != FTS_ROOTLEVEL && (f.longform || f.size))
		(void)printf("total %qu\n", (u_int64_t)howmany(dp->btotal, blocksize));

	for (p = dp->list; p; p = p->fts_link) {
		if (IS_NOPRINT(p))
			continue;
		sp = p->fts_statp;
		if (f.inode)
			(void)printf("%*lu ", dp->s_inode, (u_long)sp->st_ino);
		if (f.size)
			(void)printf("%*qu ",
			    dp->s_block, (u_int64_t)howmany(sp->st_blocks, blocksize));
		strmode(sp->st_mode, buf);
		np = p->fts_pointer;
		(void)printf("%s %*u %-*s  %-*s  ", buf, dp->s_nlink,
		    sp->st_nlink, dp->s_user, np->user, dp->s_group,
		    np->group);
		if (f.flags)
			(void)printf("%-*s ", dp->s_flags, np->flags);
		if (S_ISCHR(sp->st_mode) || S_ISBLK(sp->st_mode))
			if (minor(sp->st_rdev) > 255 || minor(sp->st_rdev) < 0)
				(void)printf("%3d, 0x%08x ",
				    major(sp->st_rdev),
				    (u_int)minor(sp->st_rdev));
			else
				(void)printf("%3d, %3d ",
				    major(sp->st_rdev), minor(sp->st_rdev));
		else if (dp->bcfile)
			(void)printf("%*s%*qu ",
			    8 - dp->s_size, "", dp->s_size, (u_int64_t)sp->st_size);
		else
			printsize(dp->s_size, sp->st_size);
		if (f.accesstime)
			printtime(sp->st_atime);
		else if (f.statustime)
			printtime(sp->st_ctime);
		else
			printtime(sp->st_mtime);
#ifdef COLORLS
		if (f.color)
			color_printed = colortype(sp->st_mode);
#endif
		(void)printname(p->fts_name);
#ifdef COLORLS
		if (f.color && color_printed)
			endcolor(0);
#endif
		if (f.type)
			(void)printtype(sp->st_mode);
		if (S_ISLNK(sp->st_mode))
			printlink(p);
		(void)putchar('\n');
	}
}

void
printstream(DISPLAY *dp)
{
	FTSENT *p;
	extern int termwidth;
	int chcnt;

	for (p = dp->list, chcnt = 0; p; p = p->fts_link) {
		if (p->fts_number == NO_PRINT)
			continue;
		if (strlen(p->fts_name) + chcnt +
		    (p->fts_link ? 2 : 0) >= (unsigned)termwidth) {
			putchar('\n');
			chcnt = 0;
		}
		chcnt += printaname(p, dp->s_inode, dp->s_block);
		if (p->fts_link) {
			printf(", ");
			chcnt += 2;
		}
	}
	if (chcnt)
		putchar('\n');
}
		
void
printcol(DISPLAY *dp)
{
	extern int termwidth;
	static FTSENT **array;
	static int lastentries = -1;
	FTSENT *p;
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

	if (f.notabs)
		tabwidth = 1;
	else
		tabwidth = 8;

	/*
	 * Have to do random access in the linked list -- build a table
	 * of pointers.
	 */
	if (dp->entries > lastentries) {
		lastentries = dp->entries;
		if ((array =
		    realloc(array, dp->entries * sizeof(FTSENT *))) == NULL) {
			warn(NULL);
			printscol(dp);
		}
	}
	for (p = dp->list, num = 0; p; p = p->fts_link)
		if (p->fts_number != NO_PRINT)
			array[num++] = p;

	colwidth = dp->maxlen;
	if (f.inode)
		colwidth += dp->s_inode + 1;
	if (f.size)
		colwidth += dp->s_block + 1;
	if (f.type)
		colwidth += 1;

	colwidth = (colwidth + tabwidth) & ~(tabwidth - 1);
	if (termwidth < 2 * colwidth) {
		printscol(dp);
		return;
	}
	numcols = termwidth / colwidth;
	numrows = num / numcols;
	if (num % numcols)
		++numrows;

	if (dp->list->fts_level != FTS_ROOTLEVEL && (f.longform || f.size))
		(void)printf("total %qu\n", (u_int64_t)howmany(dp->btotal, blocksize));

	base = 0;
	for (row = 0; row < numrows; ++row) {
		endcol = colwidth;
		if (!f.sortacross)
			base = row;
		for (col = 0, chcnt = 0; col < numcols; ++col) {
			chcnt += printaname(array[base], dp->s_inode,
			    dp->s_block);
			if (f.sortacross)
				base++;
			else
				base += numrows;
			if (base >= num)
				break;
			while ((cnt = ((chcnt + tabwidth) & ~(tabwidth - 1)))
			    <= endcol) {
				if (f.sortacross && col + 1 >= numcols)
					break;
				(void)putchar(f.notabs ? ' ' : '\t');
				chcnt = cnt;
			}
			endcol += colwidth;
		}
		(void)putchar('\n');
	}
}

/*
 * print [inode] [size] name
 * return # of characters printed, no trailing characters.
 */
static int
printaname(FTSENT *p, u_long inodefield, u_long sizefield)
{
	struct stat *sp;
	int chcnt;
#ifdef COLORLS
	int color_printed = 0;
#endif

	sp = p->fts_statp;
	chcnt = 0;
	if (f.inode)
		chcnt += printf("%*lu ", (int)inodefield, (u_long)sp->st_ino);
	if (f.size)
		chcnt += printf("%*qu ",
		    (int)sizefield, (u_int64_t)howmany(sp->st_blocks, blocksize));
#ifdef COLORLS
	if (f.color)
		color_printed = colortype(sp->st_mode);
#endif
	chcnt += printname(p->fts_name);
#ifdef COLORLS
	if (f.color && color_printed)
		endcolor(0);
#endif
	if (f.type)
		chcnt += printtype(sp->st_mode);
	return (chcnt);
}

static void
printtime(time_t ftime)
{
	char longstring[80];
	static time_t now;
	const char *format;
	static int d_first = -1;

	if (now == 0)
		now = time(NULL);

	if (f.sectime)
		/* mmm dd hh:mm:ss yyyy || dd mmm hh:mm:ss yyyy */
		format = d_first ? "%e %b %T %Y " : "%b %e %T %Y ";
	else if (ftime + SIXMONTHS > now && ftime < now + SIXMONTHS)
		/* mmm dd hh:mm || dd mmm hh:mm */
		format = d_first ? "%e %b %R " : "%b %e %R ";
	else
		/* mmm dd  yyyy || dd mmm  yyyy */
		format = d_first ? "%e %b  %Y " : "%b %e  %Y ";
	strftime(longstring, sizeof(longstring), format, localtime(&ftime));
	fputs(longstring, stdout);
}

static int
printtype(u_int mode)
{

	if (f.slash) {
		if ((mode & S_IFMT) == S_IFDIR) {
			(void)putchar('/');
			return (1);
		}
		return (0);
	}

	switch (mode & S_IFMT) {
	case S_IFDIR:
		(void)putchar('/');
		return (1);
	case S_IFIFO:
		(void)putchar('|');
		return (1);
	case S_IFLNK:
		(void)putchar('@');
		return (1);
	case S_IFSOCK:
		(void)putchar('=');
		return (1);
	case S_IFWHT:
		(void)putchar('%');
		return (1);
	default:
		break;
	}
	if (mode & (S_IXUSR | S_IXGRP | S_IXOTH)) {
		(void)putchar('*');
		return (1);
	}
	return (0);
}

#ifdef COLORLS
static int
putch(int c)
{
	(void)putchar(c);
	return 0;
}

static int
writech(int c)
{
	char tmp = c;

	(void)write(STDOUT_FILENO, &tmp, 1);
	return 0;
}

static void
printcolor(Colors c)
{
	char *ansiseq;

	if (colors[c].bold)
		tputs(enter_bold, 1, putch);

	if (colors[c].num[0] != -1) {
		ansiseq = tgoto(ansi_fgcol, 0, colors[c].num[0]);
		if (ansiseq)
			tputs(ansiseq, 1, putch);
	}
	if (colors[c].num[1] != -1) {
		ansiseq = tgoto(ansi_bgcol, 0, colors[c].num[1]);
		if (ansiseq)
			tputs(ansiseq, 1, putch);
	}
}

static void
endcolor(int sig)
{
	tputs(ansi_coloff, 1, sig ? writech : putch);
	tputs(attrs_off, 1, sig ? writech : putch);
}

static int
colortype(mode_t mode)
{
	switch (mode & S_IFMT) {
	case S_IFDIR:
		if (mode & S_IWOTH)
			if (mode & S_ISTXT)
				printcolor(C_WSDIR);
			else
				printcolor(C_WDIR);
		else
			printcolor(C_DIR);
		return (1);
	case S_IFLNK:
		printcolor(C_LNK);
		return (1);
	case S_IFSOCK:
		printcolor(C_SOCK);
		return (1);
	case S_IFIFO:
		printcolor(C_FIFO);
		return (1);
	case S_IFBLK:
		printcolor(C_BLK);
		return (1);
	case S_IFCHR:
		printcolor(C_CHR);
		return (1);
	}
	if (mode & (S_IXUSR | S_IXGRP | S_IXOTH)) {
		if (mode & S_ISUID)
			printcolor(C_SUID);
		else if (mode & S_ISGID)
			printcolor(C_SGID);
		else
			printcolor(C_EXEC);
		return (1);
	}
	return (0);
}

void
parsecolors(const char *cs)
{
	int i;
	int j;
	int len;
	char c[2];
	short legacy_warn = 0;

	if (cs == NULL)
		cs = "";	/* LSCOLORS not set */
	len = strlen(cs);
	for (i = 0; i < C_NUMCOLORS; i++) {
		colors[i].bold = 0;

		if (len <= 2 * i) {
			c[0] = defcolors[2 * i];
			c[1] = defcolors[2 * i + 1];
		} else {
			c[0] = cs[2 * i];
			c[1] = cs[2 * i + 1];
		}
		for (j = 0; j < 2; j++) {
			/* Legacy colours used 0-7 */
			if (c[j] >= '0' && c[j] <= '7') {
				colors[i].num[j] = c[j] - '0';
				if (!legacy_warn) {
					fprintf(stderr,
					    "warn: LSCOLORS should use "
					    "characters a-h instead of 0-9 ("
					    "see the manual page)\n");
				}
				legacy_warn = 1;
			} else if (c[j] >= 'a' && c[j] <= 'h')
				colors[i].num[j] = c[j] - 'a';
			else if (c[j] >= 'A' && c[j] <= 'H') {
				colors[i].num[j] = c[j] - 'A';
				colors[i].bold = 1;
			} else if (tolower((unsigned char)c[j] == 'x'))
				colors[i].num[j] = -1;
			else {
				fprintf(stderr,
				    "error: invalid character '%c' in LSCOLORS"
				    " env var\n", c[j]);
				colors[i].num[j] = -1;
			}
		}
	}
}

void
colorquit(int sig)
{
	endcolor(sig);

	(void)signal(sig, SIG_DFL);
	(void)kill(getpid(), sig);
}

#endif /* COLORLS */

static void
printlink(FTSENT *p)
{
	int lnklen;
	// char name[MAXPATHLEN + 1];
	char path[MAXPATHLEN + 1];

	// if (p->fts_level == FTS_ROOTLEVEL)
	// 	(void)snprintf(name, sizeof(name), "%s", p->fts_name);
	// else
	// 	(void)snprintf(name, sizeof(name),
	// 	    "%s/%s", p->fts_parent->fts_accpath, p->fts_name);
	if ((lnklen = readlink(p->fts_accpath, path, sizeof(path) - 1)) == -1) {
		(void)fprintf(stderr, "\nuls: %s: %s\n", p->fts_accpath, strerror(p->fts_errno));
		return;
	}
	path[lnklen] = '\0';
	(void)printf(" -> ");
	(void)printname(path);
}

static void
printsize(size_t width, off_t bytes)
{
	unit_t unit;

	if (f.humanval) {
		unit = unit_adjust(&bytes);

		if (bytes == 0)
			(void)printf("%*s ", (int)width, "0B");
		else
			(void)printf("%*lld%c ", (int)width - 1, bytes,
			    "BKMGTPE"[unit]);
	} else
		(void)printf("%*lld ", (int)width, bytes);
}

/*
 * Output in "human-readable" format.  Uses 3 digits max and puts
 * unit suffixes at the end.  Makes output compact and easy to read,
 * especially on huge disks.
 *
 */
unit_t
unit_adjust(off_t *val)
{
	double abval;
	unit_t unit;
	unsigned int unit_sz;

	abval = fabs((double)*val);

	unit_sz = abval ? ilogb(abval) / 10 : 0;

	if (unit_sz >= UNIT_MAX) {
		unit = NONE;
	} else {
		unit = unitp[unit_sz];
		*val /= (double)vals_base2[unit_sz];
	}

	return (unit);
}