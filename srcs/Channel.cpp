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
Channel&	ChannelManager::getChannel( std::string const & channel ) {}

void		setChannelName( std::string const & channelName ) {}

void		setChannelTopic( std::string const & channelName, \
								std::string const & channelTopic ) {}

/* ************************************************************************** */
/*   FUNCTIONS                                                                */
/* ************************************************************************** */

/* ************************************************************************** */
/*   REDIRECTION OPERATOR                                                     */
/* ************************************************************************** */
std::ostream&	operator<<( std::ostream& o, ChannelManager const & rhs ) {

	o << "ChannelManager";

	return o;
}
