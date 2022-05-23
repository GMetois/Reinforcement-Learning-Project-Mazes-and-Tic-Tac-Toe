#ifndef TTTENV_H
#define TTTENV_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct action{
    int x;
    int y;
    char joueur;
};
 
typedef struct action action; 

struct envOutput{
    char** future_board;
    float reward;
    int ok;
};

typedef struct envOutput envOutput;

char** alloc_board();

void free_board(char** b);

void board_render(char** b);

void board_reset(char** b);

void board_copy(char** b, char** a);

envOutput board_play(action a, char** b);

float get_reward(char** tab);

int is_winning(char j, char** tab);

int is_draw(char** tab);

action env_action_sample(char j);

#endif /* TTENV_H */
