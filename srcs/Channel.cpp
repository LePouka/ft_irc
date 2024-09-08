/* ************************************************************************** */
/*   INCLUDES                                                                 */
/* ************************************************************************** */
#include "ChannelManager.hpp"

/* ************************************************************************** */
/*   ASSIGNATION OPERATOR                                                     */
/* ************************************************************************** */
ChannelManager&	ChannelManager::operator=( ChannelManager const & rhs ) {

	if ( this != &rhs ) {

		_channels = rhs._channels;
	}

	return *this;
}

/* ************************************************************************** */
/*   ACCESSORS                                                                */
/* ************************************************************************** */
Channel&	ChannelManager::getChannel( std::string const & channel ) {

	return _channels[ channel ];
}

void	setChannelName( std::string const & channelName ) {

	_channels[ channelName ].name = channel.Name;
}

void	setChannelTopic( std::string const & channelName, \
							std::string const & channelTopic ) {

	_channels[channelName].topic = channelTopic;
}

/* ************************************************************************** */
/*   FUNCTIONS                                                                */
/* ************************************************************************** */
void	createChannel( std::string const & name, int const & clientFd ) {

	Channel	channel;

	channel.name = name;

	_channels[ name ] = name;
}

/* ************************************************************************** */
/*   REDIRECTION OPERATOR                                                     */
/* ************************************************************************** */
/*
std::ostream&	operator<<( std::ostream& o, ChannelManager const & rhs ) {

	o << "ChannelManager";

	return o;
}
*/
