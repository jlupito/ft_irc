#include "Server.hpp"
#include "Commands.hpp"

int main(int ac, char **av) {

	if (ac != 3)
		return (std::cout << "Too few arguments." << std::endl, -1);

	Server server(av[1], av[2]);
	while (true) {
		int numEvents = epoll_wait(server.getEpollFd(), server.getEventsTab(), 1024, -1);
		if (numEvents < 0)
			return -1;
		for (int i = 0; i < numEvents; ++i) {
			if (server.getEventsTab()[i].data.fd == server.getServerSocket()) {
				Client client;
				try {
					int tmp = accept(server.getServerSocket(), (struct sockaddr*)&client.getClientAddr(), &client.getClientAddrLen());
					client.setClientSocket(tmp);
					if (client.getClientSocket() < 0)
						throw Client::clientConnectFailure();
					// Configuration du socket en mode non bloquant
					int flag = fcntl(client.getClientSocket(), F_GETFL, 0); // on recupere l'indice de controle dans flag.
					fcntl(client.getClientSocket(), F_SETFL, flag | O_NONBLOCK); // on ajoute l' option "non-bloquant" aux options de socket
					// verification du retour des deux fonctions (-1?) cf commentaires

					server.getEvent().events = EPOLLIN;
					server.getEvent().data.fd = client.getClientSocket();
					if (epoll_ctl(server.getEpollFd(), EPOLL_CTL_ADD, client.getClientSocket(), &server.getEvent()) < 0) {
						throw Client::clientConnectFailure(); }
					server.getClients()[client.getClientSocket()] = &client;
					// std::cout << "CREATION // adress client: " <<  &client << std::endl;
				}
				catch (const std::exception &e) { std::cout << e.what() <<std::endl ; close(client.getClientSocket()); }
			}
			else
				processEvent(server, i);
	    //     for (std::map<const int, Client*>::iterator it = server.getClients().begin();
        //     it != server.getClients().end(); it++) {
        //         std::cout << "nick: " << it->second->getNickname() << std::endl;
        //         std::cout << "user: " << it->second->getUserName() << std::endl;
        //         std::cout << "socket client: " << &(it->second) << std::endl;
        //     }
		}
	}


	for (std::map<const int, Client* >::iterator it = server.getClients().begin(); it != server.getClients().end(); it++)
		close(it->first); // verifier gestion memoire pointeurs Client*
	close(server.getEpollFd());

	return 0;
}

/*
Pour les verifications de retour de fonctions :
int flags = fcntl(client.getClientSocket(), F_GETFL, 0);
if (flags == -1) {
	throw ...
	close(client.getClientSocket());
	throw Client::clientConnectFailure();
}

if (fcntl(client.getClientSocket(), F_SETFL, flags | O_NONBLOCK) == -1) {
	throw ...
	close(client.getClientSocket());
	throw Client::clientConnectFailure();
}
*/
