CXX = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -g3
NAME = ircserv
SRC = main.cpp \
	socket.cpp \
	epoll.cpp \
	message_handling.cpp \


SRCS = $(addprefix srcs/, $(SRC))
OBJDIR = objs
OBJS = $(addprefix $(OBJDIR)/, $(SRC:.cpp=.o))

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJDIR)/%.o: srcs/%.cpp | $(OBJDIR)
	$(CXX) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: clean fclean all re
