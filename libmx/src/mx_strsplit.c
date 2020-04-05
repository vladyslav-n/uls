#include "../inc/libmx.h"

char **mx_strsplit(const char *s, char c) {
    int words_count = mx_count_words(s, c);
    char **arr = NULL;
    int word_len = 0;
    int i = 0;
    int j = 0;

    if (!s)
        return NULL;
    arr = (char**) malloc(sizeof(char*) * (words_count + 1));
    arr[words_count] = NULL;
    if (!words_count)
        return arr;
    while (s[i]) {
        if ((word_len = mx_skip_untilchar_ncount(s, c, &i)))
            arr[j++] = mx_strndup(s + i - word_len, word_len);
        if (s[i])
            mx_skipchar(s, c, &i);
    }
    return arr;
}
