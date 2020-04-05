#include "../inc/libmx.h"

char *mx_strstr(const char *haystack, const char *needle) {
    if (!needle[0])
        return (char *)haystack;
    for (int i = 0; haystack[i]; i++)
        if (mx_is_substr(haystack + i, needle))
            return (char *)haystack + i;
    return NULL;
}
