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

typedef struct dataClient {
    pthread_mutex_t * mut;
    int socket;
    int stop;
} DATAC;

class Client {
private:
    DATAC dataC;
    pthread_mutex_t mut;
    struct hostent *server;
    struct sockaddr_in serverAddress;
public:
    Client(char* hostname, int portNumber);
    ~Client(){
        delete server;
        server = nullptr;
    }
    int run();
    static void * funRead(void * arg);
    static void * funWrite(void * arg);
};

#endif //CLOVECENEHNEVAJSA_CLIENT_H
