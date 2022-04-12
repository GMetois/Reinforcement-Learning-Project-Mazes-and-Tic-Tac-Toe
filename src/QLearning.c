#include "mazeEnv.h"

void MakeQ(){
    Q = malloc(rows*cols*sizeof(char*));

    for(int i=0; i<rows*cols; i++){
        Q[i] = malloc(4*sizeof(int));
    }
    for(int i=0; i<rows*cols; i++){
        for(int j=0; j<4; j++){
        Q[i][j] = 0;
        }
    }
}

void training (){
    for (int i=0; i<iter; i++){
        state_col = start_col;
        state_row = state_row;
        for (int j=0; j<ddv; j++){
            
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
