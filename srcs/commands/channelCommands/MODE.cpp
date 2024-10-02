#include "../../../includes/Server.hpp"

void showCurrentModes(Client &client, Channel &channel)
{
	std::string modes = "+";
	if (channel.getInvite()) modes += "i";
	if (channel.getTopicRestricted()) modes += "t";
	if (channel.getKeyNeeded()) modes += "k";
	if (channel.getUserLimit() > 0) modes += "l";
	sendMessage(client.getSocket(), RPL_CHANNELMODEIS("Server", channel.getName(), modes));
}

void sendModeChangeMessage(Client &client, Channel &channel, char mode, bool adding, const std::string &arg = "")
{
	std::string change = adding ? "+" : "-";
	change += mode;
	if (!arg.empty()) {
		change += " " + arg;
	}

	std::string message = MODE_CHANGE(client.getNick(), client.getUser(), channel.getName(), change);
	channel.broadcastMessage(client, message);
}

void	Server::handleMode(Client &client, Channel &channel, char mode, bool adding, std::istringstream &iss)
{
	switch (mode) {
		case 'i':
			channel.setInvite(adding);
			sendModeChangeMessage(client, channel, mode, adding);
			break;
		case 't':
			channel.setTopicRestricted(adding);
			sendModeChangeMessage(client, channel, mode, adding);
			break;
		case 'k': {
			if (adding) {
				std::string key;
				if (!(iss >> key)) {
					sendMessage(client.getSocket(), ERR_NEEDMOREPARAMS("Server", "MODE"));
					return;
				}
				channel.setPassword(key);
				channel.setKeyNeeded(true);
			} else {
				channel.setPassword("");
				channel.setKeyNeeded(false);
			}
			sendModeChangeMessage(client, channel, mode, adding);
			break;
		}
		case 'o': {
			std::string user;
			if (!(iss >> user)) {
				sendMessage(client.getSocket(), ERR_NEEDMOREPARAMS("Server", "MODE"));
				return;
			}
			Client *target = &this->getClient(user);
			if (!target) {
				sendMessage(client.getSocket(), ERR_NOSUCHNICK("Server", user));
				return;
			}
			if (adding) {
				channel.addOperators(*target);
			} else {
				channel.removeOperator(*target);
			}
			sendModeChangeMessage(client, channel, mode, adding, user);
			break;
		}
		case 'l':
			if (adding) {
				int limit;
				if (!(iss >> limit)) {
					sendMessage(client.getSocket(), ERR_NEEDMOREPARAMS("Server", "MODE"));
					return;
				}
				channel.setUserLimit(limit);
			} else {
				channel.setUserLimit(0);
			}
			sendModeChangeMessage(client, channel, mode, adding);
			break;
		default:
			sendMessage(client.getSocket(), ERR_UNKNOWNMODE("Server", std::string(1, mode)));
			break;
	}
}

void	Server::handleModeCommand(Client client, std::string params)
{	
	std::istringstream iss(params);
	std::string channelName, modeFlags;
	if (!(iss >> channelName))
	{
		std::string error_message = ERR_NEEDMOREPARAMS("Server", "MODE");
		sendErrorMessage(client.getSocket(), error_message);
		return;
	}
	ChannelArray &channelArray = this->getChannelArray();
	if (channelName[0] == '#' || channelName[0] == '&')
	{
		if (!channelArray.isChan(channelName))
		{
			std::string error_message = ERR_NOSUCHCHANNEL("Server", "MODE");
			sendErrorMessage(client.getSocket(), error_message);
			return;
		}
		Channel *channel = &channelArray.getChannel(channelName);
		if (!(iss >> modeFlags))
		{
			showCurrentModes(client, *channel);
			return;
		}
		if (!channel->isInOperatorList(client)) {
			std::string error_message = ERR_CHANOPRIVSNEEDED("Server", channel->getName());
			sendMessage(client.getSocket(), error_message);
			return;
		}
		bool addingMode = true;
		for (size_t i = 0; i < modeFlags.size(); ++i) {
			char mode = modeFlags[i];
			if (mode == '+') {
				addingMode = true;
			} else if (mode == '-') {
				addingMode = false;
			} else {
				handleMode(client, *channel, mode, addingMode, iss);
			}
		}
	}
}
