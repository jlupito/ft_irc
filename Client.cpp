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


/********************************** SETTERS **********************************/
void  	Client::setClientSocket(int fd) { _clientSocket = fd; }
void 	Client::setBuffer(std::string &buffer) { _buffer = buffer; }
void    Client::setCAPLS() { _capLS = true; }

/******************************** EXCEPTIONS ********************************/
const char* Client::clientConnectFailure::what() const throw() {
	return "Error : client failed to connect to server."; }


/***************************** OTHER FUNCTIONS ******************************/
void    Client::fillAttributes(void) {

}
// cette fonction va aller chercher dans le buffer le USER et le NICK pour
// remplir les attributs du client dans la map du serveur.
