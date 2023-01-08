#include <pthread.h>
#include "Client.h"


Client::Client(char *hostname, int portNumber) {
    server = gethostbyname(hostname);
    if (server == nullptr) {
        printError((char*)"Server neexistuje.");
    }
    if (portNumber <= 0) {
        printError((char*)"Port musi byt cele cislo vacsie ako 0.");
    }
    mut = PTHREAD_MUTEX_INITIALIZER;
    dataC = {&mut, -1, 0};
    dataC.socket = socket(AF_INET, SOCK_STREAM, 0);
    if (dataC.socket < 0) {
        printError((char*)"Chyba - socket.");
    }
    bzero((char *)&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serverAddress.sin_addr.s_addr, server->h_length);
    serverAddress.sin_port = htons(portNumber);

}

Client::~Client() {
    delete server;
    server = nullptr;
    pthread_mutex_destroy(&mut);
}

int Client::run() {
    if (connect(dataC.socket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
        printError((char*)"Chyba - connect.");
    }

    printf("Spojenie so serverom bolo nadviazane.\nHra sa zacne ked sa pripoja vsetci hraci.\n");

    // vlakno pre zapisovanie do socketu
    pthread_t threadWrite;
    pthread_create(&threadWrite, NULL, funWrite, (void *)&dataC);
    printf("Vlakno pre zapisovanie vytvorene.\n");
    //
    printf("V hlavnom vlakne sa spusta citanie.\n");
    funRead((void *)&dataC);
    pthread_join(threadWrite, NULL);

    //uzavretie socketu <unistd.h>
    close(dataC.socket);
    printf("Spojenie so serverom bolo ukoncene.\n");
    return (EXIT_SUCCESS);
}

void *Client::funRead(void *arg) {
    printf("starting read thread\n");

    DATAC * data = (DATAC*)arg;
    char buffer[BUFFER_LENGTH + 1];
    buffer[BUFFER_LENGTH] = '\0';
    pthread_mutex_lock(data->mut);
    bool koniec = data->stop;
    pthread_mutex_unlock(data->mut);
    while(!koniec) {
        if(read(data->socket, buffer, BUFFER_LENGTH) > 0) {
            if (strcmp(buffer, endMsg) == 0) {
                pthread_mutex_lock(data->mut);
                data->stop = true;
                pthread_mutex_unlock(data->mut);
                koniec = true;
            } else {
                printf("%s", buffer);
            }
        } else {
            pthread_mutex_lock(data->mut);
            data->stop = true;
            pthread_mutex_unlock(data->mut);
            koniec = true;
        }
    }
    return nullptr;
}

void *Client::funWrite(void *arg) {
    printf("starting write thread\n");

    DATAC * data = (DATAC*)arg;
    char buffer[BUFFER_LENGTH + 1];
    buffer[BUFFER_LENGTH] = '\0';
    pthread_mutex_lock(data->mut);
    bool koniec = data->stop;
    pthread_mutex_unlock(data->mut);
    while(!koniec) {
        fgets(buffer, BUFFER_LENGTH, stdin);
        pthread_mutex_lock(data->mut);
        write(data->socket, buffer, strlen(buffer) + 1);
        pthread_mutex_unlock(data->mut);
        if (strstr(buffer, endMsg) == buffer) {
            pthread_mutex_lock(data->mut);
            data->stop = true;
            pthread_mutex_unlock(data->mut);
        } else {
            printf("%s",buffer);
        }
        pthread_mutex_lock(data->mut);
        koniec = data->stop;
        pthread_mutex_unlock(data->mut);
    }
    return nullptr;
}
