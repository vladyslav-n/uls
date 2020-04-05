#include "../inc/libmx.h"

void mx_pop_front(t_list **head) {
    t_list *tmp = NULL;

    if (!head)
        return;
    if (!*head) {
        return;
    }
    if (!(*head)->next) {
        free(*head);
        *head = NULL;
        return;
    }
    tmp = *head;
    *head = (*head)->next;
    free(tmp);
    tmp = NULL;
}
