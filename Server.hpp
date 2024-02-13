#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <map>
#include <vector>
#include "Client.hpp"
#pragma once

class Client;
class Server {

	public:
		Server(std::string portStr, std::string password);
		~Server();

		void	initialization(void);
		void	initiateCommandHandlers(void);
		int		getPort(void);
		int		getServerSocket(void);
		int		getEpollFd(void);
		sockaddr_in&	getServerAddr(void);
		epoll_event&	getEvent(void);
		epoll_event*	getEventsTab(void);
		std::map<const int, Client *>&		getClients(void);

		std::map<std::string, std::vector<std::string>>	processClientCommands(Server& server,
				int clientSocket, std::string receivedData);
		void	executeClientCommands(Server& server, int clientSocket, std::map<std::string,
				std::vector<std::string>>& _cmds);

	private:
		struct sockaddr_in _serverAddr; //struct pour stocker les infos relatives aux adresses IP
		struct epoll_event _event;
		struct epoll_event _events[1024];
		std::map<const int, Client *> _clients; // le int est le fd attribue au client(socket) - non cessible
		int _port;
		int _serverSocket;
		int _epollFd;
		std::string _password;
		typedef void (Server::*commandFunction)(int, std::vector<std::string>&);
		std::map<std::string, commandFunction> _commandHandlers; // mp pour contenir les noms et pointeurs sur f()

		void	handleNickCommand(int clientSocket, std::vector<std::string>& params);
		void	handleUserCommand(int clientSocket, std::vector<std::string>& params);
		// ...


	class serverInitFailure : public std::exception {
		public:
		virtual const char* what() const throw();
	};
	class commandNotFound : public std::exception {
		public:
		virtual const char* what() const throw();
	};
	class errorInCommandParameters : public std::exception {
		public:
		virtual const char* what() const throw();
	};
};
