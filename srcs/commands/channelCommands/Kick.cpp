#include "../../../includes/Server.hpp"

bool	Server::kickTests( ChannelArray& channelArray,
		Client const & client,
		std::string const & channelName,
		std::string const & nickname ) {

	if ( isClientRegistered( nickname ) == false ) {

		sendErrorMessage( client.getSocket(), ERR_NOSUCHNICK( "Server", nickname ));
		return false;
	}

	if ( channelArray.isChan( channelName ) == false ) {

		sendErrorMessage( client.getSocket(), ERR_NOSUCHCHANNEL( "Server", channelName ));
		return false;
	}

	Client	targetedClient = getClient( nickname );
	Channel	channel = channelArray.getChannel( channelName );

	if ( channel.isInUserList( client ) == false ) {

		sendErrorMessage( client.getSocket(), ERR_USERNOTINCHANNEL( "Server", client.getNick() ));
		return false;
	}

	if ( channel.isInUserList( targetedClient ) == false ) {

		sendErrorMessage( client.getSocket(), ERR_USERNOTINCHANNEL( "Server", nickname ));
		return false;
	}

	if ( channel.isInOperatorList( client ) == false ) {

		sendErrorMessage( client.getSocket(), ERR_CHANOPRIVSNEEDED( "Server", channelName ));
		return false;
	}

	if ( channel.isInOperatorList( targetedClient ) == true ) {

		sendErrorMessage( client.getSocket(), "Cannot kick channel operator" );
		return false;
	}

	return true;
}

void	Server::kick( ChannelArray& channelArray, Channel& channel, Client const & client, Client& kickedClient ) {
	(void)channelArray;
	channel.broadcastMessage( client, RPL_KICK( client.getNick(), client.getUser(), channel.getName(), kickedClient.getNick(), "KICK" ));
	channel.removeUser( client );
}

void	Server::handleKickCommand( ChannelArray& channelArray,
		Client const & client,
		std::string const & args ) {

	std::istringstream iss(args);
	std::string channelName, nickname;

	if ( !( iss >> channelName >> nickname ) || channelName.empty() || nickname.empty() ) {

		sendErrorMessage( client.getSocket(), "Invalid arguments");
		return ;
	}

	if ( !kickTests( channelArray, client, channelName, nickname )) { return ; }

	kick( channelArray, channelArray.getChannel( channelName ), client, getClient( nickname ));
}
