#include "Channel.hpp"

/************************ CONSTRUCTORS & DESTRUCTORS **************************/

Channel::Channel(const std::string &name) : _name(name) {}

Channel::~Channel() {}

/********************************** GETTERS **********************************/

int				Channel::getNbrUsersLimit( void ) { return this->_nbrUsersLimit; }
std::string&	Channel::getChannelName( void ) { return this->_name; }
std::string&	Channel::getChannelPwd( void ) { return this->_channelPwd; }
std::string&	Channel::getTopic( void ) { return this->_topic; }
std::string&	Channel::getMode( void ) { return this->_mode; }

std::vector<std::string>&		Channel::getKicked( void ) { return this->_kicked; }
std::vector<std::string>&		Channel::getBanned( void ) { return this->_banned; }
std::vector<std::string>&		Channel::getOperators( void ) { return this->_operators; }
std::vector<std::string>&		Channel::getInvited() { return this->_invited; }
std::map<std::string, Client>&	Channel::getClientsList() { return this->_clients; }

/********************************** SETTERS **********************************/

void	Channel::setNbrUsersLimit(int limit) { this->_nbrUsersLimit = limit; }
void	Channel::setTopic(std::string topic) { this->_topic = topic; }
void	Channel::setChannelPwd(std::string channelPwd) { this->_channelPwd = channelPwd; }

void	Channel::addToInvited(std::string &nickname) {this->_invited.push_back(nickname);}
void	Channel::addToKicked(std::string &nickname) {this->_kicked.push_back(nickname);}
void	Channel::addToBanned(std::string &nickname) {this->_banned.push_back(nickname);}
void	Channel::addOperators(std::string &nickname) {this->_operators.push_back(nickname);}

void	Channel::addMode(std::string mode) {
	if (_mode.find(mode) == std::string::npos)
		_mode += mode;
}

void	Channel::removeMode(std::string mode) {
	if (_mode.empty())
		return;
	size_t pos = _mode.find(mode);
	if (pos != std::string::npos)
		_mode.erase(pos, 1);
}

bool	Channel::isClient(std::string &nickname) {
	if (_clients.empty())
		return false;
	std::map< std::string, Client>::iterator it = _clients.find(nickname);
	if (it != _clients.end())
		return true;
	return false;
}

bool	Channel::isInvited(std::string &nickname) {
	if (_invited.empty())
		return false;
	for (std::vector< std::string >::iterator it = _invited.begin(); it != _invited.end(); it++) {
		if (*it == nickname)
			return true;
	}
	return false;
}

bool	Channel::isOperator(std::string &nickname) {
	if (_operators.empty())
		return false;
	for (std::vector< std::string >::iterator it = _operators.begin(); it != _operators.end(); it++) {
		if (*it == nickname)
			return true;
	}
	return false;
}

bool	Channel::isBanned(std::string &nickname) {
	if (_banned.empty())
		return false;
	for (std::vector< std::string >::iterator it = _banned.begin(); it != _banned.end(); it++) {
		if (*it == nickname)
			return true;
	}
	return false;
}

void	Channel::removeClientFromChan(std::string &nickname) {
	if (_clients.empty())
		return ;
	std::map< std::string, Client>::iterator it = _clients.find(nickname);
	if (it != _clients.end())
		_clients.erase(it);
	for (std::vector< std::string >::iterator it = _operators.begin(); it != _operators.end(); it++) {
		if (*it == nickname)
			_operators.erase(it);
	}
}

void	Channel::removeOperator(std::string &nickname) {
	for (std::vector< std::string >::iterator it = _operators.begin(); it != _operators.end(); it++) {
		if (*it == nickname)
			_operators.erase(it);
	}
}

void	Channel::removeClientFromInvite(std::string &nickname) {
	for (std::vector< std::string >::iterator it = _invited.begin(); it != _invited.end(); it++) {
		if (*it == nickname)
			_invited.erase(it);
	}
}

void	Channel::addToChan(Client &client) {
	_clients[client.getNickname()] = client;
}
