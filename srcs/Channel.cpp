#include "../includes/Channel.hpp"

Channel::Channel() {}

Channel::Channel(const std::string& name) : name(name) {}

Channel::~Channel() {}

std::string Channel::getName()
{
	return (this->name);
}

std::string Channel::getTopic()
{
	return (this->topic);
}

std::string Channel::getPassword()
{
	return this->password;
}

std::set<Client> Channel::getUsers()
{
	return (this->users);
}

std::set<Client> Channel::getOperators()
{
	return (this->operators);
}

std::set<Client> Channel::getInvited()
{
	return (this->invited);
}

bool Channel::getInvite()
{
	return (this->isInviteOnly);
}

bool Channel::getTopicRestricted()
{
	return (this->isTopicRestrictedToOperators);
}

bool Channel::getKeyNeeded()
{
	return(this->isKeyNeeded);
}

unsigned int Channel::getUserLimit()
{
	return(this->userLimit);
}

void Channel::setName(std::string name)
{
	this->name = name;
}

void Channel::setTopic(std::string topic)
{
	this->topic = topic;
}

void Channel::setPassword(std::string password)
{
	this->password = password;
}

void Channel::addUser(Client client)
{
	users.insert(client);
	invited.erase(client);
}

void Channel::addOperators(Client client)
{
	operators.insert(client);
}

void Channel::addInvited(Client client)
{
	invited.insert(client);
}

void Channel::setInvite(bool invite)
{
	this->isInviteOnly = invite;
}

void Channel::setTopicRestricted(bool restricted)
{
	this->isTopicRestrictedToOperators = restricted;
}

void Channel::setKeyNeeded(bool keyNeeded)
{
	this->isKeyNeeded = keyNeeded;
}

void Channel::setUserLimit(unsigned int userLimit)
{
	this->userLimit = userLimit;
}

void Channel::removeUser(const Client& client) {
	users.erase(client);
	operators.erase(client);
}

void Channel::removeOperator(Client client)
{
	operators.erase(client);
}

void Channel::removeInvited(Client client)
{
	invited.erase(client);
}

void Channel::broadcastMessage(Client client, std::string const &msg)
{
	(void)client;
	std::set<Client> users = this->getUsers();
	for (std::set<Client>::iterator it = users.begin(); it != users.end(); ++it) {
		sendMessage(it->getSocket(), msg);
	}
}

bool Channel::isInUserList(Client client)
{
	return users.find(client) != users.end();
}

bool    Channel::isInOperatorList(Client client)
{
	return operators.find(client) != operators.end();
}

bool    Channel::isInInviteList(Client client)
{
	return invited.find(client) != invited.end();
}

bool Channel::canSendMessage(const Client &client) {
	if (!this->isInUserList(client)) return false;
	return true;
}
