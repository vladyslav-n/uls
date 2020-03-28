#include "../inc/uls.h"

bool mx_is_cycle_dir(t_file *p) {
    unsigned long long ino;
    t_file *ptr;

    if (!p)
        return 0;
    ino = p->statp->st_ino;
    for (ptr = p->parent; ptr; ptr = ptr->parent)
        if (ino == ptr->statp->st_ino) {
            p->info = MX_DC;
            return 1;
        }
    return 0;
}

t_file *mx_dirent_to_ftsent(t_file *p, struct dirent *stdp, int options) {
    t_file *np = mx_nalloc(sizeof(t_file));
    struct stat *stsp;
    int (*statp)(const char *restrict , struct stat *restrict)
        = options & MX_LOGICAL ? stat : lstat;

    if (!stdp)
        return NULL;
    mx_strcpy(np->name, stdp->d_name);
    np->namelen = stdp->d_namlen;
    if (p) {
        mx_strcpy(np->par_path, p->accpath);
        mx_strcpy(np->par_path + p->accpathlen, "/");
        np->par_pathlen = p->accpathlen + 1;
        mx_strcpy(np->accpath, np->par_path);
        mx_strcpy(np->accpath + np->par_pathlen, np->name);
        np->accpathlen = np->par_pathlen + np->namelen; 
    }
    np->parent = p;
    np->level = p->level + 1;
    np->d_type = stdp->d_type;
    np->info = mx_fts_dirent_info(stdp, np->accpath, options);
    stsp = malloc(sizeof(struct stat)); 
    if (np->info == MX_SLNONE)
        lstat(np->accpath, stsp);
    else
        statp(np->accpath, stsp);
    np->statp = stsp;
    return np;
}

void mx_follow_link(char *name, char *buf, int size) {
    int len;
    char *bp;
    char tbuf[MAXPATHLEN + 1];

    if (!*buf)
        mx_strcpy(buf, name);
    len = readlink(buf, tbuf, MAXPATHLEN + 1);
    tbuf[len] = '\0';
    if (*tbuf != '/' && (bp = mx_memrchr(buf, '/', size)))
        mx_strcpy(bp + 1, tbuf);
    else
        mx_strcpy(buf, tbuf);
    if (mx_islink_stat(buf))
        mx_follow_link(buf, buf, size);
}
