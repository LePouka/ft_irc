#include "../../includes/Server.hpp"
#include "../../includes/Util.hpp"

void Server::handleQuitCommand(Client &client, const std::string& quitMessage) {
    int client_socket = client.getSocket();
    std::set<std::string> channels = getChannelArray().getChannelsClient(client);
    std::string quitReason = "Quit: " + (quitMessage.empty() ? "" : quitMessage);
    std::string quitMsg = ":" + client.getNick() + "!" + client.getUser() + " QUIT :" + quitReason + "\r\n";

    // Convert set to vector for iteration
    std::vector<std::string> channelsToProcess(channels.begin(), channels.end());
    
    // Traditional for loop for C++98 compatibility
    for (size_t i = 0; i < channelsToProcess.size(); ++i) {
        const std::string& channelName = channelsToProcess[i];
        
        // Retrieve the channel directly
        Channel& channel = getChannelArray().getChannel(channelName);
        channel.broadcastMessage(client, quitMsg);
        channel.removeUser(client);
    }

    getChannelArray().leaveAll(client);
    std::cout << client.getNick() << " has quit: " << quitReason << std::endl;
    close(client_socket);
    clients.erase(client_socket);
}
