#include "Server.hpp"

void Server::handleNickCommand(int clientSocket, std::vector<std::string>& params) {

	if (params.size() != 1)
		throw errorInCommandParameters();
	// set le nickname, respecter un format ?
}



void Server::handleUserCommand(int clientSocket, std::vector<std::string>& params) {}



// ... FAIRE LISTE DES handlers.
