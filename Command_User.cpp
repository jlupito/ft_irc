#include "Server.hpp"
#include "Commands.hpp"
#include "Replies.hpp"
#include "Client.hpp"

/*
Command: USER
	Parameters: <username> <hostname> <servername> :<realname>
si on ignore la communication inter-srveurs, on ignore les params
<hostname>  et <servername> A confirner pour implemtation gestion erreurs
Realname peut contenir des espaces et doit être préfixé par deux-points (':')
*/

void	handleUSERCommand(Server& server, Client* client, cmdStruct* command) {

	std::string reply = "Connexion failure.\r\n";
	int connexion = client->getConnectionStatus();
	// si le flag _connected est equivalent à 0111 (0x07 vaut 00000111)

	for (int i = 0; i < command->params.size(); i++)
		std::cout << "USER Param " << i << " est : " << command->params[i] << std::endl; //ok
	std::cout << "Le dernier param stocke en message est : " << command->message << std::endl;

	if (connexion & 0x07) {

		if (command->params.size() != 4 || command->message.empty()) { // on a 4 params ET le message rempli par le realname
			reply = NEEDMOREPARAMS_ERR(command->params[0]); sendBytesToClient(client, reply.c_str()); return ; }

		for (std::map<const int, Client *>::iterator it = server.getClients().begin();
		it != server.getClients().end(); it++) {
			if (it->second->getUserName() == command->params[1]) {
				std::cout << "Error : User already exists." << std::endl;
				reply = "Test : nickname ALREADY EXISTING.\r\n"; sendBytesToClient(client, reply.c_str()); return ; }
		}
		client->setUserName(command->params[1]);
		server.setServerName(command->params[3]);
		client->setRealName(command->message);
		client->setConnectionStatus(connexion | 0xF);
		reply = RPL_WELCOME(client->getUserName(), client->getNickname()).c_str();
		std::cout << "USER - Fin de la phase." << std::endl;
		sendBytesToClient(client, reply.c_str());
	}
}