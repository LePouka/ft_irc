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


template <typename T>
std::string to_string(T value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

class Server {
public:

	//CONST/DEST
	Server(int port, const std::string& password = "");
	~Server();

	//GETTER/SETTER
	int						getServerSocket();
	ChannelArray&			getChannelArray();
	std::map<int, Client>&	getClientMap();
	Client&					getClient( std::string const & nickname );

	//METHODS
	void	run();

	bool	isClientRegistered( std::string const & nickname );


private:
	int						server_socket;
	int						epoll_fd;
	std::map<int, Client>	clients;
	ChannelArray			channels;
	epoll_event				ev;
	epoll_event				events[10];

	bool		serverPasswordRequired;
	std::string	serverPassword;
	std::string	server_config_password;

	void	setNonBlocking(int sock);
	void	createSocket();
	void	bindSocket(int port);
	void	startListening();
	void	createEpollInstance();
	void	addSocketToEpoll(int sock);
	void	eventLoop();
	void	handleClientMessage(int client_socket, const std::string& message);
	void	sendErrorMessage(int client_socket, const std::string& command);
	void	handleNickCommand(int client_socket, const std::string& nick);
	void	handleUserCommand(int client_socket, const std::string& user);
	void	handlePassCommand(int client_socket, const std::string& password);
	void	handleTopicCommand(int client_socket, const std::string& args);
	void	handlePartCommand(int client_socket, const std::string& channelName);
	void 	handleInviteCommand(Client client, std::string params, Server &server);
	void	handleModeCommand(Client client, std::string params);
	void	handleMode(Client &client, Channel &channel, char mode, bool adding, std::istringstream &iss);
	void	handleJoinCommand(Client client, std::string params, Server &server);
	void 	handlePrivmsgCommand(Client client, std::string params, Server &server);
	void	seeModeChannels(Client &client, const std::string &chanName);
	void	handleKickCommand( ChannelArray& channelArray, Client const & client, std::string const & args );

	//utils
	bool	kickTests( ChannelArray& channelArray, Client const & client, std::string const & channelName, std::string const & nickname );
	void	kick( Client& client, Channel& channel );

};
