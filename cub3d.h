/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahakki <ahakki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 23:48:01 by ahakki            #+#    #+#             */
/*   Updated: 2025/07/28 22:59:41 by ahakki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <mlx.h>
# include "./minilibx-linux/mlx.h"
# include <stdio.h>
# include <math.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <ulimit.h>
# include "./ft_libft/libft.h"

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

#define NORTH 0
#define SOUTH 1
#define EAST  2
#define WEST  3

#define MINI_WIDTH 200
#define MINI_HEIGHT 200
#define MINI_BLOCK 10



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
	int			map_width;
	int			map_height;
	t_player	player;
}	t_game;


void	init_player(t_game *game);
int		key_press(int key, t_game *game);
int 	key_release(int key, t_game *game);
int 	move_player(t_game *game);
int		mouse_move(int x, int y, t_game *game);
void 	update(t_game *game);

bool	touch(int px, int py, t_game *game);
#endif