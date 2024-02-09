#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#pragma once

class Client
{
    public:
        Client();
        ~Client();
        sockaddr_in& 	getClientAddr(void);
        socklen_t& 	getClientAddrLen(void);
        int 	getClientSocket(void);
        void 	setClientSocket(int fd);

    private:
        struct sockaddr_in _clientAddr;
        socklen_t _clientAddrLen = sizeof(_clientAddr); //taille et adresse du client est necessaire pour la focntion accept()
        int _clientSocket;


};
