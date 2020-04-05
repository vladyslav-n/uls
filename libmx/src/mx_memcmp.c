#include "../inc/libmx.h"

int mx_memcmp(const void *s1, const void *s2, size_t n) {
    unsigned char *str1 = (unsigned char *)s1;
    unsigned char *str2 = (unsigned char *)s2;

    while (str1 < (unsigned char *)s1 + n) {
        if (*str1++ != *str2++)
            return *--str1 - *--str2;
    }
    return 0;
}
