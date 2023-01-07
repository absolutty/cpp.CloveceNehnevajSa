cmake_minimum_required(VERSION 3.18)
project(CloveceNehnevajSa)

add_executable(CloveceNehnevajSa src/main.cpp
        src/Hra/HraciaPlocha.cpp src/Hra/HraciaPlocha.h
        src/Hra/Policko.cpp src/Hra/Policko.h
        src/Hra/Hrac.cpp src/Hra/Hrac.h
        src/helpers/Kocka.cpp src/helpers/Kocka.h)

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

add_executable(main-kocka src/helpers/Kocka.cpp src/helpers/Kocka.h src/main-kocka.cpp)

target_link_libraries(CloveceNehnevajSa pthread m)
target_link_libraries(main-kocka pthread m)