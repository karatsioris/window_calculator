NAME        = win_calc
CC          = gcc
CFLAGS      = -Wall -Wextra -Werror
CPPFLAGS    = -Iinclude

GTK_CFLAGS  = $(shell pkg-config --cflags gtk4)
GTK_LIBS    = $(shell pkg-config --libs gtk4)

SRCS        = src/calculator.c src/main.c src/visual.c
OBJS        = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(GTK_LIBS) -o $(NAME)
	@echo "win_calc file is ready"

%.o: %.c
	@$(CC) $(CFLAGS) $(CPPFLAGS) $(GTK_CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS)
	@echo "remove *.o files"

fclean: clean
	@rm -f $(NAME)
	@echo "remove executable file"

re: fclean all

.PHONY: all clean fclean re