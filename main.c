/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahakki <ahakki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 23:47:58 by ahakki            #+#    #+#             */
/*   Updated: 2025/07/19 20:01:09 by ahakki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"



void	put_pixel(int x, int y, int color, t_game *game)
{
	int	index;
	
	if (x >= WIDTH || y >= HEIGHT || x < 0 || y < 0)
	return ;
	index = y * game->size_line + x * game->bpp / 8;
	game->data[index] = color & 0xFF;
	game->data[index + 1] = (color >> 8) & 0xFF;
	game->data[index + 2] = (color >> 16) & 0xFF;
}

void	draw_squar(int x, int y, int size, int color, t_game *game)
{
	int	i;

	i = 0;
	while (i <= size)
	{
		put_pixel(x + i, y, color, game);
		put_pixel(x, y + i, color, game);
		put_pixel(x + i, y + size, color, game);
		put_pixel(x + size, y + i, color, game);
		i++;
	}
}
void	draw_full_squar(int x, int y, int size, int color, t_game *game)
{
	int i, j;

	i = 0;
	while (i < size)
	{
		j = 0;
		while (j < size)
		{
			put_pixel(x + j, y + i, color, game);
			j++;
		}
		i++;
	}
}

void	draw_map(t_game *game)
{
	int		y;
	int		x;
	int		color;

	color = 0x0000FF;
	y = 0;
	while (game->map[y])
	{
		x = 0;
		while (game->map[y][x])
		{
			if (game->map[y][x] == '1')
			{
				int draw_x = (x * BLOCK) - game->player.x + CAMERA_X;
				int draw_y = (y * BLOCK) - game->player.y + CAMERA_Y;	
				draw_full_squar(x * BLOCK, y * BLOCK, BLOCK, color, game);
			}
			x++;
		}
		y++;
	}
}

void	map_height(t_game *game)
{
	game->map_height = 0;
	while (game->map && game->map[game->map_height])
		game->map_height++;
}

void	map_width(t_game *game)
{
	int	i;

	i = 0;
	game->map_width = 0;
	while (game->map[i])
	{
		if (ft_strlen(game->map[i]) > game->map_width)
			game->map_width = ft_strlen(game->map[i]);
		i++;
	}
}

void	get_player_cord(t_game *game)
{
	int		y = 0;
	int		x;
	char	dir;

	while (game->map[y])
	{
		x = 0;
		while (game->map[y][x])
		{
			dir = game->map[y][x];
			if (dir == 'N' || dir == 'S' || dir == 'E' || dir == 'W')
			{
				game->player.x = x * BLOCK + BLOCK / 2;
				game->player.y = y * BLOCK + BLOCK / 2;
				if (dir == 'N')
					game->player.angle = 3 * PI / 2;
				else if (dir == 'S')
					game->player.angle = PI / 2;
				else if (dir == 'E')
					game->player.angle = 0;
				else if (dir == 'W')
					game->player.angle = PI;
				return ;
			}
			x++;
		}
		y++;
	}
}


void	get_map(t_game *game)
{
	char	**map = malloc(sizeof(char*) * 11);
	map[0] = "111111111111111111";
	map[1] = "100000000000000001";
	map[2] = "100000000000000001";
	map[3] = "100000000000000001";
	map[4] = "100001000000000001";
	map[5] = "100000000000000001";
	map[6] = "100000001000000001";
	map[7] = "100000000000000001";
	map[8] = "100000000000W00001";
	map[9] = "111111111111111111";
	map[10] = NULL;
	game->map = map;
	map_height(game);
	map_width(game);
	get_player_cord(game);
}


void	init_game(t_game *game)
{
	game->mlx = mlx_init();
	get_map(game);
	init_player(game);
	game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, "CUB3D");
	game->img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
	game->data = mlx_get_data_addr(game->img, &game->bpp, &game->size_line, &game->endian);
	mlx_put_image_to_window(game->mlx, game->win, game->img, 0 , 0);
}

void	clear_img(t_game *game)
{
	int	y;
	int	x;

	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			put_pixel(x, y, 0x000000, game);
			x++;
		}
		y++;
	}
}
bool	touch(int px, int py, t_game *game)
{
	int tile_x = (int)(px / BLOCK);
    int tile_y = (int)(py / BLOCK);

    if (tile_x < 0 || tile_x >= game->map_width || tile_y < 0 || tile_y >= game->map_height)
        return (1);

	if (game->map[py / BLOCK][px / BLOCK] == '1')
		return (true);
	return (false);
}
float	distance(float x, float y)
{
	return (sqrt(x * x + y * y));
}
float	fixed_distance(float x1, float x2, float y1, float y2, t_game *game)
{
	float	delta_x = x2 - x1;
	float	delta_y = y2 - y1;
	float	angle = atan2(delta_y, delta_x) - game->player.angle;
	float	fix_dist = distance(delta_x, delta_y) * cos(angle);
	return (fix_dist);
}

void	draw_vision(t_game *game)
{
	t_player	*player = &game->player;
	int			x;
	float		fov = PI / 3;
	float		ray_angle;
	float		angle_step = fov / WIDTH;
	float		ray_x;
	float		ray_y;
	float		cos_angle;
	float		sin_angle;

	x = 0;
	while (x < WIDTH)
	{
		ray_angle = player->angle - (fov / 2) + (x * angle_step);
		ray_x = player->x + (PLAYER_SIZE / 2.0f);
		ray_y = player->y + (PLAYER_SIZE / 2.0f);
		cos_angle = cos(ray_angle);
		sin_angle = sin(ray_angle);

		while (!touch(ray_x, ray_y, game))
		{
			put_pixel(ray_x, ray_y, 0xFF0000, game);

			ray_x += cos_angle;
			ray_y += sin_angle;
		}
		// float	dist = fixed_distance(player->x, ray_x, player->y, ray_y, game);
		// float	height = (BLOCK / dist) * (WIDTH / 2);
		// int		start_y = (HEIGHT - height) / 2;
		// int		end = start_y + height;
		// while (start_y < end)
		// {
		// 	put_pixel(x, start_y, 0x0000FF, game);
		// 	start_y++;
		// }
		x++;
	}
}

int	draw_loop(t_game *game)
{
	t_player	*player;

	player = &game->player;
	move_player(game);
	clear_img(game);
	draw_map(game);
	draw_squar(player->x, player->y, PLAYER_SIZE, 0x00FF00, game);
	draw_vision(game);
	

	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
	return (0);
}

int main(int ac, char **av)
{
	t_game	game;
	init_game(&game);
	
	mlx_hook(game.win, 2, 1L<<0, key_press, &game);
	mlx_hook(game.win, 3, 1L<<1, key_release, &game);

	mlx_loop_hook(game.mlx, draw_loop, &game);

	mlx_loop(game.mlx);
}