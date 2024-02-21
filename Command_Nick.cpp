#include "Server.hpp"
#include "Commands.hpp"
#include "Replies.hpp"
#include "Client.hpp"
#include <cstring>

# define NONICKNAMEGIVEN 431
# define ONEUSNICKNAME 432
# define NICKNAMEINUSE 433
# define UNAVAILRESOURCE 437
# define RESTRICTED 484

bool nickFormat(std::string nickname) {

	std::cout << nickname << std::endl;
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

void	handleNICKCommand(Server& server, Client* client, cmdStruct* command) {

	std::string reply = "Connexion failure";
	int errorCode = handleNICKErrors(server, client, command);
	int connexion = client->getConnectionStatus();

	if (connexion & 0x03)
	{
		switch (errorCode) {

			case NONICKNAMEGIVEN:
			reply = NONICKNAMEGIVEN_ERR();
			break;
			case ONEUSNICKNAME:
			reply = ERRONEUSNICKNAME_ERR(client->getNickname());
			break;
			case NICKNAMEINUSE:
			reply = NICKNAMEINUSE_ERR(client->getNickname());
			break;

			case 0:
			// nouveau pseudo en cas de nouvelle connexion :
			if (client->getNickname().empty()) {
				client->setNickname(command->params[1]);
				client->setConnectionStatus(connexion | 0x07);
				reply = "NICK - Nickname was successfully set.";
			} else {
			// Changement de pseudonyme
			std::string oldNickname = client->getNickname();
			client->setNickname(command->params[1]);
			reply = "NICK - Nickname changed from " + oldNickname + " to " + client->getNickname() + ".";
			//informOtherClientsOfNickChange(server, client, oldNickname);
			}
			break ;
		}
	}
	sendBytes(client, reply.c_str());
}
int	handleNICKErrors(Server& server, Client* client, cmdStruct* command) {

	int	codeError = 0;
	std::string nickName = command->params[1];

	if (nickName.empty()) {
		std::cout << "Test : nickname EMPTY." << std::endl;
		codeError = 431;
	}
	if (!nickFormat(nickName)) {
		std::cout << "Test : nickname INVALID FORMAT." << std::endl;
		codeError = 432;
	}
	for (std::map<const int, Client *>::iterator it = server.getClients().begin();
		it != server.getClients().end(); it++) {
		if (it->second->getNickname() == nickName) {
			std::cout << "Test : nickname ALREADY EXISTING." << std::endl;
			codeError = 433; }
	}
	return codeError;
}
