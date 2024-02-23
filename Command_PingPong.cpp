#include "Server.hpp"
#include "Commands.hpp"
#include "Replies.hpp"
#include "Client.hpp"

/*
Command: PING
  Parameters: <token>
Numeric replies :
	ERR_NEEDMOREPARAMS (461)
	ERR_NOORIGIN (409)
*/

void	handlePINGCommand(Server& server, Client* client, cmdStruct* command) {

	std::string	reply = "No Ping-Pong today.\r\n";
	const std::string	userName = command->params[1];

	if (!command->params[1].empty())
		reply = "PONG " + command->params[1] + "\r\n"; // ceci est un PONG
	else
		reply = NEEDMOREPARAMS_ERR(userName);
	sendBytesToClient(client, reply.c_str());
}

void	handlePONGCommand(Server& server, Client* client, cmdStruct* command) {

	std::string	reply = "No Ping-Pong today.\r\n";
	
	if (!command->params[1].empty())
		reply = server.getServerName() + "\r\n";
	sendBytesToClient(client, reply.c_str());
}
