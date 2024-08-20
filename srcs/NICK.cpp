#include "../includes/Server.hpp"
#include "../includes/Util.hpp"

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
			if (send(it->first, notification.c_str(), notification.length(), 0) == -1) {
				std::cerr << "Error notifying client " << it->first << std::endl;
			}
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
	std::string response_str = response.str();
	if (send(client_socket, response_str.c_str(), response_str.length(), 0) == -1) {
		std::cerr << "Error sending nickname change confirmation to client " << client_socket << std::endl;
	}
}
void Server::handleNickCommand(int client_socket, const std::string& new_nick) {
    if (new_nick.empty()) {
        std::string error_message = ERR_NONICKNAMEGIVEN("server");
        sendErrorMessage(client_socket, error_message);
        return;
    }
    if (!isValidNick(new_nick)) {
        std::ostringstream error_message;
        error_message << ERR_ERRONEUSNICKNAME(SERVER_NAME, new_nick);
        send(client_socket, error_message.str().c_str(), error_message.str().length(), 0);
        return;
    }
    if (isNickInUse(new_nick, clients)) {
        std::ostringstream error_message;
        error_message << ERR_NICKNAMEINUSE("server", new_nick);
        send(client_socket, error_message.str().c_str(), error_message.str().length(), 0);
        return;
    }
    std::string old_nick = clients[client_socket].getNick();
    clients[client_socket].setNick(new_nick);
    notifyClients(clients, client_socket, old_nick, new_nick);
    sendNickChangeConfirmation(client_socket, old_nick, new_nick);
    
    if (clients[client_socket].isRegistered() && !clients[client_socket].isWelcomeSent()) {
        std::string welcome_message = RPL_WELCOME(clients[client_socket].getUser(), clients[client_socket].getNick());
        send(client_socket, welcome_message.c_str(), welcome_message.length(), 0);
        clients[client_socket].setWelcomeSent(true);
    }
}

