#include "Server.hpp"
#include "Commands.hpp"
#include "Replies.hpp"
#include "Client.hpp"

/*
Command: PRIVMSG
   Parameters: <receiver>{,<receiver>} <text to be sent>
<receiver> est le nom du destinataire (User ou Channel)
Les operateurs ont une fonctionnalité en plus qui permet d'envoyer des messages
a certains utilisateurs, en utilisant les masques.

Numeric replies :
- ERR_NOSUCHNICKNAME (401): Renvoyé par le serveur lorsque le destinataire du message privé (PRIVMSG) ou la requête d'info sur un utilisateur (WHOIS) n'existe pas.
- ERR_CANNOTSENDTOCHAN (404): Renvoyé par le serveur lorsqu'un message est envoyé à un canal qui n'existe pas ou auquel le client n'est pas connecté.
- ERR_TOOMANYTARGETS (407): Renvoyé par le serveur lorsqu'un message est envoyé à trop de cibles en même temps.
- ERR_NORECIPIENT (411): Renvoyé par le serveur lorsqu'une commande nécessite un récepteur (comme PRIVMSG) mais aucun n'est spécifié.
- ERR_NOTEXTTOSEND (412): Renvoyé par le serveur lorsqu'un message PRIVMSG ou NOTICE est vide.
- ERR_NOTOPLEVEL (413): Renvoyé par le serveur lorsqu'un paramètre doit être un nom de domaine de premier niveau (TLD) mais ne l'est pas.
- ERR_WILDTOPLEVEL (414): Renvoyé par le serveur lorsqu'un paramètre doit être un masque de domaine générique mais ne l'est pas.
- RPL_AWAY (301): Renvoyé par le serveur pour indiquer qu'un utilisateur est actuellement absent.
*/

# define ERR_NOSUCHNICKNAME 401 // ok
# define ERR_CANNOTSENDTOCHAN 404
# define ERR_TOOMANYTARGETS 407
# define ERR_NORECIPIENT 411
# define ERR_NOTEXTTOSEND 412 //ok
# define ERR_NOTOPLEVEL 413
# define ERR_WILDTOPLEVEL 414
# define RPL_AWAY 301


int	handlePRVMSGrrors(Server& server, Client* client, cmdStruct* command) {

	int	codeError = 0;
	std::string receiverName = command->params[1];
   std::string textToSend = command->params[2];
   std::map<const int, Client*> clientsList = server.getClients();
   bool receiverFound = false;

   for (std::map<const int, Client*>::const_iterator it = clientsList.begin();
         it != clientsList.end(); it++) {
      std::string receiver = it->second->getNickname();
      if (receiver == receiverName) {
         receiverFound = true;
         break ; }
   }
   if (!receiverFound)
      codeError = 401;
   // if 404
   // if 407
   // etc
   if (textToSend.empty()) {
		std::cout << "Test : texttosend EMPTY." << std::endl;
		codeError = 412;
	}
   return codeError;
}



void		handlePRVMSGCommand(Server& server, Client* client, cmdStruct* command) {

   std::string receiverName = command->params[1]; // est-ce le nickname ? on part du postulat que oui
   std::map<const int, Client*> clientsList = server.getClients(); // on recupere la liste des clients


}
