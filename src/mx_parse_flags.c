#include "../inc/uls.h"

static void AFmnp(char c, t_flags *f) {
    if (c == 'F') {
        f->type = 1;
        f->slash = 0;
    }
    else if (c == 'n') {
        f->numericonly = 1;
        f->longform = 1;
    }
    else if (c == 'p') {
        f->slash = 1;
        f->type = 1;
    }
        else if (c == 'm') {
        f->stream = 1;
        f->singlecol = 0;
        f->longform = 0;
    }
    else if (c == 'A')
        f->listdot = 1;
}

static void ieGhkrRsS_xattr(char c, t_flags *f) {
    if (c == 'G')
        f->color = 1;
    if (c == 'i')
        f->inode = 1;
    if (c == 'e')
        f->acl = 1;
    else if (c == '@')
        f->xattr = 1;
    else if (c == 'R')
        f->recursive = 1;
    else if (c == 'r')
        f->reversesort = 1;
    else if (c == 'S')
        f->sizesort = 1;
    else if (c == 's')
        f->size = 1;
    else if (c == 'k')
        f->kblocks = 1;
    else if (c == 'h')
        f->humanval = 1;
}

static void adLHP(char c, t_flags *f, int *fts_options) {
    if (c == 'H')
        *fts_options |= MX_COMFOLLOW;
    else if (c == 'L') {
        *fts_options &= ~MX_PHYSICAL;
        *fts_options |= MX_LOGICAL;
    }
    else if (c == 'P') {
        *fts_options &= ~MX_COMFOLLOW;
        *fts_options &= ~MX_LOGICAL;
        *fts_options |= MX_PHYSICAL;
    }
    else if (c == 'a') {
        f->listdot = 1;
        *fts_options |= MX_SEEDOT;
    }
    else if (c == 'd') {
        f->listdir = 1;
        f->recursive = 0;
    }
}
void mx_parse_flag(char c, t_flags *f, int *fts_options) {
    mx_bBcCflqtTuUvwx1(c, fts_options, f);
    ieGhkrRsS_xattr(c, f);
    AFmnp(c, f);
    adLHP(c, f, fts_options);
}
