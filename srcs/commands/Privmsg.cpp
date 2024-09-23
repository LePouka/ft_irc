#include "../../includes/Commands.hpp"

// Numeric Replies:

// ERR_NOSUCHNICK (401)
// ERR_NOSUCHCHANNEL (403)
// ERR_CANNOTSENDTOCHAN (404)
// ERR_NORECIPIENT (411)
// ERR_NOTEXTTOSEND (412)

void privmsg(Client client, std::string params, Server &server)
{
    // (void)client;
    // (void)server;
    std::istringstream iss(params);
    std::string recipient;
    std::string message;

    // std::cout << "ahah\n";
    if (!(iss >> recipient)) {
        sendMessage(client.getSocket(), "Error: No recipient specified.");
        return;
    }
    std::getline(iss, message);
    if (message.empty()) {
        std::cout << "empty msg\n";
        sendMessage(client.getSocket(), "Error: No message specified.");
        return;
    }
    message.erase(0, message.find_first_not_of(' '));
    std::cout << "recipient :" << recipient << " message :" << message << "\n";
    message.append("\r\n");
    if (recipient[0] == '#' || recipient[0] == '&') {
        std::cout << "msg to channel\n";
        Channel *channel = NULL;
        try {
            channel = &server.getChannelArray().getChannel(recipient);
        } catch (const std::exception &e) {
            // Handle case where channel does not exist
            sendMessage(client.getSocket(), "Error: Channel not found.");
            return;
        }

        if (channel == NULL) {
            sendMessage(client.getSocket(), "Error: Channel not found.");
            return;
        }
        
        server.getChannelArray().writeMsgChannel(client, recipient, message);
    } else {
        std::cout << "msg to user\n";
        std::map<int, Client> clientMap = server.getClientMap();
        for (std::map<int, Client>::iterator it = clientMap.begin(); it != clientMap.end(); ++it)
        {
            if ((*it).second.getNick() == recipient)
            {
                sendMessage((*it).first, "Message from " + client.getNick() + ": " + message);
                return;
            }
        }
        sendMessage(client.getSocket(), "Error: User not found.");
    }
    
}