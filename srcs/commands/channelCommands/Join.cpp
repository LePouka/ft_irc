#include "../../../includes/Commands.hpp"

void join(Client client, std::string channel, Server &server)
{
    std::cout << client.getNick() << "\n";
    if (server.getChannelArray().isChan(channel) == true)
    {
        std::cout << "ahah\n";
        Channel &chan = server.getChannelArray().getChannel(channel);
        if (chan.getUsers().find(client.getSocket()) == chan.getUsers().end())
        {
            return ;
        }
        chan.addUser(client.getSocket());
        // std::cout << toString(chan.getUsers()) << "\n";
        // std::cout << *chan.getUsers().begin() << "\n";
        std::set<Client> tmp = chan.getUsers();
        for (std::set<Client>::iterator it = tmp.begin(); it != tmp.end(); ++it)
        {
            std::cout << (*it).getSocket() << " ";
        }
        std::cout << "\n";
    }
    else
    {
        server.getChannelArray().createChannel(channel, client.getSocket());
        Channel &chan = server.getChannelArray().getChannel(channel);
        std::set<Client> tmp = chan.getUsers();
        for (std::set<Client>::iterator it = tmp.begin(); it != tmp.end(); ++it)
        {
            std::cout << (*it).getSocket() << " ";
        }
        std::cout << "\n";
    }
}