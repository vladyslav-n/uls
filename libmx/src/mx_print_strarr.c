#include "../inc/libmx.h"

void mx_print_strarr(char **arr, const char *delim) {
    int i = 0;

    if (!arr || !delim)
        return;
    if (*arr) {
        while (arr[i + 1]) {
            mx_printstr(arr[i]);
            mx_printstr(delim);
            i++;
        }
        mx_printstr(arr[i]);
    }
    mx_printchar('\n');
    return;
}
