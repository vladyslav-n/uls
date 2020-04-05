#include "../inc/libmx.h"

void mx_skipchar(const char *str, char c, int *i) {
    while (str[*i] == c) {
        (*i)++;
    }
}
