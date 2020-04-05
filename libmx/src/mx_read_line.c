#include "../inc/libmx.h"

static void read_less_bufsize(t_tmp *tmp, t_read_bytes *bytes) {
    bytes->buffer = mx_strndup(bytes->buffer, bytes->read);
    free(tmp->buf);
    tmp->buf = NULL;
    bytes->buf_sz = bytes->read;
    tmp->flag = -2;
}

static void at_eof(t_tmp *tmp, t_str_len *tail,
t_delim *del, t_read_bytes *bytes) {
    while (tail->s[0] && (del->ind = mx_get_char_index(tail->s,
        del->c)) >= 0) {
        mx_push_back(&(tmp->strlist), mx_strndup(tail->s, del->ind));
        tmp->str_sum_len += del->ind;
        bytes->res += del->ind;
        tail->s = mx_strdup(tail->s + del->ind + 1);
        tail->len -= del->ind + 1;
        free(tmp->s);
        tmp->s = tail->s;
    }
    mx_push_back(&(tmp->strlist), mx_strdup(tail->s));
    tmp->str_sum_len += tail->len;
    bytes->res += tail->len;
    tail->s = mx_strnew(0);
    tail->len = 0;
    free(tmp->s);
    tmp->s = tail->s;
}

static void cycle(t_tmp *tmp, t_str_len *tail,
t_delim *del, t_read_bytes *bytes) {
    while (bytes->read > 0) {
        if (bytes->read == bytes->buf_sz) {
            mx_bufsize_chars_read(tmp, tail, del, bytes);
            if (tmp->flag == 3) {
                bytes->read = read(tmp->fd, bytes->buffer, bytes->buf_sz);
                continue;
            }
            return;
        }
        else {
            read_less_bufsize(tmp, bytes);
        }
    }
    if (!bytes->res) {
        if (!tail->s[0])
            bytes->res = -1;
        else
            at_eof(tmp, tail, del, bytes);
    }
}

static void clear(char **buffer, t_list **str_list){
    free(*buffer);
    *buffer = NULL;
    mx_clear_list(str_list);
}

int mx_read_line(char **lineptr, size_t buf_size, char delim, const int fd) {
    static t_str_len tail;
    t_read_bytes bytes = {0, 0, buf_size, mx_strnew(buf_size)};
    t_tmp tmp = {bytes.buffer, NULL, -1, lineptr, fd,
    mx_create_node(mx_strnew(0)), 0};
    t_delim del = {delim, -3};

    if (!tail.s)
        tail.s = mx_strnew(bytes.buf_sz);
    tmp.s = tail.s;
    bytes.read = read(fd, bytes.buffer, bytes.buf_sz);
    /* error case */
    if (bytes.read < 0) {
        clear(&(bytes.buffer), &(tmp.strlist));
        return -2;
    }
    cycle(&tmp, &tail, &del, &bytes);
    mx_write_str_list(tmp.strlist, tmp.str_sum_len, lineptr);
    clear(&(bytes.buffer), &(tmp.strlist));
    return bytes.res;
}
