#include "../inc/libmx.h"

static void swap(char **s1, char **s2) {
    char *tmp;

    tmp = *s1;
    *s1 = *s2;
    *s2 = tmp;
}

static void cycle(int *i, int *j, char **arr, int *count) {
    int pivot = mx_strlen(arr[(*i + *j) / 2]);

    while (*i <= *j) {
        while (mx_strlen(arr[*i]) < pivot)
            (*i)++;
        while (mx_strlen(arr[*j]) > pivot)
            (*j)--;
        if (*i <= *j) {
            swap(&arr[*i], &arr[*j]);
            *count += 1;
            (*i)++;
            (*j)--;
        }
    }
}

int mx_quicksort(char **arr, int left, int right) {
    int count = 0;
    int i = left;
    int j = right;

    if (!arr)
        return -1;
    cycle(&i, &j, arr, &count);
    if (left < j)
        count += mx_quicksort(arr, left, j);
    if (i < right)
        count += mx_quicksort(arr, i, right);
    return count;
}
