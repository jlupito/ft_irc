#include "Server.hpp"
#include "Commands.hpp"
#include "Replies.hpp"
#include "Channel.hpp"
#include <ctime>
#include <cstring>
#include <map>
#include <vector>
#include <algorithm>

# define ERR_NEEDMOREPARAMS 461
# define ERR_NOSUCHCHANNEL 403
# define ERR_CHANOPRIVSNEEDED 482
# define ERR_NOTONCHANNEL 442

//command = JOIN <channel>{,<channel>} [<key>{,<key>}]
class Channel;

bool	handleJoinErrors(Client *client, Channel* channel, std::string &user, cmdStruct* command) {

	std::string reply;
	if (command->params.size() < 2)
		reply = NEEDMOREPARAMS_ERR(command->params[0]);
	else if (!channel or channel->getClientsList().empty())
		reply = NOSUCHCHANNEL_ERR(command->params[2]);
	if (!reply.empty()) {
		sendBytesToClient(client, reply.c_str());
		return true;
	}
	return false;
}

bool joinChannel(Server& server, Channel *channel, Client* client, cmdStruct* command) {
	std::string reply;
	if (channel->getMode().find("k") != std::string::npos) {
			if (channel->getChannelPwd() != command->params[2]) {
				reply = ERR_BADCHANNELKEY(client->getNickname(), channel->getChannelName());
				sendBytesToClient(client, reply.c_str());
				return false;
			}
	}
	if (channel->getMode().find("l") != std::string::npos) {
			if (channel->getClientsList().size() >= channel->getNbrUsersLimit()) {
				reply = ERR_CHANNELISFULL(client->getNickname(), channel->getChannelName());
				sendBytesToClient(client, reply.c_str());
				return false;
			}
	}
	if (channel->getMode().find("i") != std::string::npos) {
		    std::vector<std::string>::iterator it = std::find(channel->getInvited().begin(), channel->getInvited().end(), client->getNickname());
			if (it != channel->getInvited().end()) {
				reply = ERR_INVITEONLYCHAN(client->getNickname(), channel->getChannelName());
				sendBytesToClient(client, reply.c_str());
				return false;
			}
			else
				channel->removeClientFromInvite(client->getNickname());
	}
	channel->addToChan(*client);
	client->addJoinedChan(channel->getChannelName());
	reply = ":" + server.getServerName() + " JOIN:" + channel->getChannelName();
	sendBytesToClient(client, reply.c_str());
	reply = RPL_TOPIC(client->getNickname(), channel->getChannelName(), channel->getTopic());
	sendBytesToClient(client, reply.c_str());
	std::string list;
	for (std::map< std::string, Client >::iterator it = channel->getClientsList().begin(); it != channel->getClientsList().end(); it++) {
		if (channel->isOperator((&it->second)->getNickname()))
			list += "@";
		list += (&it->second)->getNickname();
		std::map<std::string, Client>::iterator nextIt = it;
    	++nextIt;
		if (nextIt != channel->getClientsList().end())
			list += " ";
	}
	reply = RPL_NAMREPLY(client->getNickname(), channel->getChannelName(), list);
	sendBytesToClient(client, reply.c_str());
	return true;
}

void handleJOINCommand(Server& server, Client* client, cmdStruct* command) {
	std::string reply;
	std::string channelName = command->params[1];
	if (!channelName.empty() and channelName[0] != '#')
		channelName.insert(0, "#");
	Channel *channel = server.getChannels()[channelName];
	std::string user = client->getNickname();

	if (handleJoinErrors(client, channel, user, command))
		return;

	std::map<std::string, Channel * >::iterator it = std::find(server.getChannels().begin(), server.getChannels().end(), command->params[1]);
	if (it->first == command->params[1])
		joinChannel(server, it->second, client, command);
	else {
		Channel* newChannel = new Channel(command->params[1]);
		server.getChannels()[newChannel->getChannelName()] = newChannel;
	}
	return ;

}

