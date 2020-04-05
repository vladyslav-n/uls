#include "../inc/uls.h"

void mx_print_chars(char c, int count) {
    if (count > 0)
        while (count--)
            mx_printchar(c);
    else if (count < 0) {
        mx_printerror("uls: mx_print_chars: negative count\n");
        exit(2);
    }
}

int mx_print_align_c(char *s, char c, int len, bool right) {
    int slen = mx_strlen(s);

    if (len <= 0) {
        write(1, s, slen);
        return slen;
    }
    if (right)
        mx_print_chars(c, len - slen);
    mx_printstr(s);
    if (!right)
        mx_print_chars(c, len - slen);
    return len;
}

int mx_print_align(char *s, int len, bool right) {
    int slen = mx_strlen(s);

    if (len <= 0) {
        write(1, s, slen);
        return slen;
    }
    if (right)
        mx_print_spaces(len - slen);
    mx_printstr(s);
    if (!right)
        mx_print_spaces(len - slen);
    return len;
}

char *mx_usrname(uid_t st_uid, char *user, t_flags *f) {
    struct passwd *passwd_ptr = NULL;

    if (f->numericonly)
        return mx_printbuf(user, mx_itoa(st_uid));
    passwd_ptr = getpwuid(st_uid);
    if (passwd_ptr)
        return mx_printbuf(user, passwd_ptr->pw_name);
    return mx_printbuf(user, mx_itoa(st_uid));
}

char *mx_grpname(gid_t st_gid, char *group, t_flags *f) {
    struct group *group_ptr = NULL;

    if (f->numericonly)
        return mx_printbuf(group, mx_itoa(st_gid));
    group_ptr = getgrgid(st_gid);
    if (group_ptr)
        return mx_printbuf(group, group_ptr->gr_name);
    return mx_printbuf(group, mx_itoa(st_gid));
}
