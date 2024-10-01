#include "../../includes/Server.hpp"

void Server::handleUserCommand(int client_socket, const std::string& user) {
    if (user.empty()) {
        std::string error_message = ERR_NEEDMOREPARAMS("server", "USER");
        sendMessage(client_socket, error_message);
        return;
    }
    std::istringstream ss(user);
    std::string username, zero, asterisk, realname;
    std::getline(ss, username, ' ');
    std::getline(ss, zero, ' ');
    std::getline(ss, asterisk, ' ');
    std::getline(ss, realname);
    if (!realname.empty() && realname[0] == ':') {
        realname = realname.substr(1);
    }
    if (username.empty()) {
        std::string error_message = ERR_NEEDMOREPARAMS("server", "USER");
        sendMessage(client_socket, error_message);
        return;
    }
    if (clients[client_socket].isRegistered()) {
        std::string error_message = ERR_ALREADYREGISTERED("server");
        sendMessage(client_socket, error_message);
        return;
    }
    clients[client_socket].setUser(username);
    clients[client_socket].setRealName(realname);
	if (clients[client_socket].isClientSet() && !clients[client_socket].isWelcomeSent()) {
		std::string welcome_message = RPL_WELCOME(clients[client_socket].getNick(), clients[client_socket].getUser());
		sendMessage(client_socket, welcome_message);
		clients[client_socket].setWelcomeSent(true);
	}
}
