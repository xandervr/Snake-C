//
//  grid.c
//  Snake
//
//  Created by Xander Van Raemdonck on 17/12/16.
//  Copyright © 2016 TnTap. All rights reserved.
//

#include "grid.h"

static struct Cell*** grid;

struct Cell* get_cell(int x, int y) {
    return grid[x][y];
}

struct Coordinate {
    int x, y;
};

struct Cell*** allocate_grid(int width, int height) {
    struct Cell ***grid = (struct Cell ***) malloc(sizeof(struct Cell**) * height);
    for(int i = 0; i < height; i++) {
        grid[i] = (struct Cell**) malloc(sizeof(struct Cell*) * width);
        for(int j = 0; j < width; j++) {
            grid[i][j] = (struct Cell*) malloc(sizeof(struct Cell));
        }
    }
    
    return grid;
}

void deallocate_grid(int width, int heigth) {
    for(int x = 0; x < width; x++) {
        for(int y = 0; y < heigth; y++) {
            free(grid[x][y]);
        }
        free(grid[x]);
    }
    free(grid);
}

void initialize_grid(int width, int height) {
    grid = allocate_grid(width, height);
}
