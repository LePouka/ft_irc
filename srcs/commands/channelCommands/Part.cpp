#include "../../../includes/Server.hpp"

void Server::handlePartCommand(Client &client, const std::string& args) {
	int client_socket = client.getSocket();
	size_t pos = args.find(' ');
	std::string channel_name = (pos != std::string::npos) ? args.substr(0, pos) : args;
	std::string part_message = (pos != std::string::npos) ? args.substr(pos + 1) : "";

	if (!channels.isChan(channel_name)) {
		sendMessage(client_socket, ERR_NOSUCHCHANNEL("server", channel_name));
		return;
	}
	if (clients.find(client_socket) == clients.end()) {
		sendMessage(client_socket, ERR_NOTREGISTERED("server"));
		return;
	}
	Channel &channel = getChannelArray().getChannel(channel_name);
	if (!channel.isInUserList(client)) {
		sendMessage(client_socket, ERR_NOTONCHANNEL("server", channel_name));
		return;
	}
	std::string partMessage = PART_CHAN(client.getNick(), client.getUser(), channel_name, part_message);
	channel.broadcastMessage(client, partMessage);
	channel.removeUser(client);
	
	if (channel.getUsers().empty()) {
		getChannelArray().deleteChan(channel_name);
	}
}
