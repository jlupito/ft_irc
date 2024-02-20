#include "Server.hpp"
#include "Commands.hpp"
#include "Replies.hpp"
#include "Client.hpp"

/*
Command: QUIT
   Parameters: [ <Quit Message> ]
*/

void	handleQUITCommand(Server& server, Client* client, cmdStruct* command) {

	std::string reply = "QUIT";
	const std::string quitMessage = command->params[1];
	if (command->params.size() > 2)
		return ;
	if (quitMessage.empty())
		reply = "QUIT" + quitMessage;
	sendBytes(client, reply.c_str());
	server.removeClient(client->getNickname());
}
