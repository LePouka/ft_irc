#include "../../../includes/Server.hpp"

void Server::handlePrivmsgCommand(Client client, std::string params, Server &server)
{
	std::istringstream iss(params);
	std::string recipient;
	std::string message;

	if (!(iss >> recipient)) {
		std::string error_message = ERR_NORECIPIENT(client.getNick(), "PRIVMSG");
		sendMessage(client.getSocket(), error_message);
		return;
	}

	std::getline(iss, message);
	if (message.empty()) {
		std::string error_message = ERR_NOTEXTTOSEND(client.getNick());
		sendMessage(client.getSocket(), error_message);
		return;
	}

	message.erase(0, message.find_first_not_of(' '));
	message.append(NEW_LINE);

	if (recipient[0] == '#' || recipient[0] == '&') {
		Channel *channel = NULL;
		try {
			channel = &server.getChannelArray().getChannel(recipient);
		} catch (const std::exception &e) {
			std::string error_message = ERR_NOSUCHCHANNEL(client.getNick(), recipient);
			sendMessage(client.getSocket(), error_message);
			return;
		}

		if (channel == NULL) {
			std::string error_message = ERR_NOSUCHCHANNEL(client.getNick(), recipient);
			sendMessage(client.getSocket(), error_message);
			return;
		}
		
		if (channel->canSendMessage(client) == false) {
			std::string error_message = ERR_CANNOTSENDTOCHAN(client.getNick(), recipient);
			sendMessage(client.getSocket(), error_message);
			return;
		}

		server.getChannelArray().writeMsgChannel(client, recipient, message);
	} else {
		std::map<int, Client> clientMap = server.getClientMap();
		bool userFound = false;
		for (std::map<int, Client>::iterator it = clientMap.begin(); it != clientMap.end(); ++it)
		{
			if ((*it).second.getNick() == recipient)
			{
				std::string send = USER_MSG(client.getNick(), recipient, message);
				sendMessage((*it).first, send);
				userFound = true;
				break;
			}
		}
		if (!userFound) {
			std::string error_message = ERR_NOSUCHNICK(client.getNick(), recipient);
			sendMessage(client.getSocket(), error_message);
		}
	}
}
