#include "../inc/uls.h"

void mx_printlong_ischar_isblock(char *buf, struct stat *sp) {
    if (MX_MINOR(sp->st_rdev) > 255 || MX_MINOR(sp->st_rdev) < 0) {
        mx_print_align(mx_printbuf(buf, mx_llitoa(MX_MAJOR(sp->st_rdev))),
                                   3, 1);
        mx_printstr(", 0x");
        mx_print_align_c(mx_printbuf(buf, mx_nbr_to_hex(
            (u_int)MX_MINOR(sp->st_rdev))), '0', 8, 1);
        mx_printchar(' ');
    }
    else {
        mx_print_align(mx_printbuf(buf, mx_llitoa(MX_MAJOR(sp->st_rdev))),
                                   3, 1);
        mx_printstr(", ");
        mx_print_align(mx_printbuf(buf, mx_llitoa(MX_MINOR(sp->st_rdev))),
                                   3, 1);
        mx_printchar(' ');
    }
}

void mx_printlong_mode_user_group(struct s_display *dp, t_flags *f, char *buf,
                              t_file *p) {
    if (f->inode) {
        mx_print_align(mx_printbuf(buf, mx_llitoa(p->statp->st_ino)),
                        dp->s_inode, 1);
        mx_printchar(' ');
    }
    if (f->size) {
        mx_print_align(mx_printbuf(buf, mx_llitoa(howmany(
                        p->statp->st_blocks, f->blocksize))), dp->s_block, 1);
        mx_printchar(' ');
    }
    mx_print_mode(f, p);
    mx_print_align(mx_printbuf(buf, mx_llitoa(p->statp->st_nlink)),
                    dp->s_nlink, 1);
    mx_printchar(' ');
    mx_print_align(mx_printbuf(buf, ((struct s_names*)p->pointer)->user),
                    dp->s_user, 0);
    mx_printstr("  ");
    mx_print_align(mx_printbuf(buf,((struct s_names*)p->pointer)->group),
                    dp->s_group, 0);
    mx_printstr("  ");
}

void mx_printlong_size_and_time_fields(struct s_display *dp, t_flags *f,
                                   struct stat *sp, int *color_printed) {
    char buf[MX_STRBUF_SIZEOF(u_int64_t)];

    if (MX_S_ISCHR(sp->st_mode) || MX_S_ISBLK(sp->st_mode))
        mx_printlong_ischar_isblock(buf, sp);
    else if (dp->bcfile) {
        mx_print_spaces(MX_ABS((int)((u_int)8 - dp->s_size)));
        mx_print_align(mx_printbuf(buf, mx_llitoa(sp->st_size)),
                        dp->s_size, 1);
        mx_printchar(' ');
    }
    else
        mx_printsize(dp->s_size, sp->st_size, f);
    if (f->accesstime)
        mx_printtime(sp->st_atime, f);
    else if (f->statustime)
        mx_printtime(sp->st_ctime, f);
    else
        mx_printtime(sp->st_mtime, f);
    if (f->color)
        *color_printed = mx_colortype(sp->st_mode, f);
}

void mx_prlong_cycle(struct s_display *dp, t_flags *f, char *buf,
                     int *color_printed) {
    struct stat *sp;

    for (t_file *p = dp->list; p; p = p->link) {
        if (MX_IS_NOPRINT(p))
            continue;
        sp = p->statp;
        mx_printlong_mode_user_group(dp, f, buf, p);
        mx_printlong_size_and_time_fields(dp, f, sp, color_printed);
        mx_prfname(p->name, f);
        if (f->color && *color_printed)
            mx_printstr(MX_C_RESET);
        if (f->type)
            mx_printtype(sp->st_mode, f);
        if (MX_S_ISLNK(sp->st_mode))
            mx_printlink(p, f);
        mx_printchar('\n');
        mx_print_xattr(p, f);
    }
}

void mx_printlong(struct s_display *dp, t_flags *f) {
    int color_printed = 0;
    char buf[MX_STRBUF_SIZEOF(u_int64_t)];

    if (dp->list->level != MX_ROOTLEV && (f->longform || f->size)) {
        mx_printstr("total ");
        mx_print_align(mx_printbuf(buf, mx_llitoa(howmany(
                       dp->btotal, f->blocksize))), 0, 1);
        mx_printchar('\n');
    }
    mx_prlong_cycle(dp, f, buf, &color_printed);
}

