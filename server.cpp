//
// Created by Dark on 07/07/2026.
//
#include <iostream>
#include <vector>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <sstream>
using namespace std;

#define BACKLOG 5

int main(int argc, char *argv[]) {                                      //initilize Winsoc api for socket programing
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

    int status;                                    //create structure to store address info of the server
    struct addrinfo hints;
    struct addrinfo *servinfo, *p;
    struct sockaddr_storage client;
    socklen_t addr_size = sizeof client;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;                    //server uses ipv4 for connection
    hints.ai_socktype = SOCK_STREAM;              //use tcp protocol
    hints.ai_flags = AI_PASSIVE;                  //use client ip for server creation
    if ((status = getaddrinfo(nullptr, argv[1], &hints, &servinfo)) != 0) {
        cerr << "getaddrinfo failed\n" << gai_strerror(status) << endl;
        WSACleanup();
        return 2;
    }
    char ipstr[INET6_ADDRSTRLEN];
    int socket_fd, new_socket, yes=1;
    for (p = servinfo; p!= nullptr; p = p->ai_next) {           //loop through addresses and create socket with whichever connects
        if ((socket_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == SOCKET_ERROR) {
            cerr << "Socket creation failed\n" << WSAGetLastError() << endl;
            closesocket(socket_fd);
            continue;
        }

        setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(int)); //allows reuse of the same socket removing waiting time if the socket has not been rleased

        if (bind(socket_fd,p->ai_addr, p->ai_addrlen) == SOCKET_ERROR) {
            cerr << "Bind failed\n" << WSAGetLastError() << endl;
            closesocket(socket_fd);
            continue;
        }
        break;
    }
    freeaddrinfo(servinfo);

    if (p == nullptr) {                             //if no addresses found then give error and cleanup WSA
        cerr << "Server failed to bind to any address\n";
        WSACleanup();
        return 3;
    }

    if (listen(socket_fd, BACKLOG) == SOCKET_ERROR) {           //listen to incoming connections on the server
        cerr << "Listen failed\n" << WSAGetLastError() << endl;
        closesocket(socket_fd);
    }

    while (true) {                  //accept the server connection and create a new socket for it
        new_socket = accept(socket_fd, (struct sockaddr*)&client, &addr_size);     //we use while loop because the data doesnt stop coming so we keep looping until data stops
        if (new_socket == SOCKET_ERROR) {
            cerr << "Accept failed\n" << WSAGetLastError() << endl;
            continue;
        }
        const size_t BUFFER_SIZE = 1024;
        vector<char> buffer(BUFFER_SIZE);
        int recv_client;
        // check the client connected using ipv4 or ipv6
        if (((struct sockaddr*)&client)->sa_family == AF_INET) {
            inet_ntop(client.ss_family,&((struct sockaddr_in*)&client)->sin_addr,ipstr, sizeof ipstr);
        }
        if (((struct sockaddr*)&client)->sa_family == AF_INET6) {
            inet_ntop(client.ss_family,&((struct sockaddr_in6 *)&client)->sin6_addr, ipstr, sizeof ipstr);
        }

        cout << "Connected to client: " << ipstr << endl;
        recv_client = recv(new_socket, buffer.data(), buffer.size(), 0);
        //keep loop running till the client doesn't disconnect
        while (recv_client > 0) {   //the data sent from client is  seperated by ',' so we slip it using ',' and storing it into Temp and Humidity
            char* token;
            vector<float> num;
            token = strtok(buffer.data(), ",");
            while (token != nullptr) {
                num.push_back(atoi(token));
                token = strtok(nullptr, ",");

            if (num.size() >= 2) {
                float Temp = num[0];
                float Humidity = num[1];
                cout << "\nTemperature: " << Temp << "\tHumidity: " << Humidity;
            }else {
                cerr << "Invalid data received\n";
            }
            //    cout << "Received from ESP32: " << string(buffer.data(), recv_client) << endl;
            recv_client = recv(new_socket, buffer.data(), buffer.size(), 0);
        }
        if (recv_client == 0){
        cerr << "Connection closed. Client stopped communicating\n" << WSAGetLastError() << endl;
        }
        else if (recv_client  < 0 ) {
           cerr << "recv failed\n" << WSAGetLastError() << endl;
        }

        //vector<char> msg = {'M','S','G'};
        //int len , byte_sent;
        //len = strlen(msg.data());
        //msg.push_back('\0');
        // if ((byte_sent = send(new_socket, msg.data(), len, 0)) == SOCKET_ERROR) {
        //     cerr << "Send failed\n" << WSAGetLastError() << endl;
        //     closesocket(new_socket);
        // }
        // else {
        //     cout << " " << msg.data() << " sent successfully " << endl;
        // }
        closesocket(new_socket);
    }
        closesocket(socket_fd);
        WSACleanup();
        return 0;
}