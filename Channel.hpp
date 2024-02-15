#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <map>
#include <vector>
#include "Client.hpp"
#pragma once

class Client;

class Channel {

	private:
		std::vector<std::string>		_operators;
		std::vector<std::string>		_kickedUsers;
		std::vector<std::string>		_bannedUsers;
		std::map<std::string, Client> 	_clientsList;
		std::string 					_name;
		std::string						_channelPwd;
		std::string						_topic;
		std::string						_mode;
		int								_nbrUsersLimit;

	public:
		Channel(const std::string &name);
		~Channel();

		std::map<std::string, Client>&	getClientsList();
		int								getNbrUsersLimit( void );
		std::string&					getChannelPwd( void );
		std::string&					getTopic( void );
		std::string&					getMode( void );
		std::vector<std::string>&		getKickedUsers( void );
		std::vector<std::string>&		getBannedUsers( void );
		std::vector<std::string>&		getOperators( void );

		void							setNbrUsersLimit(int limit);
		void							setChannelPwd(std::string channelPwd);
		void							setTopic(std::string topic);
		void							addMode(std::string mode);
		void							removeMode(std::string mode);

		bool							isClient(std::string &username);
		bool							isOperator(std::string &username);
		bool							isBanned(std::string &username);

		void							removeOperator(std::string &username);
		void							removeClientFromChan(std::string &username);
		void							addClientToChan(Client &client);
		void							addKickedUsers(std::string &kickedUsername);
		void							addBannedUsers(std::string &bannedUsername);
		void							addOperators(std::string &OperUsername);
};
