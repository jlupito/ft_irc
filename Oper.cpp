#include "Server.hpp"
#include "Commands.hpp"
#include "Replies.hpp"
#include "Client.hpp"

/*
Command: OPER
	Parameters: <user> <password>
*/

void	handleOPERCommand(Server& server, Client* client, cmdStruct* command) {

	std::string			reply = NULL;
	const std::string	userName = command->params[1];
	const std::string	passwordSubmitted = command->params[2];
	if (userName.empty())
		reply = NEEDMOREPARAMS_ERR(userName);
	else {
		std::map<const int, Client *>::iterator it = server.getClients().find(client->getClientSocket());
		if (it != server.getClients().end()) {
			if (passwordSubmitted == server.getPassWord())
				reply = YOUREOPER_RPL();
			else
				reply = PASSWDMISMATCH_ERR();
		} else
			reply = NOOPERHOST_ERR();
	}
	sendBytes(client, reply.c_str());
}
