#include "../inc/libmx.h"

int mx_count_words(const char *str, char c) {
    int i = 0;
    int count = 0;

    if (!str)
        return -1;
    if (!str[0])
        return 0;
    if (!c)
        return 1;
    mx_skipchar(str, c, &i);
    while (str[i]) {
        mx_skip_untilchar(str, c, &i);
        mx_skipchar(str, c, &i);
        count += 1;
    }
    return count;
}
