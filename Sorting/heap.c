#include "stats.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

uint32_t max_child(Stats *stats, uint32_t *list, uint32_t first, uint32_t last) {
    uint32_t left = 2 * first;
    uint32_t right = left + 1;
    if ((right <= last) && (cmp(stats, list[right - 1], list[left - 1]) > 0)) {
        return right;
    }
    return left;
}
/*
uint32_t min_child(Stats *stats, uint32_t *list, uint32_t first, uint32_t last) {
    uint32_t left = 2 * first;
    uint32_t right = left + 1;
    if ((right <= last) && (cmp(stats, list[right - 1], list[left - 1]) < 0)) {
        return right;
    }
    return left;
}
*/
void fix_heap(Stats *stats, uint32_t *list, uint32_t first, uint32_t last) {
    bool found = false;
    uint32_t mother = first;
    uint32_t great = max_child(stats, list, mother, last); // change great name to least
    while ((mother <= (last / 2)) && (found != true)) {
        if (cmp(stats, list[mother - 1], list[great - 1]) < 0) { // >
            swap(stats, &list[mother - 1], &list[great - 1]);
            mother = great;
            great = max_child(stats, list, mother, last); // great == least
        } else {
            found = true;
        }
    }
}

void build_heap(Stats *stats, uint32_t *list, uint32_t first, uint32_t last) { // nope
    for (uint32_t father = (last / 2); father > (first - 1); father -= 1) {
        fix_heap(stats, list, father, last);
    }
}

void heap_sort(Stats *stats, uint32_t *list, uint32_t length) { // nope
    uint32_t first = 1;
    uint32_t last = length;
    build_heap(stats, list, first, last);
    for (uint32_t leaf = last; leaf > first; leaf -= 1) {
        swap(stats, &list[first - 1], &list[leaf - 1]);
        fix_heap(stats, list, first, leaf - 1);
    }
}
