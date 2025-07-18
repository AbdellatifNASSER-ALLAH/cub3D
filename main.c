/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahakki <ahakki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 23:47:58 by ahakki            #+#    #+#             */
/*   Updated: 2025/07/18 12:02:53 by ahakki           ###   ########.fr       */
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

// void	draw_map(t_game *game)
// {
// 	int		y;
// 	int		x;
// 	int		color;
// 	char	**map;

// 	color = 0x0000FF;
// 	map = game->map;
// 	y = 0;
// 	while (map[y])
// 	{
// 		x = 0;
// 		while (x < map[y][x])
// 		{
// 			if (map[y][x] == '1')
// 				draw_full_squar(x * 64, y * 64, 64, color, game);
// 			x++;
// 		}
// 		y++;
// 	}
// }

void	draw_map(t_game *game)
{
	int		y;
	int		x;
	int		color;
	int		map_width;
	int		map_height;
	int		offset_x;
	int		offset_y;
	char	**map;

	color = 0x0000FF;
	map = game->map;

	// 1. Calculate map height
	map_height = 0;
	while (map[map_height])
		map_height++;

	// 2. Calculate map width (assume all lines are same length)
	map_width = ft_strlen(map[0]);

	// 3. Compute offset to center the map
	// offset_x = (WIDTH - (map_width * BLOCK)) / 2;
	// offset_y = (HEIGHT - (map_height * BLOCK)) / 2;
	offset_x = 0;
	offset_y = 0;

	// 4. Draw the map centered
	y = 0;
	while (map[y])
	{
		x = 0;
		while (map[y][x])
		{
			if (map[y][x] == '1')
				draw_full_squar(x * BLOCK + offset_x, y * BLOCK + offset_y, BLOCK, color, game);
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

void	get_map(t_game *game)
{
	char	**map = malloc(sizeof(char*) * 11);
	map[0] = "111111111111111";
	map[1] = "100000000000001";
	map[2] = "101111111111101";
	map[3] = "100000000000101";
	map[4] = "101111111110101";
	map[5] = "101000000000101";
	map[6] = "101000000000101";
	map[7] = "101111111111101";
	map[8] = "100000000000001";
	map[9] = "111111111111111";
	map[10] = NULL;
	game->map = map;
	map_height(game);
	map_width(game);
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

void	draw_vision(t_game *game)
{
	t_player	*player = &game->player;
	int			x;
	float		fov = PI / 3; // 60 degrees
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