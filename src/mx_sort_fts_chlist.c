#include "../inc/uls.h"

static void swap_nodes(t_file **n1, t_file **n2) {
    t_file *tmp;

    tmp = *n1;
    *n1 = *n2;
    *n2 = tmp;
}

static void qsort_fts_chlist(t_file **list, bool (*cmp)(void *, void *),
int left, int right) {
    int i = left;
    int j = right;
    void *pivot = list[(left + right) / 2];

    while (i <= j) {
        while ((*cmp)(pivot, list[i]))
            i++;
        while ((*cmp)(list[j], pivot))
            j--;
        if (i <= j) {
            swap_nodes(&list[i], &list[j]);
            i++;
            j--;
        }
    }
    if (left < j)
        qsort_fts_chlist(list, cmp, left, j);
    if (i < right)
        qsort_fts_chlist(list, cmp, i, right);
}

t_file *mx_sort_fts_chlist(t_file *lst, bool (*cmp)(void *, void *)) {
    int size = -1;
    t_file **arr = NULL;
    t_file *save = NULL;

    if (!lst || !cmp)
        return lst;
    size = mx_fts_chlist_size(lst);
    arr = (t_file**) malloc(sizeof(t_file*) * size);
    for (int i = 0; i < size; i++) {
        arr[i] = lst;
        lst = lst->link;
    }
    qsort_fts_chlist(arr, cmp, 0, size - 1);
    for (int i = 0; i < size - 1; i++)
        arr[i]->link = arr[i + 1];
    arr[size - 1]->link = NULL;
    save = arr[0];
    free(arr);
    arr = NULL;
    return save;
}
