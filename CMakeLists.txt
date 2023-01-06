cmake_minimum_required(VERSION 3.18)
project(CloveceNehnevajSa)

include_directories(.)

add_executable(main-server src/main-server.cpp
        src/communication/server/Server.cpp
        src/communication/server/Server.h
        src/communication/definitions.c
        src/communication/definitions.h)

add_executable(main-client src/main-client.cpp
        src/communication/client/Client.cpp
        src/communication/client/Client.h
        src/communication/definitions.c
        src/communication/definitions.h)