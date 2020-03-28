#include "../inc/uls.h"

void mx_size_with_point(char *buf, double num, short unit) {
	int n = num * 100.0;

	if (!unit) {
		mx_strncpy(buf, "  ", 2);
		buf[2] = n / 100 + '0';
		return;
	}
	if (num <= 1.0) {
		mx_strncpy(buf, num == 0.0 ? "  0" : "1.0", 3);
		return;
	}
	buf[2] = (char)mx_round(n) + '0';
	buf[0] = n / 100 + '0';
	if (buf[2] == ':') {
		buf[2] = '0';
		buf[0]++;
	}
	buf[1] = '.';
}

void mx_hundreds_size(char *buf, int n) {
	buf[2] = (char)mx_round(n) + '0';
	buf[1] = (n / 100) % 10 + '0';
	buf[0] = n / 1000 + '0';
	if (buf[2] == ':') {
		buf[2] = '0';
		buf[1]++;
	}
	if (buf[1] == ':') {
		buf[1] = '0';
		buf[0]++;
	}
}

void mx_pointless_size(char *buf, double num) {
	int n = num * 10.0;

	if (num < 99.5) {
		buf[0] = ' ';
		buf[2] = (char)mx_round(n) + '0';
		buf[1] = n / 100 + '0';
		if (buf[2] == ':') {
			buf[2] = '0';
			buf[1]++;
		}
		return;
	}
	if (num >= 99.5 && num < 100.0) {
		mx_strncpy(buf, "100", 3);
		return;
	}
	mx_hundreds_size(buf, n);
}

char *mx_format_size(char *buf, off_t size) {
	short unit;
	double num = size;

	for (unit = 0; num >= 999.5; num /= 1024.0)
		unit++;
	buf[3] = "BKMGTPE"[unit];
	buf[4] = '\0';
	if (num < 9.95)
		mx_size_with_point(buf, num, unit);
	else if (num < 10.0)
		mx_strncpy(buf, " 10", 3);
	else
		mx_pointless_size(buf, num);
	return buf;
}

void mx_printsize(size_t width, off_t bytes, t_flags *f) {
	char buf[5];
	char sizestr[MX_STRBUF_SIZEOF(off_t) + 1];

	if (f->humanval){
		mx_printchar(' ');
		mx_printstr(mx_format_size(buf, bytes));
	}
	else {
		mx_printbuf(sizestr, mx_llitoa(bytes));
		mx_print_align(sizestr, width, true);
	}
	mx_printchar(' ');
}
