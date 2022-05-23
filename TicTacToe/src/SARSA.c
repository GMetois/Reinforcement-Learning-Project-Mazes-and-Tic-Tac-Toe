#include "TTTEnv.h"
#include "SARSA.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>


//Initialise Q.
void MakeQ(){
   Q = malloc(number_states*sizeof(float*));
   //printf("Première malloc ok");

    for(int i=0; i<number_states; i++){
        Q[i] = malloc(9*sizeof(float));
        for(int j=0; j<number_actions; j++){
            Q[i][j] = 0;
        }
    }
}

//Libère Q.
void freeQ(){
    for(int i=0; i<number_states; i++){
        free(Q[i]);
    }
    free(Q);
}

//Convertit une configuration de jeu en une coordonnée de ligne pour Q.
int Qread(char** board){
    
    //Extraction d'un nombre ternaire correspondant à la configuration du plateau.
    int temp = 0;
    for(int i =0; i<3; i++){
        for(int j=0; j<3; j++){
            if(board[i][j]=='x'){
                temp += pow(10,(i*3)+j);
            }
            else if(board[i][j]=='o'){
                temp += 2*pow(10,(i*3)+j);
            }
        }
    }

    //Conversion du nombre ternaire en décimal pour obtenir la ligne de Q.
    int coordQ = 0;
    if(temp != 0){
        int i = 0;
        int remainder;
        while(temp!=0){
            remainder = temp % 10;
            temp /= 10;
            coordQ += remainder*pow(3,i);
            i++;
        }
    }
    return(coordQ);
}

//Affiche Q.
void Qrender(){
    for (int i=0; i<number_states; i++) {
        for (int j=0; j<number_actions; j++){
             printf("%f ", Q[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

//Une fonction qui joue un coup aléatoire avec le joueur "o"
int random_play(){
    
    //Choix d'une action aléatoire et vérification de la validité du coup.
    struct action act = env_action_sample('o');
    envOutput output = board_play(act, board);
    
    //Si le coup n'est pas valide, réessayer tant qu'il est valide.
    while(output.ok ==0){
    act = env_action_sample('o');
    output = board_play(act, board);
    }
    
    //S'il est valide le jouer, et renvoyer 1 si la partie se finit sur un coup de 'o'
    board_copy(board,output.future_board);
    return(is_winning('o',board));
}


//Choix d'une action avec la politique epsilon-greedy.
struct action eps_greedy(char** board){
    struct action act = env_action_sample('x');
    
    //Choix aléatoire
    if (rand()%101>eps*100){
        float rew = Q[Qread(board)][((act.x)*3+(act.y))];
        for (int k; k<number_actions; k++){
            if (Q[Qread(board)][k] > rew){
                rew = Q[Qread(board)][k];
                act.x = k/3;
                act.y = k%3;
            }
        }
    }
    return(act);
}

//Une boucle d'entraînement du programme.
void training (){

    //Re-génération d'une clé pour l'aléatoire.
    srand(time(NULL));
    
    //Remise à zéro du labyrinthe
    board_reset(board);
    board_render(board);
    
    //On compte le nombre de coups de la partie.
    int op = 0;

    //Un indicateur de si la partie est finie.
    int fini = 0;
        
    //Choix d'une action
    struct action a1 = eps_greedy(board);
        
    //Tant que la partie n'est pas finie.    
    while(fini == 0){
    
        //Mémorisation de l'état de départ.
        char** old_board = alloc_board();
        board_copy(old_board, board);
        
        //Jeu de l'action a1.
        envOutput new_state = board_play(a1, board);
        
        //Lecture de la récompense et ajustement si le coup joué n'est pas valable.
        float reward = new_state.reward;
        char** new_board = alloc_board();
        board_copy(new_board,new_state.future_board);
        int legit = new_state.ok;

        //Choix d'une action a2 pour SARSA.
        struct action a2 = eps_greedy(new_board);

        
        //Si le coup joué n'est pas valable on rechoisit des actions aléatoires jusqu'à en trouver une valable.
        while(legit == 0){
            
            //Grosse punition pour ne pas avoir joué un coup valable.
            reward = -50;
            
            //On mémorise la punition.
            int coorda1 = ((a1.x)*3)+a1.y;
            int coorda2 = ((a2.x)*3)+a2.y;
            Q[Qread(old_board)][coorda1] = Q[Qread(old_board)][coorda1] + alp*(reward + gam*Q[Qread(new_board)][coorda2] - Q[Qread(old_board)][coorda1]);
            
            //On choisit au hasard un autre coup.
            action a1 = env_action_sample('x');
            
            //On vérifie si ce coup est valable, et on se prépare à procéder comme d'habitude.
            envOutput new_state = board_play(a1, board);
            reward = new_state.reward;
            board_copy(new_board,new_state.future_board);
            legit = new_state.ok;

        }
        
        //Conversion des actions choisies en coordonnées lisibles pour Q.
        int coorda1 = ((a1.x)*3)+a1.y;
        int coorda2 = ((a2.x)*3)+a2.y;
        
        //Application de la formule.
        Q[Qread(old_board)][coorda1] = Q[Qread(old_board)][coorda1] + alp*(reward + gam*Q[Qread(new_board)][coorda2] - Q[Qread(old_board)][coorda1]);

        //Si le coup est légal, on actualise le plateau de jeu.
        if (legit==1){
            board_copy(board,new_board);
            board_render(board);
        }
        else {
            printf("Tour non valide! Pour la peine, tu ne joues pas!\n");
        }
        printf("Fin du tour du joueur \n");
        
        //choix de l'action a2
        a1 = a2;

        if (is_winning('x',board)==1){
            fini = 1;
            printf("victoire du joueur\n");
            op+=1;
        }
        else if (is_draw(board)==1){
            fini = 1;
            printf("match nul\n");
            op+=1;
        }
        else{
            //Jeu aléatoire de l'adversaire.
            int temp_win = random_play();
            board_render(board);
            printf("fin du tour de l'adversaire\n");

            if(temp_win == 1){
                fini = 1;
                printf("victoire de l'adversaire\n");
            }
            else if (is_draw(board)){
                fini = 1;
                printf("match nul\n");
            }
        op+=1;
        }

        //Libération des tableaux temporaires utilisés pour le calcul.
        free_board(new_state.future_board);
        free_board(old_board);
        free_board(new_board);
    }
    printf("Partie jouée en %d coups\n",op);
}

int main()
{
    eps = eps_beginning;
    MakeQ();
    Qrender();

    //Génération d'un tableau de jeu courant.
    board = alloc_board();
    
    board_render(board);
    
    for(int i = 0; i<iter; i++){
        printf("itération %d avec epsilon = %f\n",i, eps);
        eps = -((eps_beginning)/iter)*i + eps_beginning;
        training();
        printf("fin de l'itération %d\n",i);
    }

    //Dernier passage sans aléatoire.
    eps=0;
    training();
    board_render(board);
    freeQ();
    return 0;
}