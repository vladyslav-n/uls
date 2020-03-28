#include "../inc/libmx.h"

bool mx_isprint(int c) {
    return c > 31 && c < 127;
}
