#include "Server.hpp"
#include "Commands.hpp"
#include "Replies.hpp"
#include "Client.hpp"

/*
Command: QUIT
   Parameters: [ <Quit Message> ]
*/

void	handleQUITCommand(Server& server, Client* client, cmdStruct* command) {

	std::string reply = NULL;
	const std::string quitMessage = command->cmd.substr(command->cmd.find(":") + 1);
	reply = "QUIT";
	if (quitMessage.empty())
		reply = "QUIT" + quitMessage;
	sendBytes(client, reply.c_str());
	// deconnexion du serveur
	// fermer la socket et tout supprimer. Sortir de ce client proprement.
}
