NAME        = win_calc
CC          = gcc
CFLAGS      = -Wall -Wextra -Werror
CPPFLAGS    = -Iinclude

GTK_CFLAGS  = $(shell pkg-config --cflags gtk4)
GTK_LIBS    = $(shell pkg-config --libs gtk4)

SRCS        =	src/calculator.c src/main.c src/renderer.c src/gui.c src/config.c src/gui_cut_list.c

OBJS        = $(SRCS:.c=.o)

SDK_VER		= 49

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(GTK_LIBS) -o $(NAME)
	@echo "win_calc file is ready"

%.o: %.c
	@$(CC) $(CFLAGS) $(CPPFLAGS) $(GTK_CFLAGS) -c $< -o $@

setup-sdk:
	@if ! flatpak list --runtime | grep -q "org.gnome.Sdk.*$(SDK_VER)"; then \
		echo "GNOME SDK $(SDK_VER) not found. Installing..."; \
		flatpak install -y flathub org.gnome.Sdk//$(SDK_VER); \
	else \
		echo "GNOME SDK $(SDK_VER) is installed."; \
	fi

meson-setup: setup-sdk
	@echo "Setting up Meson build..."
	@flatpak run \
		--command=bash \
		--filesystem=home \
		org.gnome.Sdk//$(SDK_VER) \
		-c "rm -rf build && meson setup build && meson compile -C build"

meson: setup-sdk
	@if [ ! -d "build" ]; then \
		echo "No build directory found. Running full setup..."; \
		$(MAKE) meson-setup; \
	else \
		echo "Recompiling changes with Meson..."; \
		flatpak run \
			--command=bash \
			--filesystem=home \
			org.gnome.Sdk//$(SDK_VER) \
			-c "meson compile -C build"; \
	fi
run:
	@flatpak run \
		--command=./build/win_calc \
		--filesystem=home \
		--filesystem=xdg-run/gvfsd \
		--device=dri \
		--socket=session-bus \
		--socket=wayland \
		--socket=x11 \
		org.gnome.Sdk//$(SDK_VER)

meson-re: setup-sdk
	@echo "Rebuilding entire project from scratch..."
	@$(MAKE) meson-setup

clean:
	@rm -f $(OBJS)
	@echo "remove *.o files"

fclean: clean
	@rm -f $(NAME)
	@echo "remove executable file"

re: fclean all

.PHONY: all clean fclean re meson meson-setup meson-re run 