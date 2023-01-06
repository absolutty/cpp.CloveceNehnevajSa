#include <cstdlib>
#include "src/communication/server/Server.h"

int main(int argc, char *argv[])
{
    if (argc < 3) { printError((char*)"Server je nutne spustit s nasledujucimi argumentmi: pocet hracov a port"); }
    int pocetHracov = atoi(argv[1]);
    int portNumber = atoi(argv[2]);

    auto* server = new Server(pocetHracov, portNumber);

    int retValue = server->run();
    delete server;
    return retValue;
}
