#include "../inc/libmx.h"

static unsigned long hexstrsize(unsigned long n) {
    unsigned long size = 1;
    unsigned long nbr = n;

    while (nbr >>= 4)
        size += 1;
    return size;
}

static char digit_to_hexchar(short int d) {
    if (d >= 0 && d <= 9)
        return d + 48;
    if (10 == d)
        return 'a';
    if (11 == d)
        return 'b';
    if (12 == d)
        return 'c';
    if (13 == d)
        return 'd';
    if (14 == d)
        return 'e';
    if (15 == d)
        return 'f';
    return '!';
}

char *mx_nbr_to_hex(unsigned long nbr) {
    unsigned long n = nbr;
    unsigned long size = hexstrsize(n);
    char *hex = (char*) malloc(sizeof(char) * (size + 1));

    hex[size] = 0;
    for (unsigned long i = 0; i < size; i++) {
        hex[size - 1 - i] = digit_to_hexchar(n % 16);
        n >>= 4;
    }
    return hex;
}
