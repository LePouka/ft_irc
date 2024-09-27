#include "../../includes/Server.hpp"
#include "../../includes/Util.hpp"

void Server::handleWhoisCommand(int client_socket, const std::string& target_nickname) {
	std::string nickname = target_nickname;
	nickname.erase(nickname.find_last_not_of(" \n\r") + 1);
	
	if (!isClientRegistered(nickname)) {
		std::string error_message = ERR_NOSUCHNICK("server", nickname);
		sendErrorMessage(client_socket, error_message);
		return;
	}
	Client& target_client = getClient(nickname);
	std::string username = target_client.getUser();
	std::string host = target_client.getHost();
	std::string real_name = target_client.getRealName();
	std::ostringstream response;
	response << RPL_WHOISUSER("server", nickname, username, host, real_name);
	response << RPL_WHOISSERVER("server", nickname, SERVER_NAME, "IRC Server running version 1.0");
	response << RPL_ENDOFWHOIS("server", nickname);
	sendMessage(client_socket, response.str());
}

