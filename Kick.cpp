#include "Server.hpp"
#include "Commands.hpp"
#include "Replies.hpp"
#include "Channel.hpp"
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

//<channel> *( "," <channel> ) <user> *( "," <user> ) [<comment>]

int	handleKICKErrors(Channel* channel, std::string &nick, cmdStruct* command) {
	
	if (!command->params.size())
		return ERR_NEEDMOREPARAMS;
	if (!channel)
		return ERR_NOSUCHCHANNEL;
	if (!channel->isClient(nick))
		return ERR_USERNOTINCHANNEL;
	if (!channel->isOperator(nick))
		return ERR_CHANOPRIVSNEEDED;
}

void handleKICKCommand(Server& server, Client* client, cmdStruct* command) {
	std::string reply;
	Channel *channel = server.getChannels()[command->params[1]];
	std::string nick = command->params[2];
	int errorCode = handleKICKErrors(channel, nick, command);

	switch (errorCode) {

		case ERR_NEEDMOREPARAMS:
		reply = ;
		break;

		case ERR_USERNOTINCHANNEL:
		reply = ;
		break;

		case ERR_NOSUCHCHANNEL:
		reply = ;
		break;

		case ERR_CHANOPRIVSNEEDED :
		reply = ;
		break;

		case ERR_NOTONCHANNEL:
		reply = ;
		break;

		default:
		/* kick le user et add to kicklist */ 
		channel->removeClientFromChan(nick);
		channel->addToKicked(nick);
		/*envoyer le message de kick au user kicked de la part de l operateur (client)*/ 
		reply = ":operator!operator@irc.server.com KICK #channel user123 :Motif de l'expulsion";
		sendBytes(client, reply.c_str());
		// envoyer la notif de kick a tous les users du channel
		for (std::map<std::string, Client>::iterator it = channel->getClientsList().begin();
			it != channel->getClientsList().end(); it++) {
				reply = ":user123!user@host.com PART #channel :Motif de l'expulsion";
				sendBytes(*(it->second), reply.c_str());
			}
	sendBytes(client, reply.c_str());
}