CXX = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -g3
NAME = ircserv
SRC = main.cpp \
	Server.cpp \
	Client.cpp \
	Channel.cpp \
	Message.cpp \
	commands/channelCommands/Join.cpp \
	commands/Privmsg.cpp \
	
	
SRCS = $(addprefix srcs/, $(SRC))
OBJDIR = objs
OBJS = $(addprefix $(OBJDIR)/, $(SRC:.cpp=.o))

all: $(NAME)

$(NAME): $(OBJS)
	@ $(CC) $(FLAGS) $(OBJS) -o $(NAME)
	@ printf "$(GREEN)>>> Executable ready.\n$(NULL)"

$(OBJDIR)/%.o: srcs/%.cpp | $(OBJDIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	@ mkdir -p $(OBJDIR)

clean:
	@ $(RM) $(OBJDIR)
	@ printf "$(RED)>>> Objects removed.\n$(NULL)"

fclean: clean
	@ $(RM) $(NAME)
	@ printf "$(RED)>>> Executable removed.\n$(NULL)"

re: fclean all

.PHONY: all clean fclean re
