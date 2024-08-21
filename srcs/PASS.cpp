#include "../includes/Server.hpp"
#include "../includes/Util.hpp"

void Server::handlePassCommand(int client_socket, const std::string& password) {
	const std::string &client_nick = clients[client_socket].getNick();
	
	if (clients[client_socket].isRegistered()) {
		std::string error_message = ERR_ALREADYREGISTERED("server");
		send(client_socket, error_message.c_str(), error_message.length(), 0);
		return;
	}
	if (password.empty()) {
		std::string error_message = ERR_NEEDMOREPARAMS("server", "PASS");
		send(client_socket, error_message.c_str(), error_message.length(), 0);
		return;
	}
	if (password != server_config_password) {
		std::string error_message = ERR_PASSWDMISMATCH("server", client_nick);
		send(client_socket, error_message.c_str(), error_message.length(), 0);
		return;
	}
	clients[client_socket].setPassword(password);
	std::cout << "Password set for client " << client_socket << std::endl;
}

