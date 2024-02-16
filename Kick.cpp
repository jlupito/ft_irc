#include "Server.hpp"
#include "Commands.hpp"
#include "Replies.hpp"
#include <cstring>

# define ERR_NEEDMOREPARAMS 461
# define ERR_BADCHANMASK 476 "<channel> :Bad Channel Mask"
# define ERR_USERNOTINCHANNEL 441 "<nick> <channel> :They aren't on that channel"
// Returned by the server to indicate that the target user of the command is not on the given channel.
# define ERR_NOSUCHCHANNEL 403 "<channel name> :No such channel"
// Used to indicate the given channel name is invalid.
# define ERR_CHANOPRIVSNEEDED 482 "<channel> :You're not channel operator"
// Any command requiring 'chanop' privileges (such as MODE messages) MUST 
// return this error if the client making the attempt is not a chanop on the 
// specified channel.
# define ERR_NOTONCHANNEL 442 "<channel> :You're not on that channel"
// Returned by the server whenever a client tries to perform a channel 
// affecting command for which the client isn't a member.

<channel> *( "," <channel> ) <user> *( "," <user> ) [<comment>]

int	handleNICKErrors(Server& server, Client* client, cmdStruct* command) {
	command.erase()
}

void handleKICKCommand(Server& server, Client* client, cmdStruct* command) {
	std::string reply;
	int errorCode = handleKICKErrors(server, client, command);

	switch (errorCode) {

		case NONICKNAMEGIVEN:
		reply = NONICKNAMEGIVEN_ERR();
		break;

		case ERRONEUSNICKNAME:
		reply = ERRONEUSNICKNAME_ERR(client->getNickname());
		break;

		case NICKNAMEINUSE:
		reply = NICKNAMEINUSE_ERR(client->getNickname());
		break;

		case NICKCOLLISION:
		reply = NICKCOLLISION_ERR(client->getNickname(), client->getUserID(), server.getServerName());
		break;

		case UNAVAILRESOURCE:
		// reply = UNAVAILRESOURCE_ERR(command->cmd.substr(0, command->cmd.find(' ')));
		break;

		case RESTRICTED:
		reply = RESTRICTED_ERR();
		break;
	}
	sendBytes(client, reply.c_str());
}