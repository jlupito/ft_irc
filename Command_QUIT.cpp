#include "Server.hpp"
#include "Commands.hpp"
#include "Replies.hpp"
#include "Client.hpp"

/*
Command: QUIT
   Parameters: [ <Quit Message> ]
*/

void	handleQUITCommand(Server& server, Client* client, cmdStruct* command) {

	if (command->params.size() > 2)
		return ;

	std::string reply = command->prefix + " QUIT\r\n";
	std::string user = client->getNickname();
	std::string reason = "";

	if (command->params.size() == 2 && (!command->params[1].empty())) {
		reason = command->params[1];
		reply = command->prefix + " QUIT :" + reason + "\r\n";
	}

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
			return ;
	}
	sendBytesToClient(client, reply.c_str());
	server.removeClient(client->getNickname());
}
// verfiier que le QUIT sans param et le QUIT avec param sont bien affichés dans
// les channels dans lesquels le client qui QUIT se trouve.
