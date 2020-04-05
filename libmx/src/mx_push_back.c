#include "../inc/libmx.h"

void mx_push_back(t_list **list, void *data) {
    t_list *p_node = NULL;

    if (!list)
        return;
    p_node = mx_create_node(data);
    if (!p_node)
        return;
    p_node->next = NULL;
    if (!*list) {
        *list = p_node;
        return;
    }
    while ((*list)->next) {
        list = &((*list)->next);
    }
    (*list)->next = p_node;
}
