//
//  game.c
//  Snake
//
//  Created by Xander Van Raemdonck on 13/12/16.
//  Copyright © 2016 TnTap. All rights reserved.
//

#include "game.h"

int global_width, global_height;

struct Game * game;

void initialize_game(int width, int height) {
    global_width = width;
    global_height = height;
    game = (struct Game*) malloc(sizeof(struct Game));
    game->highscores = NULL;
    load_highscores();
    game->score = 0;
}

void pause_game() {
    game->state = PAUSED;
}

void resume_game() {
    game->state = UNPAUSED;
}

int is_paused() {
    if(game->state == PAUSED) {
        return 1;
    } else {
        return 0;
    }
}

void score_up() {
    game->score += APPLE_SCORE;
}

void boost_score_up() {
    game->score += BOOST_SCORE;
}

void add_highscore(int score) {
    struct HighScore * highscore = game->highscores;
    struct HighScore * prev_highscore = NULL;
    int in_between = 0;
    int counter = 0;
    while(highscore != NULL && counter < 10) {
        
        if(score > highscore->score) {
            struct HighScore * temp_highscore = (struct HighScore*) malloc(sizeof(struct HighScore));
            temp_highscore->score = score;
            
            if(prev_highscore != NULL) {
                temp_highscore->next = prev_highscore->next;
                prev_highscore->next = temp_highscore;
            } else {
                temp_highscore->next = highscore;
                game->highscores = temp_highscore;
            }
            
            in_between = 1;
            break;
        }
        prev_highscore = highscore;
        highscore = highscore->next;
        counter++;
    }
    if(in_between == 0) {
        highscore = (struct HighScore*) malloc(sizeof(struct HighScore));
        highscore->score = score;
        highscore->next = NULL;
        if(prev_highscore != NULL) {
            prev_highscore->next = highscore;
        } else {
            game->highscores = highscore;
        }
    }
}

void save_highscores() {
    FILE * fh = fopen("scores.dat", "w+");
    struct HighScore* highscore = game->highscores;
    
    while (highscore != NULL) {
        fprintf(fh, "%d;", highscore->score);
        highscore = highscore->next;
    }
    
    fclose(fh);
}

void remove_highscores() {
    struct HighScore* highscore = game->highscores;
    
    struct HighScore* prev_highscore = NULL;
    while (highscore != NULL) {
        prev_highscore = highscore;
        highscore = highscore->next;
        free(prev_highscore);
    }
}

void load_highscores() {
    FILE * fh = fopen("scores.dat", "r+");
    
    if(fh != NULL) {
        char * field;
        char line[256];
        
        fgets(line, sizeof(line), fh);
        field = strtok(line, ";");
        while(field != NULL) {
            add_highscore(atoi(field));
            field = strtok(NULL, ";");
        }
    }
    
    fclose(fh);
}

void stop_game() {
    add_highscore(game->score);
    save_highscores();
    
    remove_snake_pieces();
    remove_highscores();
    free(snake->pieces);
    free(snake);
    free(apple);
    free(boost);
    deallocate_grid(global_width, global_height);
    free(game);
    destroy_music();
    
    stop_gui();
}

void save_game() {
    FILE * fh = fopen("save.dat", "w+");
    
    fprintf(fh, "%d;%d;%d", snake->dx, snake->dy, game->score);
    fprintf(fh, "\n");
    for(int y = 0; y < global_height; y++) {
        for(int x = 0; x < global_width; x++) {
            struct Cell* current_cell = get_cell(x, y);
            fprintf(fh, "%d;", current_cell->state);
            //printf(" %d ", current_cell->state);
        }
        fprintf(fh, "\n");
    }
    fclose(fh);
}



void load_game() {
    //pause_game();
    
    remove_snake_pieces();
    clear_field();
    FILE * fh = fopen("save.dat", "r+");
    char * field;
    if(fh != NULL) {
        char line[256];
        int line_number = 0;
        int counter = 0;
        
        fgets(line, sizeof(line), fh);
        field = strtok(line, ";");
        counter = 0;
        while(field != NULL && counter < 3) {
            if(counter == 0) {
                snake->dx = atoi(field);
            } else if(counter == 1) {
                snake->dy = atoi(field);
            } else if(counter == 2) {
                game->score = atoi(field);
            }
            counter++;
            field = strtok(NULL, ";");
        }
        counter = 0;
        while(fgets(line, sizeof(line), fh) && line_number < global_height) {
            field = strtok(line, ";");
            int field_number = 0;
            
            while(field != NULL && field_number < global_width) {
                printf(" %d ", atoi(field));
                
                if(atoi(field) == HEAD) {
                    //printf("(%d, %d)\n", field_number, line_number);
                    get_cell(snake->pieces->x, snake->pieces->y)->state = COVERED;
                    snake->pieces->x = field_number;
                    snake->pieces->y = line_number;
                } else if(atoi(field) == SNAKE) {
                    //draw_snake();
                    add_snake_piece();
                    counter++;
                } else if(atoi(field) == APPLE) {
                    //printf("(%d, %d)\n", field_number, line_number);
                    get_cell(apple->x, apple->y)->state = COVERED;
                    apple->x = field_number;
                    apple->y = line_number;
                    get_cell(apple->x, apple->y)->state = APPLE;
                } else if(atoi(field) == BOOST) {
                    //printf("(%d, %d)\n", field_number, line_number);
                    get_cell(boost->x, boost->y)->state = COVERED;
                    boost->x = field_number;
                    boost->y = line_number;
                    get_cell(boost->x, boost->y)->state = BOOST;
                } else {
                    get_cell(field_number, line_number)->state = atoi(field);
                }
                //printf("Cell(%d, %d)->state = %d\n", field_number, line_number, atoi(field));
                
                
                field_number++;
                field = strtok(NULL, ";");
            }
            printf("\n");
            line_number++;
            
        }
        
        snake->len = counter;
        
    }
    fclose(fh);
    //resume_game();
}

void run_game() {
    initialize_grid(global_width, global_height);
    initialize_gui("Snake", global_width, global_height);
    play_music();
    
    while (1) {
        read_input();
        check_collision();
        SDL_Delay(300);
    }
}
