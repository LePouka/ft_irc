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
	bool				isInviteOnly;
	bool				isTopicRestrictedToOperators;
	bool				isKeyNeeded;
	//unsigned int		userLimit;


public:

	//CONST/DEST
	Channel(const std::string& name);
	Channel();
	~Channel();

	//GETTER/SETTER 
	std::string			getName();
	std::string			getTopic();
	std::string			getPassword(std::string user);
	std::set<Client>	getUsers();
	std::set<Client>	getOperators();
	bool				getInvite();
	bool				getTopicRestricted();
	bool				getKeyNeeded();

	void				setName(std::string name);
	void				setTopic(std::string topic);
	void				setPassword(std::string password, std::string user);
	void				addUser(Client client);
	void				addOperators(Client client);
	void				setInvite(bool invite, std::string user);
	void				setTopicRestricted(bool restricted, std::string user);
	void				setKeyNeeded(bool keyNeeded, std::string user);

	//METHODS
	void				removeUser(const Client& client);
	void				removeOperator(Client client);
	void				broadcastMessage(const std::string& message, const Client& sender);
	bool				hasUser(Client client);
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
