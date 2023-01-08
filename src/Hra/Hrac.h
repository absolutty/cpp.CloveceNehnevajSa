#ifndef CLOVECENEHNEVAJSA_HRAC_H
#define CLOVECENEHNEVAJSA_HRAC_H


#include "Policko.h"

class Hrac {
private:
    char farba;
    Policko** zaciatokDomcek;
    Policko** domcek;
    int* panaci;
public:
    Hrac(char pFarba);
    ~Hrac();
    Policko * getZaciatokDomcekAt(int index);
    Policko * getDomcetAt(int index);
    int getPanak(int index);
    void setPanak(int intex, int pozicia);
    inline char getFarba(){return farba;};
};


#endif //CLOVECENEHNEVAJSA_HRAC_H
