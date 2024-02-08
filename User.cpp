#include "User.hpp"

User::User(int socket_fd, const std::string& initialNickname) : _nickname(initialNickname), _socketFd(socket_fd) {}

User::~User() {}

void	User::updateNickname(const std::string& newNickname) {
	_nickname = newNickname; }


void	User::processData() {

}


// a voir si vraiment utile ou pas, car les commandes portent souvent le sens du destinataire
void	User::sendData(const std::string& message, Destination destination,
						const std::string& target) {

	switch (destination) {

		case Destination::USER:
			//
			break;

		case Destination::CHANNEL:
			//
			break;

		case Destination::SERVER:
			//
			break;
	}
}

void	User::handleDisconnect() {

}

// Méthode pour rejoindre un canal
void	User::joinChannel(const std::string& channel) {

}

//Méthode pour quitter un canal
void	User::leaveChannel(const std::string& channel) {

}
