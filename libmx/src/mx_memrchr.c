#include "../inc/libmx.h"

void *mx_memrchr(const void *s, int c, size_t n) {
    unsigned char *str = (unsigned char *)s + n - 1;

    for (size_t i = 0; i < n; i++)
        if (*str-- == c)
            return ++str;
    return NULL;
}
