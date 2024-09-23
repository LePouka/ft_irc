#include "../../../includes/Server.hpp"
#include "../../../includes/Util.hpp"

void Server::handlePartCommand(int client_socket, const std::string& args) {
	size_t pos = args.find(' ');
	std::string channel_name = (pos != std::string::npos) ? args.substr(0, pos) : args;
	if (args.empty()) {
		std::string error_message = "Error: Invalid arguments for PART command\r\n";
		send(client_socket, error_message.c_str(), error_message.length(), 0);
		return;
	}
	if (!channels.isChan(channel_name)) {
		std::string error_message = ERR_NOSUCHCHANNEL("server", channel_name);
		send(client_socket, error_message.c_str(), error_message.length(), 0);
		return;
	}
	Channel& channel = channels.getChannel(channel_name);
	Client& client = clients[client_socket];
	if (!channel.hasUser(client)) {
		std::string error_message = ERR_NOTONCHANNEL("server", channel_name);
		send(client_socket, error_message.c_str(), error_message.length(), 0);
		return;
	}
	std::string part_message = ":" + client.getNick() + " PART " + channel_name + "\r\n";
	channel.broadcastMessage(part_message, client);
	if (channel.getUsers().empty()) {
		channels.deleteChan(channel_name);
	}
	send(client_socket, part_message.c_str(), part_message.length(), 0);
}
