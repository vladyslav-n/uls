#include "../inc/uls.h"

void mx_printcolor(enum e_colors c, t_flags *f) {
    if (f->colors[c].bold)
        mx_printstr(MX_C_BOLD);
    if (f->colors[c].num[0] != -1)
        mx_printstr(f->fg[f->colors[c].num[0]]);
    if (f->colors[c].num[1] != -1)
        mx_printstr(f->bg[f->colors[c].num[1]]);
}

static void colortype_notreg_notwoth(mode_t mode, int type, t_flags *f) {
    if (type == S_IFDIR) {
        if (mode & S_IWOTH)
            if (mode & S_ISTXT)
                mx_printcolor(C_WSDIR, f);
            else
                mx_printcolor(C_WDIR, f);
        else
            mx_printcolor(C_DIR, f);
    }
    else if (type == S_IFLNK)
        mx_printcolor(C_LNK, f);
    else if (type == S_IFSOCK)
        mx_printcolor(C_SOCK, f);
    else if (type == S_IFIFO)
        mx_printcolor(C_FIFO, f);
    else if (type == S_IFBLK)
        mx_printcolor(C_BLK, f);
    else if (type == S_IFCHR)
        mx_printcolor(C_CHR, f);
}

int mx_colortype(mode_t mode, t_flags *f) {
    int type = mode & S_IFMT;

    if (type != S_IFREG && type != S_IFWHT) {
        colortype_notreg_notwoth(mode, type, f);
        return 1;
    }
    if (mode & (S_IXUSR | S_IXGRP | S_IXOTH)) {
        if (mode & S_ISUID)
            mx_printcolor(C_SUID, f);
        else if (mode & S_ISGID)
            mx_printcolor(C_SGID, f);
        else
            mx_printcolor(C_EXEC, f);
        return 1;
    }
    return 0;
}

void mx_parsecolors(const char *cs, t_flags *f) {
    char c[2];

    if (!f->color)
        return;
    f->notabs = 1;
    for (int i = 0; i < C_NUMCOLORS; i++) {
        f->colors[i].bold = 0;
        c[0] = cs[2 * i];
        c[1] = cs[2 * i + 1];
        for (int j = 0; j < 2; j++) {
            if (c[j] >= 'a' && c[j] <= 'h')
                f->colors[i].num[j] = c[j] - 'a';
            else if (c[j] >= 'A' && c[j] <= 'H') {
                f->colors[i].num[j] = c[j] - 'A';
                f->colors[i].bold = 1;
            }
            else
                f->colors[i].num[j] = -1;
        }
    }
}
