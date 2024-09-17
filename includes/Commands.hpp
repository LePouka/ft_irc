#pragma once

#include "Client.hpp"
#include "Server.hpp"

class Server;

//                          CHANNEL COMMANDS

//JOIN
void join(Client client, std::string channel, Server &server);

//PRIVMSG
void privmsg(Client client, std::string params, Server &server);

