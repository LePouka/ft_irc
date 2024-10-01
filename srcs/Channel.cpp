#include "../includes/Channel.hpp"

//                          CHANNEL


//CONST/DEST
Channel::Channel()
{}

Channel::Channel(const std::string& name) : name(name)
{}

Channel::~Channel()
{}

//GETTER/SETTER
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
	// Add the user to the user list
	users.insert(client);

	// If the user was invited, remove them from the invite list
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

//METHODS
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

void Channel::writeMsgChannel(Client client, std::string const &msg, std::string const &command)
{
	std::set<Client> users = this->getUsers();
	for (std::set<Client>::iterator it = users.begin(); it != users.end(); ++it) {
		if (it->getSocket() != client.getSocket()) {
			std::string fullMessage = ":" + client.getNick() + " " + command + " " + this->getName() + " :" + msg + "\r\n";
			sendMessage(it->getSocket(), fullMessage);
		}
	}
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

//                          CHANNELARRAY

//CONST/DEST

ChannelArray::ChannelArray()
{
	
}

ChannelArray::~ChannelArray()
{}

//GETTER/SETTER

Channel& ChannelArray::getChannel(std::string const &channel)
{
	std::map<std::string, Channel>::iterator it = channels.find(channel);
	if (it != channels.end()) {
		return it->second;
	}
	throw std::runtime_error("Channel not found");  // Handle the error case properly
}

std::set<Client> ChannelArray::getOperators(std::string const &channel)
{
	return channels.find(channel)->second.getOperators();
}

std::set<std::string> ChannelArray::getChannelsClient(Client client)
{
	return clientChannels.find(client)->second;
}

void ChannelArray::setChannelName(std::string const &channel)
{
	channels.find(channel)->second.setName(channel);
}

void ChannelArray::setChannelTopic(std::string const &channel, std::string const &topic)
{
	channels.find(channel)->second.setTopic(topic);
}

//METHODS

void ChannelArray::createChannel(std::string const &channel, Client const &client)
{
	if (!isChan(channel))
	{
		Channel newChannel;

		newChannel.setName(channel);
		newChannel.setTopic("");
		newChannel.setPassword("");
		newChannel.addUser(client);
		newChannel.addOperators(client);
		newChannel.setInvite(false);
		newChannel.setTopicRestricted(false);
		newChannel.setKeyNeeded(false);
		newChannel.setUserLimit(0);
		channels.insert(std::make_pair(channel, newChannel));
	}
}

bool ChannelArray::userInChannel(Client client, std::string const &channel)
{
	return clientChannels[client].find(channel) != clientChannels[client].end();
}

void ChannelArray::join(Client client, std::string const &channel)
{
	if (!isChan(channel)) {
		createChannel(channel, client);  // Automatically create the channel if it doesn't exist
	}

	if (clientChannels[client].find(channel) == clientChannels[client].end())
	{
		clientChannels[client].insert(channel);
		channels[channel].addUser(client);
	}
}

void ChannelArray::leave(Client client, std::string const &channel)
{
	if (!isChan(channel)) {
		// Handle the case where the channel does not exist
		return;
	}

	if (clientChannels[client].find(channel) != clientChannels[client].end())
	{
		getChannel(channel).removeOperator(client);
		clientChannels[client].erase(channel);
		channels[channel].removeUser(client);
		if (getChannel(channel).getUsers().empty())
		{
			deleteChan(channel);
		}
	}
}

void ChannelArray::leaveAll(Client client)
{
	std::set<std::string> clientChannelsSet = getChannelsClient(client);

	// Iterate over the copy to avoid invalidation
	for (std::set<std::string>::iterator it = clientChannelsSet.begin(); it != clientChannelsSet.end(); ++it)
	{
		if (getOperators(*it).find(client) != getOperators(*it).end())
			getChannel(*it).removeOperator(client);
		channels[*it].removeUser(client);
		if (getChannel(*it).getUsers().empty())
		{
			deleteChan(*it);
		}
	}
	// Only modify the original map after the iteration
	clientChannels[client].clear();
}

void ChannelArray::deleteChan(const std::string& channel) {
	if (!isChan(channel)) {
		return;
	}
	channels.erase(channel);
}

bool ChannelArray::isChan(std::string const &channel)
{
	return channels.find(channel) != channels.end();
}

bool ChannelArray::isOperator(Client client, std::string const &channel)
{
	if (!isChan(channel)) {
		// Handle the case where the channel does not exist
		return(false);
	}

	return getChannel(channel).getOperators().find(client) != getChannel(channel).getOperators().end();
}

void ChannelArray::writeMsgChannel(Client client, std::string const &channel, std::string const &msg)
{
	if (!isChan(channel)) {
		// Handle the case where the channel does not exist
		return;
	}

	// Get all users in the channel
	std::set<Client> users = getChannel(channel).getUsers();
	
	// Broadcast message to all users except the sender
	for (std::set<Client>::iterator it = users.begin(); it != users.end(); ++it) {
		if (*it != client) {
			// Include sender's nickname in the message
			sendMessage((*it).getSocket(), ":" + client.getNick() + " PRIVMSG " + channel + " :" + msg);
		}
	}
}

void	ChannelArray::eraseChanFromClientChannels( Client const & client, std::string const & chanName ) {

	std::map< Client, std::set< std::string > >::iterator	it = clientChannels.find( client );

	if ( it != clientChannels.end() ) {

		std::set< std::string >::iterator	channelIt = it->second.find( chanName );

		if ( channelIt != it->second.end() ) {

			it->second.erase( channelIt );
		}

		if ( it->second.empty() ) {

			clientChannels.erase( it );
		}
	}
}
