#include "../inc/uls.h"

t_file *mx_argv_chlist(char **argv, int options,
                          bool (*cmp)(void *, void *)) {
    t_file *last_child = NULL;
    t_file *ch_list_start = NULL;
    char *name;

    if (!argv || !*argv)
        return NULL;
    for (name = *argv; (name = *argv); argv++) {
        if (mx_file_exist(name))
            mx_fts_child_push(&last_child, mx_root_file(name, options),
                              &ch_list_start);
        if (last_child && last_child->link)
            last_child = last_child->link;
    }
    return mx_sort_fts_chlist(ch_list_start, cmp);;
}

t_fts *mx_fts_open(char **path_argv, int options,
                 bool (*cmp)(void *, void *)) {
    t_fts *ftsp = (t_fts *)mx_nalloc(sizeof(t_fts));

    mx_push_roots(&ftsp->head, path_argv, options, cmp);
    ftsp->cur = ftsp->head;
    ftsp->cmp = cmp;
    ftsp->fts_options = options;
    ftsp->argv = path_argv;
    return ftsp;
}

t_file *mx_fts_read(t_fts *ftsp) {
    if (!ftsp->cur)
        return NULL;
    if (ftsp->read_launched)
        ftsp->cur = ftsp->cur->next;
    ftsp->read_launched = 1;
    return ftsp->cur ? ftsp->cur->data : NULL;
}

t_file *mx_fts_children(t_fts *ftsp, int options) {
    if (ftsp->read_launched)
        return mx_fts_children_1(ftsp->cur, options, ftsp->cmp);
    return mx_argv_chlist(ftsp->argv, options, ftsp->cmp);
}

void mx_fts_skip_ch(t_fts *ftsp) {
    int level;
    t_ftslist *cur;

    if (!ftsp || !ftsp->cur || !ftsp->cur->data)
        return;
    cur = ftsp->cur;
    level = cur->data->level;
    for (; cur->next && cur->next->data->level > level;)
        cur = cur->next;
    ftsp->cur = cur;
}
