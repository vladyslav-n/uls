#include "../inc/uls.h"

bool mx_birthcmp(void *a, void *b) {
    if (((t_file*)b)->statp->st_birthtimespec.tv_sec
        == ((t_file*)a)->statp->st_birthtimespec.tv_sec) {
        if (((t_file*)b)->statp->st_birthtimespec.tv_nsec
            == ((t_file*)a)->statp->st_birthtimespec.tv_nsec)
            return mx_namecmp(a, b);
        else
            return ((t_file*)b)->statp->st_birthtimespec.tv_nsec
                > ((t_file*)a)->statp->st_birthtimespec.tv_nsec;
    }
    return ((t_file*)b)->statp->st_birthtimespec.tv_sec
        > ((t_file*)a)->statp->st_birthtimespec.tv_sec;
}

bool mx_revbirthcmp(void *a, void *b) {
    if (((t_file*)b)->statp->st_birthtimespec.tv_sec
        == ((t_file*)a)->statp->st_birthtimespec.tv_sec) {
        if (((t_file*)b)->statp->st_birthtimespec.tv_nsec
            == ((t_file*)a)->statp->st_birthtimespec.tv_nsec)
            return mx_revnamecmp(a, b);
        else
            return ((t_file*)b)->statp->st_birthtimespec.tv_nsec
                < ((t_file*)a)->statp->st_birthtimespec.tv_nsec;
    }
    return ((t_file*)b)->statp->st_birthtimespec.tv_sec
        < ((t_file*)a)->statp->st_birthtimespec.tv_sec;
}

bool mx_sizecmp(void *a, void *b) {
    if (((t_file*)b)->statp->st_size == ((t_file*)a)->statp->st_size)
        return mx_namecmp(a, b);
    return ((t_file*)b)->statp->st_size > ((t_file*)a)->statp->st_size;
}

bool mx_revsizecmp(void *a, void *b) {
    if (((t_file*)b)->statp->st_size == ((t_file*)a)->statp->st_size)
        return mx_revnamecmp(a, b);
    return ((t_file*)b)->statp->st_size < ((t_file*)a)->statp->st_size;
}

