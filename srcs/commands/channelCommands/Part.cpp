#include "../../../includes/Server.hpp"
#include "../../../includes/Util.hpp"


void Server::handlePartCommand(Client &client, const std::string& args) {
	int client_socket = client.getSocket();
	size_t pos = args.find(' ');
	std::string channel_name = (pos != std::string::npos) ? args.substr(0, pos) : args;
	std::string part_message = (pos != std::string::npos) ? args.substr(pos + 1) : ""; // Message optionnel
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
	std::string full_part_message = ":" + client.getNick() + "!" + client.getUser() + " PART " + channel_name;
	if (!part_message.empty()) {
		full_part_message += " :" + part_message;
	}
	full_part_message += "\r\n";
	channel.broadcastMessage(client, full_part_message);
	channel.removeUser(client);
	if (channel.getUsers().empty()) {
		getChannelArray().deleteChan(channel_name);
	}
	sendMessage(client_socket, RPL_PARTMESSAGE(SERVER_NAME, client.getNick(), channel_name, part_message));
}
