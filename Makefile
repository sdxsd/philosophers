##
# philosophers
#
# @file
# @version 0.1

CC = clang
CFLAGS = -g -Wall -Wextra -Werror
NAME = philosophers
CFILES = \
		src/main.c \
		src/philo.c
OFILES = $(CFILES:.c=.o)

all: $(NAME)

$(NAME): $(OFILES)
	$(CC) $(CFLAGS) $(OFILES) $(LIB) -o $(NAME)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo COMPILED: $<

re: fclean all

fclean: clean
	@rm -f $(NAME)
	@echo "DEEP CLEANING"

clean:
	@rm -f $(OFILES)
	@echo "CLEANED UP"

.PHONY: clean re fclean all

# end
