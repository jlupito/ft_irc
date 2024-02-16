#include "Server.hpp"
#include "Commands.hpp"
#include "Replies.hpp"
#include "Client.hpp"
#include <cstring>

# define NONICKNAMEGIVEN 431
# define ERRONEUSNICKNAME 432
# define NICKNAMEINUSE 433
# define UNAVAILRESOURCE 437
# define RESTRICTED 484

bool nickFormat(std::string nickname) {

	if (nickname.length() > 9)
		return false;
	for (int i = 0; i < nickname.length(); i++) {
		char currentChar = nickname[i];
		if (!((currentChar == '-') || (currentChar >= 91 && currentChar <= 96)
				|| (currentChar >= 123 && currentChar <= 125)))
			return false;
	}
	return true;
}

void	handleNICKCommand(Server& server, Client* client, cmdStruct* command) {

	std::string reply;
	int errorCode = handleNICKErrors(server, client, command);

	switch (errorCode) {

		case NONICKNAMEGIVEN:
		reply = NONICKNAMEGIVEN_ERR();
		break;

		case ERRONEUSNICKNAME:
		reply = ERRONEUSNICKNAME_ERR(client->getNickname());
		break;

		case NICKNAMEINUSE:
		reply = NICKNAMEINUSE_ERR(client->getNickname());
		break;

		case UNAVAILRESOURCE:
		// reply = UNAVAILRESOURCE_ERR(command->cmd.substr(0, command->cmd.find(' ')));
		break;

		case RESTRICTED:
		reply = RESTRICTED_ERR();
		break;

		default:
		for (std::map<const int, Client *>::iterator it = server.getClients().begin();
			it != server.getClients().end(); it++) {
			if (it->first == client->getClientSocket()) {
				it->second->setNickname(command->cmd.substr(5));
				reply = "Nickname was successfully set/updated.";
				break ; }
		}
		break ;
	}
	sendBytes(client, reply.c_str());
}

int	handleNICKErrors(Server& server, Client* client, cmdStruct* command) {

	int	codeError;
	std::string nickName = command->cmd.substr(5);
	if (nickName.empty()) // si pas de nickname
		codeError = 431;
	if (!nickFormat(nickName)) // si format invalide (longueur ou caracteres)
		codeError = 432;
	for (std::map<const int, Client *>::iterator it = server.getClients().begin();
		it != server.getClients().end(); it++) {
		if (it->second->getNickname() == nickName)
			codeError = 433; // si le nickname soumis est deja utilise par un client sur le serveur
	}
	// if () // politique de changements en fonction des delais de Maj du nickname. A FAIRE ?
	// {
	// 	codeError = 437;
	// }
	// if () // Erreur liee au mode utilisateur et aux potentielles restreintes. Cf Mode avant de finir ce if()
	// {
	// 	codeError = 484;
	// }
	return codeError;
}
