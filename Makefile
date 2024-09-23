GREEN   =   \e[92;5;118m
RED     =   \033[1;31m
BLUE    =   \033[1;34m
NULL    =   \e[0m

CXX = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -g3
NAME = ircserv
SRC = main.cpp \
	Server.cpp \
	Client.cpp \
	Channel.cpp \
	Message.cpp \
	commands/channelCommands/Join.cpp \
	commands/channelCommands/Topic.cpp \
	commands/channelCommands/Part.cpp \
	commands/Privmsg.cpp \
	commands/NICK.cpp \
	commands/PASS.cpp \
	commands/USER.cpp \

SRCS = $(addprefix srcs/, $(SRC))
OBJDIR = objs
OBJS = $(addprefix $(OBJDIR)/, $(SRC:.cpp=.o))

all: $(NAME)

$(NAME): $(OBJS)
	@ $(CXX) $(CFLAGS) $(OBJS) -o $(NAME)
	@ printf "$(GREEN)>>> Executable ready.\n$(NULL)"

$(OBJDIR)/%.o: srcs/%.cpp | $(OBJDIR)
	@mkdir -p $(dir $@)
	@ $(CXX) $(CFLAGS) -c $< -o $@
	@ printf "$(BLUE)>>> Compiled $< into $@.\n$(NULL)"

$(OBJDIR):
	@ mkdir -p $(OBJDIR)

clean:
	@ rm -rf $(OBJDIR)
	@ printf "$(RED)>>> Objects removed.\n$(NULL)"

fclean: clean
	@ rm -f $(NAME)
	@ printf "$(RED)>>> Executable removed.\n$(NULL)"

re: fclean all

.PHONY: clean fclean all re
