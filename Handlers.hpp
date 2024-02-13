#include <iostream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include "Server.hpp"
#pragma once

class Server;

void	handleNickCommand(int clientSocket, std::vector<std::string>& params);
void	handleUserCommand(int clientSocket, std::vector<std::string>& params);
