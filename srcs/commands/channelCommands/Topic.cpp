#include "../../../includes/Server.hpp"
#include "../../../includes/Util.hpp"

void Server::handleTopicCommand(int client_socket, const std::string& args) {
	size_t pos = args.find(' ');
	std::string channel_name = args.substr(0, pos);
	std::string new_topic = (pos != std::string::npos) ? args.substr(pos + 1) : "";

	Channel& channel = channels.getChannel(channel_name);

	if (!channels.isChan(channel_name)) {
		std::string error_message = ERR_NOSUCHCHANNEL("server", channel_name);
		sendMessage(client_socket, error_message);
		return;
	}
	if (!channel.isInUserList(clients[client_socket])) {
		std::string error_message = ERR_NOTONCHANNEL("server", channel_name);
		sendMessage(client_socket, error_message);
		return;
	}
	if (new_topic.empty()) {
		std::string response = channel.getTopic().empty()
			? RPL_NOTOPIC(clients[client_socket].getNick(), channel_name)
			: RPL_TOPIC(clients[client_socket].getNick(), channel_name, channel.getTopic());
		sendMessage(client_socket, response);
	} else {
		if (!channels.isOperator(clients[client_socket], channel_name)) {
			std::string error_message = ERR_CHANOPRIVISNEEDED(clients[client_socket].getNick(), channel_name);
			sendMessage(client_socket, error_message);
			return;
		}
		channel.setTopic(new_topic);
		std::string confirmation_message = RPL_TOPIC(clients[client_socket].getNick(), channel_name, new_topic);
		sendMessage(client_socket, confirmation_message);
	}
}
