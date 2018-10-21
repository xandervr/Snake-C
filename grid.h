//
//  grid.h
//  Snake
//
//  Created by Xander Van Raemdonck on 17/12/16.
//  Copyright © 2016 TnTap. All rights reserved.
//

#ifndef grid_h
#define grid_h

#include <stdio.h>
#include <stdlib.h>

enum State {
    COVERED,
    SNAKE,
    HEAD,
    OBSTACLE,
    BOOST,
    APPLE
};


void initialize_grid(int width, int height);
struct Cell ***allocate_grid(int, int);
void deallocate_grid(int, int);

struct Cell {
    enum State state;
};

struct Cell* get_cell(int x, int y);

#endif /* grid_h */
