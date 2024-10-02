#include "../includes/Server.hpp"

void Server::handleClientDisconnection(int client_socket) {
    // Get the client object
    Client& client = clients[client_socket];

    // Create a set to store channel names that should be deleted after iteration
    std::set<std::string> channelsToDelete;

    // Iterate over all channels in the server
    for (std::map<std::string, Channel>::iterator it = this->channels.getChannelMap().begin(); it != this->channels.getChannelMap().end(); ++it) {
        std::string channel_name = it->first;
        Channel& channel = it->second;

        // Check if the client is part of this channel
        if (channel.isInUserList(client)) {
            // Notify other users in the channel that the client has quit
            std::string quitMessage = ":" + client.getNick() + " QUIT :Client disconnected\r\n";
            channel.broadcastMessage(client, quitMessage);

            // Remove the client from the channel
            channel.removeUser(client);

            // If the channel is empty, add it to the set of channels to delete
            if (channel.getUsers().empty()) {
                channelsToDelete.insert(channel_name);
            }
        }
    }

    // Delete channels that are now empty after the iteration
    for (std::set<std::string>::iterator it = channelsToDelete.begin(); it != channelsToDelete.end(); ++it) {
        this->channels.deleteChan(*it);
    }

    // Clean up the client
    close(client_socket);
    clients.erase(client_socket);
}
