#pragma once

# define	IP_ADDRESS	std::string("@127.0.0.1")
# define	COLON		std::string(":")
# define	SPACE		std::string(" ")
# define	NEW_LINE	std::string("\r\n")

# define	PONG_MSG(server_name, nickname)					COLON + server_name + SPACE + "PONG " + server_name + " :" + nickname + NEW_LINE
# define	JOIN_CHAN(nickname, channel)					COLON + nickname + SPACE + "JOIN" + SPACE + channel + NEW_LINE
# define	USER_MSG(source, nickname, message)				COLON + source + SPACE + "PRIVMSG" + SPACE + nickname + SPACE + COLON + message
//REPLIES
# define	RPL_WELCOME(username, nickname)						COLON + SERVER_NAME + " 001 " + nickname + " :Welcome to the " + SERVER_NAME + \
																" Network " + nickname + "!" + username + IP_ADDRESS + NEW_LINE                                        
# define	RPL_MODES(source, umodes)							COLON + source + SPACE + "221" + SPACE + COLON + umodes + NEW_LINE
# define	RPL_CHANNELMODEIS(source, channel, modes)			COLON + source + SPACE + "324" + SPACE + channel + SPACE + modes + NEW_LINE
# define	RPL_CREATIONTIME(source, channel, creationtime)		COLON + source + SPACE + "329" + SPACE + channel + SPACE + creationtime + NEW_LINE
# define	RPL_INVITELIST(source, channel, invitemask)			COLON + source + SPACE + "346" + SPACE + channel + SPACE + invitemask + NEW_LINE
# define	RPL_ENDOFINVITELIST(source, channel)				COLON + source + SPACE + "347" + SPACE + channel + SPACE + COLON + "End of invite list" + NEW_LINE
# define	RPL_BANLIST(source, channel, banmask)				COLON + source + SPACE + "367" + SPACE + channel + SPACE + banmask + NEW_LINE
# define	RPL_ENDOFBANLIST(source, channel)					COLON + source + SPACE + "368" + SPACE + channel + SPACE + COLON + "End of ban list" + NEW_LINE
# define	RPL_TOPICWHOTIME(source, channel, nickname, setat)	COLON + source + SPACE + channel + SPACE + nickname + SPACE + setat + NEW_LINE
# define	RPL_INVITING(source, nickname, channel)				COLON + source + SPACE + nickname + SPACE + channel + NEW_LINE
# define	RPL_NAMREPLY(nickname, userList) 					COLON + nickname + SPACE + "353" + SPACE + userList + NEW_LINE
# define	RPL_ENDOFNAMES(nickname, channel)					COLON + nickname + SPACE + "366" + SPACE + nickname + SPACE + channel + " :End of /NAMES list" + NEW_LINE

//ERRORS
# define	ERR_NOSUCHNICK(source, nickname)				COLON + source + SPACE + "401 " + "* " + nickname + " :No such nickname/channel" + NEW_LINE
# define	ERR_CANNOTSENDTOCHAN(source, channel)			COLON + source + SPACE + "404 " + "* " + channel + " :Cannot send to channel" + NEW_LINE
# define	ERR_TOOMANYTARGETS(source, target)				COLON + source + SPACE + "407 " + "* " + target + " :Duplicate recipients. No message delivered" + NEW_LINE
# define	ERR_NORECIPIENT(source, command)				COLON + source + SPACE + "411 " + "* " + " :No recipient given " + command + NEW_LINE
# define	ERR_NOTEXTTOSEND(source)						COLON + source + SPACE + "412 " + "* " + " :No text to send" + NEW_LINE
# define	ERR_UNKNOWNCOMMAND(source, command)				COLON + source + SPACE + "421 " + "* " + command + " :Unknown command" + NEW_LINE
# define	ERR_NONICKNAMEGIVEN(source)						COLON + source + SPACE + "431 " + "* " + " :No nickname given" + NEW_LINE
# define	ERR_ERRONEUSNICKNAME(source, nickname)			COLON + source + SPACE + "432 " + "* " + nickname + " :Erroneus nickname" + NEW_LINE
# define	ERR_NICKNAMEINUSE(source, nickname)				COLON + source + SPACE + "433 " + "* " + nickname + " :Nickname is already in use" + NEW_LINE
# define	ERR_USERNOTINCHANNEL(source, nickname)			COLON + source + SPACE + "441 " + "* " + nickname + " :They aren't on that channel" + NEW_LINE
# define	ERR_NOTONCHANNEL(source, channel)				COLON + source + SPACE + "442 " + "* " + channel + " :You're not on that channel" + NEW_LINE
# define	ERR_USERONCHANNEL(source, nickname, channel)	COLON + source + SPACE + "443 " + "* " + nickname + SPACE + channel + " :is already on channel" + NEW_LINE
# define	ERR_NOTREGISTERED(source)						COLON + source + SPACE + "451 " + "* " + " :You have not registered" + NEW_LINE
# define	ERR_NEEDMOREPARAMS(source, command)				COLON + source + SPACE + "461 " + "* " + command + " :Not enough parameters" + NEW_LINE
# define    ERR_ALREADYREGISTERED(source)                   COLON + source + SPACE + "462 " + "* " + ":You may not reregister" + NEW_LINE
# define	ERR_PASSWDMISMATCH(source, nick)	            COLON + source + SPACE + "464 " + "* " + nick + ":Password incorrect" + NEW_LINE
# define	RPL_WELCOME(username, nickname)					COLON + SERVER_NAME + " 001 " + nickname + " :Welcome to the " + SERVER_NAME + \
															" Network " + nickname + "!" + username + IP_ADDRESS + NEW_LINE
# define	ERR_NOSUCHCHANNEL(source, channel)   COLON + source + SPACE + "403 " + "* " + channel + " :No such channel" + NEW_LINE
# define	ERR_NOTONCHANNEL(source, channel)    COLON + source + SPACE + "442 " + "* " + channel + " :You're not on that channel" + NEW_LINE
# define	ERR_CHANOPRIVSNEEDED(source, channel) COLON + source + SPACE + "482 " + "* " + channel + " :You're not channel operator" + NEW_LINE
# define	RPL_NOTOPIC(nick, channel)           COLON + nick + SPACE + "331 " /* + "" */ + channel + " :No topic is set" + NEW_LINE
# define	RPL_TOPIC(nick, channel, topic)      COLON + SERVER_NAME + SPACE + "332 " + nick + SPACE + channel + " :" + topic + NEW_LINE