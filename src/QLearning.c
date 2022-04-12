#include "mazeEnv.h"

void MakeQ(){
    Q = malloc(rows*cols*sizeof(char*));

    for(int i=0; i<rows*cols; i++){
        Q[i] = malloc(4*sizeof(int));
        for(int j=0; j<4; j++){
            Q[i][j] = 0;
        }
    }
}

void training (){
    for (int i=0; i<iter; i++){
        //On se replace à la position de départ.
        maze_reset();
        //On parcourt le labyrinthe
        while(!((state_row == goal_row) && (state_col == goal_col))){
            
            //On fait une action et en tire une nouvelle future position
            a = env_action_sample();
            temp_output = maze_step(a);
            int rew = temp_output.reward;
            int temp_col = temp_output.new_col;
            int temp_row = temp_output.new_row;
            
            //Si on ne rentre pas dans un mur
            int* current = &visited[temp_output.new_row][temp_output.new_col];
            if(*current = unknown) { //ON NE PASSE PAS LA CASE EN CONNUE, à proprement parler on s'en fout de savoir si la case est connue, ça évite juste de se prendre un mur.
            
                //On trouve tous les rewards potentiels depuis cette future position et on prend le max
                int prev_rew = Q[temp_row*cols + temp_col][0];
                for (int k = 0; k<4; k++){
                    prev_rew = max(prev_rew, Q[temp_row*cols + temp_col][k]);
                }

                Q[state_row*cols + state_col][a]+= alpha*(rew + gamma*prev_rew - Q[state_row*cols + state_col][a]);
            
                /* //On remplit la matrice Q à la position initiale et l'action choisie
                if (a==up){
                    Q[state_row*cols + state_col][0]+= alpha*(rew + gamma*prev_rew - Q[state_row*cols + state_col][0]);
                }else if (a==down){
                    Q[state_row*cols + state_col][2]+= alpha*(rew + gamma*prev_rew - Q[state_row*cols + state_col][2]);
                }else if (a==right){
                    Q[state_row*cols + state_col][1]+= alpha*(rew + gamma*prev_rew - Q[state_row*cols + state_col][1]);
                }else if (a==left){
                    Q[state_row*cols + state_col][3]+= alpha*(rew + gamma*prev_rew - Q[state_row*cols + state_col][3]);
                }*/

                //On se déplace
                state_col = temp_output.new_col;
                state_row = temp_output.new_row;
            }


        }
        // ON FAIT UNE DERNIERE ITERATION POUR AFFICHER LE LABYRINTHE, LA ON PASSE LES CASES EN KNOWN

        //On se replace à la position de départ.
        maze_reset();
        //On parcourt le labyrinthe
        while(!((state_row == goal_row) && (state_col == goal_col))){
            
            //On fait une action et en tire une nouvelle future position
            a = env_action_sample();
            temp_output = maze_step(a);
            int rew = temp_output.reward;
            int temp_col = temp_output.new_col;
            int temp_row = temp_output.new_row;
            
            //Si on ne rentre pas dans un mur
            int* current = &visited[temp_output.new_row][temp_output.new_col];
            if(*current = unknown) {

                //La on passe la case en known pour les besoins d'affichage
                *current = known;

                //On trouve tous les rewards potentiels depuis cette future position et on prend le max
                int prev_rew = Q[temp_row*cols + temp_col][0];
                for (int k = 0; k<4; k++){
                    prev_rew = max(prev_rew, Q[temp_row*cols + temp_col][k]);
                }
                
                Q[state_row*cols + state_col][a]+= alpha*(rew + gamma*prev_rew - Q[state_row*cols + state_col][a]);

                /*
                //On remplit la matrice Q à la position initiale et l'action choisie
                if (a==up){
                    Q[state_row*cols + state_col][0]+= alpha*(rew + gamma*prev_rew - Q[state_row*cols + state_col][0]);
                }else if (a==down){
                    Q[state_row*cols + state_col][2]+= alpha*(rew + gamma*prev_rew - Q[state_row*cols + state_col][2]);
                }else if (a==right){
                    Q[state_row*cols + state_col][1]+= alpha*(rew + gamma*prev_rew - Q[state_row*cols + state_col][1]);
                }else if (a==left){
                    Q[state_row*cols + state_col][3]+= alpha*(rew + gamma*prev_rew - Q[state_row*cols + state_col][3]);
                }
                */

                //On se déplace
                state_col = temp_output.new_col;
                state_row = temp_output.new_row;
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
    alpha = 0.2;
    epsilon = 0.2;
    float gamma = 0.2;
    int iter = 1000;

    maze_make("maze.txt");
    init_visited();
    MakeQ();
    printf("%d, %d \n", rows, cols);
    printf("number of actions :  %d \n", number_actions); 
    maze_render();
    training();
    add_crumbs();
    maze_render();
    return 0;
}
