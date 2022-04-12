#include "mazeEnv.h"

//initialisation à 0 partout
void MakeQ(){
    Q = malloc(rows*cols*sizeof(char*)); //création de listes pour remplir les récompenses par action

    for(int i=0; i<rows*cols; i++){
        Q[i] = malloc(4*sizeof(int)); //allocation de place pour les listes
        for(int j=0; j<4; j++){
            Q[i][j] = 0; //initialisation du tableau à 0
        }
    }
}


//training
void training (){
    for (int i=0; i<iter; i++){
        maze_reset(); //on se replace à la position de départ
        a = env_action_sample(); //choose action from s using policy derived from Q
        
        //On parcourt le labyrinthe
        while (!((state_row == goal_row)&&(state_col == goal_col))){
            temp_output = maze_step(a); //on oberve le nouvel état et la récompense
            int rew = temp_output.reward;
            int temp_col = temp_output.new_col;
            int temp_row = temp_output.new_row;
            a2 = env_action_sample(); //choose action from s' using policy derived from Q
            
            //On remplit la matrice Q à la position initiale et les actions choisies
            Q[state_row*cols + state_col][a]+= alpha*(rew + gamma*(Q[temp_row*cols + temp_col][a2]) - Q[state_row*cols + state_col][a]);
           
            //On se déplace
            state_row = temp_row;
            state_col = temp_col;
            //on choisit l'action telle que précedemment
            action = action2; 
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
    dfs(start_row,start_col);
    add_crumbs();
    maze_render();
    return 0;
}
