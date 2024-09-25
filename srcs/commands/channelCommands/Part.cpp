#include "../../../includes/Server.hpp"
#include "../../../includes/Util.hpp"

void Server::handlePartCommand(int client_socket, const std::string& args) {
	if (args.empty()) {
		std::string error_message = "Error: Invalid arguments for PART command\r\n";
		sendMessage(client_socket, error_message);
		return;
	}
	size_t pos = args.find(' ');
	std::string channel_name = (pos != std::string::npos) ? args.substr(0, pos) : args;
	if (!channels.isChan(channel_name)) {
		std::string error_message = ERR_NOSUCHCHANNEL("server", channel_name);
		sendMessage(client_socket, error_message);
		return;
	}
	if (clients.find(client_socket) == clients.end()) {
		std::string error_message = "Error: Client not found\r\n";
		sendMessage(client_socket, error_message);
		return;
	}
	Client& client = clients[client_socket];
	Channel& channel = channels.getChannel(channel_name);
	if (!channel.isInUserList(client)) {
		std::string error_message = ERR_NOTONCHANNEL("server", channel_name);
		sendMessage(client_socket, error_message);
		return;
	}
	std::string part_message = ":" + client.getNick() + " PART " + channel_name + "\r\n";
	sendMessage(client_socket, part_message);
	channel.removeUser(client);
	if (channel.getUsers().empty()) {
		channels.deleteChan(channel_name);
	}
}
