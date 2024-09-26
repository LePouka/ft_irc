#include "../../../includes/Server.hpp"
#include "../../../includes/Util.hpp"
#include "../../../includes/Server.hpp"
#include "../../../includes/Util.hpp"

bool	Server::kickTests( ChannelArray& channelArray,
		Client const & client,
		std::string const & channelName,
		std::string const & nickname ) {

	if ( !isClientRegistered( nickname )) {

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

	if ( channel.isInOperatorList( targetedClient )) {

		sendErrorMessage( client.getSocket(), "Cannot kick channel operator" );
		return false;
	}

	return true;
}

void	Server::kick( Client& client, Channel& channel ) {

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

	kick( getClient( nickname ), channelArray.getChannel( channelName ));
}
