#pragma once
#include <iostream>
#include <vector>

// classe pour envoi de data depuis le user vers les autres entités
enum class Destination {

	SERVER,
	USER,
	CHANNEL,
};

class User {

	public:
		User(int socket_fd, const std::string& initialNickname);
		~User();

		/* Méthode pour traiter les données REÇUES par le User du client
			Examen des messages recus avec extraction des cmd et appel
			des autres methodes*/
    	void processData();

		/* Méthode pour traiter les données ENVOYÉES par le user vers le client
			Examen des messages recus avec extraction des cmd et appel
			des autres methodes*/
    	void sendData(const std::string& message,
				Destination destination, const std::string& target);

		// Méthode pour mettre à jour le pseudonyme de l'utilisateur
		void updateNickname(const std::string& newNickname);

		// Méthode pour gérer la déconnexion de l'utilisateur
		void handleDisconnect();

		// Méthode pour rejoindre un canal
		void joinChannel(const std::string& channel);

		// Méthode pour quitter un canal
		void leaveChannel(const std::string& channel);

	private:
	std::string					_nickname;
	// std::string					_username; utile ?
	std::vector<std::string>	_channels;
	int							_socketFd;
};
