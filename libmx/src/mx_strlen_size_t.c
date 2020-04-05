#include "../inc/libmx.h"

size_t mx_strlen_size_t(const char *s) {
    size_t i = 0;

    while (s[i])
        i++;
    return i;
}
