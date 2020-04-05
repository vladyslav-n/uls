#include "../inc/libmx.h"

static void swap_nodes(t_list **n1, t_list **n2) {
    t_list *tmp;

    tmp = *n1;
    *n1 = *n2;
    *n2 = tmp;
}

static void quicksort_list(t_list **list, bool (*cmp)(void *, void *),
int left, int right) {
    int i = left;
    int j = right;
    void *pivot = list[(left + right) / 2]->data;

    while (i <= j) {
        while ((*cmp)(pivot, list[i]->data))
            i++;
        while ((*cmp)(list[j]->data, pivot))
            j--;
        if (i <= j) {
            swap_nodes(&list[i], &list[j]);
            i++;
            j--;
        }
    }
    if (left < j)
        quicksort_list(list, cmp, left, j);
    if (i < right)
        quicksort_list(list, cmp, i, right);
}

t_list *mx_sort_list(t_list *lst, bool (*cmp)(void *, void *)) {
    int size = -1;
    t_list **arr = NULL;
    t_list *save = NULL;

    if (!lst || !cmp)
        return lst;
    size = mx_list_size(lst);
    arr = (t_list**) malloc(sizeof(t_list*) * size);
    for (int i = 0; i < size; i++) {
        arr[i] = lst;
        lst = lst->next;
    }
    quicksort_list(arr, cmp, 0, size - 1);
    for (int i = 0; i < size - 1; i++)
        arr[i]->next = arr[i + 1];
    arr[size - 1]->next = NULL;
    save = arr[0];
    free(arr);
    arr = NULL;
    return save;
}
