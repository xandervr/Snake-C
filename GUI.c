//
//  GUI.c
//  Snake
//
//  Created by Xander Van Raemdonck on 13/12/16.
//  Copyright © 2016 TnTap. All rights reserved.
//

#include "GUI.h"

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Surface *scores;

TTF_Font *font = NULL;
SDL_Color textColor = {0, 153, 0};

SDL_Surface* images[5];
SDL_Texture* textures[5];


int first_piece = 2;
int game_over = 0;
struct Boost* boost;
struct Apple* apple;
struct Snake* snake;
const static char* tune = "Resources/tune.mp3";
Mix_Music *music;

void initialize_window(char* title, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Could not initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }
    
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width * IMAGE_WIDTH, height * IMAGE_HEIGHT, SDL_WINDOW_SHOWN);
    
    if(window == NULL) {
        printf("Couldn't set screen mode to required dimensions: %s\n", SDL_GetError());
        exit(1);
    }
    
    if(TTF_Init() == -1) {
        printf("Couldn't set TTF: %s\n", TTF_GetError());
        exit(1);
    }
    
    font = TTF_OpenFont("arial.ttf", 28);
    
    if(font == NULL) {
        printf("Couldn't set TTF: %s\n", TTF_GetError());
        exit(1);
    }
    
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void initialize_sound() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        printf("Could not initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }
    
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
        printf("Could not initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }
    
    music = Mix_LoadMUS(tune);
    
}

void play_music() {
    Mix_PlayMusic(music, 1);
}

void destroy_music() {
    Mix_FreeMusic(music);
}

void add_obstacle(int x, int y) {
    get_cell(x, y)->state = OBSTACLE;
}

void remove_obstacle(int x, int y) {
    get_cell(x, y)->state = COVERED;
}

void add_wall(int start_x, int start_y, int stop_x, int stop_y) {
    if(start_x == stop_x) {
        for(int i = start_y; i < stop_y; i++) {
            add_obstacle(start_x, i);
        }
    } else if(start_y == stop_y) {
        for(int i = start_x; i < stop_x; i++) {
            add_obstacle(i, start_y);
        }
    }
}

void draw_boost() {
    if(boost->enabled == 1) {
        get_cell(boost->x, boost->y)->state = BOOST;
    } else {
        get_cell(boost->x, boost->y)->state = COVERED;
    }
}

void spawn_random_boost() {
    boost->x = rand() % global_width;
    boost->y = rand() % global_height;
    boost->time = global_width * 2;
    boost->enabled = 1;
    boost->state = UNEATEN;
    if(get_cell(boost->x, boost->y)->state != COVERED) {
        spawn_random_boost();
    }
    
}

void guess_draw_boost() {
    if(boost->state == EATEN) {
        if(rand() % BOOST_SPAWN_CHANCE == 5) {
            spawn_random_boost();
        }
    }
    if(boost->enabled == 1 && boost->state == UNEATEN) {
        boost->time--;
        if(boost->time <= 0) {
            boost->enabled = 0;
            boost->state = EATEN;
        }
    }
}

void add_boost_score() {
    boost->state = EATEN;
    boost->enabled = 0;
    get_cell(boost->x, boost->y)->state = COVERED;
    
    boost_score_up();
}

void initialize_figures() {
    boost = (struct Boost*) malloc(sizeof(struct Boost));
    apple = (struct Apple*) malloc(sizeof(struct Apple));
    snake = (struct Snake*) malloc(sizeof(struct Snake));
    snake->pieces = (struct SnakePiece*) malloc(sizeof(struct SnakePiece));
    snake->pieces->next = NULL;
    snake->dx = 1;
    snake->dy = 0;
    snake->len = 1;
    snake->pieces->x = 0;
    snake->pieces->y = 0;
    
    boost->enabled = 0;
    boost->x = 0;
    boost->y = 0;
    
    apple->x = rand() % global_width;
    apple->y = rand() % global_height;
    
    // Left wall
    add_wall(4, 4, 4, 14);
    add_wall(4, 16, 4, 26);
    
    // Right wall
    add_wall(24, 4, 24, 14);
    add_wall(24, 16, 24, 26);
    
    // Upper wall
    add_wall(10, 4, 14, 4);
    add_wall(16, 4, 20, 4);
    
    // Lower wall
    add_wall(10, 25, 14, 25);
    add_wall(16, 25, 20, 25);
    
    images[0] = SDL_LoadBMP("Resources/covered.bmp");
    images[1] = SDL_LoadBMP("Resources/apple.bmp");
    images[2] = SDL_LoadBMP("Resources/snake.bmp");
    images[3] = SDL_LoadBMP("Resources/obstacle.bmp");
    images[4] = SDL_LoadBMP("Resources/star.bmp");
    
    textures[0] = SDL_CreateTextureFromSurface(renderer, images[0]);
    textures[1] = SDL_CreateTextureFromSurface(renderer, images[1]);
    textures[2] = SDL_CreateTextureFromSurface(renderer, images[2]);
    textures[3] = SDL_CreateTextureFromSurface(renderer, images[3]);
    textures[4] = SDL_CreateTextureFromSurface(renderer, images[4]);
    
    for(int i = 0; i < 5; i++) {
        SDL_FreeSurface(images[i]);
    }
}

void draw_score() {
    char* score_text = (char*) malloc(sizeof(char));
    sprintf(score_text, "Score: %d", game->score);
    
    scores = TTF_RenderText_Solid(font, score_text, textColor);
    
    SDL_Texture * scores_texture = SDL_CreateTextureFromSurface(renderer, scores);
    
    SDL_Rect score_rect;
    score_rect.x = (global_width * IMAGE_WIDTH) - scores->w - IMAGE_WIDTH;
    score_rect.y = 0;
    score_rect.w = scores->w;
    score_rect.h = scores->h;
    
    SDL_RenderCopy(renderer, scores_texture, NULL, &score_rect);
    
    free(score_text);
    SDL_FreeSurface(scores);
    SDL_DestroyTexture(scores_texture);
}

void draw_highscores() {
    struct HighScore * highscore = game->highscores;
    
    char* score_text = (char*) malloc(sizeof(char));
    sprintf(score_text, "Highscores:");
    
    int x = (global_width * IMAGE_WIDTH) / 2 - (3 * IMAGE_WIDTH);
    int y = IMAGE_HEIGHT * 3;
    
    SDL_Surface * highscores = TTF_RenderText_Solid(font, score_text, textColor);
    
    SDL_Texture * scores_texture = SDL_CreateTextureFromSurface(renderer, highscores);
    
    SDL_Rect score_rect;
    score_rect.x = x;
    score_rect.y = y;
    score_rect.w = scores->w;
    score_rect.h = scores->h;
    
    SDL_RenderCopy(renderer, scores_texture, NULL, &score_rect);
    
    SDL_FreeSurface(highscores);
    //SDL_DestroyTexture(scores_texture);
    
    x = (global_width * IMAGE_WIDTH) / 2;
    y += IMAGE_HEIGHT;
    
    while(highscore != NULL) {
        sprintf(score_text, "%d", highscore->score);
        highscore = highscore->next;
    
        SDL_Surface * highscores = TTF_RenderText_Solid(font, score_text, textColor);
        
        SDL_Texture * scores_texture = SDL_CreateTextureFromSurface(renderer, highscores);
        
        SDL_Rect score_rect;
        score_rect.x = x;
        score_rect.y = y += IMAGE_HEIGHT;
        score_rect.w = scores->w;
        score_rect.h = scores->h;
        
        SDL_RenderCopy(renderer, scores_texture, NULL, &score_rect);
        
        SDL_FreeSurface(highscores);
        //SDL_DestroyTexture(scores_texture);
    }
    
    free(score_text);
    
}

void clear_field() {
    for(int y = 0; y < global_height; y++) {
        for(int x = 0; x < global_width; x++) {
            get_cell(x, y)->state = COVERED;
        }
    }
}

void destroy_textures() {
    for(int i = 0; i < 5; i++) {
        SDL_DestroyTexture(textures[i]);
    }
}

void stop_gui() {
    destroy_textures();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

void draw_snake() {
    struct SnakePiece* piece = snake->pieces;
    
    int x = piece->x + snake->dx;
    int y = piece->y + snake->dy;
    int counter = 0;
    while(piece != NULL){
        int temp_x = piece->x;
        int temp_y = piece->y;
        piece->x = x;
        piece->y = y;
        x = temp_x;
        y = temp_y;
        check_collision();
        //printf("Get_cell(%d, %d)", piece->x, piece->y);
        if(counter == 0) {
            get_cell(piece->x, piece->y)->state = HEAD;
        } else {
            get_cell(piece->x, piece->y)->state = SNAKE;
        }
        piece = piece->next;
        counter++;
    }
    get_cell(x, y)->state = COVERED;
}

void add_snake_piece() {
    
    struct SnakePiece* piece = snake->pieces;
    int counter = 0;
    struct SnakePiece* previous_piece = NULL;
    while(piece != NULL) {
        previous_piece = piece;
        piece = piece->next;
        counter++;
    }
    
    snake->len = counter;
    piece = (struct SnakePiece*) malloc(sizeof(struct SnakePiece));
    previous_piece->next = piece;
    piece->next = NULL;
    piece->x = previous_piece->x;
    piece->y = previous_piece->y;
    
}

void remove_snake_pieces() {
    struct SnakePiece* piece = snake->pieces->next;
    snake->pieces->next = NULL;
    struct SnakePiece* previous_piece = NULL;
    while (piece != NULL) {
        previous_piece = piece;
        piece = piece->next;
        free(previous_piece);
        
    }
}

void spawn_random_apple() {
    apple->x = rand() % global_width;
    apple->y = rand() % global_height;
    if(get_cell(apple->x, apple->y)->state != COVERED) {
        spawn_random_apple();
    }
}

void draw_apple() {
    if (apple->state == EATEN) {
        get_cell(apple->x, apple->y)->state = COVERED;
        spawn_random_apple();
        apple->state = UNEATEN;
        
    }
    get_cell(apple->x, apple->y)->state = APPLE;
}



void print_pieces() {
    struct SnakePiece* piece = snake->pieces;
    int counter = 1;
    while(piece != NULL && get_cell(piece->x, piece->y)->state == SNAKE) {
        printf("Piece-%d: (%d, %d)\n", counter, piece->x, piece->y);
        counter++;
        piece = piece->next;
    }
}


void draw_grid() {
    SDL_Rect grid;
    
    grid.x = 0;
    grid.y = 0;
    grid.w = IMAGE_WIDTH;
    grid.h = IMAGE_HEIGHT;
    
    
    for(int x = 0; x < global_width; x++) {
        for(int y = 0; y < global_height; y++) {
            grid.x = x * IMAGE_WIDTH;
            grid.y = y * IMAGE_HEIGHT;
            
            struct Cell* cell = get_cell(x, y);
            
            if(cell->state == APPLE) {
                SDL_RenderCopy(renderer, textures[1], NULL, &grid);
            } else if(cell->state == HEAD) {
                SDL_RenderCopy(renderer, textures[2], NULL, &grid);
            } else if(cell->state == SNAKE) {
                SDL_RenderCopy(renderer, textures[2], NULL, &grid);
            } else if(cell->state == COVERED) {
                SDL_RenderCopy(renderer, textures[0], NULL, &grid);
            } else if(cell->state == OBSTACLE) {
                SDL_RenderCopy(renderer, textures[3], NULL, &grid);
            } else if(cell->state == BOOST) {
                SDL_RenderCopy(renderer, textures[4], NULL, &grid);
            }
        }
    }
    
    
}

void redraw() {
    
    if (is_paused() == 0) {
        guess_draw_boost();
        draw_boost();
        draw_apple();
        draw_snake();
        
        draw_grid();
        draw_score();
        
        SDL_RenderPresent(renderer);
        print_pieces();
        
    } else if(game_over == 1) {
        draw_highscores();
        SDL_RenderPresent(renderer);
    }
    
}

int did_hit_snake() {
    int collision = 0;
    
    int head_x = snake->pieces->x;
    int head_y = snake->pieces->y;
    
    struct SnakePiece* piece = snake->pieces->next;
    
    while(piece != NULL) {
        if(piece->x == head_x && piece->y == head_y) {
            printf("%d , %d\n", head_x, head_y);
            collision = 1;
            
            printf("PIECE (%d, %d)\n", piece->x, piece->y);
            break;
        } else {
            piece = piece->next;
        }
    }
    
    return collision;
}

int did_hit_obstacle() {
    int collision = 0;
    
    int head_x = snake->pieces->x;
    int head_y = snake->pieces->y;
    
    if(head_x < global_width && head_x >= 0 && head_y < global_height && head_y >= 0) {
        if(get_cell(head_x, head_y)->state == OBSTACLE) {
            collision = 1;
            printf("OBSTACLE");
        }
    }
    
    return collision;
}

void check_collision() {
    if(apple->x == snake->pieces->x && apple->y == snake->pieces->y && apple->state == UNEATEN) {
        apple->state = EATEN;
        score_up();
        add_snake_piece();
    } else if(boost->x == snake->pieces->x && boost->y == snake->pieces->y && boost->state == UNEATEN) {
        add_boost_score();
    } else if(did_hit_snake() == 1 || did_hit_obstacle() == 1) {
        printf("GAME OVER!\n");
        pause_game();
        game_over = 1;
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            
            switch (e.type) {
                  case SDL_KEYDOWN:
                    stop_game();
                    break;
            }
        }
    } else if (snake->pieces->x >= global_width) {
        snake->pieces->x = 0;
    } else if (snake->pieces->x < 0) {
        snake->pieces->x = global_width - 1;
    } else if (snake->pieces->y >= global_height) {
        snake->pieces->y = 0;
    } else if (snake->pieces->y < 0) {
        snake->pieces->y = global_height - 1;
    }

}

void read_input() {
    
    SDL_Event event;
    
    while(SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                stop_gui();
                exit(0);
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_DOWN:
                        snake->dx = 0;
                        snake-> dy = 1;
                        break;
                    case SDLK_UP:
                        snake->dx = 0;
                        snake->dy = -1;
                        break;
                    case SDLK_RIGHT:
                        snake->dx = 1;
                        snake->dy = 0;
                        break;
                    case SDLK_LEFT:
                        snake->dx = -1;
                        snake->dy = 0;
                        break;
                    case SDLK_ESCAPE:
                        apple->state = EATEN;
                        score_up();
                        add_snake_piece();
                        break;
                    case SDLK_p:
                        if(is_paused() == 1) {
                            resume_game();
                        } else {
                            pause_game();
                        }
                        break;
                    case SDLK_s:
                        save_game();
                        break;
                    case SDLK_l:
                        load_game();
                        break;
                    default:
                        break;
                }
                
                break;
            default:
                break;
        }
        
    }
    redraw();
    
}

void initialize_gui(char* title, int width, int height) {
    initialize_sound();
    initialize_window(title, width, height);
    initialize_figures();
    redraw();
    atexit(stop_game);
}
