
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
