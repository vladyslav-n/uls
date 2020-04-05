#include "../inc/libmx.h"

char *mx_strdup(const char *str)  {
    char *str1 = mx_strnew(mx_strlen(str));

    return mx_strcpy(str1, str);
}
