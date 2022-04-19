#ifndef QLEARNING_H
#define QLEARNING_H

#include <stdio.h>
#include <stdlib.h>

char** Q;
float alp;
float eps;
float gam;
int iter;

void MakeQ();

void training();

void add_crumbs();

#endif
