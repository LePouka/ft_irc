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
# define	ERR_NOTREGISTERED(source)						COLON + source + SPACE + "451 " + ":You have not registered" + NEW_LINE
# define	ERR_NEEDMOREPARAMS(source, command)				COLON + source + SPACE + "461 " + "* " + command + " :Not enough parameters" + NEW_LINE
# define	RPL_WELCOME(username, nickname)					COLON + SERVER_NAME + " 001 " + nickname + " :Welcome to the " + SERVER_NAME + \
                                                            " Network " + nickname + "!" + username + IP_ADDRESS + NEW_LINE                                        