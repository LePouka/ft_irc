#include "../../includes/Server.hpp"
#include "../../includes/Util.hpp"

// REPLIES :
// RPL_CHANNELMODEIS(324)
// RPL_CREATIONTIME(329)
// RPL_INVITELIST(346)
// RPL_ENDOFINVITELIST(347)
// RPL_BANLIST(367)
// RPL_ENDOFBANLIST(368)
// ERRORS :
// ERR_NEEDMOREPARAMS (461)
// ERR_KEYSET (467)
// ERR_NOCHANMODES (477)
// ERR_CHANOPRIVSNEEDED (482)
// 

void showBanList(Client &client, Channel &channel)
{
    const std::set<Client>& bannedClients = channel.getBanned();  // Store the result in a local variable

    for (std::set<Client>::const_iterator it = bannedClients.begin(); it != bannedClients.end(); ++it) {
        std::string banNick = it->getNick();  // Get the nick of the banned client
        sendMessage(client.getSocket(), RPL_BANLIST(client.getNick(), channel.getName(), banNick));
    }
    
    sendMessage(client.getSocket(), RPL_ENDOFBANLIST(client.getNick(), channel.getName()));
}

void showCurrentModes(Client &client, Channel &channel)
{
    std::string modes = "+";
    if (channel.getInvite()) modes += "i";
    if (channel.getTopicRestricted()) modes += "t";
    if (channel.getKeyNeeded()) modes += "k";
    if (channel.getUserLimit() > 0) modes += "l";
    // Ban list is special, handled separately
    sendMessage(client.getSocket(), RPL_CHANNELMODEIS("Server", channel.getName(), modes));
}

void sendModeChangeMessage(Client &client, Channel &channel, char mode, bool adding, const std::string &arg = "")
{
    std::string change = adding ? "+" : "-";
    change += mode;
    if (!arg.empty()) {
        change += " " + arg;
    }

    std::string message = ":" + client.getNick() + " MODE " + channel.getName() + " " + change + NEW_LINE;
    channel.writeMsgChannel(client, message);
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
		case 'k':
			if (adding) {
				std::string key;
				if (!(iss >> key)) {
					sendMessage(client.getSocket(), ERR_NEEDMOREPARAMS("Server", "MODE"));
					return;
				}
				channel.setPassword(key);
			} else {
				channel.setPassword("");
			}
			sendModeChangeMessage(client, channel, mode, adding);
			break;

		case 'b': {
			std::string banNick;
			if (!(iss >> banNick)) {
				showBanList(client, channel);
				return;
			}

			if (adding) {
				channel.addBanned(getClient(banNick));
			} else {
				channel.removeBanned(getClient(banNick));
			}
			sendModeChangeMessage(client, channel, mode, adding, banNick);
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
				channel.setUserLimit(0); // No limit
			}
			sendModeChangeMessage(client, channel, mode, adding);
			break;
	}
}

void	Server::handleModeCommand(Client client, std::string params)
{	
	std::istringstream iss(params);
	std::string channelName, modeFlags, modeArg;
	if (!(iss >> channelName))
	{
        std::string error_message = ERR_NEEDMOREPARAMS("Server", "MODE");
        sendErrorMessage(client.getSocket(), error_message);
        return ;
	}
	ChannelArray &channelArray = this->getChannelArray();

	if (channelName[0] == '#' || channelName[0] == '&')
	{
		if (!(channelArray.isChan(channelName)))
		{
			std::string error_message = ERR_NOSUCHCHANNEL("Server", "MODE");
			sendErrorMessage(client.getSocket(), error_message);
			return ;
		}
		Channel *channel = &channelArray.getChannel(channelName);
		if (!(iss >> modeFlags))
		{
			showCurrentModes(client, *channel);
        	return;
		}

		if (!(channel->isInOperatorList(client))) {
			std::string error_message = ERR_CHANOPRIVISNEEDED("Server", channel->getName());
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