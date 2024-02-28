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
		modestring = "+" + channel->getMode() + "\r\n";
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
	if (!channelName.empty() and channelName[0] != '#')
		channelName.insert(0, "#");
	Channel *channel = server.getChannels()[channelName];
	if (!channel)
		return ;
	std::string user = client->getNickname();

	if (handleModeErrors(client, channel, user, command))
		return;

	if (command->params[2][0] != '-' and command->params[2][0] != '+')
		return ;

	int remove;
	std::vector<std::string> modeParams;
	size_t nbParams = command->params.size() - 3;

	for (size_t i = 0; i <= nbParams; i++)
		modeParams.push_back(command->params[i]);
	std::vector<std::string>::iterator itParam = modeParams.begin();

	for (size_t i = 0; command->params[2][i]; i++) {

		switch (command->params[2][i]) {
			case ('-'):
				remove = 1;
				break;

			case ('+'):
				remove = 0;
				break;

			case ('o'):
				if (remove) {
					if (channel->isClient(*itParam) and channel->isOperator(*itParam))
						channel->removeOperator(*itParam);
				}
				else {
					if (channel->isClient(*itParam) and !channel->isOperator(*itParam))
						channel->addOperators(*itParam);
				}
				if (++itParam != modeParams.end())
					itParam++;
				else
					*itParam = "";
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
						std::istringstream lim(*itParam);
						int nb;
						if (lim >> nb) {
							channel->setNbrUsersLimit(nb);
							channel->addMode("l");
						}
					}
					if (++itParam != modeParams.end())
						itParam++;
					else
						*itParam = "";
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
						channel->setChannelPwd(*itParam);
						if (++itParam != modeParams.end())
							itParam++;
						else
							*itParam = "";
					}
					else {
						reply = KEYSET_ERR(channelName);
						sendBytesToClient(client, reply.c_str());
					}
				}
				break ;

			default :
				reply = UNKNOWNMODE_ERR(command->params[2][1], channelName);
				sendBytesToClient(client, reply.c_str());
				break;
		}
	}
	return ;
}
