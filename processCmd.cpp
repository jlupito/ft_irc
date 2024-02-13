#include "Server.hpp"

void Server::executeClientCommands(Server& server, int clientSocket,
				std::map<std::string, std::vector<std::string>>& _cmds) {

	for (std::map<std::string, std::vector<std::string>>::iterator it = _cmds.begin(); it != _cmds.end(); ++it) {
		std::string commandName = it->first;
		std::map<std::string, commandFunction>::iterator handlerIt = server._commandHandlers.find(commandName);
		// On appelle la fonction de commande correspondante
		if (handlerIt != server._commandHandlers.end())
			(this->*(handlerIt->second))(clientSocket, it->second);
		else
			std::cout << "Unknown command: " << commandName << std::endl;
	}
}

// on veut injecter chaque comamnde et ses parametres dnas la map :
std::map<std::string, std::vector<std::string>>	Server::processClientCommands(Server& server,
				int clientSocket, std::string receivedData) {

	size_t start = 0;
	size_t prefixPos = receivedData.find(':');
	if (prefixPos != std::string::npos) {
		size_t sizePrefix = receivedData.find(' ');
		if (sizePrefix != std::string::npos)
			start = sizePrefix + 1;
		else
			throw Server::commandNotFound();
	}
	// une fois obtenu le debut de la suite de commandes (start a ete màj)
	std::map<std::string, std::vector<std::string>> commandMap;
	// le but est de mettre le nom de la CMD en first et de remplir le vecteur des strings du second

	return (commandMap);
}

void processEvent(Server &server, int i) {

	char buffer[1024];
	int clientSocket = server.getEventsTab()[i].data.fd;
	std::cout << "Control: client on socket #" << clientSocket << "is connected." << std::endl;

	ssize_t bytes_received = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
	if (bytes_received <= 0) {
		std::cout << "Client on socket #" << clientSocket << " is disconnected." << std::endl;
		close(clientSocket); } // faire une fonction handleDisconnect(&server, clienFd); plutot ?
	else {
		buffer[bytes_received] = '\0'; // on ajoute un octet nul
		std::string receivedData(buffer);
		std::map<std::string, std::vector<std::string>> commandMap = server.processClientCommands(server, clientSocket, receivedData);
		server.executeClientCommands(server, clientSocket, commandMap);
	}
}


