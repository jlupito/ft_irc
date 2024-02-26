#include "Server.hpp"
#include "Commands.hpp"
#include "Replies.hpp"
#include "Client.hpp"
#include <cstring>

void handleCAP_LSCommand(Server& server, Client* client, cmdStruct* command) {

	std::string reply = "Connexion failure.\r\n";
	int connexion = client->getConnectionStatus();

	for (int i = 0; i < command->params.size(); i++) // test ok
		std::cout << "NICK IRSSI Param " << i << " est : " << command->params[i] << std::endl; //ok

	if (connexion == 0)
	{
		if (command->params[1] == "LS" && command->params.size() == 2) {
			connexion = 1;
			client->setConnectionStatus(connexion);
			reply = "CAP_ACK LS\r\n";
		}
	}
	sendBytesToClient(client, reply.c_str());
	std::cout << "CAPLS received by Client." << std::endl;
}
