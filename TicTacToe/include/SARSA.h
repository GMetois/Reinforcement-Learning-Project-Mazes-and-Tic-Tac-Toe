#ifndef SARSA_H
#define SARSA_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

char** board;
float** Q;
float alp = 0.8;
float eps_beginning = 0.1;
float eps;
float gam = 0.8;
int iter = 100000;

int number_actions = 9;
int number_states = pow(3,9);

void MakeQ();

void freeQ();

int Qread();

void Qrender();

int random_play();

struct action eps_greedy(char** board);

void training();

#endif
