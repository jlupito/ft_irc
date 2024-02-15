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
        socklen_t& 	    getClientAddrLen(void);
        int 	        getClientSocket(void);
        std::string&    getBuffer(void);
        std::string&    getUserID(void);
        std::string&    getNickName(void);
        bool            getCAPLS();
        void            setCAPLS();
        void 	        setClientSocket(int fd);
        void            setBuffer(std::string &buffer);
        void            fillAttributes(void);

        class clientConnectFailure : public std::exception {
            public:
            virtual const char* what() const throw();
        };

    private:
        struct sockaddr_in  _clientAddr;
        socklen_t           _clientAddrLen = sizeof(_clientAddr);
        int                 _clientSocket;
        std::string         _buffer;
        std::string         _nickName;
        std::string         _userID;
        bool                _capLS; // penser a le passer a False
};
