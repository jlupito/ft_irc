#include "Server.hpp"
#include "Commands.hpp"
#include "Replies.hpp"
#include "Client.hpp"
#include <cstring>

// ERR_NEEDMOREPARAMS 461
// ERR_ALREADYREGISTRED 462

void	handlePASSCommand(Server& server, Client* client, cmdStruct* command) {

	std::string reply = "Connexion failure.";
	int connexion = client->getConnectionStatus();

// std::cout << "COUCOU PASS avant if" << std::endl; //ok

	if (command->params.empty()) {
		reply = NEEDMOREPARAMS_ERR(command->params[0]);
		ssize_t bytes_transfered = send(client->getClientSocket(), reply.c_str(), sizeof(reply) - 1, 0);
		if (bytes_transfered <= 0)
			std::cout << "Server failed to send a reply to client." << std::endl;
	}
	// else if (connexion & 0x03) { // si on a deja valide cette etape

	// 	std::cout << "TEST PASS 2" << std::endl; //ok
	// 	reply = ALREADYREGISTRED_ERR();
	// 	sendBytes(client, reply.c_str());
	// 	return ; // handleDisconnect();
	// }
	else if ((connexion & 0x01) && (command->params[1] == server.getPassWord())
		&& command->params.size() == 2) {

		// std::cout << "COUCOU PASS DANS LA bonne Condi" << std::endl; //ok
		reply = "PASS - valid password.";
		client->setConnectionStatus(connexion | 0x03);
		std::cout << "PASS - Valid password." << std::endl;
	}
	sendBytes(client, reply.c_str());
}
