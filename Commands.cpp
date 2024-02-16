#include "Server.hpp"
#include "Commands.hpp"

typedef void (Server::*cmdFunction)(Server, Client, cmdStruct);


void	sendBytes(Client* client, const char* reply) {

	ssize_t bytes_transfered = send(client->getClientSocket(), reply, strlen(reply), 0);
	if (bytes_transfered <= 0)
		std::cout << "Server failed to send a reply to client." << std::endl;
}

void	executeCmd(Server& server, Client* client, cmdStruct cmdCut) {

	std::string cmdName = "";
	if (!cmdCut.params.empty())
		cmdName = *cmdCut.params.begin();
	for (std::map< std::string, cmdFunction >::iterator it = server.getCmdList().begin();
		it != server.getCmdList().end(); ++it) {
		if (it->first == cmdName)
			(server.*(it->second))(server, *client, cmdCut);
	}
}

void	processCmd(Server& server, Client* client, std::string cmdFull) {

	cmdStruct cmdCut;
	size_t nextSpace = cmdFull.find(' ');
	size_t colon = cmdFull.find(':');

	if (!colon and cmdFull[colon + 1] != ' ') { 
		cmdCut.prefix = cmdFull.substr(0, nextSpace - 1);
		cmdFull.erase(0, cmdCut.prefix.size());
	}
	if (colon and colon != std::string::npos) {
		cmdCut.message = cmdFull.substr(colon + 1, *cmdFull.end() - 1);
		cmdFull.erase(colon, cmdCut.message.size() + 1);
	}
    std::istringstream iss(cmdFull);
    std::string line;
    while (std::getline(iss, line, ' '))
        cmdCut.params.push_back(line);
	executeCmd(server, client, cmdCut);
}

void processEvent(Server& server, int i) {

	char 	buffer[512];
	int 	clientSocket = server.getEventsTab()[i].data.fd;
	Client 	*client = server.getClients()[clientSocket];

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
		std::string cmdFull;
		size_t pos;
		while ((pos = receivedData.find("\r\n")) != std::string::npos) {
			cmdFull = receivedData.substr(0, pos);
			receivedData.erase(0, pos + 2);
			processCmd(server, client, cmdFull);
		}
		client->setBuffer(receivedData);
	}
}
