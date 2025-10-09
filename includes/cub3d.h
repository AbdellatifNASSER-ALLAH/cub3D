/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahakki <ahakki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 23:48:01 by ahakki            #+#    #+#             */
/*   Updated: 2025/10/09 13:46:38 by abdnasse         ###   ########.fr       */
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
# define RED     "\033[31m"
# define RESET   "\033[0m"

# define WIDTH 1280
# define HEIGHT 920
# define BLOCK 64

# define TEXTURE_WIDTH 64
# define TEXTURE_HEIGHT 64

# define W 119
# define A 97
# define S 115
# define D 100

# define LEFT 65361
# define RIGHT 65363
# define UP 65362
# define DOWN 65364

# define PI 3.14159265359
# define FOV 1.0471975512

# define PLAYER_SIZE 0

typedef enum s_tex
{
	NORTH,
	SOUTH,
	EAST,
	WEST,
	NB_TEX
}	t_tex;

# define MINI_WIDTH 200
# define MINI_HEIGHT 200
# define MINI_BLOCK 10
# define RADIUS 95

typedef struct s_config
{
	char	*path;
	char	**lines;
	int		nb_lines;
	char	*tex[NB_TEX];
	int		tex_found[4];
	int		f_rgb[3];
	int		c_rgb[3];
	int		c_found;
	int		f_found;
	int		map_start;
	int		map_end;
	int		player_count;
	char	**map;
}	t_config;

typedef struct s_texture
{
	void	*img;
	int		width;
	int		height;
	int		*data;
}	t_texture;

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

typedef struct s_ray
{
	float	ray_angle;
	float	rayDirX;
	float	rayDirY;
	int		mapX;
	int		mapY;
	int		side;
	int		wallX;
	int		wallY;
	int		stepX;
	int		stepY;
	float	perpWallDist;
	float	dist;
	float	wall_height;
	int		start_y;
	int		end_y;
	int		color;
	float	deltaDistX;
	float	deltaDistY;
	float	px;
	float	py;
	float	sideDistX;
	float	sideDistY;
	int		hit;
}	t_ray;

typedef struct s_game
{
	void		*mlx;
	void		*win;
	void		*img;

	char		*data;
	int			bpp;
	int			size_line;
	int			endian;
	char		**map;
	t_texture	textures[4];
	t_player	player;
	t_config	config;
}	t_game;

void	init_player(t_game *game);
int		key_press(int key, t_game *game);
int		key_release(int key, t_game *game);
int		move_player(t_game *game);
int		mouse_move(int x, int y, t_game *game);
void	mouse_move_handler(int x, int y, t_game *game);
void	update(t_game *game);

bool	touch(int px, int py, t_game *game);
void	put_pixel(int x, int y, int color, t_game *game);

// ====== Parse ==========
enum { U, Do, L, R, ME };
void	valid_file(char *path, char *extension, t_config *cfg);
void	parse(t_config *cfg, char *path);
void	read_file(t_config *cfg);
void	extract_configs(t_config *cfg, char *l);
void	fill_map(char **map, int start, int end, t_config *cfg);

// ====== Utils ==========
void	exit_err(const char *msg, int st, t_config *cfg);

// ====== execution ============

void	draw_full_squar(int x, int y, int size, int color, t_game *game);
void	draw_map(t_game *game);
void	draw_minimap(t_game *game);
void	init_ray(t_ray *r, t_player *player, int x);
void	perform_dda(t_ray *r, t_game *game);
int		get_texture_color(t_ray *r, int tex_y, t_game *game);
void	load_textures(t_game *game);


#endif
