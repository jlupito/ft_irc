#include "Server.hpp"
#include "Commands.hpp"
#include "Replies.hpp"
#include "Client.hpp"
#include <cstring>

void handleCAP_LSCommand(Server& server, Client* client, cmdStruct* command) {

	std::string reply = "Connexion failure.\r\n";
	int connexion = client->getConnectionStatus(); // doit être égal à 0;

	if (connexion == 0x00) {
		if (command->params[1] == "LS" && command->params.size() == 2) {
			client->setConnectionStatus(connexion | 0x01);
			reply = "CAP_ACK LS\r\n";
		}
	}
	sendBytesToClient(client, reply.c_str());
	std::cout << "CAPLS received by Client." << std::endl;
}