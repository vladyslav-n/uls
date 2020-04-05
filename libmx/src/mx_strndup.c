#include "../inc/libmx.h"

static char *strnew_size_t(size_t size) {
    if (size >= 0) {
        char *str = (char*) malloc(size + 1);

        for (size_t i = 0; i <= size; i++)
            str[i] = '\0';
        return str;
    }
    return NULL;
}

static char *strcpy_size_t(char *dst, const char *src) {
    size_t i = 0;

    while(src[i]){
        dst[i] = src[i];
        i++;
    }
    dst[i] = src[i];
    return dst;
}

static char *strncpy_size_t(char *dst, const char *src, size_t len) {
    size_t i = 0;

    if (len <= mx_strlen_size_t(src) + 1) {
        for (; i < len; i++)
            dst[i] = src[i];
    }
    else {
        for (; src[i]; i++)
            dst[i] = src[i];
        for (; i < len; i++)
            dst[i] = '\0';
    }
    return dst;
}

static char *strdup_size_t(const char *str)  {
    char *str1 = strnew_size_t(mx_strlen_size_t(str));

    return strcpy_size_t(str1, str);
}

char *mx_strndup(const char *s1, size_t n) {
    char *dst = NULL;

    if (n >= mx_strlen_size_t(s1))
        return strdup_size_t(s1);
    dst = strnew_size_t(n);
    return strncpy_size_t(dst, s1, n);
}

