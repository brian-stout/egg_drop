#include <stdio.h>

#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <sysexits.h>
#include <errno.h>
#include <math.h>
#include "egg.h"


int worst_case(double floorAmount)
{
    return (int)ceil((sqrt(1+8*floorAmount)-1)/2);
}

void binary_egg_drop(size_t floorAmount)
{
    //TODO: Calculate jump amount with lastknownsafe and lastknowndangerous
    //          instead of using a seperate double value

    egg *testEgg = lay_egg();
    size_t dropNumber = 0;
    size_t eggNumber = 1;
    double jumpAmount = ceil(floorAmount/2);
    size_t testFloor = jumpAmount;
    size_t lastKnownSafe = 0;
    size_t lastKnownDangerous = floorAmount + 1;

    printf("Dropping egg#1 from floor:\n");
    while(true)
    {
        jumpAmount = ceil(jumpAmount/2);
        egg_drop_from_floor(testEgg, testFloor);
        dropNumber++;

        if(egg_is_broken(testEgg))
        {
            printf("#%zd CRACK\n", testFloor);
            lastKnownDangerous = testFloor;
            destroy_egg(testEgg);
            if(testFloor-lastKnownSafe == 1) { break; }

            testEgg = lay_egg();
            eggNumber += 1;

            printf("Dropping egg#%zd from floor:\n", eggNumber);
            testFloor = lastKnownSafe + jumpAmount;
        }
        else
        {
            printf("#%zd safe\n", testFloor);
            if(lastKnownDangerous-testFloor == 1)
            {
                destroy_egg(testEgg);
                break;
            }
            lastKnownSafe = testFloor;
            testFloor += jumpAmount;
        }

    }
    if(lastKnownDangerous - 1 == 0)
    {
        printf("There is no safe floor to drop the eggs from.\n");
        printf("Eggs should be eaten, not dropped.\n");
    }
    else
    {
        printf("The maximum safe floor is %zd, found after %zd drops\n",
                lastKnownDangerous - 1, dropNumber);
    }
    testEgg = NULL;
}


void drop_eggs(size_t eggAmount, size_t floorAmount)
{

    //Setting up stat tracking
    size_t dropNumber = 0;
    size_t eggNumber = 1;

    //Setting variables to keep track of left and right bounds
    size_t lastKnownSafe = 0;
    size_t testFloor = 0;
    size_t lastKnownDangerous = floorAmount + 1;

    //Setting up variables to keep track of how much floor you should move up by

    int stepAmount = (eggAmount - 1 ) * worst_case((double)floorAmount);
    testFloor += stepAmount;

    
    egg *testEgg = lay_egg();

    printf("Dropping egg#1 from floor:\n");
    while(true)
    {
        if(testFloor > floorAmount)
        {
            testFloor = floorAmount;
        }
        egg_drop_from_floor(testEgg, testFloor);
        dropNumber++;

        if(egg_is_broken(testEgg))
        {
            printf("#%zd CRACK\n", testFloor);
            lastKnownDangerous = testFloor;

            destroy_egg(testEgg);
            eggAmount--;

            if(testFloor-lastKnownSafe == 1) { break; }

            testEgg = lay_egg();
            eggNumber += 1;

            printf("Dropping egg#%zd from floor:\n", eggNumber);
            if(eggAmount > 1){
                stepAmount = worst_case((double)lastKnownDangerous - lastKnownSafe);
                stepAmount *= (eggAmount - 1);
            }
            else
            {
                stepAmount = 1;            
            }
            testFloor = lastKnownSafe + stepAmount;

        }
        else
        {
            printf("#%zd safe\n", testFloor);
            if(lastKnownDangerous-testFloor == 1)
            {
                destroy_egg(testEgg);
                break;
            }
            lastKnownSafe = testFloor;
            testFloor+= stepAmount;
        }   
    }
    if(lastKnownDangerous - 1 == 0)
    {
        printf("There is no safe floor to drop the eggs from.\n");
        printf("Eggs should be eaten, not dropped.\n");
    }
    else
    {
        printf("The maximum safe floor is %zd, found after %zd drops\n",
                lastKnownDangerous - 1, dropNumber);
    }
    testEgg = NULL;
}


int main( int argc, char *argv[] )
{
    long int floorAmount;
    long int eggAmount;

    //Character pointers used in strtol later to detect alpha numeric characters
    char *floorStr, *eggStr, *endptr;

    //Setting arugments to the pointers
    floorStr = argv[1];
    eggStr = argv[2];

    //If no arguments are used, default to these values
    if (argc == 1)
    {
        floorAmount = 100;
        eggAmount = 2;
    }
    //If one argument is use, find the data for the floor
    else if (argc == 2)
    {
        floorAmount = strtol(floorStr, &endptr, 10);
        //If a user inputs alphabet, endptr wont be a null byte
        if(*endptr != '\0')
        {
            printf("Invalid characters!\n");
            return EX_USAGE;
        }
        eggAmount = 2;        
    }
    //If two arguments are used, it's the floorAmount then the eggAmount
    else if (argc == 3)
    {
        floorAmount = strtol(floorStr, &endptr, 10);
        //Detecting alphabet
        if(*endptr != '\0')
        {
            printf("Invalid characters!\n");
            return EX_USAGE;
        }
        eggAmount = strtol(eggStr, &endptr, 10);
        if(*endptr != '\0')
        {
            printf("Invalid characters!\n");
            return EX_USAGE;
        }
    }
    //If argc is greater than 3 than the user inputed too many options
    else
    {
        printf("ERROR, USAGE: [./egg_drop] [<number of floors>] [<number of eggs>]\n");
        return EX_USAGE;
    }

    //Checks to see if the inputed number exceeds the limit for strtol
    if(floorAmount == LONG_MAX || floorAmount == LONG_MIN ||
       eggAmount == LONG_MAX || eggAmount == LONG_MIN)
    {
        perror("Invalid number");
        return EX_DATAERR;
    }
    if(eggAmount <= 0 || floorAmount <= 0)
    {
        printf("Input positive numbers only!\n");
        return EX_DATAERR;
    }

    //Determines if the program has enough eggs to do a binary drop
    if(eggAmount > log2(floorAmount)){
        binary_egg_drop(floorAmount);
    }
    //If it doesn't, run the regular program
    else{
        drop_eggs(eggAmount, floorAmount);
    }
}
