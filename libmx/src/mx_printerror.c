#include "../inc/libmx.h"

void mx_printerror(const char *s) {
    write(2, s, mx_strlen(s));
}
