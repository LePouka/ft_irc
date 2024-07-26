#include "../includes/Client.hpp"

Client::Client(int socket) : socket(socket) {}

Client::Client() : socket(-1) {} 

int Client::getSocket() const {
    return socket;
}

void Client::setNick(const std::string& nick) {
    this->nick = nick;
}

void Client::setUser(const std::string& user) {
    this->user = user;
}

std::string Client::getNick() const {
    return nick;
}

std::string Client::getUser() const {
    return user;
}
