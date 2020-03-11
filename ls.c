#include "fts.c"

 void	 display(FTSENT *, FTSENT *);
 u_quad_t	 makenines(u_long);
FTS *traverse(int, char **, int);


long blocksize;			/* block size units */
int termwidth = 80;		/* default terminal width */

#ifdef COLORLS
char *ansi_bgcol;		/* ANSI sequence to set background colour */
char *ansi_fgcol;		/* ANSI sequence to set foreground colour */
char *ansi_coloff;		/* ANSI sequence to reset colours */
char *attrs_off;		/* ANSI sequence to turn off attributes */
char *enter_bold;		/* ANSI sequence to set color to bold mode */
#endif

static int rval;

int
main(int argc, char *argv[])
{
	static char dot[] = ".", *dotav[] = {dot, NULL};
	struct winsize win;
	int ch, fts_options, notused;
	char *p;
#ifdef COLORLS
	char termcapbuf[1024];	/* termcap definition buffer */
	char tcapbuf[512];	/* capability buffer */
	char *bp = tcapbuf;
#endif

	(void)setlocale(LC_ALL, "");

	/* Terminal defaults to -Cq, non-terminal defaults to -1. */
	if (isatty(STDOUT_FILENO)) {
		termwidth = 80;
		if ((p = getenv("COLUMNS")) != NULL && *p != '\0')
			termwidth = atoi(p);
		else if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &win) != -1 &&
		    win.ws_col > 0)
			termwidth = win.ws_col;
		f.nonprint = 1;
	} else {
		f.singlecol = 1;
		/* retrieve environment variable, in case of explicit -C */
		p = getenv("COLUMNS");
		if (p)
			termwidth = atoi(p);
	}

	/* Root is -A automatically. */
	if (!getuid())
		f.listdot = 1;

	fts_options = FTS_PHYSICAL;
 	while ((ch = getopt(argc, argv, "1ABCFGHLPRSTWZabcdfghiklmnopqrstuvwx")) 
	    != -1) {
		switch (ch) {
		/*
		 * The -1, -C, -x and -l options all override each other so
		 * shell aliasing works right.
		 */
		case '1':
			f.singlecol = 1;
			f.longform = 0;
			f.stream = 0;
			break;
		case 'B':
			f.nonprint = 0;
			f.octal = 1;
			f.octal_escape = 0;
			break;
		case 'C':
			f.sortacross = f.longform = f.singlecol = 0;
			break;
		case 'l':
			f.longform = 1;
			f.singlecol = 0;
			f.stream = 0;
			break;
		case 'x':
			f.sortacross = 1;
			f.longform = 0;
			f.singlecol = 0;
			break;
		/* The -c and -u options override each other. */
		case 'c':
			f.statustime = 1;
			f.accesstime = 0;
			break;
		case 'u':
			f.accesstime = 1;
			f.statustime = 0;
			break;
		case 'F':
			f.type = 1;
			f.slash = 0;
			break;
		case 'H':
			fts_options |= FTS_COMFOLLOW;
			break;
		case 'G':
			break;
		case 'L':
			fts_options &= ~FTS_PHYSICAL;
			fts_options |= FTS_LOGICAL;
			break;
		case 'P':
			fts_options &= ~FTS_COMFOLLOW;
			fts_options &= ~FTS_LOGICAL;
			fts_options |= FTS_PHYSICAL;
			break;
		case 'R':
			f.recursive = 1;
			break;
		case 'a':
			fts_options |= FTS_SEEDOT;
			/* FALLTHROUGH */
		case 'A':
			f.listdot = 1;
			break;
		/* The -d option turns off the -R option. */
		case 'd':
			f.listdir = 1;
			f.recursive = 0;
			break;
		case 'f':
			f.nosort = 1;
			break;
		case 'g':	/* Compatibility with 4.3BSD. */
			break;
		case 'h':
			f.humanval = 1;
			break;
		case 'i':
			f.inode = 1;
			break;
		case 'k':
			f.kblocks = 1;
			break;
		case 'm':
			f.stream = 1;
			f.singlecol = 0;
			f.longform = 0;
			break;
		case 'n':
			f.numericonly = 1;
			break;
		case 'o':
			f.flags = 1;
			break;
		case 'p':
			f.slash = 1;
			f.type = 1;
			break;
		case 'q':
			f.nonprint = 1;
			f.octal = 0;
			f.octal_escape = 0;
			break;
		case 'r':
			f.reversesort = 1;
			break;
		case 'S':
			/* Darwin 1.4.1 compatibility */
			f.sizesort = 1;
			break;
		case 's':
			f.size = 1;
			break;
		case 'T':
			f.sectime = 1;
			break;
		case 't':
			f.timesort = 1;
			break;
		case 'W':
			f.whiteout = 1;
			break;
		case 'v':
			/* Darwin 1.4.1 compatibility */
			f.nonprint = 0;
			break;
		case 'b':
			f.nonprint = 0;
			f.octal = 0;
			f.octal_escape = 1;
			break;
		case 'w':
			f.nonprint = 0;
			f.octal = 0;
			f.octal_escape = 0;
			break;
		case 'Z':
			f.lomac = 1;
			break;
		default:
		case '?':
			usage();
		}
	}
	argc -= optind;
	argv += optind;

	/* Enabling of colours is conditional on the environment. */
	if (getenv("CLICOLOR") &&
	    (isatty(STDOUT_FILENO) || getenv("CLICOLOR_FORCE")))
#ifdef COLORLS
		if (tgetent(termcapbuf, getenv("TERM")) == 1) {
			ansi_fgcol = tgetstr("AF", &bp);
			ansi_bgcol = tgetstr("AB", &bp);
			attrs_off = tgetstr("me", &bp);
			enter_bold = tgetstr("md", &bp);

			/* To switch colours off use 'op' if
			 * available, otherwise use 'oc', or
			 * don't do colours at all. */
			ansi_coloff = tgetstr("op", &bp);
			if (!ansi_coloff)
				ansi_coloff = tgetstr("oc", &bp);
			if (ansi_fgcol && ansi_bgcol && ansi_coloff)
				f.color = 1;
		}
#else
		(void)fprintf(stderr, "Color support not compiled in.\n");
#endif /*COLORLS*/

#ifdef COLORLS
	if (f.color) {
		/*
		 * We can't put tabs and color sequences together:
		 * column number will be incremented incorrectly
		 * for "stty oxtabs" mode.
		 */
		f.notabs = 1;
		(void)signal(SIGINT, colorquit);
		(void)signal(SIGQUIT, colorquit);
		parsecolors(getenv("LSCOLORS"));
	}
#endif

	/*
	 * If not -F, -i, -l, -s or -t options, don't require stat
	 * information, unless in color mode in which case we do
	 * need this to determine which colors to display.
	 */
	if (!f.inode && !f.longform && !f.size && !f.timesort && !f.type && !f.sizesort
#ifdef COLORLS
	    && !f.color
#endif
	    )
		fts_options |= FTS_NOSTAT;

	/*
	 * If not -F, -d or -l options, follow any symbolic links listed on
	 * the command line.
	 */
	if (!f.longform && !f.listdir && !f.type)
		fts_options |= FTS_COMFOLLOW;

	/*
	 * If not recursing down this tree and don't need stat info, just get
	 * the names.
	 */
	if (!f.recursive && fts_options & FTS_NOSTAT)
		fts_options |= FTS_NAMEONLY;		

	if (!f.recursive)
		fts_options |= FTS_NORECUR;

	/*
	 * If -W, show whiteout entries
	 */
#ifdef FTS_WHITEOUT
	if (f.whiteout)
		fts_options |= FTS_WHITEOUT;
#endif

	/* If -l or -s, figure out block size. */
	if (f.longform || f.size) {
		if (f.kblocks)
			blocksize = 2;
		else {
			(void)getbsize(&notused, &blocksize);
			blocksize /= 512;
		}
	}

	statp = fts_options & FTS_LOGICAL ? stat : lstat;


	/* Select a sort function. */
	if (f.reversesort) {
		if (f.sizesort)
			sortfcn = revsizecmp;
		else if (!f.timesort)
			sortfcn = revnamecmp;
		else if (f.accesstime)
			sortfcn = revacccmp;
		else if (f.statustime)
			sortfcn = revstatcmp;
		else		/* Use modification time. */
			sortfcn = revmodcmp;
	} else {
		if (f.sizesort)
			sortfcn = sizecmp;
		else if (!f.timesort)
			sortfcn = namecmp;
		else if (f.accesstime)
			sortfcn = acccmp;
		else if (f.statustime)
			sortfcn = statcmp;
		else		/* Use modification time. */
			sortfcn = modcmp;
	}

	/* Select a print function. */
	if (f.singlecol)
		printfcn = printscol;
	else if (f.longform)
		printfcn = printlong;
	else if (f.stream)
		printfcn = printstream;
	else
		printfcn = printcol;

	if (argc)
		traverse(argc, argv, fts_options);
	else
		traverse(1, dotav, fts_options);
	// system("leaks -q uls");
	exit(rval);
}

static int output;		/* If anything output. */

/*
 * Traverse() walks the logical directory structure specified by the argv list
 * in the order specified by the mastercmp() comparison function.  During the
 * traversal it passes linked lists of structures to display() which represent
 * a superset (may be exact set) of the files to be displayed.
 */
FTS *traverse(int argc, char *argv[], int options) {
	FTS *ftsp;
	FTSENT *p, *chp;
	// int ch_options;

	if ((ftsp =
	    mx_fts_open(argv, options, f.nosort ? NULL : mastercmp)) == NULL)
		err(1, "mx_fts_open");

	display(NULL, mx_fts_children(ftsp, 0));
	if (f.listdir)
		return ftsp;

	while ((p = mx_fts_read(ftsp)) != NULL)
		switch (p->fts_info) {
			case FTS_DC:
				warnx("%s: directory causes a cycle", p->fts_name);
				break;
			case FTS_DNR:
			case FTS_ERR:
				warnx("%s: %s", p->fts_name, strerror(p->fts_errno));
				rval = 1;
				break;
			case FTS_D:
				if (p->fts_level != FTS_ROOTLEVEL) {
					if (p->fts_name[0] == '.' && !f.listdot) {
						mx_fts_skip_ch(ftsp);
						break;
					}
					if (options & FTS_SEEDOT && mx_is_dot(p->fts_name))
						break;
				}

				/*
				* If already output something, put out a newline as
				* a separator.  If multiple arguments, precede each
				* directory with its name.
				*/
				if (output)
					(void)printf("\n%s:\n", p->fts_accpath);
				else if (argc > 1) {
					(void)printf("%s:\n", p->fts_accpath);
					output = 1;
				}
				chp = mx_fts_children(ftsp, options);
				display(p, chp);

				// if (!f.recursive && chp != NULL)
					// mx_fts_skip_ch(ftsp);
				// 	(void)fts_set(ftsp, p, FTS_SKIP);

				break;
			default:
				break;
		}
	// if (errno)
	// 	err(1, "mx_fts_read");
	return ftsp;

}

/*
 * Display() takes a linked list of FTSENT structures and passes the list
 * along with any other necessary information to the print function.  P
 * points to the parent directory of the display list.
 */
 void
display(FTSENT *p, FTSENT *list)
{
	struct stat *sp;
	DISPLAY d;
	FTSENT *cur;
	NAMES *np;
	off_t maxsize;
	u_int64_t btotal, maxblock;
	u_long lattrlen, maxinode, maxlen, maxnlink, maxlattr;
	int bcfile, maxflags;
	gid_t maxgroup;
	uid_t maxuser;
	size_t flen, ulen, glen;
	int entries, needstats;
	const char *user, *group;
	char *flags, *lattr = NULL;
	char buf[STRBUF_SIZEOF(u_quad_t) + 1];
	char ngroup[STRBUF_SIZEOF(uid_t) + 1];
	char nuser[STRBUF_SIZEOF(gid_t) + 1];

	/*
	 * If list is NULL there are two possibilities: that the parent
	 * directory p has no children, or that mx_fts_children() returned an
	 * error.  We ignore the error case since it will be replicated
	 * on the next call to mx_fts_read() on the post-order visit to the
	 * directory p, and will be signaled in traverse().
	 */
	if (list == NULL)
		return;

	needstats = f.inode || f.longform || f.size;
	flen = 0;
	btotal = 0;
	/* Fields match -lios order.  New ones should be added at the end. */
	maxlattr = maxblock = maxinode = maxlen = maxnlink =
	    maxuser = maxgroup = maxflags = maxsize = 0;
	bcfile = 0;
	flags = NULL;
	for (cur = list, entries = 0; cur; cur = cur->fts_link) {
		if (cur->fts_info == FTS_ERR || cur->fts_info == FTS_NS) {
			warnx("%s: %s",
			    cur->fts_name, strerror(cur->fts_errno));
			cur->fts_number = NO_PRINT;
			continue;
		}
		/*
		 * P is NULL if list is the argv list, to which different rules
		 * apply.
		 */
		if (p == NULL) {
			/* Directories will be displayed later. */
			if (cur->fts_info == FTS_D && !f.listdir) {
				cur->fts_number = NO_PRINT;
				continue;
			}
		}
		else {
			/* Only display dot file if -a/-A set. */
			if (cur->fts_name[0] == '.' && !f.listdot) {
				cur->fts_number = NO_PRINT;
				continue;
			}
		}
		if (cur->fts_namelen > maxlen)
			maxlen = cur->fts_namelen;
		if (f.octal || f.octal_escape) {
			u_long t = len_octal(cur->fts_name, cur->fts_namelen);

			if (t > maxlen)
				maxlen = t;
		}
		if (needstats) {
			sp = cur->fts_statp;
			if (sp->st_blocks > (blkcnt_t)maxblock)
				maxblock = sp->st_blocks;
			if (sp->st_ino > maxinode)
				maxinode = sp->st_ino;
			if (sp->st_nlink > maxnlink)
				maxnlink = sp->st_nlink;
			if (sp->st_size > maxsize)
				maxsize = sp->st_size;

			btotal += sp->st_blocks;
			if (f.longform) {
				if (f.numericonly) {
					(void)snprintf(nuser, sizeof(nuser),
					    "%u", sp->st_uid);
					(void)snprintf(ngroup, sizeof(ngroup),
					    "%u", sp->st_gid);
					user = nuser;
					group = ngroup;
				} else {
					user = user_from_uid(sp->st_uid, 0);
					group = group_from_gid(sp->st_gid, 0);
				}
				if ((ulen = strlen(user)) > maxuser)
					maxuser = ulen;
				if ((glen = strlen(group)) > maxgroup)
					maxgroup = glen;
				if (f.flags) {
					flags = fflagstostr(sp->st_flags);
					if (flags != NULL && *flags == '\0') {
						free(flags);
						flags = strdup("-");
					}
					if (flags == NULL)
						err(1, "fflagstostr");
					flen = strlen(flags);
					if (flen > (size_t)maxflags)
						maxflags = flen;
				} else
					flen = 0;
				lattr = NULL;
					lattrlen = 0;

				if ((np = malloc(sizeof(NAMES) + lattrlen +
				    ulen + glen + flen + 4)) == NULL)
					err(1, "malloc");

				np->user = &np->data[0];
				(void)strcpy(np->user, user);
				np->group = &np->data[ulen + 1];
				(void)strcpy(np->group, group);

				if (S_ISCHR(sp->st_mode) ||
				    S_ISBLK(sp->st_mode))
					bcfile = 1;

				if (f.flags) {
					np->flags = &np->data[ulen + glen + 2];
					(void)strcpy(np->flags, flags);
					free(flags);
				}
				cur->fts_pointer = np;
			}
		}
		++entries;
	}

	if (!entries) {
		mx_children_clear(list);
		return;
	}

	d.list = list;
	d.entries = entries;
	d.maxlen = maxlen;
	if (needstats) {
		d.bcfile = bcfile;
		d.btotal = btotal;
		(void)snprintf(buf, sizeof(buf), "%qu", (u_int64_t)maxblock);
		d.s_block = strlen(buf);
		d.s_flags = maxflags;
		d.s_lattr = maxlattr;
		d.s_group = maxgroup;
		(void)snprintf(buf, sizeof(buf), "%lu", maxinode);
		d.s_inode = strlen(buf);
		(void)snprintf(buf, sizeof(buf), "%lu", maxnlink);
		d.s_nlink = strlen(buf);
		(void)snprintf(buf, sizeof(buf), "%qu", (u_int64_t)maxsize);
		d.s_size = strlen(buf);
		d.s_user = maxuser;
	}
	printfcn(&d);
	output = 1;

	if (f.longform)
		for (cur = list; cur; cur = cur->fts_link)
			free(cur->fts_pointer);
	mx_children_clear(list);
}

/*
 * Ordering for mastercmp:
 * If ordering the argv (fts_level = FTS_ROOTLEVEL) return non-directories
 * as larger than directories.  Within either group, use the sort function.
 * All other levels use the sort function.  Error entries remain unsorted.
 */
