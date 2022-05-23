#include "TTTEnv.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Une fonction pour allouer le tableau de jeu.
char** alloc_board(){
    char** b = malloc(3 * sizeof(char*));

    for(int i=0; i<3; i++) {
        b[i] = malloc(3 * sizeof(char*));
        for(int j = 0; j<3; j++){
            b[i][j]='.';
        }
    }
    return(b);
}

//Une fonction pour libérer les tableaux.
void free_board(char** b){
    for(int i=0; i<3; i++){
        free(b[i]);
    }
    free(b);
}

//Une fonction pour afficher le tableau de jeu
void board_render(char** b){
    for (int i=0; i<3; i++) {
        for (int j=0; j< 3; j++){
            printf("%c ", b[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

//Une fonction pour remettre le tableau de jeu dans sa configuration initiale.
void board_reset(char** b){
    for (int i=0; i<3; i++) {
        for (int j=0;j<3;j++){
            b[i][j] = '.';
        }
    }
}

//Une fonction bien pratique pour copier une configuration a dans une configuration b.
void board_copy(char** b, char** a){
    for(int i=0; i<3; i++){
        strcpy(b[i],a[i]);
    }
}

//Applique l'action choisie sur le tableau et en tire l'état actuel.
envOutput board_play(action a, char** b){
    int coord_x = a.x;
    int coord_y = a.y;
    char symb = a.joueur;
    float reward = 0;
    int ok = 1;
    char** future_board = alloc_board();
    board_copy(future_board,b);
    envOutput output = {future_board,reward,ok};
    if(future_board[coord_x][coord_y]!='.'){
        output.ok = 0;
    }
    future_board[coord_x][coord_y]=symb;
    output.reward = get_reward(future_board);
    return(output);
}

//Une fonction pour déduire une récompense d'une configuration de jeu.
float get_reward(char** tab){
    int reward = 0;
    if(is_winning('x',tab)==1){
        reward = 100;
    }
    else if(is_winning('o',tab)==1){
        reward = -100;
    }
    return(reward);
}

//Une fonction qui vérifie si un joueur donné est gagnant dans une configuration donnée.
int is_winning(char j, char** tab){
    
    //Test sur les diagonales.
    if((tab[0][0]==j)&&(tab[1][1]==j)&&(tab[2][2]==j)){
        return(1);
    }
    else if((tab[2][0]==j)&&(tab[1][1]==j)&&(tab[0][2]==j)){
        return(1);
    }
    
    //Test sur les lignes.
    for(int i=0; i<3; i++){
        if((tab[i][0]==j)&&(tab[i][1]==j)&&(tab[i][2]==j)){
            return(1);
        }
    }

    //Test sur les colonnes.
    for(int k=0; k<3; k++){
        if((tab[0][k]==j)&&(tab[1][k]==j)&&(tab[2][k]==j)){
            return(1);
        }        
    }
    return(0);
}

//Une fonction qui dit si le tableau est plein et que la partie est match nul.
int is_draw(char** tab){
    int draw = 0;
    for (int i = 0; i < 3; ++i){
        for (int j = 0; j < 3; ++j){
            if(tab[i][j]!='.'){
                draw++;
            }
        }
    }
    if(draw == 9){
        return(1);
    }
    return(0);
}

action env_action_sample(char j){
    int x = rand()%3;
    int y =rand()%3;
    struct action a = {x,y,j};
    return(a);
}

