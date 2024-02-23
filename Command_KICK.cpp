#include "Server.hpp"
#include "Commands.hpp"
#include "Replies.hpp"
#include "Channel.hpp"
#include <cstring>

# define ERR_NEEDMOREPARAMS 461
# define ERR_BADCHANMASK 476
# define ERR_USERNOTINCHANNEL 441
# define ERR_NOSUCHCHANNEL 403
# define ERR_CHANOPRIVSNEEDED 482
# define ERR_NOTONCHANNEL 442

// KICK <channel> <user> *( "," <user> ) [<comment>]

bool handleKickErrors(Client* client, std::string &user, Channel* channel, std::string &nickKicked, cmdStruct* command) {
	
	std::string reply;

	if (!command->params.size() != 3)
		reply = NEEDMOREPARAMS_ERR(command->params[0]);
	else if (!channel)
		reply = NOSUCHCHANNEL_ERR(command->params[1]);
	else if (!channel->isClient(nickKicked))
		reply = USERNOTINCHANNEL_ERR(user, nickKicked, command->params[1]);
	else if (!channel->isClient(user))
		reply = NOTONCHANNEL_ERR(user, command->params[1]);
	else if (!channel->isOperator(user))
		reply = CHANOPRIVSNEEDED_ERR(command->params[1]);
	if (!reply.empty()) {
		sendBytesToClient(client, reply.c_str());
		return true;
	}
	return false;
}

void handleKICKCommand(Server& server, Client* client, cmdStruct* command) {
	std::string reply;
	std::string channelName = command->params[1];
	if (!channelName.empty() and channelName.find("#") == 0)
		channelName.erase(0, 1);
	Channel *channel = server.getChannels()[channelName];
	std::string user = client->getNickname();
	std::string nickKicked = command->params[2];

	if (handleKickErrors(client, user, channel, nickKicked, command))
		return ;

	Client kickedClient;
	for (std::map< const int, Client * >::iterator it = server.getClients().begin(); it != server.getClients().begin(); it++) {
		if (it->second->getNickname() == nickKicked)
			kickedClient = *it->second;
	}
	std::string reason = (!command->message.empty()) ? command->message : ":Kicked by the channel's operator";
	std::string userID = command->prefix;
	// std::string userID = (!command->prefix.empty()) ? command->prefix : (":" + client->getNickname() + "!" + client->getUserName() + "@" + client->getRealName());
	
	channel->removeClientFromChan(nickKicked);
	channel->addToKicked(nickKicked);

	reply = RPL_KICK(userID, command->params[1], nickKicked, reason);
	sendBytesToClient(client, reply.c_str());

	for (std::map<std::string, Client>::iterator it = channel->getClientsList().begin();
		it != channel->getClientsList().end(); it++) {
			userID = ":" + (&it->second)->getNickname() + "!" + (&it->second)->getUserName() + "@" + (&it->second)->getRealName();
			reply = RPL_PART(userID, command->params[1], reason);
			sendBytesToClient(&it->second, reply.c_str());
		}

	return ;
}
