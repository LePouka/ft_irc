#pragma once

#include <string>
#include <set>
#include <map>
#include "Client.hpp"
#include "Message.hpp"

class Channel {
    private:
        std::string			name;
        std::string			topic;
		std::string			password;
        std::set<Client>	users;
        std::set<Client>	operators;
        std::set<Client>    banned;
        std::set<Client>    invited;
        bool				isInviteOnly;
		bool				isTopicRestrictedToOperators;
		bool				isKeyNeeded;
		unsigned int		userLimit;


    public:

        //CONST/DEST
        Channel(const std::string& name);
        Channel();
        ~Channel();

        //GETTER/SETTER 
		std::string			getName();
        std::string			getTopic();
		std::string			getPassword();
        std::set<Client>	getUsers();
        std::set<Client>	getOperators();
        std::set<Client>	getBanned();
        std::set<Client>	getInvited();
		bool				getInvite();
		bool				getTopicRestricted();
		bool				getKeyNeeded();
		unsigned int		getUserLimit();        

		void				setName(const std::string name);
        void				setTopic(const std::string topic);
		void				setPassword(const std::string password);
        void				addUser(Client client);
        void				addOperators(Client client);
        void				addBanned(Client client);
        void				addInvited(Client client);
		void				setInvite(bool invite);
		void				setTopicRestricted(bool restricted);
		void				setKeyNeeded(bool keyNeeded);
        void				setUserLimit(const unsigned int userLimit);

        //METHODS
        void				removeUser(Client client);
        void				removeOperator(Client client);
        void				removeBanned(Client client);
        void				removeInvited(Client client);
        void				broadcastMessage(const std::string& message, const Client& sender);
        bool				isInUserList(Client client);
        bool				isInOperatorList(Client client);
        bool                isInBanList(Client client);
        bool                isInInviteList(Client client);
        //Besoin de faire MODE pour faire cette méthode
        bool                canSendMessage(Client client);
};

class ChannelArray {
    private:
        std::map<std::string, Channel>				channels;
        std::map<Client, std::set<std::string> >	clientChannels;
    public:
        //CONST/DEST
        ChannelArray();
        ~ChannelArray();

        //GETTER/SETTER
        Channel&				getChannel(std::string const &channel);
        std::set<Client>		getOperators(std::string const &channel);
        std::set<std::string>	getChannelsClient(Client client);
        void					setChannelName(std::string const  &channel);
        void					setChannelTopic(std::string const &channel, std::string const &topic);
        
        //METHODS
        void					createChannel(std::string const &channel, Client const &client);
        bool					userInChannel(Client client, std::string const &channel);
        void					join(Client client, std::string const &channel);
        void					leave(Client client, std::string const &channel);
        void					leaveAll(Client client);
        void					deleteChan(std::string const &channel);
        bool					isChan(std::string const &channel);
        bool					isOperator(Client client, std::string const &channel);
        void					writeMsgChannel(Client client, std::string const &channel, std::string const &msg);
};

