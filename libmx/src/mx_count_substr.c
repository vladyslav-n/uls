#include "../inc/libmx.h"

int mx_count_substr(const char *str, const char *sub) {
    int count = 0;
    int sublen = -1;

    if (!sub || !str)
        return -1;
    if (!sub[0])
        return 0;
    sublen = mx_strlen(sub);
    for (int i = 0; str[i]; i++)
        if (mx_is_substr(str + i, sub)) {
            count +=1;
            i += sublen - 1;
        }
    return count;
}
