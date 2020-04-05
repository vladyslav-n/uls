#include "../inc/uls.h"

bool mx_notexec_type(u_int mode) {
    switch (mode & S_IFMT) {
        case S_IFDIR:
            mx_printchar('/');
            return 1;
        case S_IFIFO:
            mx_printchar('|');
            return 1;
        case S_IFLNK:
            mx_printchar('@');
            return 1;
        case S_IFSOCK:
            mx_printchar('=');
            return 1;
        case S_IFWHT:
            mx_printchar('%');
            return 1;

        default:
            return 0;
    }
}

int mx_printtype(u_int mode, t_flags *f) {
    if (f->slash) {
        if ((mode & S_IFMT) == S_IFDIR) {
            mx_printchar('/');
            return 1;
        }
        return 0;
    }
    if (mx_notexec_type(mode))
        return 1;
    if (mode & (S_IXUSR | S_IXGRP | S_IXOTH)) {
        mx_printchar('*');
        return 1;
    }
    return 0;
}
