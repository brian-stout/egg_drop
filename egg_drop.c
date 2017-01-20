#include <stdio.h>
#include <math.h>
#include "egg.h"


int worst_case(double numberOfFloors)
{
    return (int)ceil((sqrt(1+8*numberOfFloors)-1)/2);
}
//Create list of eggs?

void test_eggs(egg **nest, size_t number_of_eggs, size_t number_of_floors, size_t current_floor)
{
    int egg;
    int worstCase;
    int decrement = 1;
    if(number_of_eggs > 1)
    {
    worstCase = worst_case(number_of_floors);
    egg = 0;
    } else {
        worstCase = 1;
        egg = 1;
        decrement = 0;
    }

    while(!egg_is_broken(nest[egg]))
    {
        current_floor+=worstCase;
        worstCase -= decrement;
        egg_drop_from_floor(nest[egg], current_floor);
        if(!egg_is_broken(nest[egg]))
        {
            printf("Floor %d; Safe!\n", current_floor);
        }
        else{
            --number_of_eggs;
            printf("Floor %d: Crack!\n", current_floor);
        }
    }
    current_floor -= worstCase += decrement;
    if(number_of_eggs != 0){
    test_eggs(nest, number_of_eggs, number_of_floors, current_floor);
    }
    else{
    printf("The last safe floor is %d\n", current_floor--); }
}


int main(void)
{
    size_t number_of_floors = 100;
    size_t number_of_eggs = 2;

    egg *nest[10];
    nest[0] = lay_egg();
    nest[1] = lay_egg();
    
    test_eggs(nest, number_of_eggs, number_of_floors, 0);


    printf("%d\n", worst_case(number_of_floors));
}
