#include "Server.hpp"
#include "Commands.hpp"
#include "Replies.hpp"
#include "Client.hpp"

/*
Command: USER
	Parameters: <username> <hostname> <servername> :<realname>
si on ignore la communication inter-srveurs, on ignore les params
<hostname>  et <servername> A confirner pour implemtation gestion erreurs
Realname peut contenir des espaces et doit être préfixé par deux-points (':')
*/

void	handleUSERCommand(Server& server, Client* client, cmdStruct* command) {

	std::string reply = "Connexion failure";
	int connexion = client->getConnectionStatus();
	// si le flag _connected est equivalent à 0111 (0x07 vaut 00000111)

	for (int i = 0; i < command->params.size(); i++)
		std::cout << "Param" << i << "est : " << command->params[i] << std::endl;
	if (connexion & 0x07) {

		if (command->params.size() != 4) { // truc chelou sur le nombre de params (5 normalement), souci avec size()?
			reply = NEEDMOREPARAMS_ERR(command->params[0]); sendBytes(client, reply.c_str()); return ; }

		for (std::map<const int, Client *>::iterator it = server.getClients().begin();
		it != server.getClients().end(); it++) {
			if (it->second->getUserName() == command->params[1]) {
				std::cout << "Error : User already exists." << std::endl;
				reply = "Test : nickname ALREADY EXISTING."; sendBytes(client, reply.c_str()); return ; }
		}
		client->setUserName(command->params[1]);
		// ATTENTION format avec les ":" a verifier -> parsing
		client->setRealName(command->params[3]);
		// on veut verifier que ca a ete bien rentre dans la map :
		std::cout << "Nickname : " << server.getClients()[client->getClientSocket()]->getNickname() << std::endl;
		std::cout << "User Name: " << server.getClients()[client->getClientSocket()]->getUserName() << std::endl;
		std::cout << "Real Name : " << server.getClients()[client->getClientSocket()]->getRealName() << std::endl;
		client->setConnectionStatus(connexion | 0xF);
		reply = RPL_WELCOME(client->getUserName(), client->getNickname()).c_str();
		sendBytes(client, reply.c_str());
	}
}
