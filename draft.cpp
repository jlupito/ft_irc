/*
BROUILLON

Rappel :

The IRC protocol includes a special type of message called a reply. When a client sends
a command to a server, the server will send a reply (except in a few special commands where
a reply should not be expected). Replies are used to acknowledge that a command was processed
correctly, to indicate errors, or to provide information when the command performs a server
query (e.g., asking for the list of users or channels).
A reply is a message with the following characteristics:
- It always includes a prefix.
- The command will be a three-digit code. The full list of possible replies is specified
in [RFC2812 §5].
- The first parameter is always the target of the reply, typically a nick.
Examples :
:irc.example.com 433 * borja :Nickname is already in use.
:irc.example.org 332 borja #cmsc23300 :A channel for CMSC 23300 students


Commandes a traiter :

CAP_LS implementee en commande
Command: PASS
	Parameters: <password>
Command: NICK
	Parameters: <nickname>
Command: USER
   Parameters: <user> <mode> <unused> <realname>
Command: OPER
	Parameters: <user> <password>
Command: QUIT
	Parameters: [<Quit message>]

Command: PRIVMSG
   Parameters: <receiver>{,<receiver>} <text to be sent>

Command: JOIN
   Parameters: <channel>{,<channel>} [<key>{,<key>}]
Command: PART
   Parameters: <channel>{,<channel>}
Command: NAMES
   Parameters: [<channel>{,<channel>}]
Command: LIST
   Parameters: [<channel>{,<channel>} [<server>]]

Commandes specifiques aux operateurs de canaux :
Command: KICK
   Parameters: <channel> <user> [<comment>]
Command: INVITE
   Parameters: <nickname> <channel>
Command: TOPIC
   Parameters: <channel> [<topic>]
Command: MODE >> cf sujet

POINTS D'ARBITRAGE :

- Limites de certaines commandes comme NICK : ressources disponibles //
 délais de modification du mdp + finir les modes.

- Stockage de sclients dans la map des channels ; sockets ou names ?
 On n'a à priori pas besoin de conserver les infos des clients entre
 deux connexions. Or les sockets sont + simples à parser et stocker.

Autres pistes de travail :
Gestion des Connexions et Déconnexions :
gérer correctement les connexions et les déconnexions des clients.
(libération de la mémoire associée à un client lors de la deconnexion).

Gestion des Canaux :
rejoindre, quitter, et interagir avec les canaux de manière appropriée.

Sécurité : verifier la politique mdp et operators.
Journal des logs ?

*/
