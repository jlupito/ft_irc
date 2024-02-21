#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <map>
#include <vector>
#include <fcntl.h>
#include "Client.hpp"
#include "Commands.hpp"
#pragma once

typedef void (*cmdFunction)(Server&, Client*, cmdStruct*);

class Client;
class Channel;
class Server {

	public:
		Server(std::string port, std::string password);
		~Server();

		void									initialization(void);
		void									initCommandMap(void);
		int										getPort(void);
		int										getServerSocket(void);
		int										getEpollFd(void);
		std::string								getServerName(void);
		std::string 							getPassWord(void);
		sockaddr_in&							getServerAddr(void);
		epoll_event&							getEvent(void);
		epoll_event*							getEventsTab(void);
		std::map<const int, Client *>&			getClients(void);
		std::map<std::string, Channel *>&		getChannels(void);
		std::map<std::string, cmdFunction>&		getCmdList(void);
		void									removeClient(const std::string& nickname);

	private:
		std::string								_serverName;
		struct sockaddr_in 						_serverAddr; //struct pour stocker les infos relatives aux adresses IP
		struct epoll_event 						_event;
		struct epoll_event 						_events[1024];
		int										_port;
		int 									_serverSocket;
		int 									_epollFd;
		std::string								_password;
		std::map< std::string, cmdFunction > 	_cmdList; // map pour contenir les noms et pointeurs sur f()
		std::map< const int, Client * > 		_clients; // le int est le fd attribue au client(socket) - non cessible
		std::map< std::string, Channel * >		_channels;

	class serverInitFailure : public std::exception {
		public:
		virtual const char* what() const throw();
	};

	class errorInCommandParameters : public std::exception {
		public:
		virtual const char* what() const throw();
	};
};
