#include "../inc/libmx.h"

int mx_bubble_sort(char **arr, int size) {
    int count = 1;
    char flag = 1;
    char *buf = 0;
    int swaps = 0;

    while (flag) {
        flag = 0;
        for (int i = 0; i < size - count; i++) {
            if (mx_strcmp(arr[i], arr[i + 1]) > 0) {
                buf = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = buf;
                flag = 1;
                swaps += 1;
            }
        }
        count += 1;
    }
    return swaps;
}
