#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <inttypes.h>
#include "universe.h"

#define ROW   0
#define DELAY 50000

int main(int argc, char **argv) {
    bool toroidal = false;
    bool silence = false;
    int opt = 0;
    FILE *filein = stdin;
    FILE *fileout = stdout;
    uint32_t generations = 100;
    while ((opt = getopt(argc, argv, "tsn:i:o:")) != -1) {
        switch (opt) {
        case 't': toroidal = true; break;
        case 's': silence = true; break;
        case 'n': generations = (uint32_t) strtoul(optarg, NULL, 10); break;
        case 'i': filein = fopen(optarg, "r"); break;
        case 'o': fileout = fopen(optarg, "w"); break;
        }
    }
    uint32_t row, col;
    fscanf(filein, "%" PRIu32, &row);
    fscanf(filein, "%" PRIu32, &col);
    Universe *a = uv_create(row, col, toroidal);
    Universe *b = uv_create(row, col, toroidal);
    Universe *t;
    uv_populate(a, filein);
    if (silence == false) {
        initscr();
        curs_set(FALSE);
    }
    for (uint32_t gen = 0; gen <= generations; ++gen) {
        for (uint32_t r = 0; r < uv_rows(a); ++r) {
            for (uint32_t c = 0; c < uv_cols(a); ++c) {
                uint32_t count = uv_census(a, r, c);
                if ((count == 2 || count == 3) && (uv_get_cell(a, r, c) == true)) {
                    uv_live_cell(b, r, c);
                    if (silence == false)
                        mvprintw(r, c, "o");
                } else if ((count == 3) && (uv_get_cell(a, r, c) == false)) {
                    uv_live_cell(b, r, c);
                    if (silence == false)
                        mvprintw(r, c, "o");
                } else {
                    uv_dead_cell(b, r, c);
                }
            }
        }
        refresh();
        usleep(DELAY);
        t = a;
        a = b;
        b = t;
        clear();
    }
    endwin();
    uv_print(b, stdout);
    fclose(filein);
    fclose(fileout);
    uv_delete(a);
    uv_delete(b);
}
