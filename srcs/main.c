/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahakki <ahakki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 23:47:58 by ahakki            #+#    #+#             */
/*   Updated: 2025/08/01 16:09:33 by ahakki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

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

void	draw_circle(int cx, int cy, int radius, int color, t_game *game)
{
	int	x;
	int	y;
	int	distance_squared;
	int	radius_squared = radius * radius;

	y = -radius;
	while (y <= radius)
	{
		x = -radius;
		while (x <= radius)
		{
			distance_squared = x * x + y * y;
			if (distance_squared >= radius_squared - radius && distance_squared <= radius_squared + radius)
				put_pixel(cx + x, cy + y, color, game);
			x++;
		}
		y++;
	}
}
void	draw_aim(int cx, int cy, int radius, int color, t_game *game)
{
	// Draw a central dot
	put_pixel(cx, cy, color, game);

	// Draw small cross lines
	for (int i = 1; i <= radius; i++)
	{
		put_pixel(cx + i, cy, color, game); // right
		put_pixel(cx - i, cy, color, game); // left
		put_pixel(cx, cy + i, color, game); // down
		put_pixel(cx, cy - i, color, game); // up
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
				draw_full_squar(x * MINI_BLOCK, y * MINI_BLOCK, MINI_BLOCK, color, game);
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
	map[0]  = "111111111111111111";
	map[1]  = "1N00000000000000001";
	map[2]  = "1000000000000010001";
	map[3]  = "1000001000000000001";
	map[4]  = "1000010000000000001";
	map[5]  = "1000000000011000001";
	map[6]  = "1000000000100100001";
	map[7]  = "1000000000100000001";
	map[8]  = "1000001000100100001";
	map[9]  = "1000000000011000001";
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
	// char **map = malloc(sizeof(char*) * 11);
	// map[0]  = "111111111111";
	// map[1]  = "1N0000000001";
	// map[2]  = "100000000001";
	// map[3]  = "100000000001";
	// map[4]  = "100000000001";
	// map[5]  = "100000000001";
	// map[6]  = "100000N00001";
	// map[7]  = "100000000001";
	// map[8]  = "100000000001";
	// map[9]  = "111111111111";
	// map[10] = NULL;
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
bool	 touch(int px, int py, t_game *game)
{
	int block_x = px / BLOCK;
	int block_y = py / BLOCK;

	if (block_x < 0 || block_x >= game->map_width || block_y < 0 || block_y >= game->map_height)
		return (true);
	return (game->map[block_y][block_x] == '1');
}
bool	touch2(int px, int py, t_game *game)
{
	int block_x = px / MINI_BLOCK;
	int block_y = py / MINI_BLOCK;

	if (block_x < 0 || block_x >= game->map_width || block_y < 0 || block_y >= game->map_height)
		return (true);
	return (game->map[block_y][block_x] == '1');
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

		float	ray_x = player->x;
		float	ray_y = player->y;
		float	cos_a = cos(ray_angle);
		float	sin_a = sin(ray_angle);

		float	prev_x, prev_y;
		
		int side = -1;
		while (1)
		{
			if (touch(ray_x + cos_a, ray_y, game)) // hit vertical wall
			{
				ray_x += cos_a;
				side = 0;
				break;
			}
			else if (touch(ray_x, ray_y + sin_a, game)) // hit horizontal wall
			{
				ray_y += sin_a;
				side = 1;
				break;
			}
			prev_x = ray_x;
			prev_y = ray_y;
			ray_x += cos_a;
			ray_y += sin_a;
		}

		// Determine wall side
		int color;
		if (side == 0)
		{
			if (cos_a > 0)
				color = 0xA52A2A; // East
			else
				color = 0x008080; // West
		}
		else
		{
			if (sin_a > 0)
				color = 0xDEB887; // South
			else
				color = 0x8A2BE2; // North
		}

		// Correct distance to avoid fish-eye distortion
		float dist = fixed_distance(player->x, ray_x, player->y, ray_y, ray_angle, player->angle);
		float wall_height = (BLOCK / dist) * (WIDTH / 2);
		int start_y = (HEIGHT - wall_height) * player->z_eye;
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
		
		x++;
	}
}

void	draw_minimap(t_game *game)
{
	t_player	*player = &game->player;

	int mini_y = 0;
	while (mini_y < MINI_HEIGHT)
	{
		int mini_x = 0;
		while (mini_x < MINI_WIDTH)
		{
			put_pixel(mini_x, mini_y, 0x000000, game);
			mini_x++;
		}
		mini_y++;
	}
	float	fov = PI / 3;
	float	angle_step = fov / MINI_WIDTH;
	float	ray_angle;
	int		x = 0;

	while (x < MINI_WIDTH)
	{
		ray_angle = player->angle - (fov / 2) + (x * angle_step);

		float	ray_x = player->x / BLOCK * MINI_BLOCK;
		float	ray_y = player->y / BLOCK * MINI_BLOCK;
		float	cos_a = cos(ray_angle) / 2;
		float	sin_a = sin(ray_angle) / 2;
		while (!touch2(ray_x + cos_a, ray_y, game) && !touch2(ray_x, ray_y + sin_a, game))
		{
			put_pixel(ray_x + cos_a, ray_y + sin_a, 0xFFFF00, game);
			ray_x += cos_a;
			ray_y += sin_a;
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
	draw_vision(game);
	draw_aim(WIDTH / 2, HEIGHT / 2, 7, 0x7FFF00, game);
	draw_minimap(game);
	draw_map(game);
	

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