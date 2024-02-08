#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#pragma once

const int MAX_EVENTS = 10;
const int PORT = 6667;

class Server {

	public:
	Server(int port);
	~Server();

	bool	initialization(); // micro parsing, condition pour demarrer le server
	void	startServer();

	class serverInitFailure : public std::exception {
		public:
		virtual const char* what() const throw();
	};

	private:
	int _port;
	int serverSocket;
	int clientSocket;
	int epollFd;

    struct sockaddr_in serverAddr; //struct pour stocker les infos relatives aux adresses IP
	struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr); //la taille e l adresse du client est necessaire pour la focntion accept()
    struct epoll_event events[MAX_EVENTS];

};
