//
// Created by adaha on 6. 1. 2023.
//

#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include "Server.h"

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

    printf("Vsetci klienti sa pripojil na server.\n");
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
