//
// Created by adaha on 6. 1. 2023.
//

#ifndef CLOVECENEHNEVAJSA_SERVER_H
#define CLOVECENEHNEVAJSA_SERVER_H


#include <netinet/in.h>
#include "src/communication/definitions.h"

class Server {
private:
    int pocetHracov;
    int serverSocket = -1;
    struct sockaddr_in serverAddress{};
    char* spracujData(char *data);
public:
    explicit Server(int pPocetHracov, int portNumber) {
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
    }
    int run();
};

#endif //CLOVECENEHNEVAJSA_SERVER_H
