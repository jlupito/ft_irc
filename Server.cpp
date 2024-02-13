#include "Server.hpp"

/************************ CONSTRUCTORS & DESTRUCTORS **************************/

Server::Server(std::string portStr, std::string password) {
	if (portStr != "6667" || password != this->_password)
		throw serverInitFailure();
	std::cout << "Hello World! - Server has been created." << std::endl;
	initialization();
	initiateCommandHandlers();
}

Server::~Server() {
	std::cout << "Server has been disconnected - Ciao bitch !" << std::endl;
}


/********************************** GETTERS **********************************/

int		Server::getPort(void) { return _port; }
int		Server::getServerSocket(void) { return _serverSocket; }
int		Server::getEpollFd(void) { return _epollFd; }
sockaddr_in&	Server::getServerAddr(void) { return this->_serverAddr; }
epoll_event&	Server::getEvent(void) { return this->_event; }
epoll_event*	Server::getEventsTab(void) { return this->_events; }
std::map<const int, Client *>&	Server::getClients(void) { return this->_clients; }

/******************************** EXCEPTIONS ********************************/

const char* Server::serverInitFailure::what() const throw() {
	return "Error : server has not been initialized."; }

const char* Server::commandNotFound::what() const throw() {
	return "Error : command not found."; }

const char* Server::errorInCommandParameters::what() const throw() {
	return "Error : wrong parameter(s)."; }

/***************************** OTHER FUNCTIONS ******************************/

// personnalisation des classes d'exceptions si besoin
void	Server::initialization() {

	try {
		/* Créer un socket pour le serveur, il s'agit de la socket d' ecoute du serveur */
		if ((_serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			throw Server::serverInitFailure();

		/* Configurer l adresse du serveur */
		memset(&_serverAddr, 0, sizeof(_serverAddr));
		_serverAddr.sin_family = AF_INET; // Famille d'adresses (AF_INET pour ipv4)
		_serverAddr.sin_addr.s_addr = INADDR_ANY; // Contient l'adresse IP du serveur en format binaire.
		_serverAddr.sin_port = htons(_port); //numero de port htons = "host to network short", garantit que le numéro de port est correctement converti dans le format réseau

		/* Lier le socket à l'adresse et au port */
		if (bind(_serverSocket, (struct sockaddr*)&_serverAddr, sizeof(_serverAddr)) < 0) {
			throw Server::serverInitFailure(); }

		/* Mettre le socket en mode ecoute */
		if (listen(_serverSocket, SOMAXCONN) < 0) {
			throw Server::serverInitFailure(); }

		/* Créer un descripteur de fichier epoll */
		_epollFd = epoll_create1(0);
		if (_epollFd < 0) {
			throw Server::serverInitFailure(); }

		/* Ajouter le socket du serveur à l'ensemble epoll */
		_event.events = EPOLLIN;
		_event.data.fd = _serverSocket;
		if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, _serverSocket, &_event) == -1) {
			close(_epollFd);
			throw Server::serverInitFailure(); }
	}
	catch (const std::exception &e) { std::cout << e.what() <<std::endl; return ; }
}

// on cree std::map<std::string, commandFunction> _commandHandlers;
void	Server::initiateCommandHandlers() {

	_commandHandlers["NICK"] = &Server::handleNickCommand;
	_commandHandlers["USER"] = &Server::handleUserCommand;
	// ... entrez toutes les commandes necessaires, cf draft.cpp
}
