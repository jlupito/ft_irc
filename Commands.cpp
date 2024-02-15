#include "Server.hpp"
#include "Commands.hpp"

typedef void (Server::*cmdFunction)(Server, Client, cmdStruct);

void	executeCmd(Server& server, Client* client, cmdStruct cmdCut) {

	std::string cmdName = cmdCut.cmd.substr(0, cmdCut.cmd.find(' '));
	for (std::map< std::string, cmdFunction >::iterator it = server.getCmdList().begin();
		it != server.getCmdList().end(); ++it) {
		if (it->first == cmdName)
			(server.*(it->second))(server, *client, cmdCut);
	}
	/******************A METTRE DANS CHAQUE FONCTION: *****************************/
	// const char* reply = RPL_WELCOME(client.user_id, client.nickname);
	// ssize_t bytes_transfered = send(client->getClientSocket(), reply, sizeof(reply) - 1, 0);
	// if (bytes_transfered <= 0)
	// 	std::cout << "Server failed to send a reply to client." << std::endl; //close??
}

void	processCmd(Server& server, Client* client, std::string cmdFull) {

	cmdStruct cmdCut;
	size_t nextSpace = cmdFull.find(' ');
	size_t colon = cmdFull.find(':');

	if (!colon and cmdFull[colon + 1] != ' ') { 
		cmdCut.prefix = cmdFull.substr(0, nextSpace - 1);
		cmdFull.erase(0, nextSpace);
	}
	else if (colon and colon != std::string::npos) {
		cmdCut.cmd = cmdFull.substr(0, colon - 1);
		cmdCut.message = cmdFull.substr(colon + 1, *cmdFull.end() - 1);
	}
	else
		cmdCut.cmd = cmdFull;
	// if (cmdCut.message == "CAP LS")
	// 	client->setCAPLS();
	// if (client->getCAPLS()) // si pas recu (false)
	// 	std::cout << "Ouesh pas reçu CAP LS throw erreur" << std::endl; //throw exception;
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
