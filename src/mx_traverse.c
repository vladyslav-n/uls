#include "../inc/uls.h"

static void choose_rev_sortfcn(bool (**sortfcn)(void *, void *), t_flags *f) {
	if (f->sizesort)
		*sortfcn = mx_revsizecmp;
	else if (!f->timesort)
		*sortfcn = mx_revnamecmp;
	else if (f->accesstime)
		*sortfcn = mx_revacccmp;
	else if (f->statustime)
		*sortfcn = mx_revstatcmp;
	else if (f->birthtime)
		*sortfcn = mx_revbirthcmp;
	else		/* Use modification time. */
		*sortfcn = mx_revmodcmp;
}

static void choose_sortfcn(bool (**sortfcn)(void *, void *), t_flags *f) {
	if (f->sizesort)
		*sortfcn = mx_sizecmp;
	else if (!f->timesort)
		*sortfcn = mx_namecmp;
	else if (f->accesstime)
		*sortfcn = mx_acccmp;
	else if (f->statustime)
		*sortfcn = mx_statcmp;
	else if (f->birthtime)
		*sortfcn = mx_birthcmp;
	else		/* Use modification time. */
		*sortfcn = mx_modcmp;
}

static void not_root_dir(int argc, int options, t_file *p, t_flags *f) {
    if (f->output || (!f->output && argc > 1)) {
        if (f->output)
            mx_printchar('\n');
        else
            f->output = 1;
        mx_printstr(p->accpath);
        mx_printstr(":\n");
    }
    mx_display(p, mx_fts_children(f->ftsp, options), f);
}

static void switch_p_info(int argc, int options, t_file *p, t_flags *f) {
    if (p->info == MX_DC) {
        mx_printerr_errnum(p->name, -1);
        mx_printerror("directory causes a cycle\n");
    }
    else if (p->info == MX_ERR) {
        mx_printerr_errnum(p->name, p->f_errno);
        f->rval = 1;
    }
    else if (p->info == MX_D) {
        if (p->level != MX_ROOTLEV) {
            if (p->name[0] == '.' && !f->listdot) {
                mx_fts_skip_ch(f->ftsp);
                return;
            }
            if (options & MX_SEEDOT && mx_is_dot(p->name))
                return;
        }
        not_root_dir(argc, options, p, f);
    }
}

/*
 * mx_traverse() walks the logical directory structure specified by 
 * the argv list
 * in the order specified by the mastercmp() comparison function.  
 * During the
 * traversal it passes linked lists of structures to display() which 
 * represent
 * a superset (may be exact set) of the files to be displayed.
 */
t_fts *mx_traverse(int argc, char *argv[], int options, t_flags *f) {
	t_fts *ftsp;
	t_file *p;
	bool (*sortfcn)(void *, void *);

	if (f->reversesort)
		choose_rev_sortfcn(&sortfcn, f);
	else
		choose_sortfcn(&sortfcn, f);
	if ((ftsp =
	    mx_fts_open(argv, options, f->nosort ? NULL : sortfcn)) == NULL)
		mx_printerr_errnum("mx_fts_open", errno);
    f->ftsp = ftsp;
    mx_display(NULL, mx_fts_children(ftsp, options), f);
    if (f->listdir)
		return ftsp;

	while ((p = mx_fts_read(ftsp)) != NULL)
        switch_p_info(argc, options, p, f);
    return ftsp;
}
