#include "../includes/Channel.hpp"
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

Channel::Channel(const std::string& name) : name(name), limit(-1) {}

void Channel::addUser(int socket) {
    users.insert(socket);
}

void Channel::removeUser(int socket) {
    users.erase(socket);
}

void Channel::setTopic(const std::string& topic) {
    this->topic = topic;
}

void Channel::setPassword(const std::string& password) {
    this->password = password;
}

void Channel::setMode(const std::string& mode) {
    this->mode = mode;
}

void Channel::setLimit(int limit) {
    this->limit = limit;
}

void Channel::addOperator(int socket) {
    operators.insert(socket);
}

void Channel::removeOperator(int socket) {
    operators.erase(socket);
}

void Channel::broadcastMessage(const std::string& message, int exceptSocket) {
    std::set<int>::iterator it;
    for (it = users.begin(); it != users.end(); ++it) {
        if (*it != exceptSocket) {
            send(*it, message.c_str(), message.length(), 0);
        }
    }
}

bool Channel::isOperator(int socket) const {
    return operators.find(socket) != operators.end();
}

