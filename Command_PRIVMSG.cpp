#include "Server.hpp"
#include "Commands.hpp"
#include "Replies.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include <vector>

void handlePRIVMSGCommand(Server& server, Client* client, cmdStruct* command) {

	const std::string message = command->message;
	std::vector<std::string> receiversList;
	std::string reply = NULL;
	std::map<const int, Client*> clientsList = server.getClients();
	std::map<std::string, Channel *> channelsList = server.getChannels();
	std::vector<std::string> errorMessages;

	std::cout << "### on entre dans la commande PRIVMSG ###" << std::endl;

	if (!command->params[1].empty()) {

		std::cout << "### Test 1 ###" << std::endl;

		std::istringstream iss(command->params[1]);
		std::string receiver;
		while (std::getline(iss, receiver, ','))
			receiversList.push_back(receiver);
		// Test erreur 413
		for (int i = 0; i < receiversList.size(); i++) {
			if (receiversList[i].find('.') == std::string::npos)
				errorMessages.push_back(ERR_NOTOPLEVEL(receiversList[i])); // erreur 413
		}
		// Test erreur 414
		for (int i = 0; i < receiversList.size(); i++) {
			if (receiversList[i].find('*') != std::string::npos)
				errorMessages.push_back(ERR_WILDTOPLEVEL(receiversList[i])); // erreur 414
		}
	}

	else if (command->message.empty()) {

		std::cout << "### Test 2 ###" << std::endl;

		reply = ERR_NOTEXTTOSEND; // erreur 412
		sendBytesToClient(client, reply.c_str());
		return;
	}

	for (int i = 0; i < receiversList.size(); i++) {
		// si c'est un format channel
		if (!receiversList[i].empty() && receiversList[i][0] == '#') {
			std::string channelReceiving = receiversList[i];
			bool channelFound = false;
			for (std::map<std::string, Channel*>::iterator it = channelsList.begin();
				it != channelsList.end(); it++) {
				if (it->first == channelReceiving) {
					sendBytesToChannel(it->second, command->message.c_str());
					channelFound = true;
					break;
				}
			}
			if (!channelFound)
				errorMessages.push_back(ERR_CANNOTSENDTOCHAN(channelReceiving)); // erreur 404
		}
		// si c'est un format user
		else if (!receiversList[i].empty()) {

		std::cout << "### Test 3 ###" << std::endl;

			std::string userReceiving = receiversList[i];
			bool userFound = false;
			for (std::map<const int, Client*>::iterator it = clientsList.begin();
				it != clientsList.end(); it++) {
				if (it->second->getNickname() == userReceiving) {
					sendBytesToClient(it->second, command->message.c_str());
					userFound = true;
					break;
				}
			}
			if (!userFound)
				errorMessages.push_back(ERR_NOSUCHNICK(userReceiving)); // erreur 401
		}
		// si elle est vide, on renvoie Erreur 401
		else
			errorMessages.push_back(ERR_NOSUCHNICK(command->params[1]));
	}
	for (const std::string& errorMsg : errorMessages)
		sendBytesToClient(client, errorMsg.c_str());
}

// Mettre ERR_NORECIPIENT
