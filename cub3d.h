/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahakki <ahakki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 23:48:01 by ahakki            #+#    #+#             */
/*   Updated: 2025/07/16 23:48:02 by ahakki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <mlx.h>
# include "./minilibx-linux/mlx.h"
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <ulimit.h>

# define WIDTH 1280
# define HEIGHT 720

# define W 119
# define A 97
# define S 115
# define D 100

# define PI 3.14159265359

typedef struct s_player
{
	float	x;
	float	y;

	bool	key_up;
	bool	key_left;
	bool	key_right;
	bool	key_down;
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
	t_player	player;
}	t_game;


void	init_player(t_player *player);
int		key_press(int key, t_player *player);
int 	key_release(int key, t_player *player);
int 	move_player(t_player *player);
#endif