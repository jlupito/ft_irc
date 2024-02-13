#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include "Server.hpp"
#pragma once

void	processClientCommands(Server& server, int clientSocket, std::string receivedData);
void	processEvent(Server &server, int i);
