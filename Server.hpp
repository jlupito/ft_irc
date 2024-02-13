#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <map>
#include "Client.hpp"
#pragma once

class Client;
class Server {

	public:
		Server(int port); // a finir
		~Server();

		void							initialization(); // micro parsing, condition pour demarrer le server
		int								getPort(void);
		int								getServerSocket(void);
		int								getEpollFd(void);
		sockaddr_in&					getServerAddr(void);
		epoll_event&					getEvent(void);
		epoll_event*					getEventsTab(void);
		std::map<const int, Client *>&	getClients(void);

	private:
		struct sockaddr_in 				_serverAddr; //struct pour stocker les infos relatives aux adresses IP
		struct epoll_event 				_event;
		struct epoll_event 				_events[1024];
		std::map<const int, Client *> 	_clients; // le int est le fd attribue au client(socket) - non cessible
		int 							_port;
		int 							_serverSocket;
		int 							_epollFd;


	class serverInitFailure : public std::exception {
		public:
		virtual const char* what() const throw();
	};
};
