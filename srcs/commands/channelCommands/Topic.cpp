#include "../../../includes/Server.hpp"
#include "../../../includes/Util.hpp"

void Server::handleTopicCommand(int client_socket, const std::string& args) {
	if (args.empty()) {
		std::string error_message = ERR_NEEDMOREPARAMS("Server", "TOPIC");
		sendMessage(client_socket, error_message);
		return;
	}
	size_t pos = args.find(' ');
	std::string channel_name = (pos != std::string::npos) ? args.substr(0, pos) : args;
	std::string new_topic = (pos != std::string::npos) ? args.substr(pos + 1) : "";
	if (!new_topic.empty() && new_topic[0] == ':') {
		new_topic = new_topic.substr(1);
	}
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
	Channel& channel = channels.getChannel(channel_name);
	Client& client = clients[client_socket];
	if (!channel.isInUserList(client)) {
		std::string error_message = ERR_NOTONCHANNEL("server", channel_name);
		sendMessage(client_socket, error_message);
		return;
	}
	if (new_topic.empty()) {
		if (channel.getTopic().empty()) {
			std::string response = RPL_NOTOPIC(client.getNick(), channel_name);
			sendMessage(client_socket, response);
		} else {
			std::string response = RPL_TOPIC(client.getNick(), channel_name, channel.getTopic());
			sendMessage(client_socket, response);
		}
	} else {
		if (!channels.isOperator(client, channel_name)) {
			std::string error_message = ERR_CHANOPRIVSNEEDED(client.getNick(), channel_name);
			sendMessage(client_socket, error_message);
			return;
		}
		channel.setTopic(new_topic);
		std::string confirmation_message = RPL_TOPIC(client.getNick(), channel_name, new_topic);
		sendMessage(client_socket, confirmation_message);
	}
}
