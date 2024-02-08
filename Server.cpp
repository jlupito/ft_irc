#include "Server.hpp"

/************************ CONSTRUCTORS & DESTRUCTORS **************************/

// pour rappel le port est donne d' emblee en parametre de l'executable
Server::Server(int port) : _port(port) {
	std::cout << "Hello World! - Server has been created." << std::endl;
}

Server::~Server() {

	std::cout << "Server has been disconnected - Ciao bitch !" << std::endl;
}

/******************************** EXCEPTIONS ********************************/

const char* Server::serverInitFailure::what() const throw() {
    return "Error : server has not been initialized.";
}

/***************************** OTHER FUNCTIONS ******************************/

bool	Server::initialization() {
	// verifs
	return true;
}

void	Server::startServer() {

	if (initialization()) {
		//lancer le serveur avec les fonctions de Jlo du main
	}
	else
		throw Server::serverInitFailure();
}
