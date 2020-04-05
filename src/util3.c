#include "../inc/uls.h"

t_file *mx_enoent(char *name) {
    t_file *np = mx_nalloc(sizeof(t_file));

    mx_strcpy(np->name, name);
    np->namelen = mx_strlen(name);
    mx_strcpy(np->accpath, np->name);
    np->accpathlen = np->namelen;
    np->level = MX_ROOTLEV;
    np->info = MX_ERR;
    np->f_errno = ENOENT;
    return np;
}

t_file *mx_cycle_dir(char *name, t_file *p) {
    t_file *np = mx_nalloc(sizeof(t_file));

    mx_strcpy(np->name, name);
    np->namelen = mx_strlen(name);
    np->info = MX_DC;
    np->level = p->level + 1;
    return np;
}

short mx_fts_root_info(char *name, int options) {
    struct stat sst;

    lstat(name, &sst);
    if (MX_S_ISDIR(sst.st_mode))
        return MX_D;
    if(!MX_S_ISLNK(sst.st_mode))
        return MX_DEFAULT;
    if (options & MX_COMFOLLOW && mx_link_info(name) == MX_DEFAULT)
        return MX_LNF;
    if (options & (MX_COMFOLLOW | MX_LOGICAL)) {
        return mx_link_info(name);
    }
    return MX_DEFAULT;
}

short mx_fts_dirent_info(struct dirent *ptr, char *path, int options) {
    if (ptr->d_type == DT_DIR)
        return MX_D;
    if (ptr->d_type != DT_LNK)
        return MX_DEFAULT;
    if (options & MX_LOGICAL)
        return mx_link_info(path);
    return MX_DEFAULT;
}

t_file *mx_root_file(char *name, int options) {
    t_file *np = mx_nalloc(sizeof(t_file));
    struct stat *stsp;

    mx_strcpy(np->name, name);
    np->namelen = mx_strlen(name);
    mx_strcpy(np->accpath, np->name);
    np->accpathlen = np->namelen;
    np->level = MX_ROOTLEV;
    np->d_type = mx_type_stat(name);
    np->info = mx_fts_root_info(name, options);
    stsp = malloc(sizeof(struct stat));
    if (np->info == MX_SLNONE || np->info == MX_LNF)
        lstat(np->accpath, stsp);
    else
        (options & (MX_LOGICAL | MX_COMFOLLOW) ? stat
                                               : lstat)(np->accpath, stsp);
    np->statp = stsp;
    return np;
}
