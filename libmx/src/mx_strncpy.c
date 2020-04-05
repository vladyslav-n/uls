#include "../inc/libmx.h"

char *mx_strncpy(char *dst, const char *src, int len) {
    int i = 0;

    if (len <= mx_strlen(src) + 1) {
        for (; i < len; i++)
            dst[i] = src[i];
        return dst;
    }
    else {
        for (; src[i]; i++)
            dst[i] = src[i];
        for (; i < len; i++)
            dst[i] = '\0';
        return dst;
    }
}
