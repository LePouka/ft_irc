#include "../../../includes/Commands.hpp"

void join(Client client, std::string channel, Server &server)
{
    ChannelArray &channelArray = server.getChannelArray();
    
    if (channelArray.isChan(channel))
    {
        Channel &chan = channelArray.getChannel(channel);
    
        if (!chan.hasUser(client))
        {
            chan.addUser(client);
            channelArray.join(client, channel);
        }
        
        std::set<Client> users = chan.getUsers();
        std::cout << "Users in channel " << channel << ": ";
        for (std::set<Client>::iterator it = users.begin(); it != users.end(); ++it)
        {
            std::cout << it->getSocket() << " ";
        }
        std::cout << "\n";
    }
    else
    {
        channelArray.createChannel(channel, client);
        Channel &chan = channelArray.getChannel(channel);
        std::set<Client> users = chan.getUsers();
        std::cout << "Users in newly created channel " << channel << ": ";
        for (std::set<Client>::iterator it = users.begin(); it != users.end(); ++it)
        {
            std::cout << it->getSocket() << " ";
        }
        std::cout << "\n";
    }
}

