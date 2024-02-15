#include "Client.hpp"

/************************ CONSTRUCTORS & DESTRUCTORS **************************/

Client::Client() : _capLS(false) {}
Client::~Client() {}


/********************************** GETTERS **********************************/

int				Client::getClientSocket(void) { return _clientSocket; }
sockaddr_in& 	Client::getClientAddr(void) { return this->_clientAddr; }
socklen_t& 		Client::getClientAddrLen(void) { return this->_clientAddrLen; }
std::string&    Client::getBuffer() { return this->_buffer; }
bool            Client::getCAPLS() { return this->_capLS; }
std::string& 	Client::getUserID() { return this->_userID; }
std::string& 	Client::getNickname() { return this->_nickName; }


/********************************** SETTERS **********************************/

void  	Client::setClientSocket(int fd) { _clientSocket = fd; }
void 	Client::setBuffer(std::string &buffer) { _buffer = buffer; }
void    Client::setCAPLS() { _capLS = true; }
void	Client::setNickname(std::string nickname) { _nickName = nickname; }

/******************************** EXCEPTIONS ********************************/

const char* Client::clientConnectFailure::what() const throw() {
	return "Error : client failed to connect to server."; }
