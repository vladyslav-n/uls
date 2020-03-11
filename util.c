#include <ctype.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "ls.h"

int
prn_printable(const char *s)
{
	char c;
	int n;

	for (n = 0; (c = *s) != '\0'; ++s, ++n)
		if (isprint((unsigned char)c))
			putchar(c);
		else
			putchar('?');
	return n;
}

/*
 * The fts system makes it difficult to replace fts_name with a different-
 * sized string, so we just calculate the real length here and do the
 * conversion in prn_octal()
 *
 * XXX when using f.octal_escape (-b) rather than f.octal (-B), the
 * length computed by len_octal may be too big. I just can't be buggered
 * to fix this as an efficient fix would involve a lookup table. Same goes
 * for the rather inelegant code in prn_octal.
 *
 *                                              DES 1998/04/23
 */

size_t
len_octal(const char *s, int len)
{
	size_t r = 0;

	while (len--)
		if (isprint((unsigned const char)*s++)) r++; else r += 4;
	return r;
}

int
prn_octal(const char *s)
{
        unsigned char ch;
	int len = 0;
	
        while ((ch = (unsigned char)*s++)) {
	        if (isprint(ch) && (ch != '\"') && (ch != '\\'))
		        putchar(ch), len++;
	        else if (f.octal_escape) {
	                putchar('\\');
		        switch (ch) {
			case '\\':
			        putchar('\\');
				break;
			case '\"':
			        putchar('"');
				break;
			case '\a':
			        putchar('a');
				break;
			case '\b':
			        putchar('b');
				break;
			case '\f':
			        putchar('f');
				break;
			case '\n':
			        putchar('n');
				break;
			case '\r':
			        putchar('r');
				break;
			case '\t':
			        putchar('t');
				break;
			case '\v':
			        putchar('v');
				break;
 		        default:
		                putchar('0' + (ch >> 6));
		                putchar('0' + ((ch >> 3) & 7));
		                putchar('0' + (ch & 7));
		                len += 2;
			        break;
		        }
		        len += 2;
	        }
		else {
			putchar('\\');
	                putchar('0' + (ch >> 6));
	                putchar('0' + ((ch >> 3) & 7));
	                putchar('0' + (ch & 7));
	                len += 4;
		}
	}
	return len;
}

void
usage(void)
{
	(void)fprintf(stderr,
#ifdef COLORLS
	"usage: uls [-ABCFGHLPRSTWZabcdfghiklnoqrstuvx1]"
#else
	"usage: uls [-ABCFHLPRSTWZabcdfghiklnoqrstuvx1]"
#endif
		      " [file ...]\n");
	exit(1);
}
