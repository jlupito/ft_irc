#pragma once

# define RPL_WELCOME(user_id, nickname) (":localhost 001 " + nickname + " :Welcome to the Internet Relay Network " + user_id + "\r\n")
# define RPL_YOURHOST(client, servername, version) (":localhost 002 " + client + " :Your host is " + servername + " , running version " + version + "\r\n")
# define RPL_CREATED() (": localhost 003 " + "This server was created <date>")
# define RPL_MYINFO() (": localhost 004 " +  "<servername> <version> <available user modes>" +  "<available channel modes>")
              
             
    