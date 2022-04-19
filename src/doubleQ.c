#include "mazeEnv.h"
#include "QLearning.h"
#include "functions.h"
#include <time.h>

void MakeQ1(){
   Q1 = malloc(rows*cols*sizeof(char*));
   //printf("Première malloc ok");

    for(int i=0; i<rows*cols; i++){
        Q1[i] = malloc(4*sizeof(int));
        for(int j=0; j<4; j++){
            Q1[i][j] = 0;
        }
        //printf("Malloc ligne %d ok", i);
    }
    //printf("Q créée");
}
void MakeQ2(){
   Q2 = malloc(rows*cols*sizeof(char*));
   //printf("Première malloc ok");

    for(int i=0; i<rows*cols; i++){
        Q2[i] = malloc(4*sizeof(int));
        for(int j=0; j<4; j++){
            Q2[i][j] = 0;
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
        //On parcourt le labyrinthe
        while(!((state_row == goal_row) && (state_col == goal_col))){
            
            //On fait une action et en tire une nouvelle future position
            action a = env_action_sample();  //USING THE POLICY EPS-GREEDY IN Q1+Q2
            printf("action choisie\n");

            envOutput temp_output = maze_step(a);
            printf("pas fait\n");
            int rew = temp_output.reward;
            int temp_col = temp_output.new_col;
            int temp_row = temp_output.new_row;
            printf("colonne %d, ligne %d\n", temp_col, temp_row);
            
            //Si on ne rentre pas dans un mur
            int* current = &visited[temp_output.new_row][temp_output.new_col];
            if((*current) == !(wall)) { //ON NE PASSE PAS LA CASE EN CONNUE, à proprement parler on s'en fout de savoir si la case est connue, ça évite juste de se prendre un mur.
                printf("On est pas dans un mur\n");
                //On trouve tous les rewards potentiels depuis cette future position et on prend le max
                
                //on modifie aléatoirement un nombre entre 0 et 1
                int nb = rand()&1;

                if (nb = 0){
                    int prev_rew = Q2[temp_row*cols + temp_col][0];
                    printf("reward trouvé\n");
                    for (int k = 0; k<4; k++){
                        prev_rew = max(prev_rew, Q2[temp_row*cols + temp_col][k]);
                    }
                    printf("Maximum trouvé\n");

                    Q1[state_row*cols + state_col][a]+= alp*(rew + gam*prev_rew - Q1[state_row*cols + state_col][a]);
                    printf("formule appliquée\n");
            
                }

                else{
                    int prev_rew = Q1[temp_row*cols + temp_col][0];
                    printf("reward trouvé\n");
                    for (int k = 0; k<4; k++){
                        prev_rew = max(prev_rew, Q1[temp_row*cols + temp_col][k]);
                    }
                    printf("Maximum trouvé\n");

                    Q2[state_row*cols + state_col][a]+= alp*(rew + gam*prev_rew - Q2[state_row*cols + state_col][a]);
                    printf("formule appliquée\n");
            
                }

                //On se déplace
                state_col = temp_output.new_col;
                state_row = temp_output.new_row;
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
            
            //Si on ne rentre pas dans un mur
            int* current = &visited[temp_output.new_row][temp_output.new_col];
            if((*current) == (unknown)) {

                //La on passe la case en known pour les besoins d'affichage
                *current = known;

                //on choisit aléatoirement
                int nb = rand()&1;

                if (nb=0){
                    //On trouve tous les rewards potentiels depuis cette future position et on prend le max
                    int prev_rew = Q2[temp_row*cols + temp_col][0];
                    for (int k = 0; k<4; k++){
                        prev_rew = max(prev_rew, Q2[temp_row*cols + temp_col][k]);
                    }
                
                    Q1[state_row*cols + state_col][a]+= alp*(rew + gam*prev_rew - Q1[state_row*cols + state_col][a]);


                }

                else{
                    //On trouve tous les rewards potentiels depuis cette future position et on prend le max
                    int prev_rew = Q1[temp_row*cols + temp_col][0];
                    for (int k = 0; k<4; k++){
                        prev_rew = max(prev_rew, Q1[temp_row*cols + temp_col][k]);
                    }
                
                    Q2[state_row*cols + state_col][a]+= alp*(rew + gam*prev_rew - Q2[state_row*cols + state_col][a]);

                }

                //On se déplace
                state_col = temp_output.new_col;
                state_row = temp_output.new_row;
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
    MakeQ1();
    MakeQ1();
    printf("%d, %d \n", rows, cols);
    printf("number of actions :  %d \n", number_actions); 
    maze_render();
    training();
    add_crumbs();
    maze_render();
    return 0;
}