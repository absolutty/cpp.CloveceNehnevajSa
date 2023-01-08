#ifndef CLOVECENEHNEVAJSA_SERVER_H
#define CLOVECENEHNEVAJSA_SERVER_H

#include <netinet/in.h>
#include "src/communication/definitions.h"
#include "src/Hra/HraciaPlocha.h"
#include "src/helpers/Kocka.h"

typedef struct dataServer {
    pthread_mutex_t * mut;
    int* poleSocketov;
    int pocetHracov;
    int jeNaRade;
    HraciaPlocha* hraciaPlocha;
    Kocka* kocka;
} DATAS;

typedef struct dataThread {
    DATAS * dataS;
    int cisloHraca;
    int socket;
    int stop;
} DATAT;

class Server {
private:
    DATAS dataS;
    pthread_mutex_t mut;
    int serverSocket = -1;
    struct sockaddr_in serverAddress{};
    static bool compareString(const char * s1, const char * s2);
public:
    Server(int pPocetHracov, int portNumber);
    ~Server();
    int run();
    static void * funClientService(void * arg);
    static void writeGameInfoToAll(DATAT *dataT, char *buffer);
};

#endif //CLOVECENEHNEVAJSA_SERVER_H
