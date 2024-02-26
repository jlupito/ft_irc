#include "Server.hpp"
#include "Commands.hpp"
#include "Replies.hpp"
#include "Channel.hpp"
#include <ctime>
#include <cstring>
#include <cstdlib>
#include <string>

# define ERR_NEEDMOREPARAMS 461
# define ERR_NOSUCHCHANNEL 403
# define ERR_CHANOPRIVSNEEDED 482
# define ERR_NOTONCHANNEL 442
# define ERR_UNKNOWNMODE 472

//command = MODE <target> [<modestring> [<mode arguments>...]]

bool	handleModeErrors(Client *client, Channel* channel, std::string &user, cmdStruct* command) {

	std::string reply;
	std::string modestring;
	if (!channel->getMode().empty())
		modestring = "+" + channel->getMode();
	if (command->params.size() <= 1)
		reply = NEEDMOREPARAMS_ERR(command->params[0]);
	else if (command->params.size() == 2)
		reply = RPL_CHANNELMODEIS(user, command->params[1], modestring);
	else if (!channel->isOperator(user))
		reply = CHANOPRIVSNEEDED_ERR(command->params[2]);
	if (!reply.empty()) {
		sendBytesToClient(client, reply.c_str());
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
	std::string user = client->getNickname();

	if (handleModeErrors(client, channel, user, command))
		return;

	int remove = 0;
	if (command->params[2][0] != '-' and command->params[2][0] != '+')
		return ;
	else if (command->params[2][0] != '-')
		remove = 1;
	std::string modeparam = command->params[4];
	switch (command->params[2][1]) {
		case ('o'):
			if (remove) {
				if (channel->isClient(modeparam) and channel->isOperator(modeparam))
					channel->removeOperator(modeparam);
			}
			else {
				if (channel->isClient(modeparam) and !channel->isOperator(modeparam))
					channel->addOperators(modeparam);
			}
			break ;

		case ('t'):
			if (remove) {
				if (channel->getMode().find("t") != std::string::npos)
					channel->removeMode("t");
			}
			else {
				if (channel->getMode().find("t") == std::string::npos)
					channel->addMode("t");
			}
			break ;
		case ('i'):
			if (remove) {
				if (channel->getMode().find("i") != std::string::npos)
					channel->removeMode("i");
			}
			else {
				if (channel->getMode().find("i") == std::string::npos)
					channel->addMode("i");
			}
			break ;

		case ('l'):
			if (remove) {
				if (channel->getMode().find("l") != std::string::npos)
					channel->removeMode("l");
			}
			else {
				if (channel->getMode().find("l") != std::string::npos) {
					std::istringstream iss(modeparam);
    				int nb;
					if (iss >> nb) {
						channel->setNbrUsersLimit(nb);
						channel->addMode("l");
					}
				}
			}
			break ;

		case ('k'):
			if (remove) {
				if (channel->getMode().find("k") != std::string::npos) {
					channel->removeMode("k");
					if (!channel->getChannelPwd().empty())
						channel->getChannelPwd().clear();
				}
			}
			else {
				if (channel->getMode().find("k") == std::string::npos) {
					channel->addMode("k");
					channel->setChannelPwd(modeparam);
				}
				else {
					reply = KEYSET_ERR(command->params[1]);
					sendBytesToClient(client, reply.c_str());
				}
			}
			break ;

		default :
			reply = UNKNOWNMODE_ERR(command->params[2][1], command->params[1]);
			sendBytesToClient(client, reply.c_str());
			break;
	}
	return ;
}
