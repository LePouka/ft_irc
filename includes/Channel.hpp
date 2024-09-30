#pragma once

#include <string>
#include <set>
#include <map>
#include <vector>
#include "Client.hpp"
#include "Message.hpp"

class Channel {
private:
	std::string			name;
	std::string			topic;
	std::string			password;
	std::set<Client>	users;
	std::set<Client>	operators;
	std::set<Client>	invited;

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
	std::set<Client>	getInvited();
	bool				getInvite();
	bool				getTopicRestricted();
	bool				getKeyNeeded();
	unsigned int		getUserLimit();

	void				setName(std::string name);
	void				setTopic(std::string topic);
	void				setPassword(std::string password);
	void				addUser(Client client);
	void				addOperators(Client client);
	void				addInvited(Client client);

	void				setInvite(bool invite);
	void				setTopicRestricted(bool restricted);
	void				setKeyNeeded(bool keyNeeded);
	void				setUserLimit(unsigned int userLimit);
	//METHODS
	void				removeUser(const Client& client);
	void				removeOperator(Client client);
	void				removeInvited(Client client);
	void				writeMsgChannel(Client client, std::string const &msg, std::string const &command);

	bool				isInUserList(Client client);
	bool    			isInOperatorList(Client client);
	bool    			isInInviteList(Client client);
	bool 				canSendMessage(const Client &client);


	void				logUserList() const; 
	void				broadcastMessage(Client client, std::string const &msg);

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
	void					eraseChanFromClientChannels( Client const & client, std::string const & chanName );
};
