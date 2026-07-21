NAME        = win_calc
CC          = gcc
CFLAGS      = -Wall -Wextra -Werror

# Mac-specific flags για τη Raylib
# LDFLAGS     = -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
GTK_CFLAGS  = $(shell pkg-config --cflags gtk4)
GTK_LIBS    = $(shell pkg-config --libs gtk4)

SRCS        = calculator.c main.c visual.c
OBJS        = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(GTK_LIBS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(GTK_CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re