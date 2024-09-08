#include "../../includes/Commands.hpp"

void privmsg(Client client, std::string params, Server &server)
{
    (void)client;
    (void)server;
    std::istringstream iss(params);
    std::string recipient;
    std::string message;

    // // Extract recipient and message from params
    // if (!(iss >> recipient)) {
    //     // Handle error: no recipient specified
    //     sendMessage(client.getSocket(), "Error: No recipient specified.");
    //     return;
    // }

    // std::getline(iss, message);
    // if (message.empty()) {
    //     // Handle error: no message specified
    //     sendMessage(client.getSocket(), "Error: No message specified.");
    //     return;
    // }

    // // Trim leading spaces from the message
    // message.erase(0, message.find_first_not_of(' '));

    // // Check if the recipient is a channel or a user
    // if (recipient[0] == '#' || recipient[0] == '&') {
    //     // Recipient is a channel
    //     Channel *channel = &server.getChannelArray().getChannel(recipient);
    //     if (!channel) {
    //         // Handle error: channel not found
    //         sendMessage(client.getSocket(), "Error: Channel not found.");
    //         return;
    //     }

    //     // Send the message to all users in the channel
    //     channel->broadcastMessage("Message from " + client.getUser() + ": " + message, client);
    // } else {
    //     // Recipient is a user
    //     Client *recipientClient = server.findClientByName(recipient);
    //     if (!recipientClient) {
    //         // Handle error: recipient not found
    //         sendMessage(client.getSocket() ,"Error: Recipient not found.");
    //         return;
    //     }

    //     // Send the message to the recipient
    //     recipientClient->sendMessage("Message from " + client.getNick() + ": " + message);
    // }
}