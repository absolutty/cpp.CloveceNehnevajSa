//
// Created by adaha on 7. 1. 2023.
//

#ifndef CLOVECENEHNEVAJSA_KOCKA_H
#define CLOVECENEHNEVAJSA_KOCKA_H

#include <string>

#define N_of_NUMBERS_ON_DICE 6
#define N_of_PLANETS 8
/**
 * Pouzivatel vysle naraz signal na vsetky planety a caka ze kt. je od neho najdalej. Robi to preto lebo pocul
 * ze ak bude pouzivat probabilities kociek na tych najvzdialenejsich planetach, postasti sa mu a vyhra :)
 *
 * - Planet: obsahuje jej name, percentualne probabilities (ich sucet musi byt 100) ze padne dane cislo od 1-6
 */
typedef struct Planet {
    std::string name;
    int* probabilities{};
} Planet;

class Kocka {
private:
    Planet arrPlanets[N_of_PLANETS];
    void initializePlanets();
    static Planet* lastPlanet;
public:
    Kocka();
    int rollDice();
    static void* distanceToPlanet(void* arg);
    static int random(int min, int max) {
        return min + rand() % (( max + 1 ) - min);
    }
};

#endif //CLOVECENEHNEVAJSA_KOCKA_H