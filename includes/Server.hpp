#pragma once

#include <map>
#include <string>
#include <sys/epoll.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sstream>
#include <stdexcept>
#include "Commands.hpp"
#include "Channel.hpp"
#include "Client.hpp"

#define SERVER_NAME "ircserv"

class Server {
public:
    
    //CONST/DEST
	Server(int port, const std::string& password = "");
	~Server();
    
    //GETTER/SETTER
    int getServerSocket();
    ChannelArray& getChannelArray();
    std::map<int, Client>& getClientMap();
    //METHODS
	void run();


private:
	int server_socket;
	int epoll_fd;
	std::map<int, Client> clients;
    ChannelArray channels;
	epoll_event ev;
	epoll_event events[10];

	bool serverPasswordRequired;
    std::string serverPassword;
	std::string server_config_password;

	void setNonBlocking(int sock);
	void createSocket();
	void bindSocket(int port);
	void startListening();
	void createEpollInstance();
	void addSocketToEpoll(int sock);
	void eventLoop();
	void handleClientMessage(int client_socket, const std::string& message);
	void sendErrorMessage(int client_socket, const std::string& command);
	void handleNickCommand(int client_socket, const std::string& nick);
	void handleUserCommand(int client_socket, const std::string& user);
	void handlePassCommand(int client_socket, const std::string& password);
};
