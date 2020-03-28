

#include "../inc/uls.h"

static char optchar(char *optstring, bool *wordbeg, int *optind, char **c) {
	if (*wordbeg) {
		if (**c != '-')
			return -1;
		else if (!*(++(*c)))  // "-" word
			return -1;
		else if (**c == '-' && !*(*c + 1)) {  // "--" word
			(*optind)++;
			return -1;
		}
		else
			*wordbeg = 0;
	}
	if (mx_get_char_index(optstring, **c) == -1) {
		mx_printerror("uls: illegal option -- ");
		write(2, *c, 1);
		mx_printerror("\n");
		return '?';
	}
	return **c;
}

char mx_getopt(int argc, char **argv, char *optstring, int *optind) {
	static bool wordbeg = 1;
	static char *c = NULL;

	if (c && !*(++c)) {
		(*optind)++;
		wordbeg = 1;
	}
	if (*optind == argc)
		return -1;
	if (!c || !*c)
		c = argv[*optind];
	return optchar(optstring, &wordbeg, optind, &c);
}
