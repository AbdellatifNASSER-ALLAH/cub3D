/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahakki <ahakki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 23:48:01 by ahakki            #+#    #+#             */
/*   Updated: 2025/08/12 15:22:55 by abdnasse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <mlx.h>
# include <stdio.h>
# include <math.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <ulimit.h>
# include <fcntl.h>
# include "libft.h"

// === Colors ===
#define RED     "\033[31m"
#define RESET   "\033[0m"

# define WIDTH 1280
# define HEIGHT 920
# define BLOCK 64

# define W 119
# define A 97
# define S 115
# define D 100

# define LEFT 65361
# define RIGHT 65363
# define UP 65362
# define DOWN 65364

# define PI 3.14159265359

# define PLAYER_SIZE 0

typedef enum s_tex{
	NORTH,
	SOUTH,
	EAST,
	WEST,
	NB_TEX
}	e_texture;

# define MINI_WIDTH 200
# define MINI_HEIGHT 200
# define MINI_BLOCK 10
# define RADIUS ((MINI_WIDTH / 2) - 5)

typedef	struct	s_config
{
	char	*path;
	char	**lines;
	int	nb_lines;
	char	*tex[NB_TEX];
	int	tex_found[4];
	int	f_rgb[3];
	int	c_rgb[3];
	int	c_found;
	int	f_found;
	int	map_start;
	int	map_end;
	char	**map;
}	t_config;

typedef struct s_texture
{
    void    *img;
    int     width;
    int     height;
    int     *data;
}   t_texture;

typedef struct s_player
{
	float	x;
	float	y;
	float	z_eye;
	float	angle;

	int		speed;

	bool	key_up;
	bool	key_left;
	bool	key_right;
	bool	key_down;

	bool	left_rotate;
	bool	up_rotate;
	bool	down_rotate;
	bool	right_rotate;

	int		prev_mouse_x;
}	t_player;

typedef struct	s_game
{
	void		*mlx;
	void		*win;
	void		*img;

	char		*data;
	int			bpp;
	int			size_line;
	int			endian;
	char		**map;
	t_texture textures[4]; // 0=NORTH, 1=SOUTH, 2=EAST, 3=WEST
	t_player	player;
	t_config	config;
}	t_game;


void	init_player(t_game *game);
int		key_press(int key, t_game *game);
int 	key_release(int key, t_game *game);
int 	move_player(t_game *game);
int		mouse_move(int x, int y, t_game *game);
void	mouse_move_handler(int x, int y, t_game *game);
void 	update(t_game *game);

bool	touch(int px, int py, t_game *game);

// ====== Parse ==========
typedef struct s_line {
	char	*val;
	int	i;
	int	size;
	char	up;
	char	down;
	char	left;
	char	right;
}	t_line;
void	wrap_line(t_line *line, char *s);
int	next_char_line(t_line *line);
int	peek_line(t_line *line);

void	valid_file(char *path, char *extension, t_config *cfg);
void	parse(t_config *cfg, char *path);
void	read_file(t_config *cfg);
void	extract_configs(t_config *cfg);
void	fill_map(char **map,int	start, int end, t_config *cfg);

// ====== Utils ==========
void	exit_err(const char *msg, int st, t_config *cfg);

#endif
