#include "../inc/libmx.h"

int mx_list_size(t_list *list) {
    int count = 0;

    if (!list) {
        return count;
    }
    while (list->next) {
        list = list->next;
        count += 1;
    }
    return count + 1;
}
