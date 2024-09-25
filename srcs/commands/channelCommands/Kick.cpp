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

	if ( !channelArray.userInChannel( client, channelName )) {

		sendErrorMessage( client.getSocket(), ERR_USERNOTINCHANNEL( "Server", nickname ));
		return false;
	}

	if ( !channelArray.isOperator( client, channelName )) {

		sendErrorMessage( client.getSocket(), ERR_CHANOPRIVISNEEDED( "Server", channelName ));
		return false;
	}

	Client	targetedClient = getClient( nickname );
	if ( channelArray.isOperator( targetedClient, channelName )) {

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
