#include "Server.hpp"
#include "Commands.hpp"
#include "Replies.hpp"
#include "Client.hpp"
#include <cstring>

# define NONICKNAMEGIVEN 431
# define ONEUSNICKNAME 432
# define NICKNAMEINUSE 433

bool nickFormat(std::string nickname) {

	if (nickname.length() > 9)
		return false;

	for (unsigned long i = 0; i < nickname.length(); i++) {
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

	(void)client;
	if (nickName.empty())
		codeError = 431;
	else if (!nickFormat(nickName))
		codeError = 432;
	else {

		for (std::map<const int, Client*>::iterator it = clientsList.begin();
			it != clientsList.end(); it++) {
			if ((it->second)->getNickname() == nickName) {
				codeError = 433;
				break;
			}
		}
	}
	return codeError;
}

void	handleNICKCommand(Server& server, Client* client, cmdStruct* command) {

	std::string reply = "Server failed to connect on NICK command.\r\n";
	if (command->params.size() < 2) {
		sendBytesToClient(client, reply.c_str());
		return;
	}
	int errorCode = handleNICKErrors(server, client, command);
	int connexion = client->getConnectionStatus();

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
				connexion = 3;
				client->setConnectionStatus(connexion);
				reply = "You're now known as " + client->getNickname() + "\r\n";
			}
		}
	}
	else if (connexion == 4) {

		if (errorCode == 0) {

			std::string oldNickname = client->getNickname();
			client->setNickname(command->params[1]);
			reply = ":" + oldNickname + " NICK " + client->getNickname() + "\r\n";
			informAllClientsOfNickChange(server, client, oldNickname);
		}
		else
			reply = ERRONEUSNICKNAME_ERR(command->params[1]);
	}
	sendBytesToClient(client, reply.c_str());
}
