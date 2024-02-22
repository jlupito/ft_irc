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

//# define RPL_INVITING 341 "<client> <nick> <channel>"
// Sent as a reply to the INVITE command to indicate that the attempt was 
// successful and the client with the nickname <nick> has been invited to <channel>.

# define ERR_USERONCHANNEL 443
// Returned when a client tries to invite <nick> to a channel they’re already joined to.

//command = INVITE <nickname> <channel>

int	handleINVITEErrors(Channel* channel, std::string &invitee, std::string &inviter, cmdStruct* command) {
	
	if (command->params.size() != 3)
		return ERR_NEEDMOREPARAMS;
	if (!channel or channel->getClientsList().empty())
		return ERR_NOSUCHCHANNEL;
	if (!channel->isClient(inviter))
		return ERR_NOTONCHANNEL;
	if (channel->getMode().find("i") != std::string::npos and !channel->isOperator(inviter))
		return ERR_CHANOPRIVSNEEDED;
	if (channel->isClient(invitee))
		return ERR_USERONCHANNEL;
}

void handleINVITECommand(Server& server, Client* client, cmdStruct* command) {
	std::string reply;
	std::string channelName = command->params[2];
	if (!channelName.empty() and channelName.find("#") == 0)
		channelName.erase(0, 1);
	Channel *channel = server.getChannels()[channelName];
	std::string invitee = command->params[1];
	std::string inviter = client->getNickName();
	Client inviteeClient;
	for (std::map< const int, Client * >::iterator it = server.getClients().begin(); it != server.getClients().begin(); it++) {
		if (it->second->getNickName() == invitee)
			inviteeClient = *it->second;
	}
	int errorCode = handleINVITEErrors(channel, invitee, inviter, command);

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

		case ERR_USERONCHANNEL:
		reply = USERONCHANNEL_ERR(inviter, invitee, command->params[2]);
		break;

		default:
		reply = ":" + inviter + "!" + client->getUserID() + " INVITE " + invitee + " :" + channelName;
		sendBytes(&inviteeClient, reply.c_str());
		reply = RPL_INVITING(inviter, invitee, channelName);
		break;

	}
	sendBytes(client, reply.c_str());

}

