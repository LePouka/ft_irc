#include "../../../includes/Commands.hpp"

bool	kickTests( ChannelArray const & channelArray,
		Client const & client,
		std::string const & channelName,
		std::string const & nickname ) {

	if ( !isClientRegistered( nickname ) ) {

		sendErrorMessage( client, ERR_NOSUCHNICK( "Server", nickname ));
		return false;
	}

	if ( !channelArray.isChan( channelName ) ) {

		sendErrorMessage( client, ERR_NOSUCHCHANNEL( "Server", channelName ));
		return false;
	}

	if ( !channelArray.userInChannel( client, channelName ) ) {

		sendErrorMessage( client, ERR_USERNOTINCHANNEL( "Server", nickname, channelName ));
		return false;
	}

	if ( !channelArray.isOperator( client, channelName ) {

		sendErrorMessage( client, ERR_CHANOPRIVSNEEDED( "Server", channelName ));
		return false;
	}

	Client	targetedClient = getClient( nickname );
	if ( channelArray.isOperator( targetedClient, channelName ) {

		sendErrorMessage( client, "Cannot kick channel operator" );
		return false;
	}

	return true;
}

void	kick( client& client, Channel& channel ) {

	channel.removeUser( client );
}

void	handleKickCommand( ChannelArray const & channelArray,
		Client const & client,
		std::string const & args ) {

	std::istringstream iss(args);
	std::string channelName, nickname;

	if ( !( iss >> channelName >> nickname ) || channelName.empty() || nickname.empty() ) {

		sendErrorMessage( client, "Invalid arguments");
		return ;
	}

	if ( !kickTests( channelArray, client, channelName, nickname )) { return ; }

	kickHandler( getClient( nickname ), channelArray.getChannel( channelName ));
}
