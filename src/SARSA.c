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
    for(int i=0; i<rows*cols; i++){

    }
}


//training
void training (){
    for (int i=0; i<iter; i++){
        maze_reset();
        action = env_action_sample(); //choose action from s using policy derived from Q
        for (int j=0; j<ddv; j++){
            tempoutput = maze_step(action); //take action a
            envOuput observe = mazestep(action);
            int reward = observe.reward;
            action2 = env_action_sample(); //choose action from s' using policy derived from Q
            Q[ state_row*cols + state_col][action] = Q[ state_row*cols + state_col][action] + alpha*(reward + gamma*Q[observe.new_row*cols + observe.new_col][action2] - Q[ state_row*cols + state_col][action]); //gamma!!
            state_row = observe.new_row;
            state_col = observe.new_col;
            action = action2;
        }
    }
}



int main(){
    alpha = 0.2;
    epsilon = 0.2;
    gamma = 0.2;
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
