#include "../../../includes/Server.hpp"
#include "../../../includes/Util.hpp"

void Server::handleTopicCommand(int client_socket, const std::string& args) {
    size_t pos = args.find(' ');
    std::string channel_name = args.substr(0, pos);
    std::string new_topic = (pos != std::string::npos) ? args.substr(pos + 1) : "";

    Channel& channel = channels.getChannel(channel_name);

    if (!channels.isChan(channel_name)) {
        std::string error_message = ERR_NOSUCHCHANNEL("server", channel_name);
        send(client_socket, error_message.c_str(), error_message.length(), 0);
        return;
    }
    if (!channel.hasUser(clients[client_socket])) {
        std::string error_message = ERR_NOTONCHANNEL("server", channel_name);
        send(client_socket, error_message.c_str(), error_message.length(), 0);
        return;
    }
    if (new_topic.empty()) {
        std::string response = channel.getTopic().empty()
            ? RPL_NOTOPIC(clients[client_socket].getNick(), channel_name)
            : RPL_TOPIC(clients[client_socket].getNick(), channel_name, channel.getTopic());
        send(client_socket, response.c_str(), response.length(), 0);
    } else {
        if (!channels.isOperator(clients[client_socket], channel_name)) {
            std::string error_message = ERR_CHANOPRIVSNEEDED(clients[client_socket].getNick(), channel_name);
            send(client_socket, error_message.c_str(), error_message.length(), 0);
            return;
        }
        channel.setTopic(new_topic);
        std::string broadcast_message = ":" + clients[client_socket].getNick() + " TOPIC " + channel_name + " :" + new_topic + "\r\n";
        channel.broadcastMessage(broadcast_message, clients[client_socket]);
    }
}
