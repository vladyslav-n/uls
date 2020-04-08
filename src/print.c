#include "../inc/uls.h"

/*
 * print name in current style
 */
int mx_prfname(char *name, t_flags *f) {
    if (f->octal || f->octal_escape)
        return mx_prn_octal(name, f);
    else if (f->nonprint)
        return mx_prn_printable(name);
    else
        return mx_print_align(name, -1, 1);
}

/*
 * print [inode] [size] name
 * return # of characters printed, no trailing characters.
 */
int mx_printname(t_file *p, u_long inodefield, u_long sizefield, t_flags *f) {
    int chcnt = 0;
    char buf[MX_STRBUF_SIZEOF(u_int64_t)];
    int color_printed = 0;

    if (f->inode)
        chcnt += mx_print_align(mx_printbuf(buf, mx_llitoa(p->statp->st_ino)),
                                            inodefield, 1);
    if (f->size)
        chcnt += mx_print_align(mx_printbuf(buf, mx_llitoa(
            MX_HOWMANY(p->statp->st_blocks, f->blocksize))), sizefield, 1);
    if (f->inode || f->size)
        mx_printchar(' ');
    if (f->color)
        color_printed = mx_colortype(p->statp->st_mode, f);
    chcnt += mx_prfname(p->name, f);
    if (f->color && color_printed)
        mx_printstr(MX_C_RESET);
    if (f->type)
        chcnt += mx_printtype(p->statp->st_mode, f);
    return chcnt;
}

void mx_printlink(t_file *p, t_flags *f) {
    int lnklen;
    char path[MAXPATHLEN + 1];

    if ((lnklen = readlink(p->accpath, path, sizeof(path) - 1)) == -1) {
        mx_printerror("\n");
        mx_printerr_errnum(p->accpath, errno);
        return;
    }
    path[lnklen] = '\0';
    mx_printstr(" -> ");
    mx_prfname(path, f);
}

void mx_printscol(struct s_display *dp, t_flags *f) {
    t_file *p;

    for (p = dp->list; p; p = p->link) {
        if (MX_IS_NOPRINT(p))
            continue;
        mx_printname(p, dp->s_inode, dp->s_block, f);
        mx_printchar('\n');
    }
}

void mx_printstream(struct s_display *dp, t_flags *f) {
    t_file *p;
    int chcnt;

    for (p = dp->list, chcnt = 0; p; p = p->link) {
        if (p->number == MX_NO_PRINT)
            continue;
        if (mx_strlen(p->name) + chcnt +
            (p->link ? 2 : 0) >= f->termwidth) {
            mx_printchar('\n');
            chcnt = 0;
        }
        chcnt += mx_printname(p, dp->s_inode, dp->s_block, f);
        if (p->link) {
            mx_printstr(", ");
            chcnt += 2;
        }
    }
    if (chcnt)
        mx_printchar('\n');
}
