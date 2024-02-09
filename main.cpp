#include "Server.hpp"

const int PORT = 6667;

void handleConnection(int clientSocket) {
    // Code pour gérer la connexion du client
    // Coder pour lire/écrire des données et/ou traiter les commandes IRC
    // Par exemple fermer simplement la connexion :
    // close(clientSocket);
}

int main(int ac, char **av) {

	//int port = atoi(av[1]); // verification du int a mettre
	Server server(PORT); // construction

	while (true) {
		int numEvents = epoll_wait(server.getEpollFd(), server.getEventsTab(), MAX_EVENTS, -1);
        // ajouter protection if numEvents < 0 throw Error, doit break
		for (int i = 0; i < numEvents; ++i) {
			if (server.getEventsTab()[i].data.fd == server.getServerSocket()) {
				/* Nouvelle connexion entrante */
				Client client;
				int tmp = accept(server.getServerSocket(), (struct sockaddr*)&client.getClientAddr(), &client.getClientAddrLen());
				client.setClientSocket(tmp);
				if (server.getServerSocket() < 0) {
					// afficher une erreur mais ne pas throw une exception
					continue; // because doit passer a l iteration suivante, le serveur ne doit pas s arreter
				}
				/*  Ajouter le nouveau socket client à l'ensemble epoll */
				server.getEvent().events = EPOLLIN;
				/*EPOLLIN: Cet événement est déclenché lorsque des données sont prêtes à être lues depuis
				le descripteur de fichier associé. Il indique que vous pouvez utiliser des appels
				de lecture (read ou équivalents) sans bloquer le processus.*/
				server.getEvent().data.fd = client.getClientSocket();
				if (epoll_ctl(server.getEpollFd(), EPOLL_CTL_ADD, client.getClientSocket(), &server.getEvent()) < 0)
					close(client.getClientSocket()); // throw erreur si impossible d ajouter un nouveau socket client
				server.getClients()[client.getClientSocket()] = &client;
			}
			else
				handleConnection(server.getEventsTab()[i].data.fd); /* Handle l evenement !!!*/
		}
	}
	for (std::map<const int, Client * >::iterator it = server.getClients().begin(); it != server.getClients().end(); it++)
		close(it->first); // verifier gestion memoire pointeurs Client*
	close(server.getEpollFd());

	return 0;
}
