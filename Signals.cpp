#include <iostream>
#include <csignal>
#include "Signals.hpp"
#include "Commands.hpp"
#include "Server.hpp"
#include "Signals.hpp"

Server* SignalMonitor::serverInstance = NULL;

SignalMonitor::SignalMonitor(Server &server) {

	serverInstance = &server;
	std::signal(SIGINT, handleSignal); // attention cette fonction ne prend rien que des int
	std::signal(SIGQUIT, handleSignal);
}

void SignalMonitor::handleSignal(int signal) {

	if (serverInstance) {

		if (signal == SIGINT)
			std::cout << std::endl << "Received SIGINT (Ctrl+C)" << std::endl;
		else if (signal == SIGQUIT)
			std::cout << "Received SIGQUIT (Ctrl+\\)" << std::endl;
		serverInstance->handleDisconnect(*serverInstance);
	}
}

SignalMonitor::~SignalMonitor() {
	serverInstance = NULL; }
