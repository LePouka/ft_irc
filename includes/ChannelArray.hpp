#pragma once

#include <string>
#include <set>
#include <map>
#include <vector>
#include "Client.hpp"
#include "Message.hpp"

class ChannelArray {
private:
	std::map<std::string, Channel>				channels;
public:

	ChannelArray();
	~ChannelArray();

	Channel&						getChannel(std::string const &channel);
	std::map<std::string, Channel>&	getChannelMap();

	void							createChannel(std::string const &channel, Client const &client);
	void							deleteChan(std::string const &channel);
	bool							isChan(std::string const &channel);
	void							writeMsgChannel(Client client, std::string const &channel, std::string const &msg);
};
