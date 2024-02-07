# ft_irc
L’objectif de ce projet est de reproduire le fonctionnement d’un serveur IRC.

- doc complete officielle:
[Protocol](https://tools.ietf.org/html/rfc1459)
[Architecture](https://tools.ietf.org/html/rfc2810)
[Channel Management](https://tools.ietf.org/html/rfc2811)   
[Client](https://tools.ietf.org/html/rfc2812)   
[Server Protocol](https://tools.ietf.org/html/rfc2813)

- Résumé de la doc:
https://modern.ircdocs.horse/#errneedmoreparams-461

- exemple d'un IRC:
http://chi.cs.uchicago.edu/chirc/index.html

- guide pour la programmation reseau/socket:
http://vidalc.chez.com/lf/socket.html
https://www.codequoi.com/programmation-reseau-via-socket-en-c/

**1.1 Servers**
---
Servers are uniquely identified by their name, which has a maximum
length of sixty three (63) characters.
**1.2 Clients**
---
For each client all servers MUST have the following information: a
netwide unique identifier (whose format depends on the type of
client) and the server which introduced the client.
**1.2.1 Users**
---
Each user is distinguished from other users by a unique nickname
having a maximum length of nine (9) characters.
**1.2.2 Services**
---
Each service is distinguished from other services by a service name
composed of a nickname and a server name. As for users, the nickname
has a maximum length of nine (9) characters.
**1.3 Channels**
---
Channels names are strings (beginning with a '&', '#', '+' or '!'
character) of length up to fifty (50) characters.  Apart from the
requirement that the first character is either '&', '#', '+' or '!',
the only restriction on a channel name is that it SHALL NOT contain
any spaces (' '), a control G (^G or ASCII 7), a comma (',').  Space
is used as parameter separator and command is used as a list item
separator by the protocol.  A colon (':') can also be used as a
delimiter for the channel mask.  Channel names are case insensitive.

***See the protocol grammar rules (section 2.3.1)***