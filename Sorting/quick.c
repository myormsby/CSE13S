#include "stats.h"

#include <stdint.h>
#include <stdlib.h>

uint32_t partition(Stats *stats, uint32_t *list, uint32_t low, uint32_t high) {
    uint32_t i = low - 1;
    for (uint32_t j = low; j < high; ++j) {
        if (cmp(stats, list[j - 1], list[high - 1]) < 0) {
            i += 1;
            swap(stats, &list[i - 1], &list[j - 1]);
        }
    }
    swap(stats, &list[i], &list[high - 1]);
    return (i + 1);
}

void quick_sorter(Stats *stats, uint32_t *list, uint32_t low, uint32_t high) {
    if (low < high) {
        uint32_t p = partition(stats, list, low, high);
        quick_sorter(stats, list, low, (p - 1));
        quick_sorter(stats, list, (p + 1), high);
    }
    return;
}

void quick_sort(Stats *stats, uint32_t *list, uint32_t length) {
    quick_sorter(stats, list, 1, length);
    return;
}
