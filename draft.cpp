/*
BROUILLON

Rappel :

The IRC protocol includes a special type of message called a reply. When a client sends
a command to a server, the server will send a reply (except in a few special commands where
a reply should not be expected). Replies are used to acknowledge that a command was processed
correctly, to indicate errors, or to provide information when the command performs a server
query (e.g., asking for the list of users or channels).
A reply is a message with the following characteristics:
- It always includes a prefix.
- The command will be a three-digit code. The full list of possible replies is specified
in [RFC2812 §5].
- The first parameter is always the target of the reply, typically a nick.
Examples :
:irc.example.com 433 * borja :Nickname is already in use.
:irc.example.org 332 borja #cmsc23300 :A channel for CMSC 23300 students


Commandes a traiter :

Command: PASS
	Parameters: <password>
Command: NICK
	Parameters: <nickname>
Command: USER
   Parameters: <user> <mode> <unused> <realname>
Command: OPER
	Parameters: <user> <password>
Command: QUIT
	Parameters: [<Quit message>]
Command: ERROR
   Parameters: <error message>
Command: KILL
   Parameters: <nickname> <comment>
Command: PRIVMSG
   Parameters: <receiver>{,<receiver>} <text to be sent>

Command: JOIN
   Parameters: <channel>{,<channel>} [<key>{,<key>}]
Command: PART
   Parameters: <channel>{,<channel>}
Command: NAMES
   Parameters: [<channel>{,<channel>}]
Command: LIST
   Parameters: [<channel>{,<channel>} [<server>]]

Command: KICK
   Parameters: <channel> <user> [<comment>]
Command: INVITE
   Parameters: <nickname> <channel>
Command: TOPIC
   Parameters: <channel> [<topic>]
Command: MODE >> cf sujet


*/
