#include "../inc/uls.h"

int mx_prn_printable(const char *s) {
	char c;
	int n;

	for (n = 0; (c = *s) != '\0'; s++) {
		if (mx_isprint((unsigned char)c))
			mx_printchar(c);
		else
			mx_printchar('?');
		n++;
	}
	return n;
}

/*
 * The fts system makes it difficult to replace fts_name with a 
 * different-sized string, so we just calculate the real length here 
 * and do the conversion in mx_prn_octal()
 *
 * XXX when using f.octal_escape (-b) rather than f.octal (-B), the
 * length computed by len_octal may be too big. I just can't be buggered
 * to fix this as an efficient fix would involve a lookup table. Same goes
 * for the rather inelegant code in prn_octal.
 *
 */
size_t mx_len_octal(const char *s, int len) {
	size_t r = 0;

	while (len--)
		if (mx_isprint((unsigned const char)*s++))
			r++;
		else
			r += 4;
	return r;
}

static void prn_bitwise(unsigned char ch, int *len) {
	mx_printchar('0' + (ch >> 6));
	mx_printchar('0' + ((ch >> 3) & 7));
	mx_printchar('0' + (ch & 7));
	(*len) += 2;
}

static void prn_escaped(unsigned char ch, int *len) {
	if (ch == '\\')
		mx_printchar('\\');
	else if (ch == '\"')
		mx_printchar('"');
	else if (ch == '\a')
		mx_printchar('a');
	else if (ch == '\b')
		mx_printchar('b');
	else if (ch == '\f')
		mx_printchar('f');
	else if (ch == '\n')
		mx_printchar('n');
	else if (ch == '\r')
		mx_printchar('r');
	else if (ch == '\t')
		mx_printchar('t');
	else if (ch == '\v')
		mx_printchar('v');
	else
		prn_bitwise(ch, len);
}

int mx_prn_octal(const char *s, t_flags *f) {
	int len = 0;
	
    for (u_char ch; (ch = (u_char)*s++); len++) {
		if (mx_isprint(ch) && (ch != '\"') && (ch != '\\'))
			mx_printchar(ch);
		else {
			mx_printchar('\\');
			if (f->octal_escape)
				prn_escaped(ch, &len);
			else
				prn_bitwise(ch, &len);
			len++;
		}
	}
	return len;
}
