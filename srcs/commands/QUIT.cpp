#include "../../includes/Server.hpp"
#include "../../includes/Util.hpp"

void Server::handleQuitCommand(Client &client, const std::string& quitMessage) {
	int client_socket = client.getSocket();
	std::set<std::string> channels = getChannelArray().getChannelsClient(client);
	std::string quitReason = "Quit: " + (quitMessage.empty() ? "" : quitMessage);
	std::string quitMsg = ":" + client.getNick() + "!" + client.getUser() + " QUIT :" + quitReason + "\r\n";
	for (std::set<std::string>::iterator it = channels.begin(); it != channels.end(); ++it) {
		Channel &channel = getChannelArray().getChannel(*it);
		channel.broadcastMessage(client, quitMsg);
		channel.removeUser(client);
	}
	getChannelArray().leaveAll(client);
	std::cout << client.getNick() << " has quit: " << quitReason << std::endl;
	close(client_socket);
	clients.erase(client_socket);
}
