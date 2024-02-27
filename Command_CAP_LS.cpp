#include "Server.hpp"
#include "Commands.hpp"
#include "Replies.hpp"
#include "Client.hpp"
#include <cstring>

void handleCAP_LSCommand(Server& server, Client* client, cmdStruct* command) {

	std::string reply = "Connexion failure.\r\n";
	int connexion = client->getConnectionStatus();

	(void)server;
	if (connexion == 0)
	{
		if (command->params[1] == "LS" && command->params.size() == 2) {
			connexion = 1;
			client->setConnectionStatus(connexion);
			reply = "CAP_ACK LS\r\n";
		}
	}
	sendBytesToClient(client, reply.c_str());
}
