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

void	Server::handleMode(Client &client, Channel &channel, char mode, bool adding, std::istringstream &iss)
{
	switch (mode) {
		case 'i':
			
			break;
		case 't':
			break;
		case 'k':
			break;
		case 'b':
			break;
		case 'o':
			break;
		case 'l':
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