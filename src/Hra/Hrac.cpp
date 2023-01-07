#include "Hrac.h"

Hrac::Hrac(char pFarba) {
    farba = pFarba;
    zaciatokDomcek = new Policko*[4*sizeof(Policko)];
    domcek = new Policko*[4*sizeof(Policko)];
    panaci = new int[4*sizeof(int)];
    for (int i = 0; i < 4; ++i) {
        zaciatokDomcek[i] = new Policko();
        domcek[i] = new Policko();
        domcek[i]->getObsah()[0] = ' ';
        domcek[i]->getObsah()[1] = ' ';
        panaci[i] = 100+i;
    }
}

Hrac::~Hrac() {
    for (int i = 0; i < 4; ++i) {
        delete zaciatokDomcek[i];
        delete domcek[i];
    }
    delete zaciatokDomcek;
    delete domcek;
    delete panaci;
}

Policko *Hrac::getZaciatokDomcekAt(int index) {
    return zaciatokDomcek[index];
}

Policko *Hrac::getDomcetAt(int index) {
    return domcek[index];
}

int Hrac::getPanak(int index) {
    return panaci[index];
}

void Hrac::setPanak(int index, int pozicia) {
    panaci[index] = pozicia;
}

