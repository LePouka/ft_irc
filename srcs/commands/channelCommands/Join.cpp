#include "../../../includes/Commands.hpp"

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

void Server::handleJoinCommand(Client &client, std::string params, Server &server) {
	if (params.empty()) {
		sendMessage(client.getSocket(), ERR_NEEDMOREPARAMS("Server", "JOIN"));
		return;
	}
	std::string channelName = params;
	ChannelArray &channelArray = server.getChannelArray();
	Channel *channel = NULL;
	if (channelArray.isChan(channelName)) {
		channel = &channelArray.getChannel(channelName);
	} else {
		channelArray.createChannel(channelName, client);
		channel = &channelArray.getChannel(channelName);
	}
	if (channel->getInvite() && !channel->isInInviteList(client)) {
		sendMessage(client.getSocket(), ERR_INVITEONLYCHAN("Server", channelName));
		return;
	}
	if (!channel->isInUserList(client)) {
		channel->addUser(client);
	}
	std::string joinMessage = JOIN_CHAN(client.getNick(), client.getUser(), channelName, "JOIN");
	channel->broadcastMessage(client, joinMessage);
	std::string topicMessage = RPL_TOPIC("Server", channelName, channel->getTopic());
	sendMessage(client.getSocket(), topicMessage);
	sendRPL_NAMREPLY(client, *channel);
}
