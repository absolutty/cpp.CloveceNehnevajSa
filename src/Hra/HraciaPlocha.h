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
    void toCharArray(char *vystup);
    bool tah(char figurka[2], int posun, int hrac);
    int getCisloHracaPodlaFarby(char farba);
    bool skoncilHrac(int hrac);
    bool maFigurkyNaZaciatkuHrac(int hrac);
    bool mozeTahatHrac(int hrac, int posun);
};

#endif //CLOVECENEHNEVAJSA_HRACIAPLOCHA_H
