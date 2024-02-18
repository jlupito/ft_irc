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
// #include "Server.hpp"
#include <vector>
#pragma once

class Server;

struct cmdStruct {

	std::string					prefix;
	std::vector<std::string>	params;
	std::string					message;
};

void 		processEvent(Server& server, int i);
void		processCmd(Server& server, Client* client, std::string cmdFull);
void		executeCmd(Server& server, Client* client, cmdStruct *cmdCut);

void		sendBytes(Client* client, const char* reply);
void		handleCAP_LSCommand(Server& server, Client* client, cmdStruct* command);
void		handlePASSCommand(Server& server, Client* client, cmdStruct* command);
int			handleNICKErrors(Server& server, Client* client, cmdStruct* command);
void		handleNICKCommand(Server& server, Client* client, cmdStruct* command);
void		handleUSERCommand(Server& server, Client* client, cmdStruct* command);
void		handleOPERCommand(Server& server, Client* client, cmdStruct* command);
void		handleQUITCommand(Server& server, Client* client, cmdStruct* command);
// void		handleERRORCommand(Server& server, Client* client, cmdStruct* command);
// void		handleKILLCommand(Server& server, Client* client, cmdStruct* command);
// void		handlePRVMSGCommand(Server& server, Client* client, cmdStruct* command);

// void		handleJOINCommand(Server& server, Client* client, cmdStruct* command);
// void		handlePARTCommand(Server& server, Client* client, cmdStruct* command);
// void		handleNAMESCommand(Server& server, Client* client, cmdStruct* command);
// void		handleLISTCommand(Server& server, Client* client, cmdStruct* command);

// void		handleKICKCommand(Server& server, Client* client, cmdStruct* command);
// void		handleINVITECommand(Server& server, Client* client, cmdStruct* command);
// void		handleTOPICCommand(Server& server, Client* client, cmdStruct* command);
// void		handleMODECommand(Server& server, Client* client, cmdStruct* command);

class dataTransferError : public std::exception {
		public:
		virtual const char* what() const throw() {
			return "Error : failure in send or receive.";
		}
};

class commandDoesntExist : public std::exception {
		public:
		virtual const char* what() const throw() {
			return "Error : this command doesn't exist.";
		}
};
