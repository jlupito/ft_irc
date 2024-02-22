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

//command = MODE <target> [<modestring> [<mode arguments>...]]

bool	handleModeErrors(Client *client, Channel* channel, std::string &user, cmdStruct* command) {
	
	std::string reply;
	std::string modestring;
	if (!channel->getMode().empty())
		modestring = "+" + channel->getMode();
	if (command->params.size() == 2)
		reply = RPL_CHANNELMODEIS(user, command->params[1], modestring);
	else if (!channel->isOperator(user))
		reply = CHANOPRIVSNEEDED_ERR(command->params[2]);
	if (!reply.empty()) {
		sendBytes(client, reply.c_str());
		return true;
	}
	return false;
}

void handleMODECommand(Server& server, Client* client, cmdStruct* command) {
	std::string reply;
	std::string channelName = command->params[1];
	if (!channelName.empty() and channelName.find("#") == 0)
		channelName.erase(0, 1);
	Channel *channel = server.getChannels()[channelName];
	std::string user = client->getNickName();

	if (handleModeErrors(client, channel, user, command))
		return;
	}



	return ;

}

