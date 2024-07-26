################################################################# BEG_CUB3D ####

NAME		:= ircserv

# ---------------------------------------------------------------------------- #
#   INGREDIENTS                                                                #
# ---------------------------------------------------------------------------- #
# LIBS			libraries to be used
# LIBS_TARGET	libraries to be built
#
# INCS			header file locations
#
# SRC_DIR		source directory
# SRCS			source files
#
# BUILD_DIR		build directory
# OBJS			object files
# DEPS			dependency files
#
# CFLAGS		compiler flags
# CPPFLAGS		preprocessor flags
# LDFLAGS		linker flags
# LDLIBS		libraries name

SRC_DIR		:= src
SRCS		:=	\
	main.cpp
SRCS		:= $(SRCS:%=$(SRC_DIR)/%)

BUILD_DIR	:= .build
OBJS		:= $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEPS		:= $(OBJS:.o=.d)

CFLAGS		:= $(CFLAGS) -Wall -Wextra -Werror
CPPFLAGS	:= $(CPPFLAGS) $(addprefix -I,$(INCS)) -MMD -MP
LDFLAGS		:= $(LDFLAGS) $(addprefix -L,$(dir $(LIBS_TARGET)))
LDLIBS		:= $(LDLIBS) $(addprefix -l,$(LIBS))

# ---------------------------------------------------------------------------- #
#   UTENSILS                                                                   #
# ---------------------------------------------------------------------------- #
# RM		force remove
# MAKE		quietly make
# DIR_DUP	duplicate directory tree
# VALGRIND	valgrind command
# ERR_MUTE	filter errors
#
# CRUSH     used to print on the same line
# ECHO      echo statement
# R         red color output
# G         green color output
# END       reset color output to default

RM			:= rm -f
MAKE		:= $(MAKE) --jobs --silent --no-print-directory
DIR_DUP		= mkdir -p $(@D)
VALGRIND	:= valgrind -q -s --leak-check=yes --show-leak-kinds=all --track-fds=yes --track-origins=yes
ERR_MUTE	:= 2>/dev/null

CRUSH		:= \r\033[K
ECHO		:= echo -n "$(CRUSH)"
R			:= $(shell tput setaf 1)
G			:= $(shell tput setaf 2)
END			:= $(shell tput sgr0)

# ---------------------------------------------------------------------------- #
#   RECIPES                                                                    #
# ---------------------------------------------------------------------------- #
# all		default goal
# $(NAME)	link .o -> archive
# $(LIBS)	build libraries
# %.o		compilation .c -> .o
# clean		remove .o
# fclean	remove .o + binary
# re		remake default goal
# update	update the repo to its most recent version

all: $(NAME)

$(NAME): $(OBJS) $(LIBS_TARGET)
	$(CC) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $(NAME)
	$(ECHO)"$(G)CREATED$(END) $(@)\n"

$(LIBS_TARGET):
	$(MAKE) -C $(@D) #$(ERR_MUTE)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(DIR_DUP)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<
	$(ECHO)"$(G)CREATED$(END) $(@)\n"

-include $(DEPS)

clean:
	$(MAKE) -C lib/libft clean
	$(RM) $(OBJS) $(DEPS)

fclean: clean
	$(MAKE) -C lib/libft fclean
	#$(MAKE) -C lib/minilibx-linux clean
	$(RM) $(NAME)

re:
	$(MAKE) fclean
	$(MAKE) all

update:
	git stash
	git pull
	git submodule update --init
	git stash pop

# ---------------------------------------------------------------------------- #
#   DIFFERENT CGLAGS BUILD                                                     #
# ---------------------------------------------------------------------------- #
# asan		address sanitizer
# ansi		ansi c89 compliance
# every		all warnings

asan: CFLAGS	+= -O0 -g3 -fsanitize=address,undefined,integer -fno-optimize-sibling-calls
asan: LDFLAGS	+= -g3 -fsanitize=address,undefined,integer
asan: all

ansi: CFLAGS	+= -W -pedantic -std=c89
ansi: all

every: CFLAGS	+= -Weverything
every: all

# ---------------------------------------------------------------------------- #
#   TESTS                                                                      #
# ---------------------------------------------------------------------------- #
# norm		norminette
# run		vanilla run
# vrun		run with valgrind
# info		print the <target> recipe whithout executing it
# pring		print the value of an arbitrary variable

run-%: $(NAME)
	-./$(NAME) $*

vrun-%: CFLAGS  += -g3
vrun-%: $(NAME)
	-$(VALGRIND) ./$(NAME) $*

norm:
	norminette src/ include/ | grep -v "OK" || true

info-%:
	$(MAKE) --dry-run --always-make $* | grep -v "info"

print-%:
	$(info '$*'='$($*)')

# ---------------------------------------------------------------------------- #
#   SPEC                                                                       #
# ---------------------------------------------------------------------------- #
.PHONY: clean fclean re update asan ansi every norm
.SILENT:

################################################################# END_CUB3D ####
