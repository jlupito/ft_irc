#pragma once

/*
https://datatracker.ietf.org/doc/html/rfc2812#section-5
*/

# define RPL_WELCOME(user_id, nickname) (":localhost 001 " + nickname + " :Welcome to the Internet Relay Network " + user_id + "\r\n")
# define RPL_YOURHOST(client, servername, version) (":localhost 002 " + client + " :Your host is " + servername + " , running version " + version + "\r\n")
# define RPL_CREATED() (": localhost 003 " + "This server was created <date>")
# define RPL_MYINFO() (": localhost 004 " +  "<servername> <version> <available user modes>" +  "<available channel modes>")

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
