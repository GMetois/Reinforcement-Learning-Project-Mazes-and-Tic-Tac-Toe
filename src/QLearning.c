#include "mazeEnv.h"
#include "QLearning.h"
#include "functions.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>


//Initialise Q.
void MakeQ(){
   Q = malloc(rows*cols*sizeof(float*));
   //printf("Première malloc ok");

    for(int i=0; i<rows*cols; i++){
        Q[i] = malloc(4*sizeof(float));
        for(int j=0; j<4; j++){
            Q[i][j] = 0;
        }
    }
}

void freeQ(){
    for(int i=0; i<rows*cols; i++){
        free(Q[i]);
    }
    free(Q);
}

//Lit dans Q.
float Qread(int row, int col, int a){
    return(Q[row*cols + col][a]);
}

//Affiche Q.
void Qrender(){
    for (int i=0; i<rows*cols; i++) {
        for (int j=0; j< number_actions; j++){
             printf("%f ", Q[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

//Choix d'une action avec la politique epsilon-greedy.
action eps_greedy(){
    action act = env_action_sample();
    
    //Choix aléatoire
    if (rand()%101>eps*100){
        action act = env_action_sample();
        float rew = Qread(state_row, state_col, act);
        for (int k; k<4; k++){
            if (Qread(state_row, state_col, k) > rew){
                rew = Qread(state_row, state_col, k);
                act = k;
            }
        }
    }
    return(act);
}

//Fonction qui trouve la récompense maximale possible depuis la case donnée en argument.

int find_rew_max(int row, int col){
    /*
    float rew = Qread(row, col, 0);
    for (int k = 0; k<4; k++){
        float a = Qread(row, col, k);
        if (a > rew){
            rew = a;
        }
    }
    return (rew);
    */
    int var = row*cols+col;
    return (fmaxf(fmaxf(Q[var][0],Q[var][1]),fmaxf(Q[var][2],Q[var][3])));
}



//Fonction qui renvoie 1 si la case actuelle est un mur, 0 sinon.
int iswall(){
    int* current = &visited[state_row][state_col];
    if(*current == wall){
        return(1);
    }
    else{
        return(0);
    }
}

//Une boucle d'entraînement du programme.
void training (){
    //Re-génération des points visités
    init_visited();

    //Re-génération d'une clé pour l'aléatoire.
    srand(time(0));
    
    //Remise à zéro du labyrinthe
    maze_reset();
    
    //On compte les pas effectués pour sortir du labyrinthe.
    int step = 0;

    //Errance jusqu'à l'arrivée.
    int sortie = 0;
    while(sortie == 0){

        //Mémorisation de la position de départ.
        int old_row = state_row;
        int old_col = state_col;
        
        //Choix d'une action et déplacement
        action a = eps_greedy();
        envOutput state = maze_step(a);
        
        //Lecture de la récompense et ajustement si on rentre dans un mur.
        float reward = state.reward;
        int w = iswall();
        if (w == 1){
            reward = -0.05;
        }
        
        //Application de la formule.
        float prevision = find_rew_max(state_row, state_col);
        Q[old_row*cols + old_col][a] = Q[old_row*cols + old_col][a] + alp*(reward + gam*prevision - Q[old_row*cols + old_col][a]);

        //Si on rentre dans un mur annulation du déplacement.
        if (w ==1){
            state_col = old_col;
            state_row = old_row;
        }
        
        int* current = &visited[state_row][state_col];

        /*
        //Si on découvre une nouvelle case on affiche le labyrinthe avec les crumbs
        if (*current != crumb){
            printf("%d\n", sortie);
            add_crumbs();
            maze_render();
        }
        */

        *current = crumb;

        if ((state_row == goal_row)&&(state_col == goal_col)){
            sortie = 1;
        }
        step+=1;
    }
    printf("sortie trouvée en %d pas\n",step);
}

int main()
{
    maze_make("maze.txt");
    init_visited();
    MakeQ();
    
    Qrender();
    maze_render();
    
    for(int i = 0; i<iter; i++){
        printf("itération %d\n",i);
        training();
        printf("fin de l'itération %d\n",i);
        printf("Q : %f %f %f %f\n",Q[(goal_row)*cols+goal_col][0],Q[(goal_row)*cols+goal_col][1],Q[(goal_row)*cols+goal_col][2],Q[(goal_row)*cols+goal_col][3]);
    }

    //Dernier passage sans aléatoire.
    eps=0;
    training();
    
    Qrender();
    add_crumbs();
    maze_render();
    freeQ();
    
    return 0;
}
