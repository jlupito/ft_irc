#include "Server.hpp"
#include "Commands.hpp"
#include "Replies.hpp"
#include "Channel.hpp"
#include <cstring>

# define ERR_NEEDMOREPARAMS 461
# define ERR_NOSUCHCHANNEL 403
// Used to indicate the given channel name is invalid.
# define ERR_CHANOPRIVSNEEDED 482
// Any command requiring 'chanop' privileges (such as MODE messages) MUST 
// return this error if the client making the attempt is not a chanop on the 
// specified channel.
# define ERR_NOTONCHANNEL 442
// Returned by the server whenever a client tries to perform a channel 
// affecting command for which the client isn't a member.

//command = TOPIC <channel> [<topic>]

int	handleINVITEErrors(Channel* channel, std::string &user, cmdStruct* command) {
	
	if (command->params.size() != 2)
		return ERR_NEEDMOREPARAMS;
	if (!channel or channel->getClientsList().empty())
		return ERR_NOSUCHCHANNEL;
	if (!channel->isClient(user))
		return ERR_NOTONCHANNEL;
	if (channel->getMode().find("t") != std::string::npos and !channel->isOperator(user) and !command->message.empty())
		return ERR_CHANOPRIVSNEEDED;
}

void handleINVITECommand(Server& server, Client* client, cmdStruct* command) {
	std::string reply;
	std::string channelName = command->params[1];
	if (!channelName.empty() and channelName.find("#") == 0)
		channelName.erase(0, 1);
	Channel *channel = server.getChannels()[channelName];
	std::string user = client->getNickName();
	int errorCode = handleINVITEErrors(channel, user, command);

	switch (errorCode) {

		case ERR_NEEDMOREPARAMS:
		reply = NEEDMOREPARAMS_ERR(command->params[0]);
		break;

		case ERR_NOSUCHCHANNEL:
		reply = NOSUCHCHANNEL_ERR(command->params[2]);
		break;

		case ERR_NOTONCHANNEL:
		reply = NOTONCHANNEL_ERR(channelName);
		break;

		case ERR_CHANOPRIVSNEEDED:
		reply = CHANOPRIVSNEEDED_ERR(command->params[2]);
		break;

		default:
		if (!command->message.empty()) {
			channel->getTopic().clear();
			if (command->message.size() > 1)
				channel->setTopic(command->message.erase(0, 1));
			// boucler sur tous les clients
			reply = ":" + user + "!" + client->getUserID() + " TOPIC " + invitee + " :" + channelName;
			sendBytes(&inviteeClient, reply.c_str());
		}
		else {
			RPL_NOTOPIC 
			RPL_TOPIC 
			RPL_NOTOPICWHOTIME 

			reply = ":" + inviter + "!" + client->getUserID() + " INVITE " + invitee + " :" + channelName;
			sendBytes(&inviteeClient, reply.c_str());
			reply = RPL_INVITING(inviter, invitee, channelName);
		}
		break;

	}
	sendBytes(client, reply.c_str());

}

