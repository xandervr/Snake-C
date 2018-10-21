//
//  score.h
//  Snake
//
//  Created by Xander Van Raemdonck on 15/01/17.
//  Copyright © 2017 TnTap. All rights reserved.
//

#ifndef score_h
#define score_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOOST_SCORE 5
#define APPLE_SCORE 1


struct HighScore {
    int score;
    struct HighScore * next;
};

struct GameScore {
    int score;
    struct HighScore * highscores;
};


void initialize_score();
void add_highscore(int);
void remove_highscores();
void save_highscores();
void load_highscores();
void score_up();
void boost_score_up();
int get_score();
void set_score();
struct HighScore* get_highscores();
void destroy_score();

#endif /* score_h */
