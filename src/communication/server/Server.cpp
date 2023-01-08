//
// Created by adaha on 6. 1. 2023.
//

#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include "Server.h"

Server::Server(int pPocetHracov, int portNumber) {
    pocetHracov = pPocetHracov;
    if (pocetHracov < 1 || pocetHracov > 4) {
        printError((char*)"Mozes zadat iba od 1 po 4 hracov!");
    }
    if (portNumber <= 0) {
        printError((char*)"Port musi byt cele cislo vacsie ako 0.");
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        printError((char*)"Chyba - socket.");
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(portNumber);
    hraciaPlocha = new HraciaPlocha(pocetHracov);
    kocka = new Kocka();
}

Server::~Server() {
    delete hraciaPlocha;
    delete kocka;
}

int Server::run() {
    //prepojenie adresy servera so socketom <sys/socket.h>
    if (bind(serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
        printError((char*)"Chyba - bind.");
    }

    //server bude prijimat nove spojenia cez socket serverSocket <sys/socket.h>
    listen(serverSocket, 10);

    //server caka na pripojenie klienta <sys/socket.h>
    struct sockaddr_in clientAddress{};
    socklen_t clientAddressLength = sizeof(clientAddress);
    int* poleSocketov = new int[pocetHracov];
    for(int i=0; i < pocetHracov; i++) {
        poleSocketov[i] = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);
    }


//    int clientSocket1 = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);
//    int clientSocket2 = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);

    //uzavretie pasivneho socketu <unistd.h>
    close(serverSocket);
    for(int i=0; i < pocetHracov; i++) {
        if (poleSocketov[i] < 0) {
            printError((char*)"Chyba - accept.");
        }
    }

//    if (clientSocket1 < 0 && clientSocket2 < 0) {
//        printError((char*)"Chyba - accept.");
//    }

    printf("Vsetci klienti sa pripojili na server.\n");

    int jeNaRade = 0;
    char buffer[BUFFER_LENGTH + 1];
    buffer[BUFFER_LENGTH] = '\0';
    bool koniec = false;
    while (!koniec) {
        hraciaPlocha->toCharArray(buffer);
        for (int i = 0; i < pocetHracov; ++i) {
            if (poleSocketov[i]) {
                write(poleSocketov[i], infoMsg, strlen(infoMsg) + 1);
                usleep(1000);
                write(poleSocketov[i], buffer, strlen(buffer) + 1);
            }
        }
        while (!koniec) {
            if (poleSocketov[jeNaRade]) {
                if (hraciaPlocha->skoncilHrac(jeNaRade)) {
                    jeNaRade = (jeNaRade + 1) % pocetHracov;
                    koniec = true;
                    continue;
                }
                for (int i = 0; i < pocetHracov; ++i) {
                    if (poleSocketov[i]) {
                        write(poleSocketov[i], infoMsg, strlen(infoMsg) + 1);
                        usleep(1000);
                        if (jeNaRade == i) {
                            sprintf(buffer, "Si na rade.\n");
                            write(poleSocketov[i], buffer, strlen(buffer) + 1);
                        } else {
                            sprintf(buffer, "Na rade je hrac %d\n", jeNaRade + 1);
                            write(poleSocketov[i], buffer, strlen(buffer) + 1);
                        }
                    }
                }
                bool tahUspesny = false;
                int hodeneCislo = kocka->rollDice();
                write(poleSocketov[jeNaRade], tahMsg, strlen(tahMsg) + 1);
                usleep(1000);
                sprintf(buffer, "Hodil si %d\n",hodeneCislo);
                write(poleSocketov[jeNaRade], buffer, strlen(buffer)+1);
                if (hraciaPlocha->mozeTahatHrac(jeNaRade, hodeneCislo)) {
                    while (!tahUspesny) {
                        usleep(1000);
                        write(poleSocketov[jeNaRade], tahMsg, strlen(tahMsg) + 1);
                        read(poleSocketov[jeNaRade], buffer, BUFFER_LENGTH);
                        tahUspesny = hraciaPlocha->tah(buffer, hodeneCislo);
                    }
                }
                usleep(1000);
                write(poleSocketov[jeNaRade], okMsg, strlen(okMsg) + 1);
                for (int i = 0; i < pocetHracov; ++i) {
                    hraciaPlocha->toCharArray(buffer);
                    if (poleSocketov[i]) {
                        usleep(1000);
                        write(poleSocketov[i], infoMsg, strlen(infoMsg) + 1);
                        usleep(1000);
                        write(poleSocketov[i], buffer, strlen(buffer) + 1);
                    }
                }
            }
            if (hraciaPlocha->skoncilHrac(jeNaRade)) {
                sprintf(buffer, "Dohral si hru.");
                write(poleSocketov[jeNaRade], buffer, strlen(buffer)+1);
            }
            jeNaRade = (jeNaRade+1)%pocetHracov;
        }

    }


//    char buffer[BUFFER_LENGTH + 1];
//    buffer[BUFFER_LENGTH] = '\0';
//    int koniec = 0;
//    while (!koniec) {
//        for(int i=0; i < pocetHracov; i++) {
//            //citanie dat zo socketu <unistd.h>
//            read(poleSocketov[i], buffer, BUFFER_LENGTH);
//            if (strcmp(buffer, endMsg) != 0) {
//                printf("Klient%d. poslal nasledujuce data:\n%s\n", (i+1), buffer);
//                spracujData(buffer);
//                //zapis dat do socketu <unistd.h>
//                write(poleSocketov[i], buffer, strlen(buffer) + 1);
//            }
//            else {
//                koniec = 1;
//            }
//        }
//    }
//    printf("Klient ukoncil komunikaciu.\n");

    //uzavretie socketu klienta <unistd.h>
    for(int i=0; i < pocetHracov; i++) {
        close(poleSocketov[i]);
    }

    delete[] poleSocketov;
    return (EXIT_SUCCESS);
}

char *Server::spracujData(char *data) {
    char *akt = data;
    while (*akt != '\0') {
        if (islower(*akt)) {
            *akt = toupper(*akt);
        }
        else if (isupper(*akt)) {
            *akt = tolower(*akt);
        }
        akt++;
    }
    return data;
}

