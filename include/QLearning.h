#ifndef QLEARNING_H
#define QLEARNING_H

#include <stdio.h>
#include <stdlib.h>

float** Q;
float alp = 0.5;
float eps = 0.5;
float gam = 0.5;
int iter = 20000;

void MakeQ();

void training();

void add_crumbs();

#endif
