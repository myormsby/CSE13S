#include "batcher.h"
#include "heap.h"
#include "insert.h"
#include "quick.h"
#include "set.h"
#include "stats.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void help(void) {
    printf("SUMMARY\n"
           "Sorts a random set of numbers.\n"
           "\n"
           "USAGE\n"
           "[ahbiqr:n:p:H] [-r seed] [-n size] [-p elements]"
           "\n"
           "OPTIONS\n"
           "-a: uses all sort functions\n"
           "-h: uses the heap sort function\n"
           "-b: uses the batcher sort function\n"
           "-i: uses the insertion sort function\n"
           "-q: uses the quick sort function\n"
           "-r seed: sets the random seed\n"
           "-n size: sets the size of the array to be sorted\n"
           "-p elements: print out only a set number of elements from the array\n"
           "-H: displays the program's usage and synopsis\n");
}

void printsort(uint32_t *A, uint32_t n) {
    for (uint32_t current = 0; current < n; ++current) {
        printf("%13" PRIu32 "", A[current]);
        if (current % 5 == 4) {
            printf("\n");
        }
    }
}

/*
void createarray(uint32_t *list, uint32_t n) {	
    uint32_t mask = 0x00003FFFFFFF;
    for (uint32_t i = 0; i < n; ++i) { //fill the array with random numbers
        list[i] = rand() & mask; // bitmasking
    }
}
*/

int main(int argc, char **argv) {
    uint32_t seed = 13371453, size = 100, elements = 100;
    int opt = 0;
    Set A = empty_set();
    while ((opt = getopt(argc, argv, "ahbiqr:n:p:H")) != -1) {
        switch (opt) {
        case 'a':
            A = insert_set(1, A); //insertion sort
            A = insert_set(2, A); //batcher sort
            A = insert_set(3, A); //heap sort
            A = insert_set(4, A); //quick sort
            break;
        case 'h': A = insert_set(3, A); break;
        case 'b': A = insert_set(2, A); break;
        case 'i': A = insert_set(1, A); break;
        case 'q': A = insert_set(4, A); break;
        case 'r': seed = (uint32_t) strtoul(optarg, NULL, 10); break;
        case 'n': size = (uint32_t) strtoul(optarg, NULL, 10); break;
        case 'p': elements = (uint32_t) strtoul(optarg, NULL, 10); break;
        default: help(); break;
        }
    }
    uint32_t *array = (uint32_t *) malloc(size * sizeof(uint32_t));
    uint32_t mask = 0x00003FFFFFFF;
    Stats stats;
    reset(&stats);
    srand(seed);
    for (uint32_t i = 0; i < size; ++i) { //fill the array with random numbers
        array[i] = rand() & mask; // bitmasking
    }
    if (member_set(1, A) == true) {
        if (elements > size) {
            elements = size;
        }
        insertion_sort(&stats, array, size);
        printf("Insertion Sort, %" PRIu32 " elements, %" PRIu64 " moves, %" PRIu64 " compares\n",
            elements, stats.moves, stats.compares);
        printsort(array, elements);
        reset(&stats);
    }

    srand(seed);
    for (uint32_t i = 0; i < size; ++i) { //fill the array with random numbers
        array[i] = rand() & mask; // bitmasking
    }
    if (member_set(2, A) == true) {
        if (elements > size) {
            elements = size;
        }
        batcher_sort(&stats, array, size);
        printf("Batcher Sort, %" PRIu32 " elements, %" PRIu64 " moves, %" PRIu64 " compares\n",
            elements, stats.moves, stats.compares);
        if (elements > size) {
            elements = size;
        }
        printsort(array, elements);
        reset(&stats);
    }
    srand(seed);
    for (uint32_t i = 0; i < size; ++i) { //fill the array with random numbers
        array[i] = rand() & mask; // bitmasking
    }

    if (member_set(3, A) == true) {
        if (elements > size) {
            elements = size;
        }
        heap_sort(&stats, array, size);
        printf("Heap Sort, %" PRIu32 " elements, %" PRIu64 " moves, %" PRIu64 " compares\n",
            elements, stats.moves, stats.compares);
        if (elements > size) {
            elements = size;
        }
        printsort(array, elements);
        reset(&stats);
    }
    srand(seed);
    for (uint32_t i = 0; i < size; ++i) { //fill the array with random numbers
        array[i] = rand() & mask; // bitmasking
    }

    if (member_set(4, A) == true) {
        if (elements > size) {
            elements = size;
        }
        reset(&stats);
        quick_sort(&stats, array, size);
        printf("Quick Sort, %" PRIu32 " elements, %" PRIu64 " moves, %" PRIu64 " compares\n",
            elements, stats.moves, stats.compares);
        if (elements > size) {
            elements = size;
        }
        printsort(array, elements);
        reset(&stats);
    }
    free(array);
    return 0;
}
