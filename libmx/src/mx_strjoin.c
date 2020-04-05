#include "../inc/libmx.h"

char *mx_strjoin(const char *s1, const char *s2) {
    char *str = NULL;

    if (!s1 && !s2)
        return NULL;
    if (!s1 && s2)
        return mx_strdup(s2);
    if (!s2 && s1)
        return mx_strdup(s1);
    str = mx_strnew(mx_strlen(s1) + mx_strlen(s2));
    str = mx_strcpy(str, s1);
    str = mx_strcat(str, s2);
    return str;
}
