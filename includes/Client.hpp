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
    Client(int socket);
    Client();

    int getSocket() const;
    void setNick(const std::string& nick);
    void setUser(const std::string& user);
    std::string getNick() const;
    std::string getUser() const;

    bool isRegistered() const;
    bool isWelcomeSent() const;
    void setWelcomeSent(bool sent);

private:
    int socket;
    std::string nick;
    std::string user;
    bool registered;
    bool welcomeSent;
};