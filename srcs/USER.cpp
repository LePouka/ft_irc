#include "../includes/Server.hpp"
#include "../includes/Util.hpp"

void Server::handleUserCommand(int client_socket, const std::string& user) {
	if (user.empty()) {
		std::string error_message = ERR_NEEDMOREPARAMS("server", "USER");
		send(client_socket, error_message.c_str(), error_message.length(), 0);
		return;
	}
	clients[client_socket].setUser(user);
	std::ostringstream response;
	response << ":server 001 " << clients[client_socket].getNick() << " :User set to " << user << "\r\n";
	send(client_socket, response.str().c_str(), response.str().length(), 0);
}