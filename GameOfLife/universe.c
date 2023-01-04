#include "universe.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

struct Universe {
    uint32_t rows;
    uint32_t cols;
    bool **grid; // container
    bool toroidal;
};

Universe *uv_create(uint32_t rows, uint32_t cols, bool toroidal) {
    Universe *u = (Universe *) malloc(sizeof(Universe));
    u->grid = (bool **) calloc(rows, sizeof(bool *));
    for (uint32_t r = 0; r < rows; r += 1) {
        u->grid[r] = (bool *) calloc(cols, sizeof(bool));
    }
    u->rows = rows;
    u->cols = cols;
    u->toroidal = toroidal;
    return u;
}

void uv_delete(Universe *u) {
    for (uint32_t r = 0; r < u->rows; r += 1) {
        free(u->grid[r]);
    }
    free(u->grid);
    free(u);
}

uint32_t uv_rows(Universe *u) {
    return u->rows;
}

uint32_t uv_cols(Universe *u) {
    return u->cols;
}

void uv_live_cell(Universe *u, uint32_t r, uint32_t c) {
    u->grid[r][c] = true;
}

void uv_dead_cell(Universe *u, uint32_t r, uint32_t c) {
    u->grid[r][c] = false;
}

bool uv_get_cell(Universe *u, uint32_t r, uint32_t c) {
    return u->grid[r][c];
}

bool uv_populate(Universe *u, FILE *infile) {
    uint32_t x, y;
    for (uint32_t r = 0; r < u->rows; r++) {
        for (uint32_t c = 0; c < u->cols; c++) {
            uv_dead_cell(u, r, c); // set all cells to dead
        }
    }
    while (!feof(infile)) {
        fscanf(infile, "%" PRIu32, &x);
        fscanf(infile, "%" PRIu32, &y);
        if (x >= u->rows || y >= u->cols) {
            return false;
        }
        uv_live_cell(u, x, y);
    }
    return true;
}

uint32_t uv_census(Universe *u, uint32_t r, uint32_t c) {
    uint32_t count = 0;
    int32_t row = r;
    int32_t col = c;
    int32_t total_row = u->rows;
    int32_t total_col = u->cols;
    if (u->toroidal == false) {
        if (row + 1 >= 0 && col >= 0 && row + 1 < total_row && col < total_col) {
            if (uv_get_cell(u, r + 1, c) == true) {
                count += 1;
            }
        }
        if (row + 1 >= 0 && col + 1 >= 0 && row + 1 < total_row && col + 1 < total_col) {
            if (uv_get_cell(u, r + 1, c + 1) == true) {
                count += 1;
            }
        }
        if (row + 1 >= 0 && col - 1 >= 0 && row + 1 < total_row && col < total_col) {
            if (uv_get_cell(u, r + 1, c - 1) == true) {
                count += 1;
            }
        }
        if (row >= 0 && col + 1 >= 0 && row < total_row && col + 1 < total_col) {
            if (uv_get_cell(u, r, c + 1) == true) {
                count += 1;
            }
        }
        if (row >= 0 && col - 1 >= 0 && row < total_row && col < total_col) {
            if (uv_get_cell(u, r, c - 1) == true) {
                count += 1;
            }
        }
        if (row - 1 >= 0 && col >= 0 && row < total_row && col < total_col) {
            if (uv_get_cell(u, r - 1, c) == true) {
                count += 1;
            }
        }
        if (row - 1 >= 0 && col + 1 >= 0 && row < total_row && col + 1 < total_col) {
            if (uv_get_cell(u, r - 1, c + 1) == true) {
                count += 1;
            }
        }
        if (row - 1 >= 0 && col - 1 >= 0 && row < total_row && col < total_col) {
            if (uv_get_cell(u, r - 1, c - 1) == true) {
                count += 1;
            }
        }
    } else {
        uint32_t testx, testy;
        for (int32_t x = 1; x >= -1; --x) {
            testx = (r + x) % (u->rows);
            for (int32_t y = 1; y >= -1; --y) {
                testy = (r + y) % (u->cols);
                if (uv_get_cell(u, testx, testy) == true && !(testx == r && testy == c)) {
                    count += 1;
                }
            }
        }
    }
    return count;
}

void uv_print(Universe *u, FILE *outfile) {
    //	fprintf(outfile, "test\n");
    for (uint32_t row = 0; row < u->rows; ++row) {
        for (uint32_t col = 0; col < u->cols; ++col) {
            if (u->grid[row][col] == false) {
                fprintf(outfile, ".");
            } else {
                fprintf(outfile, "o");
            }
        }
        fprintf(outfile, "\n");
    }
}
