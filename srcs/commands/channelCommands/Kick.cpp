#include "../../../includes/Commands.hpp"

bool	kickTests( Server const & server,
		ChannelArray const & channelArray,
		Client const & client,
		std::string const & channelName,
		std::string const & nickname ) {

	if ( !server.isClientRegistered( nickname ) ) {

		server::sendErrorMessage( client, ERR_NOSUCHNICK( "Server", nickname ));
		return false;
	}

	if ( !channelArray.isChan( channelName ) ) {

		sendErrorMessage( server::client, ERR_NOSUCHCHANNEL( "Server", channelName ));
		return false;
	}

	if ( !channelArray.userInChannel( client, channelName ) ) {

		server::sendErrorMessage( client, ERR_USERNOTINCHANNEL( "Server", nickname, channelName ));
		return false;
	}

	if ( !channelArray.isOperator( client, channelName ) {

		server::sendErrorMessage( client, ERR_CHANOPRIVSNEEDED( "Server", channelName ));
		return false;
	}

	Client	targetedClient = getClient( nickname );
	if ( channelArray.isOperator( targetedClient, channelName ) {

		server::sendErrorMessage( client, "Cannot kick channel operator" );
		return false;
	}

	return true;
}

void	kickHandler( client& client, Channel& channel ) {

	channel.removeUser( client );
}

void	kick( Server const & server,
		ChannelArray const & channelArray,
		Client const & client,
		std::string const & args ) {

	std::istringstream iss(args);
	std::string channelName, nickname;

	if ( !( iss >> channelName >> nickname ) || channelName.empty() || nickname.empty() ) {

		server::sendErrorMessage( client, "Invalid arguments");
		return ;
	}

	if ( !kickTests( server, channelArray, client, channelName, nickname )) { return ; }

	kickHandler( server.getClient( nickname ), channelArray.getChannel( channelName ));
}
