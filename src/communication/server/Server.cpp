#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include "Server.h"

Server::Server(int pPocetHracov, int portNumber) {
    mut = PTHREAD_MUTEX_INITIALIZER;
    if (pPocetHracov < 1 || pPocetHracov > 4) {
        printError((char*)"Mozes zadat iba od 1 po 4 hracov!");
    }
    if (portNumber <= 0) {
        printError((char*)"Port musi byt cele cislo vacsie ako 0.");
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        printError((char*)"Chyba - socket.");
    }

    dataS = {&mut,new int[pPocetHracov], pPocetHracov, 0, new HraciaPlocha(pPocetHracov),new Kocka()};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(portNumber);
}

Server::~Server() {
    delete dataS.hraciaPlocha;
    delete dataS.kocka;
    pthread_mutex_destroy(&mut);
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
    for(int i=0; i < dataS.pocetHracov; i++) {
        dataS.poleSocketov[i] = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);
    }

    //uzavretie pasivneho socketu <unistd.h>
    close(serverSocket);
    for(int i=0; i < dataS.pocetHracov; i++) {
        if (dataS.poleSocketov[i] < 0) {
            printError((char*)"Chyba - accept.");
        }
    }

    printf("Vsetci klienti sa pripojili na server.\n");

    int pocet = dataS.pocetHracov;
    int * polSoc = new int[pocet];
    for (int i = 0; i < pocet; ++i) {
        polSoc[i] = dataS.poleSocketov[i];
    }

    pthread_t klienti[pocet];
    DATAT klientiD[dataS.pocetHracov];
    for (int i = 0; i < pocet; ++i) {
        klientiD[i].dataS = &dataS;
        klientiD[i].cisloHraca = i;
        klientiD[i].socket = polSoc[i];
        klientiD[i].stop = 0;
        pthread_create(&klienti[i], NULL, funClientService, &klientiD[i]);
    }
    for (int i = 0; i < pocet; ++i) {
        pthread_join(klienti[i], NULL);
    }
    delete[] polSoc;

    for(int i=0; i < dataS.pocetHracov; i++) {
        close(dataS.poleSocketov[i]);
    }

    delete[] dataS.poleSocketov;
    return (EXIT_SUCCESS);
}

void *Server::funClientService(void *arg) {
    printf("Spustil sa thread klienta\n");
    DATAT * dataT = (DATAT*)arg;
    usleep(100);
    printf("Spustil sa klient c. %d\n", dataT->cisloHraca);
    char buffer[BUFFER_LENGTH + 1];
    buffer[BUFFER_LENGTH] = '\0';
    int naRade = 0;
    pthread_mutex_lock(dataT->dataS->mut);
    dataT->dataS->hraciaPlocha->toCharArray(buffer);
    pthread_mutex_unlock(dataT->dataS->mut);
    write(dataT->socket, buffer, strlen(buffer) + 1);
    if (naRade == dataT->cisloHraca) {
        sprintf(buffer, "Si na rade.\n");
        usleep(100);
        write(dataT->socket, buffer, strlen(buffer) + 1);
    } else {
        sprintf(buffer, "Na rade je hrac %d\n", naRade + 1);
        usleep(100);
        write(dataT->socket, buffer, strlen(buffer) + 1);
    }
    while(!dataT->stop) {
        read(dataT->socket, buffer, BUFFER_LENGTH);
        pthread_mutex_lock(dataT->dataS->mut);
        naRade = dataT->dataS->jeNaRade;
        pthread_mutex_unlock(dataT->dataS->mut);
//        if (strcmp(buffer, endMsg) == 0) {
        if (strstr(buffer, endMsg) == buffer) {
            dataT->stop = true;
            continue;
//        } else if (strcmp(buffer, "hod") != 0) {
        } else if (strstr(buffer, hodMsg) != buffer) {
            sprintf(buffer, "Neznamy prikaz, ak si na rade zadaj 'hod' pre hodenie kockou.\n");
            usleep(100);
            write(dataT->socket,buffer, strlen(buffer)+1);
            continue;
        }
        if (naRade != dataT->cisloHraca) {
            sprintf(buffer, "Nie si na rade, na rade je hrac c. %d\n",naRade+1);
            usleep(100);
            write(dataT->socket,buffer, strlen(buffer)+1);
        } else {
            sprintf(buffer, "Hadzes kockou ... \n");
            usleep(100);
            write(dataT->socket,buffer, strlen(buffer)+1);
            pthread_mutex_lock(dataT->dataS->mut);
            int hodeneCislo = dataT->dataS->kocka->rollDice();
            pthread_mutex_unlock(dataT->dataS->mut);
            sprintf(buffer, "Hodil si cislo %d\n", hodeneCislo);
            usleep(100);
            write(dataT->socket,buffer, strlen(buffer)+1);
            bool tahUspesny = false;
            pthread_mutex_lock(dataT->dataS->mut);
            bool mozemTahat = dataT->dataS->hraciaPlocha->mozeTahatHrac(dataT->cisloHraca, hodeneCislo);
            pthread_mutex_unlock(dataT->dataS->mut);

            printf("mozemTahat = %s", mozemTahat ? "true" : "false");

            if (mozemTahat) {
                while (!tahUspesny) {
                    sprintf(buffer, "Zadaj svoj tah\n");
                    usleep(100);
                    write(dataT->socket,buffer, strlen(buffer)+1);
                    read(dataT->socket, buffer, BUFFER_LENGTH);
                    tahUspesny = dataT->dataS->hraciaPlocha->tah(buffer, hodeneCislo, dataT->cisloHraca);
                    if (!tahUspesny) {
                        sprintf(buffer, "Zle zadany/neplatny tah!!!\n");
                        usleep(100);
                        write(dataT->socket,buffer, strlen(buffer)+1);
                    } else {
                        pthread_mutex_lock(dataT->dataS->mut);
                        dataT->dataS->jeNaRade = (dataT->dataS->jeNaRade+1)%dataT->dataS->pocetHracov;
                        pthread_mutex_unlock(dataT->dataS->mut);
                        writeGameInfoToAll(dataT, buffer);
                    }
                }
            } else {
                sprintf(buffer, "Zial nemozes vykonat tah.\n");
                usleep(100);
                write(dataT->socket,buffer, strlen(buffer)+1);
                pthread_mutex_lock(dataT->dataS->mut);
                dataT->dataS->jeNaRade = (dataT->dataS->jeNaRade+1)%dataT->dataS->pocetHracov;
                pthread_mutex_unlock(dataT->dataS->mut);
                writeGameInfoToAll(dataT, buffer);
            }
        }
    }
    return nullptr;
}

void Server::writeGameInfoToAll(DATAT *dataT, char *buffer) {
    printf("writing game info to all\n");
    pthread_mutex_lock(dataT->dataS->mut);
    dataT->dataS->hraciaPlocha->toCharArray(buffer);
    for (int i = 0; i < dataT->dataS->pocetHracov; ++i) {
        if (dataT->dataS->poleSocketov[i]) {
            write(dataT->dataS->poleSocketov[i], buffer, strlen(buffer) + 1);
        }
    }
    for (int i = 0; i < dataT->dataS->pocetHracov; ++i) {
        if (dataT->dataS->poleSocketov[i]) {
            if (dataT->dataS->jeNaRade == i) {
                sprintf(buffer, "Si na rade.\n");
                usleep(100);
                write(dataT->dataS->poleSocketov[i], buffer, strlen(buffer) + 1);
            } else {
                sprintf(buffer, "Na rade je hrac %d\n", dataT->dataS->jeNaRade + 1);
                usleep(100);
                write(dataT->dataS->poleSocketov[i], buffer, strlen(buffer) + 1);
            }
        }
    }
    pthread_mutex_unlock(dataT->dataS->mut);
}

bool Server::compareString(const char *s1, const char *s2) {
//    int i = 0;
//    while (s1[i] != '\0' || s2[i] != '\0') {
//        if (s1[i] != s2[i]) {
//            return false;
//        }
//        i++;
//    }
    return true;
}


