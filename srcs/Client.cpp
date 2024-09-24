#include "../includes/Client.hpp"

Client::Client(int socket) : socket(socket), registered(false), welcomeSent(false) {}

Client::Client() : socket(-1), registered(false), welcomeSent(false) {}

Client::~Client() {}

int Client::getSocket() const {
	return socket;
}

void Client::setRegistered(bool status) {
    registered = status;
}

void Client::setNick(const std::string& nick) {
	this->nick = nick;
	registered = !this->nick.empty() && !this->user.empty();
}

void Client::setUser(const std::string& user) {
	this->user = user;
	registered = !this->nick.empty() && !this->user.empty();
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

bool Client::isWelcomeSent() const {
	return welcomeSent;
}

void Client::setWelcomeSent(bool sent) {
	welcomeSent = sent;
}

void Client::setPassword(const std::string& password) {
	this->password = password;
}

std::string Client::getPassword() const {
	return password;
}

bool Client::hasPassword() const {
	return !password.empty();
}

bool Client::operator<(const Client& other) const {
	return this->socket < other.socket;
}

bool Client::operator==(const Client &other) const {
return this->getSocket() == other.getSocket();
}

bool Client::operator!=(const Client &other) const {
	return !(*this == other);
}