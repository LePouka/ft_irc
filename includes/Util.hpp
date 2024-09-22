#pragma once

# define	IP_ADDRESS	std::string("@127.0.0.1")
# define	COLON		std::string(":")
# define	SPACE		std::string(" ")
# define	NEW_LINE	std::string("\r\n")

# define	PONG_MSG(server_name, nickname)					COLON + server_name + SPACE + "PONG " + server_name + " :" + nickname + NEW_LINE
# define	ERR_UNKNOWNCOMMAND(source, command)				COLON + source + SPACE + "421 " + "" + command + " :Unknown command" + NEW_LINE
# define	ERR_NONICKNAMEGIVEN(source)						COLON + source + SPACE + "431 " + "* " + ":No nickname given" + NEW_LINE
# define	ERR_ERRONEUSNICKNAME(source, nick)				COLON + source + SPACE + "432 " + "* " + nick + " :Erroneus nickname" + NEW_LINE
# define	ERR_NICKNAMEINUSE(source, nick)					COLON + source + SPACE + "433 " + "* " + nick + " :Nickname is already in use" + NEW_LINE
# define	ERR_NOTREGISTERED(source)						COLON + source + SPACE + "451 " + "* " + ":You have not registered" + NEW_LINE
# define	ERR_NEEDMOREPARAMS(source, command)				COLON + source + SPACE + "461 " + "* " + command + " :Not enough parameters" + NEW_LINE
# define    ERR_ALREADYREGISTERED(source)                   COLON + source + SPACE + "462 " + "* " + ":You may not reregister" + NEW_LINE
# define	ERR_PASSWDMISMATCH(source, nick)	            COLON + source + SPACE + "464 " + "* " + nick + ":Password incorrect" + NEW_LINE
# define	RPL_WELCOME(username, nickname)					COLON + SERVER_NAME + " 001 " + nickname + " :Welcome to the " + SERVER_NAME + \
															" Network " + nickname + "!" + username + IP_ADDRESS + NEW_LINE
# define	ERR_NOSUCHCHANNEL(source, channel)   COLON + source + SPACE + "403 " + channel + "* " + ":No such channel" + NEW_LINE
# define	ERR_NOTONCHANNEL(source, channel)    COLON + source + SPACE + "442 " + channel + "* " + ":You're not on that channel" + NEW_LINE
# define	ERR_CHANOPRIVSNEEDED(source, channel) COLON + source + SPACE + "482 " + channel + "* " + ":You're not channel operator" + NEW_LINE
# define	RPL_NOTOPIC(nick, channel)           COLON + SERVER_NAME + SPACE + "331 " + nick + SPACE + channel + "* " + " :No topic is set" + NEW_LINE
# define	RPL_TOPIC(nick, channel, topic)      COLON + SERVER_NAME + SPACE + "332 " + nick + SPACE + channel + " :" + topic + NEW_LINE
