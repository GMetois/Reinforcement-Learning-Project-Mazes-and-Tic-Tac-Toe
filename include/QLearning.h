#ifndef QLEARNING_H
#define QLEARNING_H

#include <stdio.h>
#include <stdlib.h>
#include <mazeEnv.h>

float** Q;
float alp = 0.8;
float eps_beginning = 0.1;
float eps;
float gam = 0.8;
int iter = 100000;

void MakeQ();

void freeQ();

void Qrender();

action eps_greedy();

float find_new_reward(int row, int col);

int iswall();

void training();

#endif
