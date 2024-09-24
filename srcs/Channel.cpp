#include "../includes/Channel.hpp"

//                          CHANNEL


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

std::string Channel::getPassword(std::string user)
{
    if (this->getKeyNeeded() == false)
    {
        return (NULL);
    }
    std::set<Client> tmp = getOperators();
    for(std::set<Client>::iterator it = tmp.begin(); it != tmp.end(); ++it)
    {
        if ((*it).getUser() == user)
        {
            return this->password;
        }
    }
    return (NULL);
}

std::set<Client> Channel::getUsers()
{
    return (this->users);
}

std::set<Client> Channel::getOperators()
{
    return (this->operators);
}

bool Channel::getInvite()
{
    return (this->isInviteOnly);
}

bool Channel::getTopicRestricted()
{
    return (this->isTopicRestrictedToOperators);
}

bool Channel::getKeyNeeded()
{
    return(this->isKeyNeeded);
}

void Channel::setName(std::string name)
{
    this->name = name;
}

void Channel::setTopic(std::string topic)
{
    this->topic = topic;
}

void Channel::setPassword(std::string password, std::string user)
{
    if (this->getKeyNeeded() == true)
    {
        std::set<Client> tmp = getOperators();
        for(std::set<Client>::iterator it = tmp.begin(); it != tmp.end(); ++it)
        {
            if ((*it).getUser() == user)
            {
                this->password = password;
            }
        }
    }
    return ;
}

void Channel::addUser(Client client)
{
    users.insert(client);
}

void Channel::addOperators(Client client)
{
    operators.insert(client);
}

void Channel::setInvite(bool invite, std::string user)
{
    if (this->getKeyNeeded() == true)
    {
        std::set<Client> tmp = getOperators();
        for(std::set<Client>::iterator it = tmp.begin(); it != tmp.end(); ++it)
        {
            if ((*it).getUser() == user)
            {
                this->isInviteOnly = invite;
            }
        }
    }
    return ;
}

void Channel::setTopicRestricted(bool restricted, std::string user)
{
    if (this->getKeyNeeded() == true)
    {
        std::set<Client> tmp = getOperators();
        for(std::set<Client>::iterator it = tmp.begin(); it != tmp.end(); ++it)
        {
            if ((*it).getUser() == user)
            {
                this->isTopicRestrictedToOperators = restricted;
            }
        }
    }
    return ;
}

void Channel::setKeyNeeded(bool keyNeeded, std::string user)
{
    if (this->getKeyNeeded() == true)
    {
        std::set<Client> tmp = getOperators();
        for(std::set<Client>::iterator it = tmp.begin(); it != tmp.end(); ++it)
        {
            if ((*it).getUser() == user)
            {
                this->isKeyNeeded = keyNeeded;
            }
        }
    }
    return ;
}

//METHODS
void Channel::removeUser(const Client& client){
    users.erase(client);
}

void Channel::removeOperator(Client client)
{
    operators.erase(client);
}

bool Channel::hasUser(Client client)
{
    return users.find(client) != users.end();
}

bool    Channel::isInOperatorList(Client client)
{
    return operators.find(client) != operators.end();
}

bool    Channel::isInBanList(Client client)
{
    return banned.find(client) != banned.end();
}

bool    Channel::isInInviteList(Client client)
{
    return invited.find(client) != invited.end();
}

bool Channel::canSendMessage(const Client &client) {
    // Check if the channel is invite-only
    if (this->getInvite() && this->getOperators().find(client) != this->getOperators().end() && this->getUsers().find(client) != this->getUsers().end()) {
        return false; // Cannot send messages if not invited
    }
    
    // Check if the user is banned from the channel
    if (this->getBanned().find(client) != this->getBanned().end()) {
        return false; // Cannot send messages if banned
    }

    // Check if the channel is restricted to certain users (e.g., operators)
    // (This part is optional, depending on your requirements)
    if (this->getTopicRestricted() && this->getOperators().find(client) != this->getOperators().end()) {
        return false; // If topic is restricted, only operators can send messages
    }

    return true; // Client is allowed to send messages to the channel
}

void Channel::broadcastMessage(const std::string& message, const Client& sender) 
{
    (void)message;
    (void)sender;
    // int senderSocket = sender.getSocket();
    // for (std::set<Client>::iterator it = users.begin(); it != users.end(); ++it) {
    //     if (*it != sender) {
    //         // Assuming you have a way to get a Client object from a socket
    //         Client recipient = getClientFromSocket(*it);
    //         recipient.sendMessage(message);
    //     }
    // }
}

//                          CHANNELARRAY

//CONST/DEST

ChannelArray::ChannelArray()
{}

ChannelArray::~ChannelArray()
{}

//GETTER/SETTER

Channel& ChannelArray::getChannel(std::string const &channel)
{
    return channels[channel];
    // return channels.find(channel)->second;
}

std::set<Client> ChannelArray::getOperators(std::string const &channel)
{
    return channels.find(channel)->second.getOperators();
}

std::set<std::string> ChannelArray::getChannelsClient(Client client)
{
    return clientChannels.find(client)->second;
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

void ChannelArray::createChannel(std::string const &channel, Client const &client)
{
    std::cout << channel << " was created\n";
    if (!isChan(channel))
    {
        Channel newChannel;

        newChannel.setName(channel);
        newChannel.setTopic("");
        newChannel.addUser(client);
        newChannel.addOperators(client);
        channels.insert(std::make_pair(channel, newChannel));
    }
}

bool ChannelArray::userInChannel(Client client, std::string const &channel)
{
    return clientChannels[client].find(channel) != clientChannels[client].end();
}

void ChannelArray::join(Client client, std::string const &channel)
{
    if (clientChannels[client].find(channel) == clientChannels[client].end())
    {
        clientChannels[client].insert(channel);
        channels[channel].addUser(client);
    }
}

void ChannelArray::leave(Client client, std::string const &channel)
{
    if (!isChan(channel)) {
        // Handle the case where the channel does not exist
        return;
    }

    if (clientChannels[client].find(channel) != clientChannels[client].end())
    {
        if (getOperators(channel).find(client) != getOperators(channel).end())
            getChannel(channel).removeOperator(client);
        clientChannels[client].erase(channel);
        channels[channel].removeUser(client);
        if (getChannel(channel).getUsers().empty())
        {
            deleteChan(channel);
        }
    }
}

void ChannelArray::leaveAll(Client client)
{
    std::set<std::string> clientChannelsSet = getChannelsClient(client);

    for (std::set<std::string>::iterator it = clientChannelsSet.begin(); it != clientChannelsSet.end(); ++it)
    {
        if (getOperators(*it).find(client) != getOperators(*it).end())
            getChannel(*it).removeOperator(client);
        clientChannels[client].erase(*it);
        channels[*it].removeUser(client);
        if (getChannel(*it).getUsers().empty())
        {
            deleteChan(*it);
        }
    }
}

void ChannelArray::deleteChan(std::string const &channel)
{
    if (!isChan(channel)) {
        // Handle the case where the channel does not exist
        return;
    }

    channels[channel].~Channel();
    channels.erase(channel);
}

bool ChannelArray::isChan(std::string const &channel)
{
    return channels.find(channel) != channels.end();
}

bool ChannelArray::isOperator(Client client, std::string const &channel)
{
    if (!isChan(channel)) {
        // Handle the case where the channel does not exist
        return(false);
    }

    return getChannel(channel).getOperators().find(client) != getChannel(channel).getOperators().end();
}

void ChannelArray::writeMsgChannel(Client client, std::string const &channel, std::string const &msg)
{
    if (!isChan(channel)) {
        // Handle the case where the channel does not exist
        return;
    }

    // Get all users in the channel
    std::set<Client> users = getChannel(channel).getUsers();
    
    // Broadcast message to all users except the sender
    for (std::set<Client>::iterator it = users.begin(); it != users.end(); ++it) {
        if (*it != client) {
            // Include sender's nickname in the message
            sendMessage((*it).getSocket(), ":" + client.getNick() + " PRIVMSG " + channel + " :" + msg);
        }
    }
}

