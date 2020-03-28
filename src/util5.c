#include "../inc/uls.h"

void mx_usage(void) {
	mx_printerror("usage: uls [-" MX_FL_USAGE "] [file ...]\n");
	exit(1);
}

short mx_round(int n) {
	return n % 10 >= 5 ? ((n / 10) % 10) + 1 : (n / 10) % 10;
}

void mx_printerr_errnum(char *s, int errnum) {
    mx_printerror("uls: ");
    mx_printerror(s);
    mx_printerror(": ");
    if (errnum < 0)
        return;
    mx_printerror(strerror(errnum));
    mx_printerror("\n");
}

void *mx_printbuf(void *buf, char *s) {
    mx_strcpy(buf, s);
    if (malloc_size(s))
        free(s);
    return buf;
}

void mx_print_spaces(int count) {
    if (count > 0)
        while (count--)
            mx_printchar(' ');
    else if (count < 0) {
        mx_printerror("uls: mx_print_spaces: negative count\n");
        exit(2);
    }
}
