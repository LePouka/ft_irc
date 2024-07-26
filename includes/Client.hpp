#pragma once

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <map>

#define max_events 10

class client {
	public:
		client(int socket);
		client();
		int getSocket() const;
		void setNick(const std::string& nick);
		void setUser(const std::string& user);
		std::string getNick() const;
		std::string getUser() const;

	private:
		int socket;
		std::string nick;
		std::string user;
};
