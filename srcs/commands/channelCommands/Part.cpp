#include "../../../includes/Server.hpp"
#include "../../../includes/Util.hpp"


// void Server::handlePartCommand(Client &client, const std::string& args) {
// 	int client_socket = client.getSocket();
    
//     if (args.empty()) {
// 		std::string error_message = "Error: Invalid arguments for PART command\r\n";
// 		sendMessage(client_socket, error_message);
// 		return;
// 	}
// 	size_t pos = args.find(' ');
// 	std::string channel_name = (pos != std::string::npos) ? args.substr(0, pos) : args;
// 	if (!channels.isChan(channel_name)) {
// 		std::string error_message = ERR_NOSUCHCHANNEL("server", channel_name);
// 		sendMessage(client_socket, error_message);
// 		return;
// 	}
// 	if (clients.find(client_socket) == clients.end()) {
// 		std::string error_message = "Error: Client not found\r\n";
// 		sendMessage(client_socket, error_message);
// 		return;
// 	}
// 	Client& client = clients[client_socket];
// 	Channel& channel = channels.getChannel(channel_name);
// 	if (!channel.isInUserList(client)) {
// 		std::string error_message = ERR_NOTONCHANNEL("server", channel_name);
// 		sendMessage(client_socket, error_message);
// 		return;
// 	}
// 	std::string part_message = ":" + client.getNick() + " PART " + channel_name + "\r\n";
// 	sendMessage(client_socket, part_message);
// 	channel.removeUser(client);
// 	if (channel.getUsers().empty()) {
// 		channels.deleteChan(channel_name);
// 	}
// }

void Server::handlePartCommand(Client &client, const std::string &channelName)
{
    Channel &channel = getChannelArray().getChannel(channelName);

    if (channel.isInUserList(client)) {
        // Construct the PART message
        std::string partMessage = ":" + client.getNick() + " PART " + channelName + "\r\n";

        // Send PART message to all users in the channel
        channel.broadcastMessage(client, partMessage);  // Send to everyone including the leaving user

        // Remove the user from the channel
        channel.removeUser(client);
        std::cout << "Removed user: " << client.getNick() << " from channel: " << channelName << std::endl;

        // Optionally delete the channel if no users remain
        if (channel.getUsers().empty()) {
            getChannelArray().deleteChan(channelName);
        }
    } else {
        // If the user is not in the channel, send an error message
        std::string error_message = ERR_NOTONCHANNEL("server", channelName);
        sendMessage(client.getSocket(), error_message);
    }
}
