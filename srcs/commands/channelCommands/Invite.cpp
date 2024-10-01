#include "../../../includes/Server.hpp"

void Server::handleInviteCommand(Client client, std::string params, Server &server)
{
	std::istringstream iss(params);
	std::string recipient;
	std::string channel;

	if (params.empty())
	{
		std::string error_message = ERR_NEEDMOREPARAMS("server", "INVITE");
		sendErrorMessage(client.getSocket(), error_message);
		return ;
	}

	ChannelArray &channelArray = server.getChannelArray();
	// std::istringstream iss(params);

	if (!(iss >> recipient))
	{
		std::string error_message = ERR_NEEDMOREPARAMS("server", "INVITE");
		sendErrorMessage(client.getSocket(), error_message);
		return ;
	}
	if (!(iss >> channel)) {
		std::string error_message = ERR_NEEDMOREPARAMS("server", "INVITE");
		sendErrorMessage(client.getSocket(), error_message);
		return ;
	}
	std::map<int, Client> &clients = server.getClientMap();
	bool isInClients = false;
	Client invited;
	for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if ((*it).second.getNick() == recipient)
		{
			invited = (*it).second;
			isInClients = true;
			break;
		}
	}
	if (isInClients == false) {
		return ;
	}
	if (!channelArray.isChan(channel))
	{
		std::string error_message = ERR_NOSUCHCHANNEL(client.getNick(), channel);
		sendErrorMessage(client.getSocket(), error_message);
		return ;
	}
	Channel &chan = channelArray.getChannel(channel);
	if (!(chan.isInUserList(client)))
	{
		std::string error_message = ERR_NOTONCHANNEL(client.getNick(), channel);
		sendErrorMessage(client.getSocket(), error_message);
		return ;
	}
	if (!(chan.isInOperatorList(client)) && chan.getInvite())
	{
		std::string error_message = ERR_CHANOPRIVSNEEDED(client.getNick(), channel);
		sendErrorMessage(client.getSocket(), error_message);
		return ;
	}
	if (chan.isInUserList(invited))
	{
		std::string error_message = ERR_USERONCHANNEL(client.getNick(), recipient, channel);
		sendErrorMessage(client.getSocket(), error_message);
		return ;
	}
	chan.addInvited(invited);
	std::string reply = RPL_INVITING(client.getNick(), recipient, channel);
	sendMessage(client.getSocket(), reply);
	std::string invite = client.getNick() + " has invited you to the channel" + channel + NEW_LINE;
	sendMessage(invited.getSocket(), invite);
}