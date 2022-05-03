#ifndef QLEARNING_H
#define QLEARNING_H

#include <stdio.h>
#include <stdlib.h>

float** Q;
float alp = 0.8;
float eps = 0.1;
float gam = 0.8;
int iter = 50000;

void MakeQ();

void training();

void add_crumbs();

#endif
