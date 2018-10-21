//
//  GUI.h
//  Snake
//
//  Created by Xander Van Raemdonck on 13/12/16.
//  Copyright © 2016 TnTap. All rights reserved.
//

#ifndef GUI_h
#define GUI_h

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "grid.h"
#include "game.h"


#define IMAGE_HEIGHT 25
#define IMAGE_WIDTH 25
#define SCORE_WIDTH 75
#define SCORE_HEIGHT 25


void initialize_gui(char*, int, int);
void check_collision();
void read_input();
void add_snake_piece();
void draw_snake();
void clear_field();
void remove_snake_pieces();
void stop_gui();
void initialize_sound();
void play_music();
void destroy_music();

extern struct Boost* boost;
extern struct Apple* apple;
extern struct Snake* snake;

#endif /* GUI_h */
