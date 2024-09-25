#include "../../../includes/Commands.hpp"

bool	kickTests( Server const & server,
		ChannelArray const & channelArray,
		Client const & client,
		std::string const & channelName,
		std::string const & nickname ) {

	if ( !server.isClientRegistered( nickname ) ) {

		server::sendErrorMessage( client, ERR_NOSUCHNICK( SERVER_NAME, nickname ));
		return false;
	}

	if ( !channelArray.isChan( channelName ) ) {

		sendErrorMessage( server::client, ERR_NOSUCHCHANNEL( SERVER_NAME, channelName ));
		return false;
	}

	if ( !channelArray.userInChannel( client, channelName ) ) {

		server::sendErrorMessage( client, ERR_USERNOTINCHANNEL( SERVER_NAME, nickname, channel_name ));
		return false;
	}

	if ( !channelArray.isOperator( client, channelName ) {

		server::sendErrorMessage( client, ERR_CHANOPRIVSNEEDED( SERVER_NAME, channel_name ));
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

		server::sendErrorMessage( client, RPL_KICK( kicker.get_nickname(), kicker.get_username(), channel_name, nickname, "KICK" ));
		return ;
	}

	if ( !kickTests( server, channelArray, client, channelName, nickname )) { return ; }

	kickHandler( server.getClient( nickname ), channelArray.getChannel( channelName ));
}
