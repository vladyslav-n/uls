#include "../inc/libmx.h"

int mx_get_char_index(const char *str, char c) {
    int len = -1;

    if (!str)
        return -2;
    len = mx_strlen(str);
    for (int i = 0; i < len; i++)
        if (str[i] == c)
            return i;
    return -1;
}
