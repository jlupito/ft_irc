#pragma once

/*
https://datatracker.ietf.org/doc/html/rfc2812#section-5
*/

# define RPL_WELCOME(user_id, nickname) (":localhost 001 " + nickname + " :Welcome to the Internet Relay Network " + user_id + "\r\n")
# define RPL_YOURHOST(client, servername, version) (":localhost 002 " + client + " :Your host is " + servername + " , running version " + version + "\r\n")
# define RPL_CREATED() (": localhost 003 " + "This server was created <date>")
# define RPL_MYINFO() (": localhost 004 " +  "<servername> <version> <available user modes>" +  "<available channel modes>")
# define RPL_INVITING(client, nickname, channel) (":localhost 341 " + client + " " + nickname + " " + channel)
// # define RPL_INVITELIST(client, channel) (":localhost 336 " + client + " " + channel)
// # define RPL_ENDOFINVITELIST(client) (":localhost 337 " + client + ":End of /INVITE list")

/* PASS ERROR REPLIES */
# define NEEDMOREPARAMS_ERR(command) (": localhost 461 " + command + " :Not enough parameters")
# define ALREADYREGISTRED_ERR() (": localhost 462 :Unauthorized command (already registered)")

/* NICK ERROR REPLIES */
# define NONICKNAMEGIVEN_ERR() (":localhost 431 :No nickname given")
# define ERRONEUSNICKNAME_ERR(nickname) (":localhost 432 " + nickname + " :Erroneous nickname")
# define NICKNAMEINUSE_ERR(nickname) (":localhost 433 " + nickname + ":Nickname is already in use")
# define NICKCOLLISION_ERR(nickname, username, sername) (":localhost 436 " + nickname + ":Nickname collision KILL from " + username + "@" + sername)
# define UNAVAILRESOURCE_ERR() (":localhost 437 ") // "<nick/channel> :Nick/channel is temporarily unavailable" cf avec channels
# define RESTRICTED_ERR() (":localhost 484 :Your connection is restricted!")

/* USER ERROR REPLIES */

/* NICK ERROR REPLIES */

/* INVITE ERROR REPLIES */
# define NOSUCHCHANNEL_ERR(channel) (":localhost 403 " + channel + ":No such channel" + "\r\n")
# define USERNOTINCHANNEL_ERR(client, nickname, channel) (":localhost 441 " + client + " " + nickname + " " + channel + ":They aren't on that channel" + "\r\n")
# define NOTONCHANNEL_ERR(client, channel) (":localhost 442 " + client + " " + channel + ":You're not on that channel" + "\r\n")
# define USERONCHANNEL_ERR(client, nickname, channel) (":localhost 443 " + client + " " + nickname + " " + channel + ":is already on channel" + "\r\n")
# define CHANOPRIVSNEEDED_ERR(channel) (":localhost 482 " + channel + ":You're not channel operator" + "\r\n")

# define RPL_KICK(user_id, channel, kicked, reason) (user_id + " KICK #" + channel + " " + kicked + " " + reason + "\r\n")
//:operator!operator@irc.server.com KICK #channel user123 :Motif de l'expulsion
# define RPL_PART(user_id, channel, reason) (user_id + " PART #" + channel + " " + (reason.empty() ? "." : reason ) + "\r\n")
//:user123!user@host.com PART #channel :Motif de l'expulsion
# define RPL_INVITE(user_id, invited, channel) (user_id + " INVITE " + invited + " #" + channel + "\r\n")
# define RPL_INVITING(client, nick, channel) (":localhost 341 " + client + " " + nick + " " + channel + "\r\n")
# define RPL_NOTOPIC(client, channel) (":localhost 331 " + client + " " + channel + ":No topic is set" + "\r\n")
# define RPL_TOPIC(client, channel, topic) (":localhost 332 " + client + " " + channel + " :" + topic + "\r\n")
# define RPL_TOPICWHOTIME(client, channel, nick, setat) (":localhost 333 " + client + " " + channel + " " + nick + " " + setat + "\r\n")

# define RPL_CHANNELMODEIS(client, channel, modestring) (":localhost 324 " + client + " " + channel + " " + modestring + " " + "\r\n")