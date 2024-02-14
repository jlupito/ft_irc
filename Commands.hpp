#include <iostream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include "Server.hpp"
#pragma once

class Server;

struct cmdStruct {
	std::string prefix;
	std::string	cmd;
	std::string message;
};

typedef void (Server::*cmdFunction)(int, std::vector<std::string>&);

void		handleNickCommand(int clientSocket, std::vector<std::string>& params);
void		handleUserCommand(int clientSocket, std::vector<std::string>& params);
void 		processEvent(Server& server, int i);
void		processCmd(Server& server, Client* client, std::string cmdFull);
void		executeCmd(Server& server, Client* client, cmdStruct cmdCut);

class dataTransferError : public std::exception {
		public:
		virtual const char* what() const throw() {
			return "Error : failure in send or receive.";
		}
	};