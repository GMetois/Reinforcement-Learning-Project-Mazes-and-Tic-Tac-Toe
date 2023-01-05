# Reinforcement Learning Project : Mazes and Tic-Tac-Toe

Here's our repository dedicated to Reinforcement Learning. A small project will be conducted and used as a course project at ENSTA Paris.

We aim here to implement QLearning algorithm in order to exit a maze first, then improve it with SARSA or DOUBLE Q algorithm for more efficiency.

Then, we will try to adapt our algorith in order to create a tic-tac-toe bot.

You'll find here several folders :

/Include is for the headers

/src is for the algorithms and the environnements by themselves.

There's 2 src : due to time constraints, we couldn't properly work on a clear distinction between environnement files and algorithms and factorize our code. We created a second src for the tic-tac-toe where some files may be slightly different from the maze counterpart. You'll find it in the folder "TicTacToe" where there is the same workspace configuration as mentionned above.

How to execute the program : Simply go in the src folder and launch ./name_of_the_program. if you can't find the executable, it might be necessary to rebuil it using the makefile : you can easily do it by returning in the parent folder and execute the makefile using make.
