#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sysexits.h>
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
    size_t lastKnownDangerous = 0;

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
            if(lastKnownDangerous-testFloor == 1) { break; }
            lastKnownSafe = testFloor;
            testFloor += jumpAmount;
        }

    }
    printf("The maximum safe floor is %zd, found after %zd drops\n", lastKnownSafe, dropNumber);
}


void drop_eggs(size_t eggAmount, size_t floorAmount)
{

}


int main( int argc, char *argv[] )
{
    size_t floorAmount;
    size_t eggAmount;

    //TODO: ADD ERROR HANDLING FOR ARGUMENTS
    if (argc == 1)
    {
        floorAmount = 100;
        eggAmount = 2;
    }
    else if (argc == 2)
    {
        floorAmount = atoi(argv[1]);
        eggAmount = 2;        
    }
    else if (argc == 3)
    {
        floorAmount = atoi(argv[1]);
        eggAmount = atoi(argv[2]);
    }
    else
    {
        printf("ERROR, USAGE: [./egg_drop] [<number of floors>] [<number of eggs>]\n");
        return EX_USAGE;
    }

    if(eggAmount > log2(floorAmount)){
        binary_egg_drop(floorAmount);
    }


}
