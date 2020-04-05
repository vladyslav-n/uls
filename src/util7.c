#include "../inc/uls.h"

ssize_t mx_xattr_size(t_flags *f, t_file *file) {
    int xattropt = (file->level == MX_ROOTLEV && f->comfollow)
        || f->lfollow ? 0 : XATTR_NOFOLLOW;
        
    return listxattr(file->accpath, NULL, 0, xattropt);
}

void mx_print_xattr_line(char *path, char *xattr_name, t_file *file,
                         t_flags *f) {
    ssize_t xattr_value;
    int xattropt = (file->level == MX_ROOTLEV && f->comfollow)
        || f->lfollow ? 0 : XATTR_NOFOLLOW;

    xattr_value = getxattr(path, xattr_name, NULL, 0, 0, xattropt);
    mx_printchar('\t');
    mx_printstr(xattr_name);
    mx_printchar('\t');
    mx_print_spaces(3);
    mx_printint(xattr_value);
    mx_printstr(f->humanval ? "B \n" : " \n");
}

void mx_print_xattr(t_file *file, t_flags *f) {
    char *buf;
    ssize_t bufsize = mx_xattr_size(f, file);
    int xattropt = (file->level == MX_ROOTLEV && f->comfollow)
        || f->lfollow ? 0 : XATTR_NOFOLLOW;

    if (!f->xattr)
        return;
    if (bufsize == -1)
        return;
    buf = malloc(bufsize);
    listxattr(file->accpath, buf, bufsize, xattropt);
    for (int i = 0; bufsize; i += mx_strlen(buf +i) + 1) {
        mx_print_xattr_line(file->accpath, buf + i, file, f);
        bufsize -= mx_strlen(buf + i) + 1;
    }
    free(buf);
}
