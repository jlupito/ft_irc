#include "Server.hpp"
#include "Handlers.hpp"

void Server::handleNickCommand(int clientSocket, std::vector<std::string>& params) {

	if (params.size() != 1)
		throw errorInCommandParameters();
	// set le nickname, respecter un format ?
}



void Server::handleUserCommand(int clientSocket, std::vector<std::string>& params) {}

// ... FAIRE LISTE DES handlers.


void Server::executeClientCommands(int clientSocket,
				std::map<std::string, std::vector<std::string>>& _cmds) {

	for (std::map<std::string, std::vector<std::string>>::iterator it = _cmds.begin(); it != _cmds.end(); ++it) {
		std::string commandName = it->first;
		std::map<std::string, commandFunction>::iterator handlerIt = _commandHandlers.find(commandName);
		// On appelle la fonction de commande correspondante
		if (handlerIt != _commandHandlers.end())
			(this->*(handlerIt->second))(clientSocket, it->second);
		else
			std::cout << "Unknown command: " << commandName << std::endl;
	}
}

const char*	Server::processCommand(int clientSocket, std::string cmd) {

	const char* reply;
	size_t start = 0;
	size_t prefixPos = cmd.find(':');
	if (prefixPos != std::string::npos) {
		size_t sizePrefix = cmd.find(' ');
		if (sizePrefix != std::string::npos)
			start = sizePrefix + 1;
		else
			throw Server::commandNotFound();
	}
	// une fois obtenu le debut de la suite de commandes (start a ete màj)
	std::map<std::string, std::vector<std::string>> commandMap;
	// le but est de mettre le nom de la CMD en first et de remplir le vecteur des strings du second

	return (reply);
}

// // on veut injecter chaque comamnde et ses parametres dnas la map :
// std::map<std::string, std::vector<std::string>>	Server::processClientCommands(Server& server,
// 				int clientSocket, std::string receivedData) {

// 	size_t start = 0;
// 	size_t prefixPos = receivedData.find(':');
// 	if (prefixPos != std::string::npos) {
// 		size_t sizePrefix = receivedData.find(' ');
// 		if (sizePrefix != std::string::npos)
// 			start = sizePrefix + 1;
// 		else
// 			throw Server::commandNotFound();
// 	}
// 	// une fois obtenu le debut de la suite de commandes (start a ete màj)
// 	std::map<std::string, std::vector<std::string>> commandMap;
// 	// le but est de mettre le nom de la CMD en first et de remplir le vecteur des strings du second

// 	return (commandMap);
// }

void Server::processEvent(int i) {

	char buffer[512];
	int clientSocket = _events[i].data.fd;
	Client *client = _clients[clientSocket];

	ssize_t bytes_transfered = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
	if (bytes_transfered <= 0) {
		std::cout << "Server failed to receive client's reply." << std::endl;
		close(clientSocket); // faire une fonction handleDisconnect(&server, clienFd); plutot ?
	} 

	else {
		buffer[bytes_transfered] = '\0'; // on ajoute un octet nul
		std::string receivedData(buffer);
		if (!client->getBuffer().empty())
			receivedData = client->getBuffer() + receivedData;
		std::string cmd;
		size_t pos;
		while ((pos = receivedData.find("\r\n")) != std::string::npos) {
			cmd = receivedData.substr(0, pos - 1);
			receivedData.erase(0, pos - 1);
			std::cout << "Cmd is : " << cmd << std::endl;
			const char* reply = processCommand(clientSocket, cmd);
			bytes_transfered = send(clientSocket, reply, sizeof(reply) - 1, 0);
			if (bytes_transfered <= 0)
				std::cout << "Server failed to send a reply to client." << std::endl; //close??
		}
		client->setBuffer(receivedData);
		std::cout << "Buffer now contains: " << client->getBuffer() << std::endl;
		// std::map<std::string, std::vector<std::string>> commandMap = server.processClientCommands(server, clientSocket, receivedData);
		// server.executeClientCommands(server, clientSocket, commandMap);
	}
}
