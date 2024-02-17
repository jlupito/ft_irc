#include "Server.hpp"
#include "Commands.hpp"
#include "Replies.hpp"
#include "Client.hpp"

// Command: USER
// 	Parameters: <username> <hostname> <servername> :<realname>


void	handleUSERCommand(Server& server, Client* client, cmdStruct* command) {

	std::string reply = "Connexion failure";
	int connexion = client->getConnectionStatus();

	if (connexion & 0x07) // si le flag _connected est equivalent à 0111 (0x07 vaut 00000111)
	{
		std::cout << "Entrée dans la commande USER " << std::endl;
		std::cout << "test :" << client->getConnectionStatus() << std::endl;
		std::cout << "test :" << client->getNickname() << std::endl;
		reply = "USER OK";
	}
	sendBytes(client, reply.c_str());
	std::cout << "test :" << client->get << std::endl;
}
	// else if (command->message == "CAP LS") // WHAT ??
	// 	client->setCAPLS();
	// reply = RPL_WELCOME(client->getUserID(), client->getNickname()).c_str();
	// ssize_t bytes_transferred = send(client->getClientSocket(), reply.c_str(), strlen(reply.c_str()), 0);
	// if (bytes_transferred <= 0)
	// 	std::cout << "Server failed to send a reply to client." << std::endl;
