//
// Created by adaha on 6. 1. 2023.
//

#ifndef CLOVECENEHNEVAJSA_SERVER_H
#define CLOVECENEHNEVAJSA_SERVER_H


#include <netinet/in.h>
#include "src/communication/definitions.h"
#include "src/Hra/HraciaPlocha.h"

class Server {
private:
    int pocetHracov;
    int serverSocket = -1;
    HraciaPlocha* hraciaPlocha;
    struct sockaddr_in serverAddress{};
    char* spracujData(char *data);
public:
    Server(int pPocetHracov, int portNumber);
    ~Server();
    int run();
};

#endif //CLOVECENEHNEVAJSA_SERVER_H
