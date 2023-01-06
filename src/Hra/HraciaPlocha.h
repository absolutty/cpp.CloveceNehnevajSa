#ifndef CLOVECENEHNEVAJSA_HRACIAPLOCHA_H
#define CLOVECENEHNEVAJSA_HRACIAPLOCHA_H


#include "Policko.h"
#include "Hrac.h"

class HraciaPlocha {
private:
    Policko** hraciePole;
    int aPocetHracov;
    Hrac** hraci;
    char farbyHracov[4*sizeof(char)];
public:
    explicit HraciaPlocha(int pocetHracov);
    ~HraciaPlocha();
    void vykresli();
};


#endif //CLOVECENEHNEVAJSA_HRACIAPLOCHA_H
