#include "../../includes/Server.hpp"

bool have_channel_prefix(const std::string& nickname) {
	if (!nickname.empty() && 
		(nickname[0] == '#' || 
		 nickname[0] == '&' || 
		 nickname[0] == '~' || 
		 nickname[0] == '@' || 
		 nickname[0] == '%' || 
		 nickname[0] == ':'))
	{
		return true;
	}
	if (nickname.size() > 1 && 
		nickname[0] == '+' &&
		(nickname[1] == 'q' || 
		 nickname[1] == 'a' || 
		 nickname[1] == 'o' || 
		 nickname[1] == 'h' || 
		 nickname[1] == 'v'))
	{
		return true;
	}

	return false;
}

bool isValidNick(const std::string& nickname) {
	std::string invalid_chars = " ,*?!@.$:";
	return !have_channel_prefix(nickname) && 
		   nickname.find_first_of(invalid_chars) == std::string::npos;
}

void notifyClients(const std::map<int, Client>& clients, int sender_socket, const std::string& old_nick, const std::string& new_nick) {
    std::ostringstream notification_message;
    notification_message << ":" << old_nick << " NICK " << new_nick << "\r\n";
    std::string notification = notification_message.str();
    std::map<int, Client>::const_iterator it;
    for (it = clients.begin(); it != clients.end(); ++it) {
        if (it->first != sender_socket) {
            sendMessage(it->first, notification);
        }
    }
}


bool isNickInUse(const std::string& new_nick, const std::map<int, Client>& clients) {
	std::map<int, Client>::const_iterator it;
	for (it = clients.begin(); it != clients.end(); ++it) {
		if (it->second.getNick() == new_nick) {
			return true;
		}
	}
	return false;
}

void sendNickChangeConfirmation(int client_socket, const std::string& old_nick, const std::string& new_nick) {
    std::ostringstream response;
    response << ":" << old_nick << " NICK " << new_nick << "\r\n";
    sendMessage(client_socket, response.str());
}

void Server::handleNickCommand(int client_socket, const std::string& new_nick) {
	if (new_nick.empty()) {
		std::string error_message = ERR_NONICKNAMEGIVEN("Server");
		sendErrorMessage(client_socket, error_message);
		return;
	}
	if (serverPasswordRequired && clients[client_socket].getPassword() != serverPassword) {
		std::string error_message = ERR_NOTREGISTERED("Server");
		sendMessage(client_socket, error_message);
		return;
	}
	if (!isValidNick(new_nick)) {
		std::ostringstream error_message;
		error_message << ERR_ERRONEUSNICKNAME(SERVER_NAME, new_nick);
		sendMessage(client_socket, error_message.str());
		return;
	}
	if (isNickInUse(new_nick, clients)) {
		std::ostringstream error_message;
		error_message << ERR_NICKNAMEINUSE("Server", new_nick);
		sendMessage(client_socket, error_message.str());
		return;
	}
	std::string old_nick = clients[client_socket].getNick();
	if (old_nick != new_nick && isNickInUse(new_nick, clients)) {
		std::ostringstream error_message;
		error_message << ERR_NICKCOLLISION("Server", new_nick);
		sendMessage(client_socket, error_message.str());
		return;
	}
	clients[client_socket].setNick(new_nick);
	notifyClients(clients, client_socket, old_nick, new_nick);
	sendNickChangeConfirmation(client_socket, old_nick, new_nick);
	if (clients[client_socket].isClientSet() && !clients[client_socket].isWelcomeSent()) {
		std::string welcome_message = RPL_WELCOME(clients[client_socket].getUser(), clients[client_socket].getNick());
		sendMessage(client_socket, welcome_message);
		clients[client_socket].setWelcomeSent(true);
	}
}
