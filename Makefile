NAME = game
CC = cc
OBJ = $(SRC:.c=.o)

LFLAGE = -L./mlx -lmlx -lXext -lX11 -lm -lz
INCLUDES = libmlx.a
SRC = main.c

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(SRC) -o $(NAME) $(INCLUDES) $(LFLAGE)

fclean:
	rm -rf $(OBJ)
	rm -rf $(NAME)