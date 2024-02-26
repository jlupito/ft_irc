#include "Client.hpp"

/************************ CONSTRUCTORS & DESTRUCTORS **************************/

Client::Client() : _connexion(0) { _clientAddrLen = sizeof(_clientAddr); }
Client::~Client() {}

/********************************** GETTERS **********************************/

int				Client::getClientSocket(void) { return _clientSocket; }
sockaddr_in& 	Client::getClientAddr(void) { return this->_clientAddr; }
socklen_t& 		Client::getClientAddrLen(void) { return this->_clientAddrLen; }
std::string&    Client::getBuffer() { return this->_buffer; }
std::string& 	Client::getNickname() { return this->_nickname; }
std::string& 	Client::getUserName() { return this->_userName; }
std::string& 	Client::getRealName() { return this->_realName; }
int				Client::getConnectionStatus() { return this->_connexion; }

/********************************** SETTERS **********************************/

void  	Client::setClientSocket(int& fd) { _clientSocket = fd; }
void 	Client::setBuffer(std::string &buffer) { _buffer = buffer; }
void    Client::setConnectionStatus(int &connexion) { _connexion = connexion; }
void	Client::setNickname(std::string& nickname) { _nickname = nickname; }
void    Client::setUserName(std::string& username) { _userName = username; }
void    Client::setRealName(std::string& realname) { _realName = realname; }

/******************************** EXCEPTIONS ********************************/

const char* Client::clientConnectFailure::what() const throw() {
	return "Error : client failed to connect to server."; }
