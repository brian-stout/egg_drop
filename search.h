#ifndef SEARCH_H 
#define SEARCH_H 

#include <stdio.h>

#include <stdbool.h>
#include <math.h>
#include "egg.h"

int worst_case(double floorAmount);

void binary_egg_drop(size_t floorAmount);

void drop_eggs(size_t eggAmount, size_t floorAmount);

#endif
