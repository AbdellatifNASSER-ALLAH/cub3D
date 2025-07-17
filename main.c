/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahakki <ahakki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 23:47:58 by ahakki            #+#    #+#             */
/*   Updated: 2025/07/17 12:47:07 by ahakki           ###   ########.fr       */
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
	int		tile_size;
	int		map_width;
	int		map_height;
	int		offset_x;
	int		offset_y;
	char	**map;

	color = 0x0000FF;
	tile_size = 64;
	map = game->map;

	// 1. Calculate map height
	map_height = 0;
	while (map[map_height])
		map_height++;

	// 2. Calculate map width (assume all lines are same length)
	map_width = ft_strlen(map[0]);

	// 3. Compute offset to center the map
	offset_x = (WIDTH - (map_width * tile_size)) / 2;
	offset_y = (HEIGHT - (map_height * tile_size)) / 2;

	// 4. Draw the map centered
	y = 0;
	while (map[y])
	{
		x = 0;
		while (map[y][x])
		{
			if (map[y][x] == '1')
				draw_full_squar(x * tile_size + offset_x, y * tile_size + offset_y, tile_size, color, game);
			x++;
		}
		y++;
	}
}




char **get_map(void)
{
	char	**map = malloc(sizeof(char*) * 11);
	map[0] = "111111111111111";
	map[1] = "100000000000001";
	map[2] = "100000000000001";
	map[3] = "100000000000001";
	map[4] = "100000000000001";
	map[5] = "100000000000001";
	map[6] = "100000000000001";
	map[7] = "100000000000001";
	map[8] = "100000000000001";
	map[9] = "111111111111111";
	map[10] = NULL;
	return (map);
}


void	init_game(t_game *game)
{
	init_player(&game->player);
	game->mlx = mlx_init();
	game->map = get_map();
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

int	draw_loop(t_game *game)
{
	t_player	*player;

	player = &game->player;
	move_player(player);
	clear_img(game);
	draw_map(game);
	draw_squar(player->x, player->y, 15, 0x00FF00, game);
	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
	return (0);
}

int main(int ac, char **av)
{
	t_game	game;
	init_game(&game);
	
	mlx_hook(game.win, 2, 1L<<0, key_press, &game.player);
	mlx_hook(game.win, 3, 1L<<1, key_release, &game.player);

	mlx_loop_hook(game.mlx, draw_loop, &game);

	mlx_loop(game.mlx);
}