#include "../inc/libmx.h"

void mx_push_front(t_list **list, void *data) {
    t_list *node = NULL;

    if (!list)
        return;
    node = mx_create_node(data);
    if (!node)
        return;
    node->next = *list;
    *list = node;
}
