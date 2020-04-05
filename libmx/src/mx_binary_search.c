#include "../inc/libmx.h"

static void cycle(char **arr, const char *s, int *count, t_bot_mid_up *pts) {
    while (mx_strcmp(s, arr[pts->mid]) && pts->bot != pts->up) {
        if (mx_strcmp(s, arr[pts->mid]) > 0) {
            pts->bot = pts->mid + 1;
            pts->mid = (pts->bot + pts->up) / 2;
        }
        else {
            pts->up = pts->mid - 1;
            pts->mid = (pts->bot + pts->up) / 2;
        }
        *count += 1;
    }
}

int mx_binary_search(char **arr, int size, const char *s, int *count) {
    t_bot_mid_up points = {0, (size - 1) / 2, size -1};

    *count = 0;
    if (size < 1 || !s || !arr || !count)
        return -1;
    cycle(arr, s, count, &points);
    if (!mx_strcmp(s, arr[points.mid])) {
        *count += 1;
        return points.mid;
    }
    else
        *count = 0;
    return -1;
}
