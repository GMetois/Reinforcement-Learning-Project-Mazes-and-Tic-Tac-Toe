#ifndef QLEARNING_H
#define QLEARNING_H

#include <stdio.h>
#include <stdlib.h>

float** Q;
float alp = 0.2;
float eps = 0.2;
float gam = 0.2;
int iter = 10;

void MakeQ();

void training();

void add_crumbs();

#endif
