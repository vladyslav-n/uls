#include "../inc/uls.h"

void mx_fts_child_push(t_file **last_child, t_file *child,
t_file **ch_list_start) {
    if (!*last_child) {
        *last_child = child;
        *ch_list_start = child;
        return;
    }
    (*last_child)->link = child;
}

static bool return_null(t_ftslist *list, int options, DIR **dp) {
    char path_buf[MAXPATHLEN + 1] = "";

    if (!list || list->data->info != MX_D || (options & MX_SEEDOT
        && list->data->level != 0 && mx_is_dot(list->data->name))
        || mx_is_cycle_dir(list->data))
        return true;
    if (list->data->d_type != DT_LNK)
        *dp = opendir(list->data->accpath);
    else {
        mx_follow_link(list->data->accpath, path_buf, MAXPATHLEN + 1);
        *dp = opendir(path_buf);
    }
    if (!*dp) {
        mx_printerr_errnum(list->data->d_type != DT_LNK ? list->data->name
                                                        : path_buf, errno);
        return true;
    }
    return false;
}

t_file *mx_fts_children_1(t_ftslist *list, int options,
                          bool (*cmp)(void *, void *)) {
    DIR *dp;
    struct dirent *stdp;
    t_file *last_child = NULL;
    t_file *ch_list_start = NULL;

    if (return_null(list, options, &dp))
        return NULL;
    while ((stdp = readdir(dp))) {
        if (!(options & MX_SEEDOT) && mx_is_dot(stdp->d_name))
            continue;
        mx_fts_child_push(&last_child,
        mx_dirent_to_ftsent(list->data, stdp, options), &ch_list_start);
        if (last_child && last_child->link)
            last_child = last_child->link;
    }
    closedir(dp);
    return mx_sort_fts_chlist(ch_list_start, cmp);;
}

void mx_children_clear(t_file *p) {
    t_file *save = p;

    for (t_file *tmp = save; tmp; tmp = save->link) {
        save = tmp;
        free(tmp->statp);
        free(tmp);
    }
}

short mx_fts_paste_chlist(t_ftslist *parent, int options,
                          bool (*cmp)(void *, void *)) {
    t_file *ch_list = mx_fts_children_1(parent, options, cmp);

    if (!ch_list)
        return -1;
    for (t_file *p = ch_list; p; p = p->link) {
        mx_paste_node((t_list **)&parent, p);
        parent = parent->next;
    }
    return 0;
}
