#include "../inc/libmx.h"

static void skip_spaces(const char *str, int *i) {
    while (mx_isspace(str[*i])) {
        (*i)++;
    }
}

static void skip_untilspace(const char *str, int *i) {
    while (!mx_isspace(str[*i]) && str[*i]) {
        (*i)++;
    }
}

char *mx_strtrim(const char *str) {
    int len = 0;
    int min_index = 0;;
    int nul_index = 0;

    if (!str)
        return NULL;
    skip_spaces(str, &len);
    min_index = len;
    while (str[len]) {
        skip_untilspace(str, &len);
        nul_index = len;
        skip_spaces(str, &len);
    }
    return mx_strndup(str + min_index, nul_index - min_index);
}
