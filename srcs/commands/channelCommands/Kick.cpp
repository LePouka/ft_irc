#include "../../../includes/Commands.hpp"

bool	kickTests( Server const & server,
		ChannelArray const & channelArray,
		Client const & client,
		std::string const & channelName,
		std::string const & nickname ) {

	if ( !server.isClientRegistered( nickname ) ) {

		std::cout << "Kick error message" << std::endl;
		return false;
	}

	if ( !channelArray.isChan( channelName ) ) {

		std::cout << "Kick error message" << std::endl;
		return false;
	}

	if ( !channelArray.userInChannel( client, channelName ) ) {

		std::cout << "Kick error message" << std::endl;
		return false;
	}

	if ( !channelArray.isOperator( client, channelName ) {

		std::cout << "Kick error message" << std::endl;
		return false;
	}

	Client targetedClient = getClient( nickname );
	if ( channelArray.isOperator( targetedClient, channelName ) {

		std::cout << "Kick error message" << std::endl;
		return false;
	}

	return true;
}

void	kickHandler( Server const & server,
		ChannelArray const & channelArray,
		Client const & client,
		std::string const & channelName,
		std::string const & nickname ) {

		;
}

void	kick( Server const & server,
		ChannelArray const & channelArray,
		Client const & client,
		std::string const & args ) {

	std::istringstream iss(args);
	std::string channelName, nickname;

	if ( !( iss >> channelName >> nickname ) || channelName.empty() || nickname.empty() ) {

		std::cout << "Kick error message" << std::endl;
		return ;
	}

	if ( !kickTests( server, channelArray, client, channelName, nickname )) { return ; }

	kickHandler( server, channelArray, client, channelName, nickname );
}
