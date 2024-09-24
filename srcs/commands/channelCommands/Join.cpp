#include "../../../includes/Commands.hpp"

//Numeric Replies:

// ERR_NEEDMOREPARAMS (461)
// ERR_BADCHANNELKEY (475)
// ERR_BANNEDFROMCHAN (474)
// ERR_CHANNELISFULL (471)
// ERR_INVITEONLYCHAN (473)
// RPL_TOPIC (332)
// RPL_TOPICWHOTIME (333)
// RPL_NAMREPLY (353)
// RPL_ENDOFNAMES (366)


void join(Client client, std::string channel, Server &server)
{
    if (params.empty())
    {
        std::string error_message = ERR_NEEDMOREPARAMS("Server", "JOIN");
        sendErrorMessage(client.getSocket(), error_message);
        return ;
    }

    ChannelArray &channelArray = server.getChannelArray();
    
    std::istringstream iss(params);
    std::string channelPasswordPair;
    

    while (iss >> channelPasswordPair) 
    {
        std::string channel;
        std::string key;
        size_t commaPos = channelPasswordPair.find(',');

        if (commaPos != std::string::npos) {
            channel = channelPasswordPair.substr(0, commaPos);
            key = channelPasswordPair.substr(commaPos + 1);
        } else {
            channel = channelPasswordPair;
        }
        if (channelArray.isChan(channel)) {
            Channel &chan = channelArray.getChannel(channel);

            if (chan.getKeyNeeded()) {
                if (key.empty() || key != chan.getPassword()) {
                    std::string error_message = ERR_BADCHANNELKEY("Server", channel);
                    sendErrorMessage(client.getSocket(), error_message);
                    continue;
                }
            }
            else if (chan.isInBanList(client)) {
                std::string error_message = ERR_BANNEDFROMCHAN("Server", channel);
                sendErrorMessage(client.getSocket(), error_message);
                continue;
            }
            else if (chan.getUserLimit() > 0 && chan.getUsers().size() == chan.getUserLimit()) {
                std::string error_message = ERR_CHANNELISFULL("Server", channel);
                sendErrorMessage(client.getSocket(), error_message);
                continue;
            }
            else if (chan.getInvite() && chan.isInInviteList(client) == false) {
                std::string error_message = ERR_INVITEONLYCHAN("Server", channel);
                sendErrorMessage(client.getSocket(), error_message);
                continue;
            }
            if (!chan.isInUserList(client)) {
                chan.addUser(client);
                channelArray.join(client, channel);
            }
            std::string joinMessage = JOIN_CHAN(client.getNick(), channel);
            //Peut-etre besoin de changer la facon d'envoyer des messages a un seul chan
            channelArray.writeMsgChannel(client, chan.getName(), joinMessage);
            if (!chan.getTopic().empty()) {
                std::string topicMessage = RPL_TOPIC("Server", channel, chan.getTopic());
                sendMessage(client.getSocket(), topicMessage);
                //jsp comment tu récuperes le Nick de l'User qui a set le topic Noé
                // std::string topicWhoTimeMessage = RPL_TOPICWHOTIME(chan.getTopicName(), channel, client.getNick(), chan.getTopicSetTime());
                // sendMessage(client.getSocket(), topicWhoTimeMessage);
            }
            std::set<Client> users = chan.getUsers();
            std::string userList = "=" + channel + " :";
            for (std::set<Client>::iterator it = users.begin(); it != users.end(); ++it) {
                userList += (*it).getNick() + " ";
            }
            sendMessage(client.getSocket(), RPL_NAMREPLY("Server", userList));

            sendMessage(client.getSocket(), RPL_ENDOFNAMES("Server", channel));
        }
        else {
            channelArray.createChannel(channel, client);
            // Channel &chan = channelArray.getChannel(params);
            // std::set<Client> users = chan.getUsers();
            // std::cout << "Users in newly created channel " << params << ": ";
            // for (std::set<Client>::iterator it = users.begin(); it != users.end(); ++it)
            // {
            //     std::cout << it->getSocket() << " ";
            // }
            // std::cout << "\n";
        }
    }
}