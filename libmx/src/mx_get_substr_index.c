#include "../inc/libmx.h"

int mx_get_substr_index(const char *str, const char *sub) {
    if (!sub || !str)
        return -2;
    if (!sub[0])
        return 0;
    for (int i = 0; str[i]; i++)
        if (mx_is_substr(str + i, sub))
            return i;
    return -1;
}
