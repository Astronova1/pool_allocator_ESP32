//
// Created by Dark on 07/07/2026.
//
#include <iostream>
#include <vector>
#include <winsock2.h>
#include <ws2tcpip.h>
using namespace std;

#define PORT "4360"
#define BACKLOG "5"

int main (int argc, char* argv[]) {

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

    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " PORT\n";
        WSACleanup();
        return 1;
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
    int socket, new_socket, yes=1;
    for (p = servinfo; p!= nullptr; p = p->ai_next) {
        if ((socket = (socket, p->ai_family, p->ai_socktype, p->ai_protocol)) == SOCKET_ERROR) {
            cerr << "Socket creation failed\n" << WSAGetLastError() << endl;
            closesocket(socket);
            continue;
        }

        setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(int));

        if (bind(socket,p->ai_addr, p->ai_addrlen) == SOCKET_ERROR) {
            cerr << "Bind failed\n" << WSAGetLastError() << endl;
            closesocket(socket);
            continue;
        }

        if ((connect(socket,p->ai_addr, p->ai_addrlen)) == SOCKET_ERROR) {
            cerr << "Connect failed\n" << WSAGetLastError() << endl;
            closesocket(socket);
            continue;
        }
        if (listen(socket, *BACKLOG) == SOCKET_ERROR) {
            cerr << "Listen failed\n" << WSAGetLastError() << endl;
            closesocket(socket);
            continue;
        }
        struct sockaddr_storage client;
        socklen_t addr_size = sizeof client;

        new_socket = accept(socket, (struct sockaddr*)&client, &addr_size);
        if (new_socket == SOCKET_ERROR) {
            cerr << "Accept failed\n" << WSAGetLastError() << endl;
            closesocket(socket);

        }
        const size_t BUFFER_SIZE = 1024;
        vector<char> buffer(BUFFER_SIZE);
        int recv_client;

        if ((recv_client = recv(socket, buffer.data(), buffer.size(), 0)) <0 ) {
            cerr << "recv failed\n" << WSAGetLastError() << endl;
            closesocket(socket);
        }

        vector<char> msg = {'M','S','G'};
        int len , byte_sent;
        len = strlen(msg.data());
        msg.push_back('\0');
        if ((byte_sent = send(new_socket, msg.data(), len, 0)) == SOCKET_ERROR) {
            cerr << "Send failed\n" << WSAGetLastError() << endl;
            closesocket(new_socket);
        }
        else {
            cout << " " << msg.data() << " sent successfully " << endl;
        }
        closesocket(socket);
    }

    closesocket(new_socket);
    freeaddrinfo(servinfo);
    WSACleanup();
    return 0;
}
