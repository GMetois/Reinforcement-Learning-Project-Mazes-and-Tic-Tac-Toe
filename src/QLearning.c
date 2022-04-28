#include "mazeEnv.h"
#include "QLearning.h"
#include "functions.h"
#include <stdlib.h>
#include <time.h>


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
    float a = (rand())/(RAND_MAX);
    action act = env_action_sample();
    
    //Choix aléatoire
    if (a > eps){
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
    float rew = Qread(row, col, 0);
    for (int k = 0; k<4; k++){
        float a = Qread(row, col, k);
        if (a > rew){
            rew = a;
        }
    }
    return (rew);
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
            reward = -50;
        }
        
        //Application de la formule.
        float prevision = find_rew_max(state_row, state_col); 
        Q[old_row*cols + old_row][a] += alp*(reward + gam*prevision - Q[old_row*cols + old_row][a]);

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
    }
    
    //Qrender();
    //add_crumbs();
    //maze_render();
    
    return 0;
}
