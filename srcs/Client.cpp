#include "../includes/Client.hpp"

Client::Client(int socket) : socket(socket), registered(false) {}

Client::Client() : socket(-1), registered(false) {}

int Client::getSocket() const {
    return socket;
}

void Client::setNick(const std::string& nick) {
    this->nick = nick;
    registered = !this->nick.empty() && !this->user.empty(); // Mise à jour de l'état d'enregistrement
}

void Client::setUser(const std::string& user) {
    this->user = user;
    registered = !this->nick.empty() && !this->user.empty(); // Mise à jour de l'état d'enregistrement
}

std::string Client::getNick() const {
    return nick;
}

std::string Client::getUser() const {
    return user;
}

bool Client::isRegistered() const {
    return registered;
}

