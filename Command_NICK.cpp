#include "Server.hpp"
#include "Commands.hpp"
#include "Replies.hpp"
#include "Client.hpp"
#include <cstring>

# define NONICKNAMEGIVEN 431
# define ONEUSNICKNAME 432
# define NICKNAMEINUSE 433

bool nickFormat(std::string nickname) {

	std::cout << "Le NICK utilisé est : " << nickname << std::endl;
	if (nickname.length() > 9)
		return false;

	for (int i = 0; i < nickname.length(); i++) {
		char currentChar = nickname[i];
		if (!((currentChar == '-') || (currentChar >= 'A' && currentChar <= 'Z')
			|| (currentChar >= 'a' && currentChar <= 'z')
			|| (currentChar >= '0' && currentChar <= '9')
			|| (currentChar >= 91 && currentChar <= 95)))
			return false;
	}
	return true;
}

void	informAllClientsOfNickChange(Server& server, Client* client, std::string oldNickname) {

	std::string messageToAll = ":" + oldNickname + " NICK " + client->getNickname() + "\r\n";
	const std::map<const int, Client*> clientsList = server.getClients();

	for (std::map<const int, Client*>::const_iterator it = clientsList.begin();
		it != clientsList.end(); ++it) {
		int clientSocket = it->first;
		if (clientSocket != client->getClientSocket())
			sendBytesToClient(it->second, messageToAll.c_str());
	}
}

int handleNICKErrors(Server& server, Client* client, cmdStruct* command) {

	int codeError = 0;
	std::string nickName = command->params[1];
	std::map<const int, Client*>&  clientsList = server.getClients();

	if (nickName.empty()) {
		// std::cout << "Test : nickname EMPTY." << std::endl; //ok
		codeError = 431;
	}
	else if (!nickFormat(nickName)) {
		// std::cout << "Test : nickname INVALID FORMAT." << std::endl; //ok
		codeError = 432;
	}
	else {

		std::cout << "Size of ClientsList" << clientsList.size() << std::endl;
		for (std::map<const int, Client*>::iterator it = clientsList.begin();
			it != clientsList.end(); it++) {
			std::cout << "socket of client: " << it->first << std::endl;
			if (it->second->getNickname() == nickName) {
				std::cout << "Test : nickname ALREADY EXISTING." << std::endl;
				codeError = 433;
				break;
			}
		}
	}
	return codeError;
}

void	handleNICKCommand(Server& server, Client* client, cmdStruct* command) {

	std::string reply = "Connexion failure.\r\n";
	int errorCode = handleNICKErrors(server, client, command);
	std::cout << "errorCode is :" << errorCode << std::endl;
	int connexion = client->getConnectionStatus();

	for (int i = 0; i < command->params.size(); i++) // test ok
		std::cout << "NICK IRSSI Param " << i << " est : " << command->params[i] << std::endl; //ok

	if (connexion == 2) {

		switch (errorCode) {

			case NONICKNAMEGIVEN:
			reply = NONICKNAMEGIVEN_ERR;
			break;

			case NICKNAMEINUSE:
			reply = NICKNAMEINUSE_ERR(client->getNickname());
			break;

			case 0:
			if (client->getNickname().empty()) {

				client->setNickname(command->params[1]);
				std::cout << "Nom dans la liste des clients : " << client->getNickname() << std::endl;
				std::cout << "CE qui est mis en NICK lors de la connexion w/ parameters is : " << command->params[1] << std::endl;
				connexion = 3;
				client->setConnectionStatus(connexion);
				reply = "NICK - Nickname was successfully set.\r\n";
			}
			else {

				std::string oldNickname = client->getNickname();
				client->setNickname(command->params[1]);
				reply = "NICK - Nickname changed from " + oldNickname + " to " + client->getNickname() + ". \r\n";
				informAllClientsOfNickChange(server, client, oldNickname);
			}
			break ;
		}
	}
	sendBytesToClient(client, reply.c_str());
}
