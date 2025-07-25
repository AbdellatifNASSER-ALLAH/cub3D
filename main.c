/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahakki <ahakki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 23:47:58 by ahakki            #+#    #+#             */
/*   Updated: 2025/07/25 11:55:53 by ahakki           ###   ########.fr       */
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
				// int draw_x = (x * BLOCK) - game->player.x + CAMERA_X;
				// int draw_y = (y * BLOCK) - game->player.y + CAMERA_Y;	
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
		if ((int)ft_strlen(game->map[i]) > game->map_width)
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
	char **map = malloc(sizeof(char*) * 20);
	map[0]  = "1111111111111111111";
	map[1]  = "1000000000000000001";
	map[2]  = "1000001000000010001";
	map[3]  = "1000000000000000001";
	map[4]  = "1000010000001000001";
	map[5]  = "1000000000000000001";
	map[6]  = "1000000010000000001";
	map[7]  = "10000000000N0000001";
	map[8]  = "1000001000000000001";
	map[9]  = "1000000000000000001";
	map[10] = "1000100000000000001";
	map[11] = "1000000010000000001";
	map[12] = "1000000000000100001";
	map[13] = "1000010000000000001";
	map[14] = "1000000000000000001";
	map[15] = "1000000000010000001";
	map[16] = "1000000000000100001";
	map[17] = "1000000000000000001";
	map[18] = "1111111111111111111";
	map[19] = NULL;
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
	int tile_x = px / BLOCK;
	int tile_y = py / BLOCK;

	if (tile_x < 0 || tile_x >= game->map_width || tile_y < 0 || tile_y >= game->map_height)
		return (true);
	return (game->map[tile_y][tile_x] == '1');
}

float	distance(float x, float y)
{
	return (sqrt(x * x + y * y));
}

float	fixed_distance(float x1, float x2, float y1, float y2, float ray_angle, float player_angle)
{
	float dx = x2 - x1;
	float dy = y2 - y1;
	float raw_dist = distance(dx, dy);
	float angle_diff = ray_angle - player_angle;
	return (raw_dist * cos(angle_diff));
}

void	draw_vision(t_game *game)
{
	t_player	*player = &game->player;
	float		fov = PI / 3;
	float		angle_step = fov / WIDTH;
	float		ray_angle;
	int			x = 0;

	while (x < WIDTH)
	{
		ray_angle = player->angle - (fov / 2) + (x * angle_step);

		float	ray_x = player->x + (PLAYER_SIZE / 2);
		float	ray_y = player->y + (PLAYER_SIZE / 2);
		float	cos_a = cos(ray_angle);
		float	sin_a = sin(ray_angle);

		float	prev_x, prev_y;

		while (!touch(ray_x, ray_y, game))
		{
			prev_x = ray_x;
			prev_y = ray_y;
			ray_x += cos_a;
			ray_y += sin_a;
		}

		// Determine wall side
		int color;
		if ((int)(prev_x / BLOCK) != (int)(ray_x / BLOCK)) // Vertical hit (East or West)
		{
			if (ray_x > prev_x)
				color = 0xA52A2A; // East wall - Brown
			else
				color = 0x008080; // West wall - Teal
		}
		else // Horizontal hit (North or South)
		{
			if (ray_y > prev_y)
				color = 0xDEB887; // South wall - BurlyWood
			else
				color = 0x8A2BE2; // North wall - BlueViolet
		}

		// Correct distance to avoid fish-eye distortion
		float dist = fixed_distance(player->x, ray_x, player->y, ray_y, ray_angle, player->angle);
		float wall_height = (BLOCK / dist) * (WIDTH / 2);
		int start_y = (HEIGHT - wall_height) / 2;
		int end_y = start_y + wall_height;

		
		int y = 0;
		// Draw sky (top half above wall)
		while (y < start_y)
			put_pixel(x, y++, 0x87CEEB, game); // Sky blue
		
		// Draw wall
		while (y < end_y && y < HEIGHT)
			put_pixel(x, y++, color, game);
		
		// Draw floor (bottom half below wall)
		while (y < HEIGHT)
			put_pixel(x, y++, 0x654321, game); // Dark brown floor

		// while (start_y < end_y)
		// 	put_pixel(x, start_y++, color, game);
		
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

	(void)ac;
	(void)av;
	mlx_hook(game.win, 2, 1L<<0, key_press, &game);
	mlx_hook(game.win, 3, 1L<<1, key_release, &game);
	// mlx_hook(game.win, 6, 1L << 6, mouse_move, &game);
	mlx_loop_hook(game.mlx, draw_loop, &game);
	// mlx_mouse_hide(game.mlx, game.win);

	mlx_loop(game.mlx);
}