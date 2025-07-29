
vpath %.c ./srcs


CC=cc
CFLAGS= 
#CFLAGS= -Wall -Wextra -Werror
INCLUDE = -I./libft/includes -I./includes -I/usr/local/include
LIBS = -L./libft -lft -L/usr/local/lib -lmlx -lXext -lX11 -lm

LIBFT_DIR=./libft
BUILD_DIR=./build/
LIBFT=./libft/libft.a

HEADER=./includes/cub3d.h

SRC = main.c player.c
OBJ = $(addprefix $(BUILD_DIR), $(SRC:.c=.o))

NAME = game

all:$(NAME)
	@echo "\vEnjoy the game"

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) $(LIBS) -o $@
	@echo "Done!"

$(BUILD_DIR)%.o: %.c $(HEADER) $(LIBFT) | $(BUILD_DIR)
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(BUILD_DIR):
	@echo -n "Builing cub3d..."
	@mkdir -p $(BUILD_DIR)

$(LIBFT):
	@echo -n "Builing libft..."
	@$(MAKE) -s -C $(LIBFT_DIR)
	@echo "Done!"

clean:
	@$(MAKE) -s -C $(LIBFT_DIR) clean
	rm -rf $(BUILD_DIR)

fclean: clean
	@$(MAKE) -s -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
.SECONDARY: $(OBJ) $(BUILD_DIR)
