#include "Server.hpp"
#include "Commands.hpp"
#include "Replies.hpp"
#include "Client.hpp"

/*
Command: DIE
   Parameters: None
*/

void	handleDIECommand(Server& server, Client* client, cmdStruct* command) {

	std::string reply = "DIE\r\n";
	const std::string quitMessage = command->params[1];
	if (command->params.size() > 2)
		return ;
	if (quitMessage.empty())
		reply = "DIE" + quitMessage + "\r\n";
	sendBytesToClient(client, reply.c_str());
	server.removeClient(client->getNickname());
}

/*

An operator can use the DIE command to shutdown the server.  This
   message is optional since it may be viewed as a risk to allow
   arbitrary people to connect to a server as an operator and execute
   this command.

   The DIE command MUST always be fully processed by the server to which
   the sending client is connected and MUST NOT be passed onto other
   connected servers.

   Numeric Replies:

           ERR_NOPRIVILEGES

   Example:

   DIE                             ; no parameters required.

*/
