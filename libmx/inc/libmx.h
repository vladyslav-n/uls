#pragma once
#include <stddef.h>
#include <stdlib.h>
#include <malloc/malloc.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <limits.h>
#include <sys/cdefs.h>
#include <ctype.h>

typedef struct s_list {
    void *data;
    struct s_list *next;
} t_list;

typedef struct s_str_len {
    char *s;
    int len;
} t_str_len;

typedef struct s_bot_mid_up {
    int bot;
    int mid;
    int up;
} t_bot_mid_up;

typedef struct s_substr_replace {
    char *str;
    char *replace;
} t_swap_strings;

typedef struct s_tmp {
    char *buf;
    char *s;
    char flag;
    char **line_p;
    int fd;
    t_list *strlist;
    int str_sum_len;
} t_tmp;

typedef struct s_delim {
    char c;
    signed long long int ind;
} t_delim;

typedef struct s_read_bytes {
    size_t read;
    signed long long int res;
    size_t buf_sz;
    char *buffer;
} t_read_bytes;

void mx_printchar(char c);
void mx_print_unicode(wchar_t c);
void mx_printstr(const char *s);
void mx_printerror(const char *s);
void mx_print_strarr(char **arr, const char *delim);
void mx_printint(int n);
void mx_printnum(long long n);
char *mx_nbr_to_hex(unsigned long nbr);
unsigned long mx_hex_to_nbr(const char *hex);
bool mx_isdigit(int c);
bool mx_isalpha(int c);
bool mx_isprint(int c);
char *mx_nbr_to_hex(unsigned long nbr);
char *mx_itoa(int number);
char *mx_llitoa(long long number);
int mx_atoi(const char *str);
void mx_foreach(int *arr, int size, void (*f)(int));
int mx_binary_search(char **arr, int size, const char *s, int *count);
int mx_bubble_sort(char **arr, int size);
int mx_strcmp(const char *s1, const char *s2);
int mx_quicksort(char **arr, int left, int right);
int mx_strlen(const char *s);
size_t mx_strlen_size_t(const char *s);
void mx_swap_char(char *s1, char *s2);
void mx_str_reverse(char *s);
void mx_strdel(char **str);
void mx_del_strarr(char ***arr);
int mx_get_char_index(const char *str, char c);
char *mx_strdup(const char *s1);
char *mx_strnew(const int size);
char *mx_strcpy(char *dst, const char *src);
char *mx_strncpy(char *dst, const char *src, int len);
char *mx_strndup(const char *s1, size_t n);
char *mx_strcat(char *restrict s1, const char *restrict s2);
char *mx_strstr(const char *haystack, const char *needle);
int mx_get_substr_index(const char *str, const char *sub);
int mx_count_substr(const char *str, const char *sub);
int mx_count_words(const char *str, char c);
char *mx_strtrim(const char *str);
char *mx_del_extra_spaces(const char *str);
char **mx_strsplit(const char *s, char c);
char **mx_strsplit_ncount(const char *s, char c, int *count);
char *mx_strjoin(const char *s1, const char *s2);
char *mx_file_to_str(const char *file);
int mx_read_line(char **lineptr, size_t buf_size, char delim, const int fd);
void mx_bufsize_chars_read(t_tmp *tmp, t_str_len *tail,
t_delim *del, t_read_bytes *bytes);
void mx_write_str_list(t_list *strlist, int str_sum_len, char **lineptr);
char *mx_replace_substr(const char *str, const char *sub, const char *replace);
void *mx_memset(void *b, int c, size_t len);
void *mx_memcpy(void *restrict dst, const void *restrict src, size_t n);
void *mx_memccpy(void *restrict dst, const void *restrict src,
int c, size_t n);
int mx_memcmp(const void *s1, const void *s2, size_t n);
void *mx_memchr(const void *s, int c, size_t n);
void *mx_memrchr(const void *s, int c, size_t n);
void *mx_memmem(const void *big, size_t big_len, const void *little,
size_t little_len);
void *mx_memmove(void *dst, const void *src, size_t len);
void *mx_realloc(void *ptr, size_t size);
void *mx_nalloc(size_t size);
bool mx_isspace(char c);
void mx_skipchar(const char *str, char c, int *i);
void mx_skip_untilchar(const char *str, char c, int *i);
int mx_skip_untilchar_ncount(const char *str, char c, int *i);
bool mx_is_substr(const char *haystack, const char *needle);
t_list *mx_create_node(void *data);
void mx_push_front(t_list **list, void *data);
void mx_push_back(t_list **list, void *data);
void mx_pop_front(t_list **head);
void mx_pop_back(t_list **head);
int mx_list_size(t_list *list);
t_list *mx_sort_list(t_list *lst, bool (*cmp)(void *, void *));
void mx_clear_list(t_list **head);

