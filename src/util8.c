#include "../inc/uls.h"

void mx_print_typechar(int mode) {
    int type = mode & S_IFMT;

    if (type == S_IFREG)
        mx_printchar('-');
    else if (type == S_IFDIR)
        mx_printchar('d');
    else if (type == S_IFBLK)
        mx_printchar('b');
    else if (type == S_IFCHR)
        mx_printchar('c');
    else if (type == S_IFLNK)
        mx_printchar('l');
    else if (type == S_IFIFO)
        mx_printchar('p');
    else if (type == S_IFSOCK)
        mx_printchar('s');
    else
        mx_printchar('w');
}

void mx_print_9modes(int m) {
    char modes369[3];

    mx_printchar(S_IRUSR & m ? 'r' : '-');
    mx_printchar(S_IWUSR & m ? 'w' : '-');
    modes369[0] = m & S_IXUSR ? 'x' : '-';
    if (m & S_ISUID)
        modes369[0] = modes369[0] == 'x' ? 's' : 'S';
    mx_printchar(modes369[0]);
    mx_printchar(S_IRGRP & m ? 'r' : '-');
    mx_printchar(S_IWGRP & m ? 'w' : '-');
    modes369[1] = m & S_IXGRP ? 'x' : '-';
    if (m & S_ISGID)
        modes369[1] = modes369[1] == 'x' ? 's' : 'S';
    mx_printchar(modes369[1]);
    mx_printchar(S_IROTH & m ? 'r' : '-');
    mx_printchar(S_IWOTH & m ? 'w' : '-');
    modes369[2] = m & S_IXOTH ? 'x' : '-';
    if (m & S_ISVTX)
        modes369[2] = modes369[2] == 'x' ? 't' : 'T';
    mx_printchar(modes369[2]);
}

bool mx_has_acl(char *name) {
    acl_t acl = acl_get_file(name, ACL_TYPE_EXTENDED);
    bool has_acl = acl;

    acl_free(acl);
    return has_acl;
}

void mx_print_at_or_plus(t_flags *f, t_file *file) {
    if (mx_xattr_size(f, file) > 0)
        mx_printstr("@ ");
    else if (mx_has_acl(file->accpath))
        mx_printstr("+ ");
    else
        mx_print_spaces(2);
}

void mx_print_mode(t_flags *f, t_file *file) {
    mx_print_typechar(file->statp->st_mode);
    mx_print_9modes(file->statp->st_mode);
    mx_print_at_or_plus(f, file);
}
