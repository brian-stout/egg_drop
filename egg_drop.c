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

    //Setting up stat tracking
    size_t dropNumber = 0;
    size_t eggNumber = 1;

    //Setting variables to determine what floors to test
    double stepAmount = ceil(floorAmount/2);
    size_t testFloor = stepAmount;

    //Setting variables to keep track of left and right bounds
    size_t lastKnownSafe = 0;
    size_t lastKnownDangerous = floorAmount + 1;

    //Make our egg to be tested.
    egg *testEgg = lay_egg();

    printf("Dropping egg#1 from floor:\n"); //Preface never changes
    while(true)
    {
        //Everytime loop the amount of options are halved so the stepAmount
        // is as well.
        stepAmount = ceil(stepAmount/2);

        //Time to drop the egg
        egg_drop_from_floor(testEgg, testFloor);
        dropNumber++;  //Keeping track of which test number this is

        //If the egg breaks then the floor is in the lower half of our data
        if(egg_is_broken(testEgg))
        {
            printf("\t#%zd CRACK\n", testFloor);

            //Moving the right bounds to the left
            lastKnownDangerous = testFloor;

            //Have to remake the egg since there's no way to fix the egg
            destroy_egg(testEgg);

            //This is a logic test to determine if the highest possible safe floor
            //  has been found. Basically the L and R bounds are next to eachother.
            if(testFloor-lastKnownSafe == 1) { break; }

            //Make a new egg to use later
            testEgg = lay_egg();
            eggNumber += 1;

            //Shows user which egg we're on
            printf("Dropping egg#%zd from floor:\n", eggNumber);

            //If the egg broke we need to start at the left bound again
            //  with our halved stepAmount
            testFloor = lastKnownSafe + stepAmount;
        }
        //If the egg is safe our floor is in the higher half of our data
        else
        {
            printf("#%zd safe\n", testFloor);

            //This is a logic test to determine if the highest possible safe floor
            //  has been found. Basically the L and R bounds are next to eachother.
            if(lastKnownDangerous-testFloor == 1)
            {
                //The egg hasn't been cracked but no more testing is being done
                //  so the memory needs to be free'd manually
                destroy_egg(testEgg);
                break;
            }
        
            //Move the left bounds further to the right
            lastKnownSafe = testFloor;

            //Move further to the right
            testFloor += stepAmount;
        }

    }

    //Determines if the eggs can't survive a fall from any height
    if(lastKnownDangerous - 1 == 0)
    {
        printf("There is no safe floor to drop the eggs from.\n");
        printf("We determined the obvious after %zd drops.\n", dropNumber);
    }
    else
    {
        printf("The maximum safe floor is %zd, found after %zd drops\n",
                lastKnownDangerous - 1, dropNumber);
    }
    testEgg = NULL; //Set test to null cause paranoid
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

    /*  The worst_cast() function determines the mininum number of drops it would take
    *        to determine the highest safest floor in the worst case scenario.
    *        We step by this number to have an optimized algorithm for the worst case
    *        scenario.
    *
    *    We multiple that value by eggAmount - 1 because with multiple eggs we can be
    *        "riskier" and divide the data into smaller subsets.  The code isn't
    *        mathetmatically optimal and the search can be more efficient.
    *        The multiplaction provide linear growth with amount of eggs.
    */
    int stepAmount = (eggAmount - 1 ) * worst_case((double)floorAmount);
    testFloor += stepAmount;

    //Make our egg to be tested
    egg *testEgg = lay_egg();

    printf("Dropping egg#1 from floor:\n"); //preface never changes.
    while(true)
    {
        //Checks to see if we stepped past our fixed right limit
        if(testFloor > floorAmount)
        {
            testFloor = floorAmount;
        }
        
        //Time to drop our egg
        egg_drop_from_floor(testEgg, testFloor);
        dropNumber++;

        //If egg is broken we've found the subset that contains the floor being
        //  searched for
        if(egg_is_broken(testEgg))
        {
            printf("#%zd CRACK\n", testFloor);

            //Moves our right limit to the left
            lastKnownDangerous = testFloor;

            //Destroy the egg so we can remake it later
            destroy_egg(testEgg);
            eggAmount--;

            //This is a logic test to determine if the highest possible safe floor
            //  has been found. Basically the L and R bounds are next to eachother.
            if(testFloor-lastKnownSafe == 1)
            {
                break;
            }

            //Recreate our egg
            testEgg = lay_egg();
            eggNumber += 1;

            printf("Dropping egg#%zd from floor:\n", eggNumber);
            if(eggAmount > 1)
            {
                //Calculates a new worst case for the range of the remaining data
                stepAmount = worst_case((double)lastKnownDangerous - lastKnownSafe);

                //Linear growth
                stepAmount *= (eggAmount - 1);
            }
            //If there's only one egg we need to step by one to brute force
            //  the remaining floors
            else
            {
                stepAmount = 1;            
            }

            //Moving back to the left limit
            testFloor = lastKnownSafe + stepAmount;

        }
        //If the egg is safe we need to move our search to the right
        else
        {
            printf("#%zd safe\n", testFloor);
            
            //This is a logic test to determine if the highest possible safe floor
            //  has been found. Basically the L and R bounds are next to eachother.
            if(lastKnownDangerous-testFloor == 1)
            {
                destroy_egg(testEgg);
                break;
            }
            //Moving our left limits to the right
            lastKnownSafe = testFloor;
            testFloor+= stepAmount;
        }   
    }
    //Determines if the eggs can't survive a fall from any height
    if(lastKnownDangerous - 1 == 0)
    {
        printf("There is no safe floor to drop the eggs from.\n");
        printf("We determined the obvious %zd drops.\n", dropNumber);
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
