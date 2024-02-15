#include "Server.hpp"
#include "Commands.hpp"
#include "Replies.hpp"
#include "Client.hpp"
#include <cstring>

// ERR_NEEDMOREPARAMS 461
// ERR_ALREADYREGISTRED 462

void	handlePASSCommand(Server& server, Client* client, cmdStruct* command) {

	std::string reply = NULL;
	if (command->cmd.empty()) // attention a corriger car la cmdStruct differente
	{
		reply = NEEDMOREPARAMS_ERR(command->cmd.substr(0, command->cmd.find(' ')));
		ssize_t bytes_transfered = send(client->getClientSocket(), reply.c_str(), sizeof(reply) - 1, 0);
		if (bytes_transfered <= 0)
			std::cout << "Server failed to send a reply to client." << std::endl;
	}
	if (client->getCAPLS()== true) {
		reply = ALREADYREGISTRED_ERR();
		ssize_t bytes_transfered = send(client->getClientSocket(), reply.c_str(), sizeof(reply) - 1, 0);
		if (bytes_transfered <= 0)
			std::cout << "Server failed to send a reply to client." << std::endl;
	}
	else if (command->message == "CAP LS")
		client->setCAPLS();
	reply = RPL_WELCOME(client->getUserID(), client->getNickname()).c_str();
	ssize_t bytes_transferred = send(client->getClientSocket(), reply.c_str(), strlen(reply.c_str()), 0);
	if (bytes_transferred <= 0)
		std::cout << "Server failed to send a reply to client." << std::endl;
}
