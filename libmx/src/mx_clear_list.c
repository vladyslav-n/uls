#include "../inc/libmx.h"

void mx_clear_list(t_list **head) {
    t_list *tmp = NULL;

    if (!head || !*head)
        return;
    while ((*head)->next) {
        tmp = *head;
        *head = (*head)->next;
        free(tmp->data);
        free(tmp);
    }
    free((*head)->data);
    free(*head);
    *head = NULL;
}
