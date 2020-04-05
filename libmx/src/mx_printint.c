#include "../inc/libmx.h"

static void print_pos(int n) {
    if (n > 9) {
        print_pos(n / 10);
    }
    mx_printchar (n % 10 + '0');
}

void mx_printint(int n) {
    if (n < 0) {
        if (n == -2147483648) {
            write(1, "-2147483648", 11);
        }
        else {
            n = -n;
            mx_printchar('-');
            print_pos(n);
        }
    }
    else {
        print_pos(n);
    }
}
