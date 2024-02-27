#include "Server.hpp"
#include "Commands.hpp"
#include "Replies.hpp"
#include "Client.hpp"

/*
Command: KILL
   Parameters: <nickname> <comment>
*/

/*
void	Cmd::KILL(const Cmd &c)
{
	if (!c.get_client().is_operator())
		return (Server::reply(Reply::ERR_NOPRIVILEGES, c.get_client()));
	if (!c.get_arg().size() || !c.get_trailing().size())
		return (Server::reply(Reply::ERR_NEEDMOREPARAMS, c.get_client(), c.get_cmd_name()));
	if (!c.get_server().nick_exists(c.get_arg()[0]))
		return (Server::reply(Reply::ERR_NOSUCHNICK, c.get_client(), c.get_arg()[0]));

	c.get_server().get_client(c.get_arg()[0])->set_status(KILLED);
	c.get_server().get_client(c.get_arg()[0])->write_buffer(
		std::string(c.get_cmd_name() + " " + c.get_arg()[0] + " :" + c.get_trailing()));
}


*/
void	handleKILLErrors(Server& server, Client* client, cmdStruct* command) {

	if (command->params.size() > 2)
		return ;
}


void	handleKILLCommand(Server& server, Client* client, cmdStruct* command) {

	std::string reply = "";
	const std::string killMessage = command->params[1];

   handleKILLErrors(server, client,command);
	if (!killMessage.empty())
		reply = "KILL" + killMessage + "\r\n";
   else
      std::string reply = "KILL\r\n";
	sendBytesToClient(client, reply.c_str());
	server.removeClient(client->getNickname());
}

/*

The KILL message is used to cause a client-server connection to be
   closed by the server which has the actual connection.  KILL is used
   by servers when they encounter a duplicate entry in the list of valid
   nicknames and is used to remove both entries.  It is also available
   to operators.

   Clients which have automatic reconnect algorithms effectively make
   this command useless since the disconnection is only brief.  It does
   however break the flow of data and can be used to stop large amounts
   of being abused, any user may elect to receive KILL messages
   generated for others to keep an 'eye' on would be trouble spots.

   In an arena where nicknames are required to be globally unique at all
   times, KILL messages are sent whenever 'duplicates' are detected
   (that is an attempt to register two users with the same nickname) in
   the hope that both of them will disappear and only 1 reappear.

   The comment given must reflect the actual reason for the KILL.  For
   server-generated KILLs it usually is made up of details concerning
   the origins of the two conflicting nicknames.  For users it is left
   up to them to provide an adequate reason to satisfy others who see
   it.  To prevent/discourage fake KILLs from being generated to hide
   the identify of the KILLer, the comment also shows a 'kill-path'
   which is updated by each server it passes through, each prepending
   its name to the path.

   Numeric Replies:

           ERR_NOPRIVILEGES                ERR_NEEDMOREPARAMS
           ERR_NOSUCHNICK                  ERR_CANTKILLSERVER


   KILL David (csd.bu.edu <- tolsun.oulu.fi)
                                   ; Nickname collision between csd.bu.edu
                                   and tolson.oulu.fi


   NOTE:
   It is recommended that only Operators be allowed to kill other users
   with KILL message.  In an ideal world not even operators would need
   to do this and it would be left to servers to deal with.

*/
