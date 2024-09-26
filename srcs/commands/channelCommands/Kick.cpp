#include "../../../includes/Server.hpp"
#include "../../../includes/Util.hpp"

// bool	kickTests( ChannelArray const & channel,
// 		Client const & client,
// 		std::string const & channelName,
// 		std::string const & nickname ) {

// 	if ( !client.isRegistered() ) {

// 		std::cout << "Kick error message" << std::endl;
// 		return false;
// 	}

// 	if ( !channel.isChan( channelName ) ) {

// 		std::cout << "Kick error message" << std::endl;
// 		return false;
// 	}

// 	if ( !channel.userInChannel( client, channelName ) ) {

// 		std::cout << "Kick error message" << std::endl;
// 		return false;
// 	}

// 	if ( !channel.isOperator( client, channelName ) {

// 		std::cout << "Kick error message" << std::endl;
// 		return false;
// 	}

// 	Client targetedClient = getClient( nickname );
// 	if ( channel.isOperator( targetedClient, channelName ) {

// 		std::cout << "Kick error message" << std::endl;
// 		return false;
// 	}

// 	return true;
// }

// void	kickHandler( ChannelArray const & channel,
// 		Client const & client,
// 		std::string channelName,
// 		std::string nickname ) {}

// void	kick( ChannelArray const & channel,
// 		Client const & client,
// 		std::string const & args ) {

// 	std::istringstream iss(args);
// 	std::string channelName, nickname;

// 	if ( !( iss >> channelName >> nickname ) || channelName.empty() || nickname.empty() ) {

// 		std::cout << "Kick error message" << std::endl;
// 		return ;
// 	}

// 	if ( !kickTests( channel, client, channelName, nickname )) { return ; }

// 	kickHandler( channel, client, channelName, nickname );
// }
