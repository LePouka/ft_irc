#include "../../includes/Server.hpp"
#include "../../includes/Util.hpp"

void Server::handleUserCommand(int client_socket, const std::string& user) {
	std::istringstream ss(user);
	std::string username;
	std::getline(ss, username, ' ');
	if (user.empty()) {
		std::string error_message = ERR_NEEDMOREPARAMS("server", "USER");
		sendMessage(client_socket, error_message);
		return;
	}
	clients[client_socket].setUser(username);
	if (clients[client_socket].isRegistered() && !clients[client_socket].isWelcomeSent()) {
		std::string welcome_message = RPL_WELCOME(clients[client_socket].getUser(), clients[client_socket].getNick());
		sendMessage(client_socket, welcome_message);
		clients[client_socket].setWelcomeSent(true);
	}
}
