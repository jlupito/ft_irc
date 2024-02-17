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

	if (connexion & 0x07) // si le flag _connected est equivalent à 0111 (0x07 vaut 00000111)
	{
		std::cout << "Entrée dans la commande USER " << std::endl;
		if (command->params.size() != 5)
			reply = NEEDMOREPARAMS_ERR(command->params[0]);

		std::map<const int, Client *>::iterator it = server.getClients().find(client->getClientSocket());
		if (it != server.getClients().end())
			reply = ALREADYREGISTRED_ERR();

		else {
			client->setUserName(command->params[1]);
			// ATTENTION format avec les ":" a verifier -> parsing
			client->setRealName(command->params[4]);
			std::cout << "test :" << client->getConnectionStatus() << std::endl;
			// on veut verifier que ca a ete bien rentre dans la map :
			std::cout << "test :" << server.getClients()[client->getClientSocket()]->getNickname() << std::endl;
			std::cout << "test :" << server.getClients()[client->getClientSocket()]->getUserName() << std::endl;
			std::cout << "test :" << server.getClients()[client->getClientSocket()]->getRealName() << std::endl;
			reply = "USER is valid";
		}
	}
	sendBytes(client, reply.c_str()); // verifier le message de retour cas echeant
}
	// else if (command->message == "CAP LS") // WHAT ??
	// 	client->setCAPLS();
	// reply = RPL_WELCOME(client->getUserID(), client->getNickname()).c_str();
	// ssize_t bytes_transferred = send(client->getClientSocket(), reply.c_str(), strlen(reply.c_str()), 0);
	// if (bytes_transferred <= 0)
	// 	std::cout << "Server failed to send a reply to client." << std::endl;
