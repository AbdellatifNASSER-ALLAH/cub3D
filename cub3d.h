/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahakki <ahakki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 23:48:01 by ahakki            #+#    #+#             */
/*   Updated: 2025/07/18 11:59:45 by ahakki           ###   ########.fr       */
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
# define HEIGHT 720
# define BLOCK 64

# define W 119
# define A 97
# define S 115
# define D 100

# define LEFT 65361
# define RIGHT 65363

# define PI 3.14159265359

# define PLAYER_SIZE 15

typedef struct s_player
{
	float	x;
	float	y;
	float	angle;

	int		speed;
	
	bool	key_up;
	bool	key_left;
	bool	key_right;
	bool	key_down;
	
	bool	left_rotate;
	bool	right_rotate;
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

bool	touch(int px, int py, t_game *game);
#endif