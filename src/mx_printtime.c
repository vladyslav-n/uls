#include "../inc/uls.h"

static void write_day_mon(short format, char *tstr, char **p) {
    if (format & MX_TDAYFIRST) {
        mx_strncpy(*p, tstr + DAY, 2);
        mx_strncpy((*p += 3), tstr + MON, 3);
        *p += 4;
    }
    else {
        mx_strncpy(*p, tstr + MON, 3);
        mx_strncpy((*p += 4), tstr + DAY, 2);
        *p += 3;
    }
}

static void mx_print_format_time(time_t ftime, short format) {
    char *tstr = ctime(&ftime);
    char buf[] = "                     ";
    char *p = buf;

    write_day_mon(format, tstr, &p);
    if (format & MX_TNOSEC) {
        mx_strncpy(p, tstr + HOUR, 5);
        p[6] = '\0';
    }
    else if (format & MX_TSEC) {
        mx_strncpy(p, tstr + HOUR, 13);
        p[14] = '\0';
    }
    else {
        mx_strncpy(++p, tstr + YEAR, 4);
        p[5] = '\0';
    }
    mx_printstr(buf);
}

void mx_printtime(time_t ftime, t_flags *f) {
    short format = 0;
    static time_t now;

    if (now == 0)
        now = time(NULL);

    if (f->sectime)
        format |= MX_TSEC;
    else if (ftime + MX_SIXMONTHS > now && ftime < now + MX_SIXMONTHS)
        format |= MX_TNOSEC;
    mx_print_format_time(ftime, format);
}

