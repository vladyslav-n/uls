#include "../inc/uls.h"

void mx_push_ftsent(t_ftslist **list, t_file *ptr) {
    if (!list || !ptr)
        return;
    mx_push_back((t_list**)list, ptr);
    if((*list)->next)
        *list = (*list)->next;
}

short mx_fts_push_name(t_ftslist **list, char *name, int options,
                       t_list **errlist) {
    if (!list)
        return -2;
    if (!mx_file_exist(name)) {
        mx_push_front(errlist, name);
        return -1;
    }
    mx_push_ftsent(list, mx_root_file(name, options));
    return 0;
}

bool mx_print_errlist_names(t_list *errlist, bool sort) {
    bool enoent = errlist;

    errlist = mx_sort_list(errlist, sort ? mx_str_cmp : NULL);
    for (; errlist; mx_pop_front(&errlist))
        mx_printerr_errnum(errlist->data, ENOENT);
    return enoent;
}

t_ftslist *mx_push_roots(t_ftslist **list, char **argv, int *options,
                         bool (*cmp)(void *, void *)) {
    t_ftslist *cur = NULL;
    t_list *errlist = NULL;
    char *name;
    short flag = 0;

    if (!list)
        return NULL;
    for (name = *argv; (name = *argv); argv++)
        if (!(flag = mx_fts_push_name(&cur, name, *options, &errlist))
            && !*list)
            *list = cur;
    if (mx_print_errlist_names(errlist, cmp ? 1 : 0))
        *options |= MX_ENOENT;
    *list = (t_ftslist*)mx_sort_list((t_list*)*list, cmp);
    if (*options & MX_NORECUR)
        return *list;
    for (cur = *list; cur; cur = cur->next) {
        mx_fts_paste_chlist(cur, *options, cmp);
    }
    return *list;
}
