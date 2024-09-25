#include "../../includes/Server.hpp"
#include "../../includes/Util.hpp"

void Server::seeModeChannels(Client &client, const std::string &chanName) {
    Channel *channel = NULL;

	std::cout << chanName << "\n";
    // Try to retrieve the channel by name
    try {
        channel = &this->getChannelArray().getChannel(chanName);
    } catch (const std::exception &e) {
        // If the channel doesn't exist, send an error message
		std::cout << "ahah\r\n";
        sendMessage(client.getSocket(), ERR_NOSUCHCHANNEL(client.getNick(), chanName));
        return;
    }

	std::string modes = "+";
    if (channel->getInvite()) {
        modes += "i"; // Invite-only mode
    }
	std::cout << client.getNick() << "\n";
    if (channel->getPassword() != "") {
        modes += "k"; // Keyed mode
    }
    if (channel->getTopicRestricted()) {
        modes += "t"; // Topic-restricted mode
    }
    if (channel->getUserLimit() > 0) {
        modes += "l"; // Limited user count mode
    }

	std::cout << channel->getName() << "\n";

    // Safeguard for null or empty strings when sending the message
    std::string clientNick = client.getNick().empty() ? "unknown" : client.getNick();
    std::string channelStr = channel->getName().empty() ? "unknown" : channel->getName();

    // Send the mode string to the client
    std::string response = RPL_CHANNELMODEIS(client.getNick(), channel->getName(), modes);
    sendMessage(client.getSocket(), response);

    // If +k (keyed mode) is active, also send the key to the client if they have the right to see it
    if (!channel->getPassword().empty() && channel->isInOperatorList(client)) {
        sendMessage(client.getSocket(), "Key: " + channel->getPassword());
    }

    // If +l (limit mode) is active, send the user limit
    if (channel->getUserLimit() > 0) {
		sendMessage(client.getSocket(), "User limit: " + to_string(channel->getUserLimit()));
    }
}