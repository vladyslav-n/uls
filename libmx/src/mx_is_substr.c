#include "../inc/libmx.h"

bool mx_is_substr(const char *haystack, const char *needle) {
    for (int i = 0; needle[i]; i++)
        if (needle[i] != haystack[i])
            return 0;
    return 1;
}
