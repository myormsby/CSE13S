#include "stats.h"

#include <inttypes.h>
#include <stdlib.h>

uint32_t bit_length(uint32_t number) {
    uint32_t len = 0;
    uint32_t bit_a = number;
    while (bit_a != 0) {
        bit_a >>= 1;
        len += 1;
    }
    return len;
}

void comparator(Stats *stats, uint32_t *array, uint32_t x, uint32_t y) {
    if (cmp(stats, array[x], array[y]) > 0) {
        swap(stats, &array[x], &array[y]);
    }
}

void batcher_sort(Stats *stats, uint32_t *list, uint32_t length) {
    if (length == 0) {
        return;
    }
    uint32_t n = length;
    uint32_t t = bit_length(n);
    uint32_t p = 1 << (t - 1);

    while (p > 0) {
        uint32_t q = 1 << (t - 1);
        uint32_t r = 0;
        uint32_t d = p;

        while (d > 0) {
            for (uint32_t i = 0; i < (n - d); ++i) {
                if ((i & p) == r) {
                    comparator(stats, list, i, i + d);
                }
            }
            d = q - p;
            q >>= 1;
            r = p;
        }
        p >>= 1;
    }
}
