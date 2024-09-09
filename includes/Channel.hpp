#pragma once

#include <string>
#include <set>
#include <map>
#include "Client.hpp"
#include "Message.hpp"

class Channel {
    private:
        std::string name;
        std::string topic;
        std::set<Client> users;
        std::set<Client> operators;
    public:

        //CONST/DEST
        Channel(const std::string& name);
        Channel();
        ~Channel();

        //GETTER/SETTER 
       std::string getName();
        std::string getTopic();
        std::set<Client> getUsers();
        std::set<Client> getOperators();
        void setName(std::string name);
        void setTopic(std::string topic);
        void addUser(Client client);
        void addOperators(Client client);

        //METHODS
        void removeUser(Client client);
        void removeOperator(Client client);
        void broadcastMessage(const std::string& message, const Client& sender);
        bool hasUser(Client client);
};

class ChannelArray {
    private:
        std::map<std::string, Channel> channels;
        std::map<Client, std::set<std::string> > clientChannels;
    public:
        //CONST/DEST
        ChannelArray();
        ~ChannelArray();

        //GETTER/SETTER
        Channel& getChannel(std::string const &channel);
        std::set<Client> getOperators(std::string const &channel);
        std::set<std::string> getChannelsClient(Client client);
        void setChannelName(std::string const  &channel);
        void setChannelTopic(std::string const &channel, std::string const &topic);
        
        //METHODS
        void createChannel(std::string const &channel, Client const &client);
        bool userInChannel(Client client, std::string const &channel);
        void join(Client client, std::string const &channel);
        void leave(Client client, std::string const &channel);
        void leaveAll(Client client);
        void deleteChan(std::string const &channel);
        bool isChan(std::string const &channel);
        bool isOperator(Client client, std::string const &channel);
        void writeMsgChannel(Client client, std::string const &channel, std::string const &msg);
};

