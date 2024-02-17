#include "Server.hpp"
#include "Commands.hpp"
#include "Replies.hpp"
#include "Client.hpp"
#include <cstring>

// ERR_NEEDMOREPARAMS 461
// ERR_ALREADYREGISTRED 462

void	handlePASSCommand(Server& server, Client* client, cmdStruct* command) {

	// std::cout << "UTILISATION COMMANDE PASS" << std::endl; //ok
	std::string reply = "Connexion failure";
	int connexion = client->getConnectionStatus();
	if (command->params.empty()) {
		// std::cout << "Test mdp sur PASS" << std::endl;
		reply = NEEDMOREPARAMS_ERR(command->params[0]);
		ssize_t bytes_transfered = send(client->getClientSocket(), reply.c_str(), sizeof(reply) - 1, 0);
		if (bytes_transfered <= 0)
			std::cout << "Server failed to send a reply to client." << std::endl;
	}
	else if (connexion & 0x02) { // si on a deja valide cette etape

	// std::cout << "Test regsiterded  sur PASS" << std::endl;
		reply = ALREADYREGISTRED_ERR();
		ssize_t bytes_transfered = send(client->getClientSocket(), reply.c_str(), sizeof(reply) - 1, 0);
		if (bytes_transfered <= 0)
			std::cout << "Server failed to send a reply to client." << std::endl;
	}
	else if ((connexion & 0x01) && (command->params[1] == server.getPassWord())
		&& command->params.size() == 2) // si on a juste passe l'etape CAPLS
		{
			// std::cout << "Test réussi sur PASS" << std::endl; // ok
			client->setConnectionStatus(connexion | 0x02);
			std::cout << "Valid password." << std::endl;
		}
}
