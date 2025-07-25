NAME = game
CC = cc
OBJ = $(SRC:.c=.o)

LFLAGE =  -Wall -Wextra -Werror -I -L./mlx -lmlx -lXext -lX11 -lm -lz
INCLUDES = libmlx.a ft_libft/libft.a
SRC = main.c player.c

all: $(NAME)

%.o: %.c cub3d.h
	$(CC) -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $(SRC) -o $(NAME) $(INCLUDES) $(LFLAGE)

fclean: clean
	rm -rf $(NAME)

clean:
	rm -rf $(OBJ)

re:fclean all