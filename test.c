#include <stdio.h>
#include <math.h>

int main(void)
{
    double n = 1000;
    double val = 0;
    val = (sqrt(1+8*n)-1)/2;
    val = ceil(val);
    printf("%lf\n", val);
}
