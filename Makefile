NAME = game
CC = cc
OBJ = $(SRC:.c=.o)

LFLAGE = -L./mlx -lmlx -lXext -lX11 -lm -lz
INCLUDES = libmlx.a
SRC = main.c player.c

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(SRC) -o $(NAME) $(INCLUDES) $(LFLAGE)

fclean: clean
	rm -rf $(NAME)

clean:
	rm -rf $(OBJ)