#include "../inc/uls.h"

void mx_paste_node(t_list **list, void *data) {
    t_list *save;

    if (!list || !*list || !(save = (*list)->next)) {
        mx_push_back(list, data);
        return;
    }
    mx_push_front(&save, data);
    (*list)->next = save;
}

bool mx_is_dot(char *name) {
    return !mx_strcmp(name, ".") || !mx_strcmp(name, "..");
}

int mx_fts_chlist_size(t_file *list) {
    int count = 0;

    if (!list) {
        return count;
    }
    while (list->link) {
        list = list->link;
        count += 1;
    }
    return count + 1;
}
