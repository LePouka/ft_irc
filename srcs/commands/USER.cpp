#include "../../includes/Server.hpp"

// Helper function to generate a unique username based on nickname
std::string generateUniqueUsernameFromNickname(std::map<int, Client>& clients, const std::string& baseUsername) {
    int counter = 1;
    std::string newUsername = baseUsername;

    // Loop to ensure username is unique
    bool isUnique = false;
    while (!isUnique) {
        isUnique = true;  // Assume it's unique until proven otherwise
        for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
            if (it->second.getUser() == newUsername) {
                isUnique = false;
                break;
            }
        }

        if (!isUnique) {
            // If the username is not unique, append a number and increment the counter
            std::ostringstream ss;
            ss << baseUsername << counter++;
            newUsername = ss.str();
        }
    }
    return newUsername;
}

void Server::handleUserCommand(int client_socket, const std::string& user) {
    if (user.empty()) {
        std::string error_message = ERR_NEEDMOREPARAMS("server", "USER");
        sendMessage(client_socket, error_message);
        return;
    }

    // Parse the USER command parameters
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

    if (serverPasswordRequired && clients[client_socket].getPassword() != serverPassword) {
		std::string error_message = ERR_NOTREGISTERED("Server");
		sendMessage(client_socket, error_message);
		return;
	}

    if (clients[client_socket].isClientSet()) {
        std::string error_message = ERR_ALREADYREGISTERED("server");
        sendMessage(client_socket, error_message);
        return;
    }

    // Use nickname as the base for the username if it exists
    std::string baseUsername = clients[client_socket].getNick().empty() ? username : clients[client_socket].getNick();
    
    // Generate a unique username based on the nickname
    std::string uniqueUsername = generateUniqueUsernameFromNickname(clients, baseUsername);
    
    clients[client_socket].setUser(uniqueUsername);  // Set the generated username
    clients[client_socket].setRealName(realname);

    // Send welcome message if the client is ready
	if (clients[client_socket].isClientSet() && !clients[client_socket].isWelcomeSent()) {
		std::string welcome_message = RPL_WELCOME(clients[client_socket].getNick(), clients[client_socket].getUser());
		sendMessage(client_socket, welcome_message);
		clients[client_socket].setWelcomeSent(true);
	}
}
