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
}

Server::~Server() {
    delete hraciaPlocha;
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
    char buffer[BUFFER_LENGTH + 1];
    buffer[BUFFER_LENGTH] = '\0';
    int koniec = 0;
    while (!koniec) {
        for(int i=0; i < pocetHracov; i++) {
            //citanie dat zo socketu <unistd.h>
            read(poleSocketov[i], buffer, BUFFER_LENGTH);
            if (strcmp(buffer, endMsg) != 0) {
                printf("Klient%d. poslal nasledujuce data:\n%s\n", (i+1), buffer);
                spracujData(buffer);
                //zapis dat do socketu <unistd.h>
                write(poleSocketov[i], buffer, strlen(buffer) + 1);
            }
            else {
                koniec = 1;
            }
        }
    }
    printf("Klient ukoncil komunikaciu.\n");

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

