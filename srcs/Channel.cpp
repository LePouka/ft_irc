#include "../includes/Channel.hpp"

//CHANNEL


//CONST/DEST
Channel::Channel()
{}

Channel::Channel(const std::string& name) : name(name)
{}

Channel::~Channel()
{}

//GETTER/SETTER
std::string Channel::getName()
{
    return (this->name);
}

std::string Channel::getTopic()
{
    return (this->topic);
}

std::set<int> Channel::getUsers()
{
    return (this->users);
}

std::set<int> Channel::getOperators()
{
    return (this->operators);
}

void Channel::setName(std::string name)
{
    this->name = name;
}

void Channel::setTopic(std::string topic)
{
    this->topic = topic;
}

void Channel::addUser(int clientSocket)
{
    users.insert(clientSocket);
}

void Channel::addOperators(int clientSocket)
{
    operators.insert(clientSocket);
}

//METHODS
void Channel::removeUser(int clientSocket)
{
    users.erase(clientSocket);
}

void Channel::removeOperator(int clientSocket)
{
    operators.erase(clientSocket);
}

bool Channel::hasUser(int clientSocket)
{
    return users.find(clientSocket) != users.end();
}

//CHANNELARRAY

//CONST/DEST

ChannelArray::ChannelArray()
{}

ChannelArray::~ChannelArray()
{}

//GETTER/SETTER

Channel& ChannelArray::getChannel(std::string const &channel)
{
    return channels.find(channel)->second;
}

std::set<int> ChannelArray::getOperators(std::string const &channel)
{
    return channels.find(channel)->second.getOperators();
}

std::set<std::string> ChannelArray::getChannelsClient(int client_socket)
{
    return clientChannels.find(client_socket)->second;
}

void ChannelArray::setChannelName(std::string const &channel)
{
    channels.find(channel)->second.setName(channel);
}

void ChannelArray::setChannelTopic(std::string const &channel, std::string const &topic)
{
    channels.find(channel)->second.setTopic(topic);
}

//METHODS

void ChannelArray::createChannel(std::string const &channel, int const &clientSocket)
{
    if (channels.find(channel) == channels.end())
    {
        Channel newChannel;

        newChannel.setName(channel);
        newChannel.setTopic("");
        newChannel.addOperators(clientSocket);
        channels.emplace(channel, newChannel);
    }
}

bool ChannelArray::userInChannel(int client_socket, std::string const &channel)
{
    return clientChannels[client_socket].find(channel) != clientChannels[client_socket].end();
}

void ChannelArray::join(int clientSocket, std::string const &channel)
{
    if (clientChannels[clientSocket].find(channel) == clientChannels[clientSocket].end())
    {
        clientChannels[clientSocket].insert(channel);
        channels[channel].addUser(clientSocket);
    }
}

void ChannelArray::leave(int clientSocket, std::string const &channel)
{
    if (clientChannels[clientSocket].find(channel) != clientChannels[clientSocket].end())
    {
        if (getOperators(channel).find(clientSocket) != getOperators(channel).end())
            getChannel(channel).removeOperator(clientSocket);
        clientChannels[clientSocket].erase(channel);
        channels[channel].removeUser(clientSocket);
        if (getChannel(channel).getUsers().empty())
        {
            deleteChan(channel);
        }
    }
}

void ChannelArray::leaveAll(int clientSocket)
{

    for (std::set<std::string>::iterator it = getChannelsClient(clientSocket).begin(); it != getChannelsClient(clientSocket).end(); ++it)
    {
        if (getOperators(*it).find(clientSocket) != getOperators(*it).end())
            getChannel(*it).removeOperator(clientSocket);
        clientChannels[clientSocket].erase(*it);
        channels[*it].removeUser(clientSocket);
        if (getChannel(*it).getUsers().empty())
        {
            deleteChan(*it);
        }
    }
}

void ChannelArray::deleteChan(std::string const &channel)
{
    channels[channel].~Channel();
    channels.erase(channel);
}

bool ChannelArray::isChan(std::string const &channel)
{
    return channels.find(channel) != channels.end();
}

bool ChannelArray::isOperator(int clientSocket, std::string const &channel)
{
    return getChannel(channel).getOperators().find(clientSocket) != getChannel(channel).getOperators().end();
}