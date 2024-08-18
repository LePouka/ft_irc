#pragma once

/* ************************************************************************** */
/*   INCLUDES                                                                 */
/* ************************************************************************** */
# include <iostream>
# include <string>
# include <map>
# include <set>
# include <vector>

/* ************************************************************************** */
/*   STRUCTURS                                                                */
/* ************************************************************************** */
struct Channel {

	std::string			name;
	std::string			topic;
	std::set< int >		cliendtsFd;
	bool				isInviteOnly;
	bool				isTopicRestrictedToOperators;
	bool				isKeyNeeded;
	unsigned int		userLimit;

};

/* ************************************************************************** */
/*   CLASSES                                                                  */
/* ************************************************************************** */
class ChannelManager {

private:
	std::map< std::string, Channel >	_channels;

protected:

public:

	// Constructors
	ChannelManager( void ) {}
	ChannelManager( ChannelManager const & src ) : _channels( src._channels ) {}

	// Destructor
	virtual ~ChannelManager( void ) {}

	// Operators
	ChannelManager&	operator=( ChannelManager const & rhs );

	// Accessors
	Channel&	getChannel( std::string const & channel );
	void		setChannelName( std::string const & channelName );
	void		setChannelTopic( std::string const & channelName, \
									std::string const & channelTopic );

};

/* ************************************************************************** */
/*   redirection operator                                                     */
/* ************************************************************************** */
std::ostream&	operator<<( std::ostream& o, channelmanager const & rhs );
