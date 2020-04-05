#include "../inc/libmx.h"

static void skip_spaces(const char *str, int *i) {
    while (mx_isspace(str[*i])) {
        (*i)++;
    }
}

static void skip_spaces_ncount(const char *str, int *i, int *count) {
    for (; mx_isspace(str[*i]); (*i)++)
        *count += 1;
}

static void skip_untilspace(const char *str, int *i) {
    while (!mx_isspace(str[*i]) && str[*i]) {
        (*i)++;
    }
}

static char *create_fin_s(int fin_size, const char *s) {
    char *fin = mx_strnew(fin_size);
    int i = 0;
    int j = 0;

    for (j = 0; j < fin_size; j++) {
        fin[j] = s[i];
        if (mx_isspace(s[i])) {
            fin[j] = ' ';
            skip_spaces(s, &i);
            continue;
        }
        i++;
    }
    return fin;
}

char *mx_del_extra_spaces(const char *str) {
    char *s = mx_strtrim(str);
    int i = 0;
    int sp_count = 0;
    int sp_total = 0;
    char *fin = NULL;

    if (!s)
        return NULL;
    while (1) {
        skip_untilspace(s, &i);
        if (!s[i])
            break;
        sp_count += 1;
        skip_spaces_ncount(s, &i, &sp_total);
    }
    fin = create_fin_s(i - sp_total + sp_count, s);
    free(s);
    return fin;
}
