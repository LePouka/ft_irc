#include "../../../includes/Commands.hpp"

void join(Client client, std::string channel, Server &server)
{
    if (server.getChannelArray().isChan(channel) == true)
    {
        Channel &chan = server.getChannelArray().getChannel(channel);
        if (chan.getUsers().find(client.getSocket()) == chan.getUsers().end())
        {
            return ;
        }
        chan.addUser(client.getSocket());
        // std::cout << toString(chan.getUsers()) << "\n";
        // std::cout << *chan.getUsers().begin() << "\n";
        std::set<int> tmp = chan.getUsers();
        for (std::set<int>::iterator it = tmp.begin(); it != tmp.end(); ++it)
        {
            std::cout << *it << " ";
        }
        std::cout << "\n";
    }
    else
    {
        server.getChannelArray().createChannel(channel, client.getSocket());
        Channel &chan = server.getChannelArray().getChannel(channel);
        std::set<int> tmp = chan.getUsers();
        for (std::set<int>::iterator it = tmp.begin(); it != tmp.end(); ++it)
        {
            std::cout << *it << " ";
        }
        std::cout << "\n";
    }
}