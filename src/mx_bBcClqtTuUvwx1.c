#include "../inc/uls.h"

static void Clx1(char c, t_flags *f) {
    if (c == '1') {
        f->singlecol = 1;
        f->longform = 0;
        f->stream = 0;
    }
    else if (c == 'C') {
        f->sortacross = 0;
        f->longform = 0;
        f->singlecol = 0;
    }
    else if (c == 'x') {
        f->sortacross = 1;
        f->longform = 0;
        f->singlecol = 0;
    }
    else if (c == 'l') {
        f->longform = 1;
        f->singlecol = 0;
        f->stream = 0;
    }
}

static void ctTuU(char c, t_flags *f) {
    if (c == 'T')
        f->sectime = 1;
    else if (c == 't')
        f->timesort = 1;
    else if (c == 'c') {
        f->statustime = 1;
        f->accesstime = 0;
        f->birthtime = 0;
    }
    else if (c == 'u') {
        f->accesstime = 1;
        f->statustime = 0;
        f->birthtime = 0;
    }
    else if (c == 'U') {
        f->birthtime = 1;
        f->accesstime = 0;
        f->statustime = 0;
    }
}

static void bBqvw(char c, t_flags *f) {
    if (c == 'q') {
        f->nonprint = 1;
        f->octal = 0;
        f->octal_escape = 0;
    }
    else if (c == 'v')
        f->nonprint = 0;
    else if (c == 'b' || c == 'B' || c == 'w') {
        f->nonprint = 0;
        if (c != 'b') {
            f->octal_escape = 0;
            if (c == 'B')
                f->octal = 1;
            else if (c == 'w')
                f->octal = 0;
        }
        else
        f->octal_escape = 1;
        f->octal = 0;
    }
}

void mx_bBcClqtTuUvwx1(char c, t_flags *f) {
    bBqvw(c, f);
    ctTuU(c, f);
    Clx1(c, f);
}
