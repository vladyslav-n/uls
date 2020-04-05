#include "../inc/libmx.h"

static void delim_in_tail(t_tmp *tmp, t_str_len *tail,
t_delim *del, t_read_bytes *bytes) {
    mx_push_back(&(tmp->strlist), mx_strndup(tail->s, del->ind));
    tmp->str_sum_len += del->ind;
    bytes->res += del->ind;
    tail->s = mx_strjoin(tail->s + del->ind + 1, bytes->buffer);
    tail->len += bytes->buf_sz - del->ind - 1;
}

static void delim_in_buf(t_tmp *tmp, t_str_len *tail,
t_delim *del, t_read_bytes *bytes) {
    mx_push_back(&(tmp->strlist), mx_strdup(tail->s));
    mx_push_back(&(tmp->strlist), mx_strndup(bytes->buffer, del->ind));
    tmp->str_sum_len += tail->len + del->ind;
    bytes->res += tail->len + del->ind;
    tail->s = mx_strdup(bytes->buffer + del->ind + 1);
    tail->len = bytes->buf_sz - del->ind - 1;
}

static void no_delim_found(t_tmp *tmp, t_str_len *tail, t_read_bytes *bytes) {
    mx_push_back(&(tmp->strlist), mx_strdup(tail->s));
    mx_push_back(&(tmp->strlist), mx_strdup(bytes->buffer));
    tmp->str_sum_len += tail->len + bytes->buf_sz;
    bytes->res += tail->len + bytes->buf_sz;
    tail->s = mx_strnew(0);
    tail->len = 0;
    if (tmp->flag == -1) {
        tmp->flag = 3;
    }
}

void mx_bufsize_chars_read(t_tmp *tmp, t_str_len *tail,
t_delim *del, t_read_bytes *bytes) {
    if (tmp->flag == 3)
        tmp->flag = -1;
    if ((del->ind = mx_get_char_index(tail->s, del->c)) >= 0)
        delim_in_tail(tmp, tail, del, bytes);
    else if ((del->ind = mx_get_char_index(bytes->buffer, del->c)) >= 0)
        delim_in_buf(tmp, tail, del, bytes);
    else
        no_delim_found(tmp, tail, bytes);
    free(tmp->s);
    tmp->s = tail->s;
}
