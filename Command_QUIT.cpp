#include "Server.hpp"
#include "Commands.hpp"
#include "Replies.hpp"
#include "Client.hpp"

/*
Command: QUIT
   Parameters: [ <Quit Message> ]
*/

void	handleQUITCommand(Server& server, Client* client, cmdStruct* command) {

	std::string reply = command->prefix + " QUIT\r\n";
	std::string user = client->getNickname();
	const std::string reason = command->params[1];
	if (command->params.size() > 2)
		return ;
	if (!reason.empty())
		reply = command->prefix + " QUIT :" + reason + "\r\n";
	sendBytesToClient(client, reply.c_str());

	for (std::vector< std::string >::iterator it = client->getJoinedChan().begin(); it != client->getJoinedChan().begin(); it++) {
		std::string channelName = *it;
		if (!channelName.empty() and channelName[0] != '#')
			channelName.insert(0, "#");
		Channel *channel = server.getChannels()[channelName];
			channel->removeClientFromChan(user);
			if (channel->isOperator(user))
				channel->removeOperator(user);
			client->getJoinedChan().erase(it);
			sendBytesToChannel(channel, reply.c_str());
		}
	server.removeClient(client->getNickname());
}
