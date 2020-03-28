#include "../inc/uls.h"

bool mx_islink_stat(char *path) {
    struct stat s_stp;

    lstat(path, &s_stp);
    return MX_S_ISLNK(s_stp.st_mode);
}

short mx_type_stat(char *path) {
    struct stat s_stp;

    lstat(path, &s_stp);
    if MX_S_ISLNK(s_stp.st_mode)
        return DT_LNK;
    if MX_S_ISDIR(s_stp.st_mode)
        return DT_DIR;
    return DT_REG;
}

bool mx_islink_dirent(struct dirent *file) {
    return (file->d_type == DT_LNK);
}

bool mx_file_exist(char *name) {
    struct stat sst;

    return !lstat(name, &sst);
}

int mx_link_info(char *name) {
    struct stat sst;

    if (stat(name, &sst) == -1)
        return MX_SLNONE;
    if (MX_S_ISDIR(sst.st_mode))
        return MX_D;
    return MX_DEFAULT;
}
