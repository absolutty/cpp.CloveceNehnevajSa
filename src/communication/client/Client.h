//
// Created by adaha on 6. 1. 2023.
//

#ifndef CLOVECENEHNEVAJSA_CLIENT_H
#define CLOVECENEHNEVAJSA_CLIENT_H


#include <netdb.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#include "src/communication/definitions.h"

class Client {
private:
    int sock;
    struct hostent *server;
    struct sockaddr_in serverAddress;
public:
    Client(char* hostname, int portNumber) {
        server = gethostbyname(hostname);
        if (server == nullptr) {
            printError((char*)"Server neexistuje.");
        }
        if (portNumber <= 0) {
            printError((char*)"Port musi byt cele cislo vacsie ako 0.");
        }
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            printError((char*)"Chyba - socket.");
        }

        bzero((char *)&serverAddress, sizeof(serverAddress));
        serverAddress.sin_family = AF_INET;
        bcopy((char *)server->h_addr, (char *)&serverAddress.sin_addr.s_addr, server->h_length);
        serverAddress.sin_port = htons(portNumber);
    }
    ~Client(){
        delete server;
        server = nullptr;
    }
    int run();
};


#endif //CLOVECENEHNEVAJSA_CLIENT_H
