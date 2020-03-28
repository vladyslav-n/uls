#include "../inc/libmx.h"

static char *cycle(const char *str, t_swap_strings strings,
char *s, int sub_count) {
    int rep_len = mx_strlen(strings.replace);
    int sub_len = mx_strlen(strings.str);
    int sub_index = 0;

    for (int i = 0; i < sub_count; i++) {
        sub_index = mx_get_substr_index(str, strings.str);
        mx_strncpy(s, str, sub_index);
        mx_strcat(s, strings.replace);
        str += sub_index + sub_len;
        s += sub_index + rep_len;
    }
    return s;
}

char *mx_replace_substr(const char *str, const char *sub,
const char *replace) {
    char *s = NULL;
    int sub_count = 0;
    char *save = NULL;
    t_swap_strings strings = {(char *)sub, (char *)replace};

    if (!str || !sub || !replace)
        return NULL;
    sub_count = mx_count_substr(str, sub);
    if (!sub_count || !*sub || !*replace)
        return mx_strdup(str);
    save = mx_strnew(mx_strlen(str)
    + (mx_strlen(replace) - mx_strlen(sub)) * sub_count);
    s = cycle(str, strings, save, sub_count);
    if (*str)
        mx_strcpy(s, str);
    return save;
}

