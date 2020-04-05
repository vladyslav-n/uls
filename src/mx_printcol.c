#include "../inc/uls.h"


static void print_column(struct s_display *dp, t_flags *f, t_col_param *c,
                         t_file **array) {
    c->chcnt = 0;
    for (c->col = 0; c->col < c->numcols; c->col++) {
        c->chcnt += mx_printname(array[c->base], dp->s_inode,
            dp->s_block, f);
        if (f->sortacross)
            c->base++;
        else
            c->base += c->numrows;
        if (c->base >= c->num)
            break;
        while ((c->cnt = ((c->chcnt + c->tabwidth) & ~(c->tabwidth - 1)))
            <= c->endcol) {
            if (f->sortacross && c->col + 1 >= c->numcols)
                break;
            mx_printchar(f->notabs ? ' ' : '\t');
            c->chcnt = c->cnt;
        }
        c->endcol += c->colwidth;
    }
}

static void print(struct s_display *dp, t_flags *f, t_col_param *c,
                            t_file **array) {
    c->numcols = f->termwidth / c->colwidth;
    c->numrows = c->num / c->numcols;
    if (c->num % c->numcols)
        c->numrows++;
    if (dp->list->level != MX_ROOTLEV && (f->longform || f->size)){
        mx_printstr("total ");
        mx_print_align(mx_printbuf(c->buf, mx_llitoa(howmany(
                       dp->btotal, f->blocksize))), 0, 1);
        mx_printchar('\n');
    }
    for (; c->row < c->numrows; c->row++) {
        c->endcol = c->colwidth;
        if (!f->sortacross)
            c->base = c->row;
        print_column(dp, f, c, array);
        mx_printchar('\n');
    }
}

static void init_array_and_count(struct s_display *dp, t_flags *f,
                                 t_col_param *c, t_file ***array) {
    if (dp->entries > *c->lastentries) {
        *c->lastentries = dp->entries;
        if ((*array =
            mx_realloc(*array, dp->entries * sizeof(t_file*))) == NULL) {
            mx_printerror("uls: mx_realloc failed");
            mx_printscol(dp, f);
        }
    }
    for (t_file *p = dp->list; p; p = p->link)
        if (p->number != MX_NO_PRINT)
            (*array)[c->num++] = p;
    if (f->inode)
        c->colwidth += dp->s_inode + 1;
    if (f->size)
        c->colwidth += dp->s_block + 1;
    if (f->type)
        c->colwidth += 1;
    c->colwidth = (c->colwidth + c->tabwidth) & ~(c->tabwidth - 1);
}

void mx_printcol(struct s_display *dp, t_flags *f) {
    char buf[MX_STRBUF_SIZEOF(u_int64_t)];
    static t_file **array;
    static int lastentries = -1;
    t_col_param c = {.tabwidth = f->notabs ? 1 : 8, .colwidth = dp->maxlen,
                      .buf = buf, .lastentries = &lastentries};

    init_array_and_count(dp, f, &c, &array);
    if (f->termwidth < 2 * c.colwidth) {
        mx_printscol(dp, f);
        return;
    }
    print(dp, f, &c, array);
}
