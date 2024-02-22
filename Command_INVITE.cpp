#include "Server.hpp"
#include "Commands.hpp"
#include "Replies.hpp"
#include "Channel.hpp"
#include <cstring>

# define ERR_NEEDMOREPARAMS 461
# define ERR_NOSUCHCHANNEL 403
# define ERR_CHANOPRIVSNEEDED 482
# define ERR_NOTONCHANNEL 442
# define ERR_USERONCHANNEL 443

//command = INVITE <nickname> <channel>

bool	handleInviteErrors(Client *client, Channel* channel, std::string &invitee, std::string &inviter, cmdStruct* command) {
	
	std::string reply;
	if (command->params.size() != 3)
		reply = NEEDMOREPARAMS_ERR(command->params[0]);
	else if (!channel or channel->getClientsList().empty())
		reply = NOSUCHCHANNEL_ERR(command->params[2]);
	else if (!channel->isClient(inviter))
		reply = NOTONCHANNEL_ERR(inviter, command->params[1]);
	else if (channel->getMode().find("i") != std::string::npos and !channel->isOperator(inviter))
		reply = CHANOPRIVSNEEDED_ERR(command->params[2]);
	else if (channel->isClient(invitee))
		reply = USERONCHANNEL_ERR(inviter, invitee, command->params[2]);
	if (!reply.empty()) {
		sendBytes(client, reply.c_str());
		return true;
	}
	return false;
}

void handleINVITECommand(Server& server, Client* client, cmdStruct* command) {
	std::string reply;

	std::string channelName = command->params[2];
	if (!channelName.empty() and channelName.find("#") == 0)
		channelName.erase(0, 1);
	Channel *channel = server.getChannels()[channelName];

	std::string invitee = command->params[1];
	std::string inviter = client->getNickName();

	if (handleInviteErrors(client, channel, invitee, inviter, command))
		return ;

	Client inviteeClient;
	for (std::map< const int, Client * >::iterator it = server.getClients().begin(); it != server.getClients().begin(); it++) {
		if (it->second->getNickName() == invitee)
			inviteeClient = *it->second;
	}

	std::string userID = (!command->prefix.empty()) ? command->prefix : (":" + client->getNickName() + "!" + client->getUserName() + "@" + client->getRealName());
	
	reply = RPL_INVITE(userID, invitee, command->params[1]);
	sendBytes(&inviteeClient, reply.c_str());
	reply = RPL_INVITING(inviter, invitee, command->params[1]);
	sendBytes(client, reply.c_str());
	
	return ;
}

