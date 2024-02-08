#include "Server.hpp"

const int MAX_EVENTS = 10;
const int PORT = 6667;

void handleConnection(int clientSocket) {
    // Code pour gérer la connexion du client
    // Coder pour lire/écrire des données et/ou traiter les commandes IRC
    // Par exemple fermer simplement la connexion :
    close(clientSocket);
}

int main(int ac, char **av) {

	// int serverSocket;
	// int clientSocket;
	// int epollFd;

    // struct sockaddr_in serverAddr; //struct pour stocker les infos relatives aux adresses IP
	// struct sockaddr_in clientAddr;
    // socklen_t clientAddrLen = sizeof(clientAddr); //la taille e l adresse du client est necessaire pour la focntion accept()
    // struct epoll_event events[MAX_EVENTS];

	Server server;

	/* Créer un socket pour le serveur, il s'agit de la socket d' ecoute du serveur */
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
		// rejouter protection si serverSocket < 0 retourner une erreur try/catch?

	/* Configurer l adresse du serveur */
	memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET; // Famille d'adresses (AF_INET pour ipv4)
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Contient l'adresse IP du serveur en format binaire.
    serverAddr.sin_port = htons(PORT); //numero de port htons = "host to network short", garantit que le numéro de port est correctement converti dans le format réseau

	/* Lier le socket à l'adresse et au port */
	if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        close(serverSocket);
        //throw une exception
    }

	/* Mettre le socket en mode ecoute */
	if (listen(serverSocket, SOMAXCONN) < 0) {
        close(serverSocket);
		//throw une exception
    }

	/* Créer un descripteur de fichier epoll */
    epollFd = epoll_create1(0);
    if (epollFd < 0) {
        close(serverSocket);
		//throw une exception
    }

	/* Ajouter le socket du serveur à l'ensemble epoll */
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = serverSocket;
    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, serverSocket, &event) == -1) {
        close(serverSocket);
        close(epollFd);
		//throw une exception "Erreur a l'ajout du socket du serveur"
    }

	while (true) {
		int numEvents = epoll_wait(epollFd, events, MAX_EVENTS, -1);
        // ajouter protection if numEvents < 0 throw Error, doit break
		for (int i = 0; i < numEvents; ++i) {
			if (events[i].data.fd == serverSocket) {
				/* Nouvelle connexion entrante */
				clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
				if (clientSocket < 0) {
					// afficher une erreur mais ne pas throw une exception
					continue; // because doit passer a l iteration suivante, le serveur ne doit pas s arreter
				}
				/*  Ajouter le nouveau socket client à l'ensemble epoll */
				event.events = EPOLLIN;
				/*EPOLLIN: Cet événement est déclenché lorsque des données sont prêtes à être lues depuis
				le descripteur de fichier associé. Il indique que vous pouvez utiliser des appels
				de lecture (read ou équivalents) sans bloquer le processus.*/
				event.data.fd = clientSocket;
				if (epoll_ctl(epollFd, EPOLL_CTL_ADD, clientSocket, &event) < 0)
					close(clientSocket); // throw erreur si impossible d ajouter un nouveau socket client
			}
			else
				handleConnection(events[i].data.fd); /* Handle l evenement !!!*/
		}
	}
	close(serverSocket);
    close(epollFd);

	return 0;
}
