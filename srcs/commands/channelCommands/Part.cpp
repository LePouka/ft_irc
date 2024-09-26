#include "../../../includes/Server.hpp"
#include "../../../includes/Util.hpp"


void Server::handlePartCommand(Client &client, const std::string& args) {
	Channel &channel = getChannelArray().getChannel(args);

	int client_socket = client.getSocket();
	size_t pos = args.find(' ');
	std::string channel_name = (pos != std::string::npos) ? args.substr(0, pos) : args;
	if (!channels.isChan(channel_name)) {
		std::string error_message = ERR_NOSUCHCHANNEL("server", channel_name);
		sendMessage(client_socket, error_message);
		return;
	}
	if (clients.find(client_socket) == clients.end()) {
		std::string error_message = ERR_NOTREGISTERED("server");
		sendMessage(client_socket, error_message);
		return;
	}
	if (!channel.isInUserList(client)) {
		std::string error_message = ERR_NOTONCHANNEL("server", channel_name);
		sendMessage(client_socket, error_message);
		return;
	}
	std::string part_message = ":" + client.getNick() + " PART " + channel_name + "\r\n";
	channel.broadcastMessage(client, part_message);
	channel.removeUser(client);
	if (channel.getUsers().empty()) {
		getChannelArray().deleteChan(channel_name);
	}
}