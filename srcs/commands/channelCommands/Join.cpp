#include "../../../includes/Server.hpp"

void Server::sendRPL_NAMREPLY(Client &client, Channel &channel)
{
	std::string userList;
	std::set<Client> users = channel.getUsers();

	for (std::set<Client>::iterator it = users.begin(); it != users.end(); ++it) {
		if (!userList.empty()) {
			userList += " ";
		}
		
		if (channel.isInOperatorList(*it)) {
			userList += "@" + (*it).getNick();
		} else {
			userList += (*it).getNick();
		}
	}
	std::string RPL_NAMREPLY = ":Server 353 " + client.getNick() + " = " + channel.getName() + " :" + userList + "\r\n";
	sendMessage(client.getSocket(), RPL_NAMREPLY);
	std::string RPL_ENDOFNAMES = ":Server 366 " + client.getNick() + " " + channel.getName() + " :End of /NAMES list\r\n";
	sendMessage(client.getSocket(), RPL_ENDOFNAMES);
}

void Server::handleJoinCommand(Client &client, std::string params) {
	std::istringstream iss(params);
	std::string channelName;
	if (!(iss >> channelName)) {
        std::string error_message = ERR_NEEDMOREPARAMS("Server", "JOIN");
        sendErrorMessage(client.getSocket(), error_message);
		return;
	}
	ChannelArray &channelArray = this->getChannelArray();
	Channel *channel = NULL;
	if (channelArray.isChan(channelName)) {
		channel = &channelArray.getChannel(channelName);
		if (channel->getInvite() && !channel->isInInviteList(client)) {
			sendMessage(client.getSocket(), ERR_INVITEONLYCHAN("Server", channelName));
			return;
		}
		if (channel->getKeyNeeded())
		{
			std::string password;
			if (!(iss >> password))
			{
				std::string error_message = ERR_BADCHANNELKEY("Server", channel->getName());
        		sendErrorMessage(client.getSocket(), error_message);
				return;
			}
			password.erase(password.find_last_not_of(" \n\r") + 1);
			if (channel->getPassword() != password)
			{
				std::string error_message = ERR_PASSWDMISMATCH("Server", client.getNick());
				sendErrorMessage(client.getSocket(), error_message);
				return;
			}
		}
		if (channel->getUserLimit() != 0)
		{
			if (channel->getUserLimit() == channel->getUsers().size())
			{
				std::string error_message = ERR_CHANNELISFULL("Server", channel->getName());
				sendErrorMessage(client.getSocket(), error_message);
				return;
			}
		}
		if (!channel->isInUserList(client)) {
			channel->addUser(client);
			if (channel->getInvite())
			{
				channel->removeInvited(client);
			}
		}
	} else {
		channelArray.createChannel(channelName, client);
		channel = &channelArray.getChannel(channelName);
	}
	std::string joinMessage = JOIN_CHAN(client.getNick(), client.getUser(), channelName, "JOIN");
	channel->broadcastMessage(client, joinMessage);
	std::string topicMessage = RPL_TOPIC("Server", channelName, channel->getTopic());
	sendMessage(client.getSocket(), topicMessage);
	sendRPL_NAMREPLY(client, *channel);
}
