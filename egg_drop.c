#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <sysexits.h>
#include <errno.h>
#include <math.h>
#include "egg.h"
#include "search.h"

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
