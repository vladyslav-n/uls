#include "../inc/uls.h"

void mx_display_process_name_and_stat(t_flags *f, struct s_names **np,
    struct s_display_max *max, struct s_disp_buf *db) {
    if (db->cur->namelen > max->len)
        max->len = db->cur->namelen;
    if (f->octal || f->octal_escape) {
        u_long t = mx_len_octal(db->cur->name, db->cur->namelen);

        if (t > max->len)
            max->len = t;
    }
    if (db->needstats)
        mx_display_if_need_stat(f, np, max, db);
    ++db->entries;
}

void mx_find_max(t_flags *f, struct s_names **np,
                 struct s_display_max *max, struct s_disp_buf *db) {
    for (db->cur = db->ch_list; db->cur; db->cur = db->cur->link) {
        if (db->cur->info == MX_ERR) {
            mx_printerr_errnum(db->cur->name, db->cur->f_errno);
            db->cur->number = MX_NO_PRINT;
            continue;
        }
        if (db->parent == NULL) {
            if (db->cur->info == MX_D && !f->listdir) {
                db->cur->number = MX_NO_PRINT;
                continue;
            }
        }
        else {
            if (db->cur->name[0] == '.' && !f->listdot) {
                db->cur->number = MX_NO_PRINT;
                continue;
            }
        }
        mx_display_process_name_and_stat(f, np, max, db);
    }
}

void mx_finalize_display_info(struct s_display_max *max, struct s_disp_buf *db,
    struct s_display *d, t_file *list) {
    d->list = list;
    d->entries = db->entries;
    d->maxlen = max->len;
    if (db->needstats) {
        d->bcfile = db->bcfile;
        d->btotal = db->btotal;
        mx_printbuf(db->buf, mx_llitoa(max->block));
        d->s_block = mx_strlen(db->buf);
        d->s_group = max->group;
        mx_printbuf(db->buf, mx_llitoa(max->inode));
        d->s_inode = mx_strlen(db->buf);
        mx_printbuf(db->buf,  mx_llitoa(max->nlink));
        d->s_nlink = mx_strlen(db->buf);
        mx_printbuf(db->buf, mx_llitoa((u_int64_t)max->size));
        d->s_size = mx_strlen(db->buf);
        d->s_user = max->user;
    }
}

void mx_print(t_flags *f, struct s_display *d) {
    void (*printfcn)(struct s_display *, t_flags *);

    if (f->singlecol)
        printfcn = mx_printscol;
    else if (f->longform)
        printfcn = mx_printlong;
    else if (f->stream)
        printfcn = mx_printstream;
    else
        printfcn = mx_printcol;

    printfcn(d, f);
}

/*
 * mx_display() takes a linked list of t_file structures and passes the list
 * along with any other necessary information to the print function.  P
 * points to the parent directory of the display list.
 */
void mx_display(t_file *p, t_file *list, t_flags *f) {
    struct s_display d;
    struct s_names *np;
    struct s_display_max max = {0};
    struct s_disp_buf db = {.needstats = f->inode || f->longform || f->size,
                      .parent = p, .ch_list = list};

    if (list == NULL)
        return;
    mx_find_max(f, &np, &max, &db);
    if (!db.entries) {
        mx_children_clear(list);
        return;
    }
    mx_finalize_display_info(&max, &db, &d, list);
    mx_print(f, &d);
    f->output = 1;
    if (f->longform)
        for (db.cur = list; db.cur; db.cur = db.cur->link)
            free(db.cur->pointer);
    mx_children_clear(list);
}
