#include "../../includes/Server.hpp"
#include "../../includes/Util.hpp"

void Server::handlePassCommand(int client_socket, const std::string& password) {
	if (clients.find(client_socket) == clients.end()) {
		std::string error_message = "Error: Client not found\r\n";
		sendMessage(client_socket, error_message);
		return;
	}
	Client& client = clients[client_socket];
	const std::string &client_nick = client.getNick();
	if (client.isRegistered()) {
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
		std::string error_message = ERR_PASSWDMISMATCH("server", client_nick);
		sendMessage(client_socket, error_message);
		return;
	}
	client.setPassword(password);
	client.setRegistered(true);
	std::cout << "Password set for client " << client_nick << " (" << client_socket << ")" << std::endl;
}
