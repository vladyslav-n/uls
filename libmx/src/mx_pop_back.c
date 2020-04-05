#include "../inc/libmx.h"

void mx_pop_back(t_list **head) {
    t_list *last = NULL;

    if (!head)
        return;
    if (!*head)
        return;
    if (!(*head)->next) {
        free(*head);
        *head = NULL;
        return;
    }
    last = *head;
    while ((last->next)->next) {
        last = last->next;
    }
    free(last->next);
    last->next = NULL;
}
