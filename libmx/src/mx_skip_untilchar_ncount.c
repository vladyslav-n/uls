#include "../inc/libmx.h"

int mx_skip_untilchar_ncount(const char *str, char c, int *i) {
    int count = 0;

    for (; str[*i] != c && str[*i]; (*i)++)
        count++;
    return count;
}
