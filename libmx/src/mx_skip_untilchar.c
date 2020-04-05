#include "../inc/libmx.h"

void mx_skip_untilchar(const char *str, char c, int *i) {
    while (str[*i] != c && str[*i]) {
        (*i)++;
    }
}
