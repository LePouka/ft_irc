#pragma once

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <string>
#include <map>

#define MAX_EVENTS 10

class Client {
public:
	// Const/Destructeur
	Client(int socket);
	Client();
	~Client();

	// Getters/Setters
	int getSocket() const;
	void setRegistered(bool status);
	void setNick(const std::string& nick);
	void setUser(const std::string& user);
	void setPassword(const std::string& password);
	void setHost(const std::string& host);
	void setRealName(const std::string& real);

	std::string getNick() const;
	std::string getUser() const;
	std::string getPassword() const;
	std::string getHost() const;
	std::string getRealName() const;

	bool isRegistered() const;
	bool isWelcomeSent() const;
	bool hasPassword() const;

	void setWelcomeSent(bool sent);
	bool operator<(const Client& other) const;
	bool operator==(const Client &other) const;
	bool operator!=(const Client &other) const;

private:
	int socket;
	std::string nick;
	std::string user;
	std::string password;
	std::string host;
	std::string realName;
	bool registered;
	bool welcomeSent;
};
