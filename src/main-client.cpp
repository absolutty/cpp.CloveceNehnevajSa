#include "src/communication/definitions.h"
#include "src/communication/client/Client.h"

//
// Created by adaha on 6. 1. 2023.
//
int main(int argc, char *argv[]) {
    if (argc < 3) {
        printError((char*)"Klienta je nutne spustit s nasledujucimi argumentmi: adresa port.");
    }
    char* hostname = argv[1];
    int portNumber = atoi(argv[2]);

    auto* client = new Client(hostname, portNumber);

    int retValue = client->run();
    delete client;
    return retValue;
}