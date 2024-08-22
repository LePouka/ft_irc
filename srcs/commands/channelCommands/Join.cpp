#include "../../../includes/Commands.hpp"

void join(Client client, std::string channel, Server &server)
{
    if (server.getChannelArray().isChan(channel) == true)
    {
        Channel chan = server.getChannelArray().getChannel(channel);
        if (chan.getUsers().find(client.getSocket()) == chan.getUsers().end())
        {
            return ;
        }
        chan.addUser(client.getSocket());
    }
    else
    {
        server.getChannelArray().createChannel(channel, client.getSocket());
    }
}