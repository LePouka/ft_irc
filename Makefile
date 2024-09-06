GREEN   =   \e[92;5;118m
RED     =   \033[1;31m
BLUE    =   \033[1;34m
NULL    =   \e[0m

NAME    = ircserv
CC      = c++
FLAGS   = -Wall -Wextra -Werror -std=c++98
RM      = rm -rf
SRCDIR  = srcs
INCDIR  = inc
SRC     = $(wildcard $(SRCDIR)/*.cpp)
OBJDIR  = objs
OBJS    = $(addprefix $(OBJDIR)/, $(notdir $(SRC:.cpp=.o)))

all: $(NAME)

$(NAME): $(OBJS)
	@ $(CC) $(FLAGS) $(OBJS) -o $(NAME)
	@ printf "$(GREEN)>>> Executable ready.\n$(NULL)"

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	@ $(CC) $(FLAGS) -I $(INCDIR) -c $< -o $@
	@ printf "$(BLUE)>>> Compiled $< into $@.\n$(NULL)"

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
