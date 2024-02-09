#include "Server.hpp"

const int PORT = 6667;

void handleEvent(int clientSocket) {
    // Code pour gérer la connexion du client
    // Coder pour lire/écrire des données et/ou traiter les commandes IRC
    // Par exemple fermer simplement la connexion :
    // close(clientSocket);
}

int main(int ac, char **av) {

	// int port = atoi(av[1]); // verification du int a mettre
	Server server(atoi(av[1]));

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

					/*  Ajouter le nouveau socket client à l'ensemble epoll */
					server.getEvent().events = EPOLLIN;
					/*EPOLLIN: Cet événement est déclenché lorsque des données sont prêtes à être lues depuis
					le descripteur de fichier associé. Il indique que vous pouvez utiliser des appels
					de lecture (read ou équivalents) sans bloquer le processus.*/
					server.getEvent().data.fd = client.getClientSocket();
					if (epoll_ctl(server.getEpollFd(), EPOLL_CTL_ADD, client.getClientSocket(), &server.getEvent()) < 0) {
						throw Client::clientConnectFailure(); }
					server.getClients()[client.getClientSocket()] = &client;
				}
				catch (const std::exception &e) { std::cout << e.what() <<std::endl ; close(client.getClientSocket()); }
			}
			else
				handleEvent(server.getEventsTab()[i].data.fd);
		}
	}
	for (std::map<const int, Client * >::iterator it = server.getClients().begin(); it != server.getClients().end(); it++)
		close(it->first); // verifier gestion memoire pointeurs Client*
	close(server.getEpollFd());

	return 0;
}
