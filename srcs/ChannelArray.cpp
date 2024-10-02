#include "../includes/ChannelArray.hpp"

ChannelArray::ChannelArray() {}

ChannelArray::~ChannelArray() {}

Channel& ChannelArray::getChannel(std::string const &channel)
{
	std::map<std::string, Channel>::iterator it = channels.find(channel);
	if (it != channels.end()) {
		return it->second;
	}
	throw std::runtime_error("Channel not found");
}

std::map<std::string, Channel>&	ChannelArray::getChannelMap() {
	return this->channels;
}

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

void ChannelArray::writeMsgChannel(Client client, std::string const &channel, std::string const &msg)
{
	if (!isChan(channel)) {
		return;
	}
	std::set<Client> users = getChannel(channel).getUsers();
	for (std::set<Client>::iterator it = users.begin(); it != users.end(); ++it) {
		if (*it != client) {
			sendMessage((*it).getSocket(), ":" + client.getNick() + " PRIVMSG " + channel + " :" + msg);
		}
	}
}
