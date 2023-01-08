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
#include "src/Hra/HraciaPlocha.h"

typedef struct data {
    pthread_mutex_t * mut;
    int sock;
} DATA;

class Client {
private:
    DATA data;
    struct hostent *server;
    struct sockaddr_in serverAddress;
    static void *readFromServer(void *arg);
    void vypisInfo(char *buffer);
    void vlastnyTah(char *buffer);
public:
    Client(char* hostname, int portNumber);
    ~Client(){
        delete server;
        server = nullptr;
    }
    int run();
};


#endif //CLOVECENEHNEVAJSA_CLIENT_H
