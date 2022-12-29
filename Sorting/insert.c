#include "set.h"
#include "stats.h"

#include <stdint.h>
#include <stdlib.h>

void insertion_sort(Stats *stats, uint32_t list[], uint32_t length) {
    for (uint32_t i = 1; i < length; i += 1) {
        uint32_t j = i;
        uint32_t temp = move(stats, list[i]);
        while (j > 0 && cmp(stats, temp, list[j - 1]) < 0) {
            list[j] = move(stats, list[j - 1]);
            j -= 1;
        }
        list[j] = move(stats, temp);
    }
    return;
}
