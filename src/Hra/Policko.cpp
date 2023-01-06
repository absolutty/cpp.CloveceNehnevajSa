#include "Policko.h"

Policko::Policko() {
    obsah = new char[2*sizeof(char)];
}

char *Policko::getObsah() {
    return obsah;
}

Policko::~Policko() {delete obsah;}

