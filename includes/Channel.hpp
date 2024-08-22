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
        std::set<int> users;
        std::set<int> operators;
    public:

        //CONST/DEST
        Channel(const std::string& name);
        Channel();
        ~Channel();

        //GETTER/SETTER 
       std::string getName();
        std::string getTopic();
        std::set<int> getUsers();
        std::set<int> getOperators();
        void setName(std::string name);
        void setTopic(std::string topic);
        void addUser(int clientSocket);
        void addOperators(int clientSocket);

        //METHODS
        void removeUser(int clientSocket);
        void removeOperator(int clientSocket);
        bool hasUser(int client_socket);
};

class ChannelArray {
    private:
        std::map<std::string, Channel> channels;
        std::map<int, std::set<std::string> > clientChannels;
    public:
        //CONST/DEST
        ChannelArray();
        ~ChannelArray();

        //GETTER/SETTER
        Channel& getChannel(std::string const &channel);
        std::set<int> getOperators(std::string const &channel);
        std::set<std::string> getChannelsClient(int clientSocket);
        void setChannelName(std::string const  &channel);
        void setChannelTopic(std::string const &channel, std::string const &topic);
        
        //METHODS
        void createChannel(std::string const &channel, int const &clientSocket);
        bool userInChannel(int clientSocket, std::string const &channel);
        void join(int clientSocket, std::string const &channel);
        void leave(int clientSocket, std::string const &channel);
        void leaveAll(int clientSocket);
        void deleteChan(std::string const &channel);
        bool isChan(std::string const &channel);
        bool isOperator(int clientSocket, std::string const &channel);
        void writeMsgChannel(int clientSocket, std::string const &channel, std::string const &msg);
};

