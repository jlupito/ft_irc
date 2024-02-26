#include "Server.hpp"
#include "Commands.hpp"
#include "Replies.hpp"
#include "Channel.hpp"
#include <ctime>
#include <cstring>

# define ERR_NEEDMOREPARAMS 461
# define ERR_NOSUCHCHANNEL 403
# define ERR_CHANOPRIVSNEEDED 482
# define ERR_NOTONCHANNEL 442

//command = TOPIC <channel> [<topic>]

bool	handleTopicErrors(Client *client, Channel* channel, std::string &user, cmdStruct* command) {

	std::string reply;
	if (command->params.size() != 2)
		reply = NEEDMOREPARAMS_ERR(command->params[0]);
	else if (!channel or channel->getClientsList().empty())
		reply = NOSUCHCHANNEL_ERR(command->params[2]);
	else if (!channel->isClient(user))
		reply = NOTONCHANNEL_ERR(user, command->params[1]);
	else if (channel->getMode().find("t") != std::string::npos and !channel->isOperator(user) and !command->message.empty())
		reply = CHANOPRIVSNEEDED_ERR(command->params[2]);
	if (!reply.empty()) {
		sendBytesToClient(client, reply.c_str());
		return true;
	}
	return false;
}

void handleTOPICCommand(Server& server, Client* client, cmdStruct* command) {
	std::string reply;
	std::string channelName = command->params[1];
	if (!channelName.empty() and channelName.find("#") == 0)
		channelName.erase(0, 1);
	Channel *channel = server.getChannels()[channelName];
	std::string user = client->getNickname();

	if (handleTopicErrors(client, channel, user, command))
		return;

	time_t timestamp = time(NULL);
	struct tm *timeinfo;
	timeinfo = std::gmtime(&timestamp);
	std::string time = asctime(timeinfo);

	if (!command->message.empty()) {
		channel->getTopic().clear();
		if (command->message.size() > 1)
			channel->setTopic(command->message.erase(0, 1));
		for (std::map<std::string, Client>::iterator it = channel->getClientsList().begin();
		it != channel->getClientsList().end(); it++) {
			reply = RPL_TOPIC((&it->second)->getNickname(), command->params[1], channel->getTopic());
			sendBytesToClient(&it->second, reply.c_str());
			reply = RPL_TOPICWHOTIME((&it->second)->getNickname(), command->params[1], client->getNickname(), time);
			sendBytesToClient(&it->second, reply.c_str());
		}
	}
	else {
		if (!channel->getTopic().empty()) {
			reply = RPL_TOPIC(user, command->params[1], channel->getTopic());
			sendBytesToClient(client, reply.c_str());
			reply = RPL_TOPICWHOTIME(user, command->params[1], client->getNickname(), time);
			sendBytesToClient(client, reply.c_str());
		}
		else {
			reply = RPL_NOTOPIC(user, command->params[1]);
			sendBytesToClient(client, reply.c_str());
		}
	}
	return ;

}

