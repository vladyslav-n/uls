#include "../inc/libmx.h"

static void print_pos(long long n) {
    if (n > 9)
        print_pos(n / 10);
    mx_printchar (n % 10 + '0');
}

void mx_printnum(long long n) {
    if (n < 0) {
        n = -n;
        mx_printchar('-');
        print_pos(n);
    }
    else
        print_pos(n);
}
