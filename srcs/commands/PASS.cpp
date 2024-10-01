#include "../../includes/Server.hpp"

void Server::handlePassCommand(int client_socket, const std::string& password) {
	if (clients.find(client_socket) == clients.end()) {
		std::string error_message = "Error: Client not found\r\n";
		sendMessage(client_socket, error_message);
		return;
	}
	Client& client = clients[client_socket];
	if (client.isClientSet()) {
		std::string error_message = ERR_ALREADYREGISTERED("server");
		sendMessage(client_socket, error_message);
		return;
	}
	if (password.empty()) {
		std::string error_message = ERR_NEEDMOREPARAMS("server", "PASS");
		sendMessage(client_socket, error_message);
		return;
	}
	if (password != server_config_password) {
		std::string error_message = ERR_PASSWDMISMATCH("server", client.getNick());
		sendMessage(client_socket, error_message);
		return;
	}
	client.setPassword(password);
	if (clients[client_socket].isClientSet() && !clients[client_socket].isWelcomeSent()) {
		std::string welcome_message = RPL_WELCOME(clients[client_socket].getUser(), clients[client_socket].getNick());
		sendMessage(client_socket, welcome_message);
		clients[client_socket].setWelcomeSent(true);
	}
}
