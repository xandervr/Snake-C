//
//  game.h
//  Snake
//
//  Created by Xander Van Raemdonck on 13/12/16.
//  Copyright © 2016 TnTap. All rights reserved.
//

#ifndef game_h
#define game_h

#include <stdio.h>
#include <time.h>
#include <string.h>
#include "GUI.h"

#define BOOST_SPAWN_CHANCE 100
#define BOOST_SCORE 5
#define APPLE_SCORE 1

void initialize_game(int, int);
void run_game();

enum Gamestate {
    UNPAUSED,
    PAUSED
};

struct HighScore {
    int score;
    struct HighScore * next;
};

struct Game {
    enum Gamestate state;
    int score;
    struct HighScore * highscores;
};

enum EatenState {
    EATEN,
    UNEATEN
};


struct Apple {
    enum EatenState state;
    int x;
    int y;
};

struct Boost {
    enum EatenState state;
    int x;
    int y;
    int time;
    int enabled;
};

struct SnakePiece {
    int x;
    int y;
    struct SnakePiece* next;
};

struct Snake {
    struct SnakePiece* pieces;
    int dx;
    int dy;
    int len;
};




int is_paused();
void pause_game();
void resume_game();
void stop_game();
void save_game();
void load_game();
void save_highscores();
void load_highscores();
void score_up();
void boost_score_up();

extern struct Game * game;
extern int global_width, global_height;

#endif /* game_h */
