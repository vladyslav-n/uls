#include "../inc/libmx.h"

static int ato_posi(const char *str1) {
    int num = 0;
    int i = 0;

    while (mx_isdigit(str1[i])) {
        num = num * 10 + str1[i] - 48;
        i++;
    }
    return num;
}

int mx_atoi(const char *str) {
    int x = 0;

    if (mx_isspace(str[x]))
        return mx_atoi(&str[x + 1]);
    if (str[x] == '-' && mx_isdigit(str[x+1]))
        return -ato_posi(&str[x + 1]);
    if (mx_isdigit(str[x]))
        return ato_posi(&str[x]);
    else
        return 0;
}
