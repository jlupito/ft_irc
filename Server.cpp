#include "Server.hpp"
#include "Commands.hpp"

/************************ CONSTRUCTORS & DESTRUCTORS **************************/

Server::Server(std::string port, std::string password)
	: _port(std::atoi(port.c_str())), _password(password) {
	std::cout << "Hello World! - Server has been created." << std::endl;
	initialization();
}

Server::~Server() {
	std::cout << "Server has been disconnected - Ciao bitch !" << std::endl;
}

/********************************** SETTERS **********************************/

void	Server::setServerName(std::string servername) { this->_serverName = servername; }

/********************************** GETTERS **********************************/

typedef void (*cmdFunction)(Server&, Client*, cmdStruct*);

int		Server::getPort(void) { return _port; }
int		Server::getServerSocket(void) { return _serverSocket; }
int		Server::getEpollFd(void) { return _epollFd; }
std::string		Server::getServerName(void) { return _serverName; }
std::string 	Server::getPassWord(void) { return _password; }
sockaddr_in&	Server::getServerAddr(void) { return this->_serverAddr; }
epoll_event&	Server::getEvent(void) { return this->_event; }
epoll_event*	Server::getEventsTab(void) { return this->_events; }
std::map<const int, Client *>&		Server::getClients(void) { return this->_clients; }
std::map<std::string, Channel *>&	Server::getChannels(void) { return this->_channels; }
std::map<std::string, cmdFunction>& Server::getCmdList() { return this->_cmdList;
}

/******************************** EXCEPTIONS ********************************/

const char* Server::serverInitFailure::what() const throw() {
	return "Error : server has not been initialized."; }

const char* Server::errorInCommandParameters::what() const throw() {
	return "Error : wrong parameter(s)."; }

/***************************** OTHER FUNCTIONS ******************************/

void	Server::initCommandMap(void) {

    _cmdList.insert(std::make_pair("CAP", &handleCAP_LSCommand));
    _cmdList.insert(std::make_pair("PASS", &handlePASSCommand));
    _cmdList.insert(std::make_pair("NICK", &handleNICKCommand));
	_cmdList.insert(std::make_pair("USER", &handleUSERCommand));
	_cmdList.insert(std::make_pair("OPER", &handleOPERCommand));
    _cmdList.insert(std::make_pair("QUIT", &handleQUITCommand));
    _cmdList.insert(std::make_pair("PRIVMSG", &handlePRIVMSGCommand));
	_cmdList.insert(std::make_pair("NOTICE", &handleNOTICECommand));
	_cmdList.insert(std::make_pair("PING", &handlePINGCommand));
	_cmdList.insert(std::make_pair("PONG", &handlePONGCommand));

    // _cmdList.insert(std::make_pair("JOIN", &handleJOINCommand));
	// // _cmdList.insert(std::make_pair("PART", &handlePARTCommand));
    // // _cmdList.insert(std::make_pair("NAMES", &handleNAMESCommand));
	// // _cmdList.insert(std::make_pair("LIST", &handleLISTCommand));

    // _cmdList.insert(std::make_pair("KICK", &handleKICKCommand));
	// _cmdList.insert(std::make_pair("INVITE", &handleINVITECommand));
    // _cmdList.insert(std::make_pair("TOPIC", &handleTOPICCommand));
	// _cmdList.insert(std::make_pair("MODE", &handleMODECommand));
}

void	Server::initialization(void) {

	try {
		if ((_serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			throw Server::serverInitFailure();

		memset(&_serverAddr, 0, sizeof(_serverAddr));
		_serverAddr.sin_family = AF_INET;
		_serverAddr.sin_addr.s_addr = INADDR_ANY;
		_serverAddr.sin_port = htons(_port);

		if (bind(_serverSocket, (struct sockaddr*)&_serverAddr, sizeof(_serverAddr)) < 0) {
			throw Server::serverInitFailure(); }

		if (listen(_serverSocket, SOMAXCONN) < 0) {
			throw Server::serverInitFailure(); }

		_epollFd = epoll_create1(0);
		if (_epollFd < 0) {
			throw Server::serverInitFailure(); }

		_event.events = EPOLLIN;
		_event.data.fd = _serverSocket;
		if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, _serverSocket, &_event) == -1) {
			close(_epollFd);
			throw Server::serverInitFailure(); }

		initCommandMap(); // fonction d'init de la map _cmdList contenant le repertoire des commandes
	}
	catch (const std::exception &e) { std::cout << e.what() <<std::endl; return ; }
}

/*
Fonction initialization(), étapes :
- Créer un socket pour le serveur, il s'agit de la socket d' ecoute du serveur.
- Configurer l adresse du serveur : Famille d'adresses (AF_INET pour ipv4), adresse IP
 du serveur en format binaire. numero de port htons = "host to network short", garantit
 que le numéro de port est correctement converti dans le format réseau
- Lier le socket à l'adresse et au port
- Mettre le socket en mode ecoute avec la fonction listen(),
- Créer un descripteur de fichier epoll(),
- Ajouter le socket du serveur à l'ensemble epoll().
*/

void Server::removeClient(const std::string& nickname) {

	std::map<const int, Client*>& clientList = this->getClients();
	std::map<const int, Client*>::iterator it;

	for (it = clientList.begin(); it != clientList.end(); ++it) {

		if (it->second->getNickname() == nickname) {
			const int clientSocket = it->first;
			close(clientSocket);
			delete it->second;
			clientList.erase(it);
			break;
		}
	}
}
