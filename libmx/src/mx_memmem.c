#include "../inc/libmx.h"

static bool is_submem(const void *big, size_t big_len,
                      const void *little, size_t little_len) {
    unsigned char *needle = (unsigned char *)little;
    unsigned char *haystack = (unsigned char *)big;

    if (big_len < little_len)
        return 0;
    for (size_t i = 0; i < little_len; i++)
        if (needle[i] != haystack[i])
            return 0;
    return 1;
}

void *mx_memmem(const void *big, size_t big_len,
                const void *little, size_t little_len) {
    unsigned char *needle = (unsigned char *)little;
    unsigned char *haystack = (unsigned char *)big;
    size_t biglen_save = big_len;

    if (big_len < little_len || little_len == 0 || big_len == 0
        || !big || !little) {
        return NULL;
        }
    for (size_t i = 0; i < biglen_save; i++)
        if (is_submem(haystack++, big_len--, needle, little_len))
            return (void*)--haystack;
    return NULL;
}
