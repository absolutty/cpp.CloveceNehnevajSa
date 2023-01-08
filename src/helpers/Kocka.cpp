//
// Created by adaha on 7. 1. 2023.
//

#include <unistd.h>
#include <iostream>
#include "Kocka.h"

Planet* Kocka::lastPlanet = nullptr;

int Kocka::rollDice() {
    srand( time(nullptr) );
    pthread_t planetaThreads[N_of_PLANETS];

    for (int i = 0; i < N_of_PLANETS; i++) {
        pthread_create(&planetaThreads[i], nullptr, distanceToPlanet, &arrPlanets[i]);
    }
    for (unsigned long i : planetaThreads) {
        pthread_join(i, nullptr);
    }

//    std::cout << "Pri hode kockou rozhoduje planeta [" << lastPlanet->name << "]: ";

    int randomPercentage = random(0, 100);
    int hodeneCislo, sucty=0;
    for (hodeneCislo=0; sucty<randomPercentage; hodeneCislo++) {
        sucty+= lastPlanet->probabilities[hodeneCislo];
    }
//    std::cout << "padlo cislo " << hodeneCislo << std::endl;

    lastPlanet = nullptr;
    return hodeneCislo;
}

void* Kocka::distanceToPlanet(void* arg) {
    auto* planeta = static_cast<Planet *>(arg);
    int randSleep = random(1000000,6000000);

    usleep(randSleep);
//    printf("Planeta %s je vzdialena %dkm\n", planeta->name.c_str(), randSleep);
    lastPlanet = planeta;

    return nullptr;
}

void Kocka::initializePlanets() {
    arrPlanets[0].name = "Merkur";
    arrPlanets[0].probabilities = new int[N_of_NUMBERS_ON_DICE]{20, 10, 5, 10, 5, 50};
    arrPlanets[1].name = "Venuša";
    arrPlanets[1].probabilities = new int[N_of_NUMBERS_ON_DICE]{10, 10, 10, 10, 30, 30};
    arrPlanets[2].name = "Zem";
    arrPlanets[2].probabilities = new int[N_of_NUMBERS_ON_DICE]{10, 10, 5, 5, 60, 10};
    arrPlanets[3].name = "Mars";
    arrPlanets[3].probabilities = new int[N_of_NUMBERS_ON_DICE]{20, 5, 5, 20, 5, 40};
    arrPlanets[4].name = "Jupiter";
    arrPlanets[4].probabilities = new int[N_of_NUMBERS_ON_DICE]{40, 10, 20, 10, 10, 10};
    arrPlanets[5].name = "Saturn";
    arrPlanets[5].probabilities = new int[N_of_NUMBERS_ON_DICE]{0, 0, 10, 70, 20, 0};
    arrPlanets[6].name = "Urán";
    arrPlanets[6].probabilities = new int[N_of_NUMBERS_ON_DICE]{20, 25, 30, 10, 5, 10};
    arrPlanets[7].name = "Neptun";
    arrPlanets[7].probabilities = new int[N_of_NUMBERS_ON_DICE]{30, 30, 10, 20, 5, 5};
}

Kocka::Kocka() {
    initializePlanets();
}
