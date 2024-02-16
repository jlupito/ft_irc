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
#include <vector>
// #include "Server.hpp"
#pragma once

class Server;

class Channel;

struct cmdStruct {

	std::string 				prefix;
	std::vector<std::string>	params;
	std::string 				message;
};

void 		processEvent(Server& server, int i);
void		processCmd(Server& server, Client* client, std::string cmdFull);
void		executeCmd(Server& server, Client* client, cmdStruct cmdCut);

void		sendBytes(Client* client, const char* reply);
void		handlePASSCommand(Server& server, Client* client, cmdStruct* command);
int			handleNICKErrors(Server& server, Client* client, cmdStruct* command);
void		handleNICKCommand(Server& server, Client* client, cmdStruct* command);

class dataTransferError : public std::exception {
		public:
		virtual const char* what() const throw() {
			return "Error : failure in send or receive.";
		}
};
