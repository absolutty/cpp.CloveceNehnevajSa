#include <cstdio>
#include <unistd.h>
#include "Hra/HraciaPlocha.h"
int main(int argc, char *argv[])
{
    printf("Hello world!\n");
    HraciaPlocha* hp = new HraciaPlocha(4);
    hp->toCharArray();

    //sleep(3);
    char hrac[] = {'R', '1'};
    hp->tah(hrac,6);
    hp->toCharArray();

    //sleep(3);
    hrac[0] = 'R'; hrac[1] = '1';
    hp->tah(hrac,3);
    hp->toCharArray();

    //sleep(3);
    hrac[0] = 'B'; hrac[1] = '1';
    hp->tah(hrac,5);
    hp->toCharArray();

    //sleep(3);
    hrac[0] = 'G'; hrac[1] = '2';
    hp->tah(hrac,6);
    hp->toCharArray();

    //sleep(3);
    hrac[0] = 'R'; hrac[1] = '1';
    hp->tah(hrac,2);
    hp->toCharArray();

    for (int i = 0; i < 8; ++i) {
    //sleep(3);
    hrac[0] = 'G'; hrac[1] = '2';
    hp->tah(hrac,5);
        hp->toCharArray();

    }

    hrac[0] = 'B'; hrac[1] = '3';
    hp->tah(hrac,6);
    hp->toCharArray();
    sleep(3);
    for (int i = 0; i < 14; ++i) {
        sleep(2);
        hrac[0] = 'B'; hrac[1] = '3';
        hp->tah(hrac,3);
        hp->toCharArray();

    }
    
    delete hp;
    return 0;
}
