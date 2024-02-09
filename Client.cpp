#include "Client.hpp"

Client::Client() {}

Client::~Client() {}

int	Client::getClientSocket(void) { return _clientSocket; }

void  Client::setClientSocket(int fd) { _clientSocket = fd; }

sockaddr_in& 	Client::getClientAddr(void) { return this->_clientAddr; }

socklen_t& 	Client::getClientAddrLen(void) { return this->_clientAddrLen; }

const char* Client::clientConnectFailure::what() const throw() {
	return "Error : client failed to connect to server."; }
