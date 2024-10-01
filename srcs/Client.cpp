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

void Client::setHost(const std::string& host) {
	this->host = host;
}

void Client::setRealName(const std::string& real) {
	this->realName = real;
}

void Client::addBuffer(const std::string& buffer) {
	this->buffer.append(buffer);
}

std::string Client::getNick() const {
	return nick;
}

std::string Client::getUser() const {
	return user;
}

std::string Client::getHost() const {
	return host;
}

std::string Client::getRealName() const {
	return realName;
}

std::string& Client::getBuffer() {
	return buffer;
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

bool	Client::isUserSet( void ) const { return !user.empty(); }

bool	Client::isNickSet( void ) const { return !nick.empty(); }

bool	Client::isClientSet( void ) const { return hasPassword() && isUserSet() && isNickSet(); }

void Client::deleteBuffer() {
	buffer.clear();
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
