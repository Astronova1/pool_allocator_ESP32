//
// Created by Dark on 07/07/2026.
//
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
using namespace std;

#define PORT "4360"

int main () {

    WSAData wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        cerr << "WSA Startup failed\n";
        return 1;
    }

    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
        cerr << "WSA Version Not Supported\n";
        WSACleanup();
        return 2;
    }

    int status;
    struct addrinfo hints;
    struct addrinfo *servinfo, *p;

    memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;
    if (status = getaddrinfo(nullptr, PORT, &hints, &servinfo) != 0) {
        cerr << "getaddrinfo failed\n" << gai_strerror(status) << endl;
        WSACleanup();
        return 2;
    }
    char ipstr[INET6_ADDRSTRLEN];
    int socket, new_socket;
    for (p = servinfo; p!= nullptr; p = p->ai_next) {
        if ((socket = (socket, p->ai_family, p->ai_socktype, p->ai_protocol)) == SOCKET_ERROR) {
            cerr << "Socket creation failed\n";
            closesocket(socket);
            continue;
        }

        
    }


    freeaddrinfo(servinfo);
}
