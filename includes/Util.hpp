#pragma once

// Format
# define	DOTS		std::string(":")
# define	SPACE		std::string(" ")
# define	TILD		std::string("!~")
# define	NEW_LINE	std::string("\r\n")
# define	IP_ADD		std::string("@127.0.0.1")




/*****		USED				*****/

// NON-NUMERIC REPLIES
# define	WELCOME_MESSAGE(username, nickname)										DOTS + SERVER_NAME + " 001 " + nickname + \
																			" :Welcome to the " + SERVER_NAME + " IRC Network " + nickname + \
																			TILD + username + IP_ADD + NEW_LINE + \
																			DOTS + SERVER_NAME + " 002 " + nickname + \
																			" :Your host is ircserv, running version 1.0" + NEW_LINE + \
																			DOTS + SERVER_NAME + " 003 " + nickname + \
																			" :This server was created Tue Apr 30 2024 at 16:38:57 UTC" + NEW_LINE

# define	NICK_CHANGED(old_nick, username, new_nick, command)				DOTS + old_nick + TILD + username + IP_ADD + SPACE + command + " :" + new_nick + NEW_LINE
# define	MSG_RECEIVED(sdr_nick, rcv_usrnm, rcv_nick, msg, command)		DOTS + sdr_nick + TILD + rcv_usrnm + IP_ADD + SPACE + command + SPACE + rcv_nick + " :" + msg + NEW_LINE
# define	JOINED_CHANNEL(nickname, username, chan_name, command)			DOTS + nickname + TILD + username + IP_ADD + SPACE + command + " :" + chan_name + NEW_LINE
# define	RPL_TOPIC(nickname, username, chan_name, chan_top, command)		DOTS + nickname + TILD + username + IP_ADD + SPACE + command + SPACE + chan_name + " " + chan_top + NEW_LINE																			
# define	RPL_KICK(nickname, username, chan_name, kcked_nick, command)	DOTS + nickname + TILD + username + IP_ADD + SPACE + command + SPACE + chan_name + SPACE + kcked_nick + " :" + nickname + NEW_LINE
# define	PONG_MSG(server_name, nickname)									DOTS + server_name + SPACE + "PONG " + server_name + " :" + nickname + NEW_LINE
# define	RPL_MODEUPDATECHANOP(og_nick, og_user, chan_name, sign, nick)	DOTS + og_nick + TILD + og_user + IP_ADD + " MODE " + chan_name + SPACE + sign +"o " + nick + NEW_LINE
# define		CHAN_TOPIC(chan_name, topic)									DOTS + SERVER_NAME + SPACE + "332 " + "casomarr " + chan_name + " :" + topic + NEW_LINE

// ERRORS
# define	ERR_NOSUCHNICK(prefix, nickname)				DOTS + prefix + SPACE + "401 " + "* " + nickname + " :No such nick/channel" + NEW_LINE
# define	ERR_NOSUCHSERVER(prefix, server_name)			DOTS + prefix + SPACE + "402 " + "* " + server_name + " :No such server" + NEW_LINE
# define	ERR_NOSUCHCHANNEL(prefix, channel_name)			DOTS + prefix + SPACE + "403 " + "* " + channel_name + " :No such channel" + NEW_LINE
# define	ERR_UNKNOWNCOMMAND(prefix, command)				DOTS + prefix + SPACE + "421 " + "" + command + " :Unknown command" + NEW_LINE
# define	ERR_NONICKNAMEGIVEN(prefix)						DOTS + prefix + SPACE + "431 " + "* " + ":No nickname given" + NEW_LINE
# define	ERR_ERRONEUSNICKNAME(prefix, nick)				DOTS + prefix + SPACE + "432 " + "* " + nick + " :Erroneus nickname" + NEW_LINE
# define	ERR_NICKNAMEINUSE(prefix, nick)					DOTS + prefix + SPACE + "433 " + "* " + nick + " :Nickname is already in use" + NEW_LINE
# define	ERR_NOTONCHANNEL(prefix, channel)				DOTS + prefix + SPACE + "442 " + "* " + channel + " :You're not on that channel" + NEW_LINE
# define	ERR_USERONCHANNEL(prefix, user, invitee_nickname, channel)		DOTS + prefix + SPACE + "443 " + "" + user + SPACE + invitee_nickname + SPACE + channel + " :is already on channel" + NEW_LINE
# define	ERR_NOTREGISTERED(prefix)						DOTS + prefix + SPACE + "451 " + ":You have not registered" + NEW_LINE
# define	ERR_NEEDMOREPARAMS(prefix, command)				DOTS + prefix + SPACE + "461 " + "* " + command + " :Not enough parameters" + NEW_LINE
# define	ERR_CHANNELISFULL(prefix, channel)				DOTS + prefix + SPACE + "471 " + "" + channel + " :Cannot join channel (+l)" + NEW_LINE
# define	ERR_UNKNOWNMODE(prefix, char)					DOTS + prefix + SPACE + "472 " + "" + char + " :is unknown mode char to me" + NEW_LINE
# define	ERR_INVITEONLYCHAN(prefix, channel)				DOTS + prefix + SPACE + "473 " + "* " + channel + " :Cannot join channel (+i)" + NEW_LINE
# define	ERR_BADCHANNELKEY(prefix, channel)				DOTS + prefix + SPACE + "475 " + "* " + channel + " :Cannot join channel (+k)" + NEW_LINE
# define	ERR_CHANOPRIVSNEEDED(prefix, channel)			DOTS + prefix + SPACE + "482 " + "* " + channel + " :You're not channel operator" + NEW_LINE


// NUMERIC REPLIES
# define	RPL_INVITING(prefix, channel, inviting_usr, invitee_usr)		DOTS + prefix + SPACE + "341 " + "" + inviting_usr + SPACE + invitee_usr + SPACE + channel + NEW_LINE
#define		RPL_INVITED(inviter_nick, inviter_usr, invitee_nick, chan_name, command)	DOTS + inviter_nick + TILD + inviter_usr + IP_ADD + SPACE + command + SPACE + invitee_nick + " :" + chan_name + NEW_LINE
