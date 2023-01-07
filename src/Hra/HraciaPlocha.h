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
    HraciaPlocha(int pocetHracov);
    ~HraciaPlocha();
    void vykresli();
    bool tah(char figurka[2], int posun);
    int getCisloHracaPodlaFarby(char farba);
    bool skoncilHrac(int hrac);
};


#endif //CLOVECENEHNEVAJSA_HRACIAPLOCHA_H
