#include <cstdio>
#include "Hra/HraciaPlocha.h"
int main(int argc, char *argv[])
{
    printf("Hello world!\n");
    HraciaPlocha* hp = new HraciaPlocha(4);
    hp->vykresli();
    delete hp;
    return 0;
}
