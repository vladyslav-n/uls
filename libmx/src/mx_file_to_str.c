#include "../inc/libmx.h"

static int check_fd(const char *file) {
    int fd = open(file, O_RDONLY | O_DIRECTORY);

    if (fd < 0) {
        fd = open(file, O_RDONLY);
        if (fd > 0) {
            return fd;
        }
    }
    close(fd);
    return -1;
}

char *mx_file_to_str(const char *file) {
    int fd = check_fd(file);
    char buf[1024] = {0};
    int total = 0;
    char *s = NULL;
    int bytes = 0;

    if (fd < 0) {
        return NULL;
    }
    while ((bytes = read(fd, buf, 1024)))
        total += bytes;
    close(fd);
    s = mx_strnew(total);
    fd = open(file, O_RDONLY);
    for (total = 0; (bytes = read(fd, s + total, 1024));)
        total += bytes;
    close(fd);
    if (*s)
        return s;
    return NULL;
}
