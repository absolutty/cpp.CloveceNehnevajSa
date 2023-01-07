//
// Created by adaha on 6. 1. 2023.
//

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
    data.sock = socket(AF_INET, SOCK_STREAM, 0);
    if (data.sock < 0) {
        printError((char*)"Chyba - socket.");
    }
    bzero((char *)&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serverAddress.sin_addr.s_addr, server->h_length);
    serverAddress.sin_port = htons(portNumber);
}

int Client::run() {
    if (connect(data.sock,(struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
        printError((char*)"Chyba - connect.");
    }

    printf("Spojenie so serverom bolo nadviazane.\n");
    char buffer[BUFFER_LENGTH + 1];
    buffer[BUFFER_LENGTH] = '\0';
    int koniec = 0;

    pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
    data.mut = &mut;
    // vlakno pre citanie zo servera
    pthread_t thread;
    pthread_create(&thread, NULL, readFromServer, &data);

    while (!koniec) {
        fgets(buffer, BUFFER_LENGTH, stdin);
        char* pos = strchr(buffer, '\n');
        if (pos != nullptr) {
            *pos = '\0';
        }
        //zapis dat do socketu <unistd.h>
        write(data.sock, buffer, strlen(buffer) + 1);
        if (strcmp(buffer, endMsg) != 0) {
            //citanie dat zo socketu <unistd.h>
            read(data.sock, buffer, BUFFER_LENGTH);
            printf("Server poslal nasledujuce data:\n%s\n", buffer);
        }
        else {
            koniec = 1;
        }
    }
    //uzavretie socketu <unistd.h>
    close(data.sock);
    printf("Spojenie so serverom bolo ukoncene.\n");
    pthread_mutex_destroy(&mut);
    return (EXIT_SUCCESS);
}

void * Client::readFromServer(void *arg) {
    DATA * data = (DATA *)arg;
    char buffer[BUFFER_LENGTH + 1];
    buffer[BUFFER_LENGTH] = '\0';
    read(data->sock, buffer, BUFFER_LENGTH);
    printf("%s",buffer);
    return nullptr;
}
