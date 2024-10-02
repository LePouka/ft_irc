#include "../includes/Server.hpp"

void Server::handleClientDisconnection(int client_socket) {
	Client& client = clients[client_socket];
	std::set<std::string> channelsToDelete;
	for (std::map<std::string, Channel>::iterator it = this->channels.getChannelMap().begin(); it != this->channels.getChannelMap().end(); ++it) {
		std::string channel_name = it->first;
		Channel& channel = it->second;
		if (channel.isInUserList(client)) {
			std::string quitMessage = ":" + client.getNick() + " QUIT :Client disconnected\r\n";
			channel.broadcastMessage(client, quitMessage);
			channel.removeUser(client);
			if (channel.getUsers().empty()) {
				channelsToDelete.insert(channel_name);
			}
		}
	}
	for (std::set<std::string>::iterator it = channelsToDelete.begin(); it != channelsToDelete.end(); ++it) {
		this->channels.deleteChan(*it);
	}
	close(client_socket);
	clients.erase(client_socket);
}
