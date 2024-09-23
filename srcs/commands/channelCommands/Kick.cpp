#include "../../../includes/Commands.hpp"

bool	kickTests( Server const & server,
		ChannelArray const & channelArray,
		Client const & client,
		std::string const & channelName,
		std::string const & nickname ) {

	if ( !server.isClientRegistered( nickname ) ) {

		// client.write(ERR_NOSUCHNICK(SERVER_NAME, nickname));
		std::cout << "Kick error message" << std::endl;
		return false;
	}

	if ( !channelArray.isChan( channelName ) ) {

		// client.write(ERR_NOSUCHCHANNEL(SERVER_NAME, channel_name));
		std::cout << "Kick error message" << std::endl;
		return false;
	}

	if ( !channelArray.userInChannel( client, channelName ) ) {

		// client.write(ERR_USERNOTINCHANNEL(SERVER_NAME, nickname, channel_name));
		std::cout << "Kick error message" << std::endl;
		return false;
	}

	if ( !channelArray.isOperator( client, channelName ) {

		// client.write(ERR_CHANOPRIVSNEEDED(SERVER_NAME, channel_name));
		std::cout << "Kick error message" << std::endl;
		return false;
	}

	Client	targetedClient = getClient( nickname );
	if ( channelArray.isOperator( targetedClient, channelName ) {

		std::cout << "Cannot kick channel operator" << std::endl;
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

		// curr_client.write(RPL_KICK(kicker.get_nickname(), kicker.get_username(), channel_name, nickname, "KICK"));
		std::cout << "Kick error message" << std::endl;
		return ;
	}

	if ( !kickTests( server, channelArray, client, channelName, nickname )) { return ; }

	kickHandler( server.getClient( nickname ), channelArray.getChannel( channelName ));
}
