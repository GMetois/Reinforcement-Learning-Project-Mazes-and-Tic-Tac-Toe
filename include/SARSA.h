#ifndef SARSA_H
#define SARSA_H

#include <stdio.h>
#include <stdlib.h>

float** Q;
float alp = 0.8;
float eps_beginning = 0.1;
float eps;
float gam = 0.8;
int iter = 100000;

void MakeQ();

void training();

void add_crumbs();

#endif
