#include "../inc/uls.h"

static void mx_display_if_need_stat_and_longform(t_flags *f, struct s_names **np,
    struct s_display_max *max, struct s_disp_buf *db) {
    if (f->longform) {
        mx_usrname(db->sp->st_uid, db->nuser, f);
        mx_grpname(db->sp->st_gid, db->ngroup, f);
        if ((db->ulen = mx_strlen(db->nuser)) > max->user)
            max->user = db->ulen;
        if ((db->glen = mx_strlen(db->ngroup)) > max->group)
            max->group = db->glen;
        if ((*np = malloc(sizeof(struct s_names) + db->ulen
                + db->glen + 2)) == NULL)
            mx_printerr_errnum("malloc", errno);
        (*np)->user = &(*np)->data[0];
        mx_strcpy((*np)->user, db->nuser);
        (*np)->group = &(*np)->data[db->ulen + 1];
        mx_strcpy((*np)->group, db->ngroup);
        if (MX_S_ISCHR(db->sp->st_mode) ||
            MX_S_ISBLK(db->sp->st_mode))
            db->bcfile = 1;
        db->cur->pointer = *np;
    }
}

void mx_display_if_need_stat(t_flags *f, struct s_names **np,
    struct s_display_max *max, struct s_disp_buf *db) {
    db->sp = db->cur->statp;
    if (db->sp->st_blocks > (blkcnt_t)max->block)
        max->block = db->sp->st_blocks;
    if (db->sp->st_ino > max->inode)
        max->inode = db->sp->st_ino;
    if (db->sp->st_nlink > max->nlink)
        max->nlink = db->sp->st_nlink;
    if (db->sp->st_size > max->size)
        max->size = db->sp->st_size;
    db->btotal += db->sp->st_blocks;
    mx_display_if_need_stat_and_longform(f, np, max, db);
}
