#include "mazeEnv.h"
#include "QLearning.h"
#include "functions.h"

void MakeQ(){
   Q = malloc(rows*cols*sizeof(char*));
   //printf("Première malloc ok");

    for(int i=0; i<rows*cols; i++){
        Q[i] = malloc(4*sizeof(int));
        for(int j=0; j<4; j++){
            Q[i][j] = 0;
        }
        //printf("Malloc ligne %d ok", i);
    }
    //printf("Q créée");
}

void training (){
    for (int i=0; i<iter; i++){
        //On se replace à la position de départ.
        maze_reset();
        printf("Boucle %d\n", i);
        action a = env_action_sample();
        printf("action choisie\n");
        //On parcourt le labyrinthe
        while(!((state_row == goal_row) && (state_col == goal_col))){
    
            envOutput temp_output = maze_step(a);
            printf("pas fait\n");
            int rew = temp_output.reward;
            int temp_col = temp_output.new_col;
            int temp_row = temp_output.new_row;
            printf("colonne %d, ligne %d\n", temp_col, temp_row);

            action a2 = env_action_sample();
            printf("action choisie a2\n");
            
            //Si on ne rentre pas dans un mur
            int* current = &visited[temp_output.new_row][temp_output.new_col];
            if((*current) == !(wall)) { //ON NE PASSE PAS LA CASE EN CONNUE, à proprement parler on s'en fout de savoir si la case est connue, ça évite juste de se prendre un mur.
                printf("On est pas dans un mur\n");
                
                Q[state_row*cols + state_col][a]+= alp*(rew + gam*Q[temp_row*cols + temp_col][a2] - Q[state_row*cols + state_col][a]);
                printf("formule appliquée\n");
            
            
                //On se déplace
                state_col = temp_output.new_col;
                state_row = temp_output.new_row;

                //on choisit l'action a2
                a = a2;
                printf("déplacement fait\n");
            }


        }
        // ON FAIT UNE DERNIERE ITERATION POUR AFFICHER LE LABYRINTHE, LA ON PASSE LES CASES EN KNOWN

        //On se replace à la position de départ.
        maze_reset();
        //On parcourt le labyrinthe
        while(!((state_row == goal_row) && (state_col == goal_col))){
            
            //On fait une action et en tire une nouvelle future position
            action a = env_action_sample();
            envOutput temp_output = maze_step(a);
            int rew = temp_output.reward;
            int temp_col = temp_output.new_col;
            int temp_row = temp_output.new_row;
            
            action a2 = env_action_sample();
            //Si on ne rentre pas dans un mur
            int* current = &visited[temp_output.new_row][temp_output.new_col];
            if((*current) == (unknown)) {

                //La on passe la case en known pour les besoins d'affichage
                *current = known;
                
                Q[state_row*cols + state_col][a]+= alp*(rew + gam*Q[temp_row*cols + temp_col][a2] - Q[state_row*cols + state_col][a]);
                
                
                //On se déplace
                state_col = temp_output.new_col;
                state_row = temp_output.new_row;

                //on prend l'action a2
                a = a2;
            }
        }
    }
}



void add_crumbs(){
     for (int i=0; i<rows; i++){
          for (int j=0; j<cols; j++){
              if (visited[i][j] ==crumb){
                  maze[i][j] ='.';
              }
          }
     }
     maze[start_row][start_col]= 's';
}

int main(){
    //printf("programme lancé");
    iter = 1000;
    maze_make("maze.txt");
    //printf("Labyrinthe extrait");
    init_visited();
    //printf("Matrice visitée ok");
    MakeQ();
    printf("%d, %d \n", rows, cols);
    printf("number of actions :  %d \n", number_actions); 
    maze_render();
    training();
    add_crumbs();
    maze_render();
    return 0;
}
