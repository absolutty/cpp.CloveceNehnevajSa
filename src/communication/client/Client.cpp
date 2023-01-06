//
// Created by adaha on 6. 1. 2023.
//

#include "Client.h"

int Client::run() {
    if (connect(sock,(struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
        printError((char*)"Chyba - connect.");
    }

    printf("Spojenie so serverom bolo nadviazane.\n");
    char buffer[BUFFER_LENGTH + 1];
    buffer[BUFFER_LENGTH] = '\0';
    int koniec = 0;
    while (!koniec) {
        fgets(buffer, BUFFER_LENGTH, stdin);
        char* pos = strchr(buffer, '\n');
        if (pos != nullptr) {
            *pos = '\0';
        }
        //zapis dat do socketu <unistd.h>
        write(sock, buffer, strlen(buffer) + 1);
        if (strcmp(buffer, endMsg) != 0) {
            //citanie dat zo socketu <unistd.h>
            read(sock, buffer, BUFFER_LENGTH);
            printf("Server poslal nasledujuce data:\n%s\n", buffer);
        }
        else {
            koniec = 1;
        }
    }
    //uzavretie socketu <unistd.h>
    close(sock);
    printf("Spojenie so serverom bolo ukoncene.\n");

    return (EXIT_SUCCESS);
}