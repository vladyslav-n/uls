#include "../inc/libmx.h"

void *mx_nalloc(size_t size) {
    void *ptr = malloc(size);

    return mx_memset(ptr, 0, size);
}
