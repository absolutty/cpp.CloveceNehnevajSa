#include <cstdio>
#include <cstring>
#include <iostream>
#include "HraciaPlocha.h"

HraciaPlocha::HraciaPlocha(int pocetHracov) {
    aPocetHracov = pocetHracov;
    hraciePole = new Policko*[40*sizeof(Policko)];
    hraci = new Hrac*[aPocetHracov*sizeof(Hrac)];
    strcpy(farbyHracov, "RBGY");
    for (int i = 0; i < 40; ++i) {
        hraciePole[i] = new Policko();
        hraciePole[i]->getObsah()[0] = ' ';
        hraciePole[i]->getObsah()[1] = ' ';
    }
    for (int i = 0; i < aPocetHracov; ++i) {
        hraci[i] = new Hrac(farbyHracov[i]);
        for (int j = 0; j < 4; ++j) {
            (hraci[i]->getZaciatokDomcekAt(j)->getObsah())[0] = hraci[i]->getFarba();
            (hraci[i]->getZaciatokDomcekAt(j)->getObsah())[1] = *std::to_string(j + 1).c_str();
        }
    }
}

HraciaPlocha::~HraciaPlocha() {
    for (int i = 0; i < 40; ++i) {
        delete hraciePole[i];
    }
    delete hraciePole;
}

bool HraciaPlocha::tah(char *figurka, int posun) {
    bool tahUspesny = false;
    int cisloHraca = getCisloHracaPodlaFarby(figurka[0]);
    if (cisloHraca == -1) return false;
    int cisloFigurky = atoi(&figurka[1]);
    int poziciaFigurky = hraci[cisloHraca]->getPanak(cisloFigurky-1);
    if(poziciaFigurky>=100) {
        if (posun < 6) {
            return false;
        }
        if (hraciePole[(cisloHraca*10+30)%40]->getObsah()[0] == ' ') {
            hraci[cisloHraca]->setPanak(cisloFigurky-1,(cisloHraca*10+30)%40);
            hraci[cisloHraca]->getZaciatokDomcekAt(cisloFigurky-1)->getObsah()[0] = ' ';
            hraci[cisloHraca]->getZaciatokDomcekAt(cisloFigurky-1)->getObsah()[1] = ' ';
            hraciePole[(cisloHraca*10+30)%40]->getObsah()[0] = farbyHracov[cisloHraca];
            hraciePole[(cisloHraca*10+30)%40]->getObsah()[1] = '0'+cisloFigurky;
        }
//    } else if (/*poziciaFigurky>=((cisloHraca*10+30)%40) &&*/ poziciaFigurky>=(cisloHraca*10+30-posun)%40) {
    } else if (poziciaFigurky>(cisloHraca*10+29)%40-posun && poziciaFigurky<(cisloHraca*10+29)%40+1) {
        hraciePole[poziciaFigurky]->getObsah()[0] = ' ';
        hraciePole[poziciaFigurky]->getObsah()[1] = ' ';
        for (int i = 3; i >= 0; --i) {
            if (hraci[cisloHraca]->getDomcetAt(i)->getObsah()[0] == ' ') {
                hraci[cisloHraca]->getDomcetAt(i)->getObsah()[0] = farbyHracov[cisloHraca];
                hraci[cisloHraca]->getDomcetAt(i)->getObsah()[1] = '0'+cisloFigurky;
                hraci[cisloHraca]->setPanak(cisloFigurky-1, -1);
                break;
            }
        }
    } else {
        if (hraciePole[(poziciaFigurky+posun)%40]->getObsah()[0] != ' ') {
            int hrac = getCisloHracaPodlaFarby(hraciePole[(poziciaFigurky+posun)%40]->getObsah()[0]);
            hraci[hrac]->getZaciatokDomcekAt(hraciePole[(poziciaFigurky+posun)%40]->getObsah()[1]-'0'-1)->getObsah()[0] = hraciePole[(poziciaFigurky+posun)%40]->getObsah()[0];
            hraci[hrac]->getZaciatokDomcekAt(hraciePole[(poziciaFigurky+posun)%40]->getObsah()[1]-'0'-1)->getObsah()[1] = hraciePole[(poziciaFigurky+posun)%40]->getObsah()[1];
            hraci[hrac]->setPanak(hraciePole[(poziciaFigurky+posun)%40]->getObsah()[1]-'0'-1,100+hraciePole[(poziciaFigurky+posun)%40]->getObsah()[1]-'0'-1);
        }
        hraciePole[(poziciaFigurky+posun)%40]->getObsah()[0] = farbyHracov[cisloHraca];
        hraciePole[(poziciaFigurky+posun)%40]->getObsah()[1] = '0'+cisloFigurky;
        hraciePole[poziciaFigurky]->getObsah()[0] = ' ';
        hraciePole[poziciaFigurky]->getObsah()[1] = ' ';
        hraci[cisloHraca]->setPanak(cisloFigurky-1, (poziciaFigurky+posun)%40);
    }
    return true;
}

int HraciaPlocha::getCisloHracaPodlaFarby(char farba) {
    int cisloHraca = 0;
    if(farba == farbyHracov[0]) {
        cisloHraca = 0;
    } else if (farba == farbyHracov[1]) {
        cisloHraca = 1;
    } else if (farba == farbyHracov[2]) {
        cisloHraca = 2;
    } else if (farba == farbyHracov[3]) {
        cisloHraca = 3;
    } else {
        cisloHraca = -1;
    }
    return cisloHraca;
}

bool HraciaPlocha::skoncilHrac(int hrac) {
    for (int i = 0; i < 4; ++i) {
        if (hraci[hrac]->getDomcetAt(i)->getObsah()[0] == ' ') {
            return false;
        }
    }
    return true;
}

void HraciaPlocha::vykresli() {
    printf("┌────┬────┐         ┌────┬────┬────┐         ┌────┬────┐\n"
           "│ %s │ %s │         │ %s │ %s │ %s │ ↓       │ %s │ %s │\n" // riadok 1
           "├────┼────┤         ├────╔────╗────┤         ├────┼────┤\n"
           "│ %s │ %s │         │ %s ║ %s ║ %s │         │ %s │ %s │\n" // riadok 2
           "└────┴────┘         ├────╠────╣────┤         └────┴────┘\n"
           "                    │ %s ║ %s ║ %s │\n"                     // riadok 3
           "                    ├────╠────╣────┤\n"
           "  →                 │ %s ║ %s ║ %s │\n"                     // riadok 4
           "┌────┬────┬────┬────┼────╠────╣────┼────┬────┬────┬────┐\n"
           "│ %s │ %s │ %s │ %s │ %s ║ %s ║ %s │ %s │ %s │ %s │ %s │\n" // riadok 5
           "├────╔════╦════╦════╦════╬════╬════╦════╦════╦════╗────┤\n"
           "│ %s │ %s │ %s │ %s │ %s │▓▓▓▓│ %s │ %s │ %s │ %s │ %s │\n" // riadok 6
           "├────╚════╩════╩════╩════╬════╬════╩════╩════╩════╝────┤\n"
           "│ %s │ %s │ %s │ %s │ %s ║ %s ║ %s │ %s │ %s │ %s │ %s │\n" // riadok 7
           "└────┴────┴────┴────┼────╠────╣────┼────┴────┴────┴────┘\n"
           "                    │ %s ║ %s ║ %s │                 ←\n"   // riadok 8
           "                    ├────╠────╣────┤\n"
           "                    │ %s ║ %s ║ %s │\n"                     // riadok 9
           "┌────┬────┐         ├────╠────╣────┤         ┌────┬────┐\n"
           "│ %s │ %s │         │ %s ║ %s ║ %s │         │ %s │ %s │\n" // riadok 10
           "├────┼────┤         ├────╚────╝────┤         ├────┼────┤\n"
           "│ %s │ %s │       ↑ │ %s │ %s │ %s │         │ %s │ %s │\n" // riadok 11
           "└────┴────┘         └────┴────┴────┘         └────┴────┘\n",
           // riadok 1
           hraci[0]->getZaciatokDomcekAt(0)->getObsah(),
           hraci[0]->getZaciatokDomcekAt(1)->getObsah(),
           hraciePole[38]->getObsah(),
           hraciePole[39]->getObsah(),
           hraciePole[0]->getObsah(),
           hraci[1]->getZaciatokDomcekAt(0)->getObsah(),
           hraci[1]->getZaciatokDomcekAt(1)->getObsah(),
           // riadok 2
           hraci[0]->getZaciatokDomcekAt(2)->getObsah(),
           hraci[0]->getZaciatokDomcekAt(3)->getObsah(),
           hraciePole[37]->getObsah(),
           hraci[1]->getDomcetAt(0)->getObsah(),
           hraciePole[1]->getObsah(),
           hraci[1]->getZaciatokDomcekAt(2)->getObsah(),
           hraci[1]->getZaciatokDomcekAt(3)->getObsah(),
           // riadok 3
           hraciePole[36]->getObsah(),
           hraci[1]->getDomcetAt(1)->getObsah(),
           hraciePole[2]->getObsah(),
           // riadok 4
           hraciePole[35]->getObsah(),
           hraci[1]->getDomcetAt(2)->getObsah(),
           hraciePole[3]->getObsah(),
           // riadok 5
           hraciePole[30]->getObsah(),
           hraciePole[31]->getObsah(),
           hraciePole[32]->getObsah(),
           hraciePole[33]->getObsah(),
           hraciePole[34]->getObsah(),
           hraci[1]->getDomcetAt(3)->getObsah(),
           hraciePole[4]->getObsah(),
           hraciePole[5]->getObsah(),
           hraciePole[6]->getObsah(),
           hraciePole[7]->getObsah(),
           hraciePole[8]->getObsah(),
           // riadok 6
           hraciePole[29]->getObsah(),
           hraci[0]->getDomcetAt(0)->getObsah(),
           hraci[0]->getDomcetAt(1)->getObsah(),
           hraci[0]->getDomcetAt(2)->getObsah(),
           hraci[0]->getDomcetAt(3)->getObsah(),
           hraci[2]->getDomcetAt(3)->getObsah(),
           hraci[2]->getDomcetAt(2)->getObsah(),
           hraci[2]->getDomcetAt(1)->getObsah(),
           hraci[2]->getDomcetAt(0)->getObsah(),
           hraciePole[9]->getObsah(),
           // riadok 7
           hraciePole[28]->getObsah(),
           hraciePole[27]->getObsah(),
           hraciePole[26]->getObsah(),
           hraciePole[25]->getObsah(),
           hraciePole[24]->getObsah(),
           hraci[3]->getDomcetAt(3)->getObsah(),
           hraciePole[14]->getObsah(),
           hraciePole[13]->getObsah(),
           hraciePole[12]->getObsah(),
           hraciePole[11]->getObsah(),
           hraciePole[10]->getObsah(),
           // riadok 8
           hraciePole[23]->getObsah(),
           hraci[3]->getDomcetAt(3)->getObsah(),
           hraciePole[15]->getObsah(),
           // riadok 9
           hraciePole[22]->getObsah(),
           hraci[3]->getDomcetAt(2)->getObsah(),
           hraciePole[16]->getObsah(),
           // riadok 10
           hraci[3]->getZaciatokDomcekAt(0)->getObsah(),
           hraci[3]->getZaciatokDomcekAt(1)->getObsah(),
           hraciePole[21]->getObsah(),
           hraci[3]->getDomcetAt(0)->getObsah(),
           hraciePole[17]->getObsah(),
           hraci[2]->getZaciatokDomcekAt(0)->getObsah(),
           hraci[2]->getZaciatokDomcekAt(1)->getObsah(),
           // riadok 11
           hraci[3]->getZaciatokDomcekAt(2)->getObsah(),
           hraci[3]->getZaciatokDomcekAt(3)->getObsah(),
           hraciePole[20]->getObsah(),
           hraciePole[19]->getObsah(),
           hraciePole[18]->getObsah(),
           hraci[2]->getZaciatokDomcekAt(2)->getObsah(),
           hraci[2]->getZaciatokDomcekAt(3)->getObsah()
    );
}
