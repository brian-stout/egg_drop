#ifndef SEARCH_H
#define SEARCH_H

#include <stdio.h>

#include <stdbool.h>
#include <math.h>
#include "egg.h"

/**	worst_case() uses a simplified quadratic equation to a solve a summation
*       that gives the largest number of drops required for the worst case search
*       the equation solves a summation that determines the lowest sum of the number
*       of sets the data is being split into and the number of iterations required to
*       solve that set
*/
int worst_case(double floorAmount);

/**	in the case that there is enough eggs to run a Binary Search instead of the quadratic
*       solution the program runs this function.  The function splits the data in half
*       during each loop till it finds the highest safest floor.
*
*   The function doesn't need to keep track of eggAmount because it wont run if there
*       isn't enough eggs to solve the problem to reach the bottom of the "BST"
*/
void binary_egg_drop(size_t floorAmount);



/**	drop_eggs() is ran if a binary search isn't possible
*       It uses n(n+1)/2 <= Number of floors equation to determine the worst case
*        and steps by that
*
*        because of this the function is faster than Binary Search in the worst case
*        but because there's a limited amount of eggs available it's usually the optimal
*        solution.
*
*    The step amount has a linear growth with the egg amount.  It's not a perfect
*    solution but is adequate.
*/
void drop_eggs(size_t eggAmount, size_t floorAmount);

#endif
