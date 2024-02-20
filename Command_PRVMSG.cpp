#include "Server.hpp"
#include "Commands.hpp"
#include "Replies.hpp"
#include "Client.hpp"

/*
Command: PRIVMSG
   Parameters: <receiver>{,<receiver>} <text to be sent>
<receiver> est le nom du destinataire
Les operateurs ont une fonctionnalité en plus qui permet d'envoyer des messages
a certains utilisateurs, en utilisant les masques.
*/

void		handlePRVMSGCommand(Server& server, Client* client, cmdStruct* command) {

}
