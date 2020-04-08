#include "../inc/uls.h"

t_flags mx_init_flags(void) {
    t_flags f = {.fg = {MX_C_FBLACK, MX_C_FRED, MX_C_FGREEN, MX_C_FYELLOW,
        MX_C_FBLUE, MX_C_FMAGENTA, MX_C_FCYAN, MX_C_FWHYTE},
        .bg = {MX_C_BBLACK, MX_C_BRED, MX_C_BGREEN, MX_C_BYELLOW, MX_C_BBLUE,
        MX_C_BMAGENTA, MX_C_BCYAN, MX_C_BWHYTE}, .termwidth = 80,
        .dotav = {".", NULL}
    };

    return f;
}

void mx_process_flags_after_parsing(t_flags *f, int *fts_options) {
    if (!f->inode && !f->longform && !f->size && !f->timesort
        && !f->type && !f->sizesort && !f->color)
        *fts_options |= MX_NOSTAT;
    if (!f->longform && !f->listdir && !f->type)
        *fts_options |= MX_COMFOLLOW;
    if (!f->recursive && *fts_options & MX_NOSTAT)
        *fts_options |= MX_NAMEONLY;
    if (!f->recursive)
        *fts_options |= MX_NORECUR;
    if (*fts_options & MX_COMFOLLOW)
        f->comfollow = 1;
    if (*fts_options & MX_LOGICAL)
        f->lfollow = 1;
    if (f->longform || f->size) {
        if (f->kblocks)
            f->blocksize = 2;
        else
            f->blocksize = 1;
    }
}

t_fts *mx_traverse_display_print(int argc, char *argv[], int options,
                               t_flags *f) {
    t_fts *ftsp;

    if (argc)
        ftsp = mx_traverse(argc, argv, options, f);
    else
        ftsp = mx_traverse(1, f->dotav, options, f);
    if (ftsp->enoent)
        f->rval = 1;
    // system("leaks -q uls");
    exit(f->rval);
}

void mx_parse_terminal(t_flags *f) {
    struct winsize win;

    if (isatty(STDOUT_FILENO)) {
        if (ioctl(STDOUT_FILENO, MX_TIOCGWINSZ, &win) != -1 && win.ws_col > 0)
            f->termwidth = win.ws_col;
        f->nonprint = 1;
    }
    else
        f->singlecol = 1;
}

int main(int argc, char *argv[]) {
    t_flags f = mx_init_flags();
    int fts_options = MX_PHYSICAL;
    struct s_opt opt = {.optind = 1};

    mx_parse_terminal(&f);
     while ((opt.c = mx_getopt(argc, argv, MX_FL_USAGE, &opt.optind)) != -1)
        if (opt.c != '?')
            mx_parse_flag(opt.c, &f, &fts_options);
        else
            mx_usage();
    argc -= opt.optind;
    argv += opt.optind;
    mx_parsecolors(MX_DEF_COLORS, &f);
    mx_process_flags_after_parsing(&f, &fts_options);
    f.options = fts_options;
    mx_traverse_display_print(argc, argv, fts_options, &f);
}
