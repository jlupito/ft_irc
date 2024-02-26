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

std::vector<std::string>&		Channel::getKickedUsers( void ) { return this->_kickedUsers; }
std::vector<std::string>&		Channel::getBannedUsers( void ) { return this->_bannedUsers; }
std::vector<std::string>&		Channel::getOperators( void ) { return this->_operators; }
std::map<std::string, Client>&	Channel::getClientsList() { return this->_clientsList; }

/********************************** SETTERS **********************************/

void	Channel::setNbrUsersLimit(int limit) { this->_nbrUsersLimit = limit; }
void	Channel::setTopic(std::string topic) { this->_topic = topic; }
void	Channel::setChannelPwd(std::string channelPwd) { this->_channelPwd = channelPwd; }

void	Channel::addToKicked(std::string &kickedUsername) {this->_kickedUsers.push_back(kickedUsername);}
void	Channel::addToBanned(std::string &bannedUsername) {this->_bannedUsers.push_back(bannedUsername);}
void	Channel::addOperators(std::string &OperUsername) {this->_operators.push_back(OperUsername);}


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

	if (_clientsList.empty())
		return false;
	std::map< std::string, Client>::iterator it = _clientsList.find(nickname);
	if (it != _clientsList.end())
		return true;
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

	if (_bannedUsers.empty())
		return false;
	for (std::vector< std::string >::iterator it = _bannedUsers.begin(); it != _bannedUsers.end(); it++) {
		if (*it == nickname)
			return true;
	}
	return false;
}

void	Channel::removeClientFromChan(std::string &nickname) {

	if (_clientsList.empty())
		return ;
	std::map< std::string, Client>::iterator it = _clientsList.find(nickname);
	if (it != _clientsList.end())
		_clientsList.erase(it);
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

void	Channel::addClientToChan(Client &client) {
	_clientsList[client.getNickname()] = client;
}
