/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahakki <ahakki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 23:47:58 by ahakki            #+#    #+#             */
/*   Updated: 2025/09/14 09:05:50 by ahakki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void    put_pixel(int x, int y, int color, t_game *game);
void    draw_aim(int cx, int cy, int radius, int color, t_game *game);
void    get_player_cord(t_game *game);
void    get_map(t_game *game);
void    init_game(t_game *game);
void    clear_img(t_game *game);
bool    touch(int px, int py, t_game *game);
bool    touch2(int px, int py, t_game *game);
void    draw_stripe(int x, int start_y, int end_y, int color, t_game *game);
void    draw_vision(t_game *game);
void    draw_full_squar(int x, int y, int size, int color, t_game *game);
void    draw_map(t_game *game);
void    draw_minimap(t_game *game);
int     draw_loop(t_game *game);
int     main(int ac, char **av);

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
	map[2]  = "1000000000000010001";
	map[3]  = "100001N100000000001";
	map[4]  = "1000001000000000001";
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

	if (game->map[block_y][block_x] == '1' || game->map[block_y][block_x] == 'D')
		return (true);
	return (false);
}
bool	touch2(int px, int py, t_game *game)
{
	int block_x = (px + (game->player.x / BLOCK * MINI_BLOCK) - MINI_WIDTH / 2) / MINI_BLOCK;
	int block_y = (py + (game->player.y / BLOCK * MINI_BLOCK) - MINI_HEIGHT / 2) / MINI_BLOCK;


	if (game->map[block_y][block_x] == '1' || game->map[block_y][block_x] == 'D')
		return (true);
	return (false);
}
// Handles drawing a vertical stripe
void draw_stripe(int x, int start_y, int end_y, int color, t_game *game)
{
	int y = 0;
	while (y < start_y)
		put_pixel(x, y++, 0x87CEEB, game); // Sky
	while (y < end_y && y < HEIGHT)
		put_pixel(x, y++, color, game);    // Wall
	while (y < HEIGHT)
		put_pixel(x, y++, 0x654321, game); // Floor
}

void draw_vision(t_game *game)
{
	t_player	*player;
	int x;

	x = 0;
	player = &game->player;
	while (x < WIDTH)
	{
		// Calculate ray direction for this column
		float ray_angle;
		float rayDirX;
		float rayDirY;
		int mapX;
		int mapY;
		int side;
		int wallX;
		int wallY;
		int stepX;
		int stepY;
		float perpWallDist;
		float dist;
		float wall_height;
		int start_y;
		int end_y;
		int color;

		ray_angle = player->angle - (FOV / 2) + (x * (FOV / WIDTH));
		rayDirX = cos(ray_angle);
		rayDirY = sin(ray_angle);

		mapX = (int)(player->x) / BLOCK;
		mapY = (int)(player->y) / BLOCK;
		side = -1;
		wallX = mapX;
		wallY = mapY;

		// DDA algorithm (no helper functions, all inline)
		float deltaDistX;
		float deltaDistY;
		float px;
		float py;
		float sideDistX;
		float sideDistY;
		int hit;

		px = player->x / BLOCK;
		py = player->y / BLOCK;
		deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1.0f / rayDirX);
		deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1.0f / rayDirY);

		if (rayDirX < 0)
		{
			stepX = -1;
			sideDistX = (px - mapX) * deltaDistX;
		}
		else
		{
			stepX = 1;
			sideDistX = (mapX + 1.0f - px) * deltaDistX;
		}
		if (rayDirY < 0)
		{
			stepY = -1;
			sideDistY = (py - mapY) * deltaDistY;
		}
		else
		{
			stepY = 1;
			sideDistY = (mapY + 1.0f - py) * deltaDistY;
		}
		hit = 0;
		while (!hit)
		{
			if (sideDistX < sideDistY)
			{
				sideDistX += deltaDistX;
				mapX += stepX;
				side = 0;
			}
			else
			{
				sideDistY += deltaDistY;
				mapY += stepY;
				side = 1;
			}
			if (game->map[mapY][mapX] == '1' || game->map[mapY][mapX] == 'D')
			{
				hit = 1;
				wallX = mapX;
				wallY = mapY;
			}
		}

		// Distance calculation (no helper)
		if (side == 0)
			perpWallDist = (mapX - px + (1 - stepX) / 2) / rayDirX;
		else
			perpWallDist = (mapY - py + (1 - stepY) / 2) / rayDirY;

		dist = perpWallDist * BLOCK * cos(ray_angle - player->angle);
		if (dist < 0.01f)
			dist = 0.01f;

		wall_height = (BLOCK / dist) * (WIDTH / 2);
		start_y = (HEIGHT - wall_height) * player->z_eye;
		end_y = start_y + wall_height;

		// Color selection (no helper)
		if (game->map[wallY][wallX] == 'D')
			color = 0xFFFFFF;
		else if (side == 0)
			color = (rayDirX > 0) ? 0xA52A2A : 0x008080;
		else
			color = (rayDirY > 0) ? 0xDEB887 : 0x8A2BE2;
		draw_stripe(x, start_y, end_y, color, game);
		x++;
	}
}

void draw_full_squar(int x, int y, int size, int color, t_game *game)
{
	int i = 0;
	int center_x = MINI_WIDTH / 2;
	int center_y = MINI_HEIGHT / 2;

	while (i < size)
	{
		int j = 0;
		while (j < size)
		{
			int px = x + j;
			int py = y + i;

			int dist_x = px - center_x;
			int dist_y = py - center_y;

			// Draw pixel only if inside the circle
			if (dist_x * dist_x + dist_y * dist_y <= RADIUS * RADIUS)
				put_pixel(px, py, color, game);
			j++;
		}
		i++;
	}
}

void draw_map(t_game *game)
{
	int	y;
	int	x;
	int	dx;
	int	dy;

	y = 0;
	while (game->map[y])
	{
		x = 0;
		while (game->map[y][x])
		{
			dx = (game->player.x / BLOCK * MINI_BLOCK) - MINI_WIDTH / 2;
			dy = (game->player.y / BLOCK * MINI_BLOCK) - MINI_HEIGHT / 2;
			// Calculate block position on minimap
			int block_x = x * MINI_BLOCK + MINI_BLOCK / 2;
			int block_y = y * MINI_BLOCK + MINI_BLOCK / 2;

			// Calculate distance from center of minimap circle
			int dist_x = block_x - dx - MINI_WIDTH / 2;
			int dist_y = block_y - dy - MINI_WIDTH / 2;

			// Check if block is inside the circle (distance squared <= RADIUS squared)
			if (dist_x * dist_x + dist_y * dist_y <= RADIUS * RADIUS)
			{
				if (game->map[y][x] == '1')
					draw_full_squar((x * MINI_BLOCK) - dx, (y * MINI_BLOCK) - dy, MINI_BLOCK, 0x0000FF, game);
			}
			x++;
		}
		y++;
	}
}

void	draw_minimap(t_game *game)
{
	t_player	*player = &game->player;
	int		x;
	int		y;
	float	ray_angle;
	float	ray_x;
	float	ray_y;

	x = 0;
	while (x < MINI_HEIGHT)
	{
		y = 0;
		while (y < MINI_WIDTH)
		{
			if (sqrt(pow(x - MINI_WIDTH / 2, 2) + pow(y - MINI_HEIGHT / 2, 2)) <= MINI_WIDTH / 2)
				put_pixel(y, x, 0x000000, game);
			y++;
		}
		x++;
	}
	x = 0;
	while (x < MINI_WIDTH)
	{
		ray_angle = player->angle - (PI / 6) + (x * PI / 3 / MINI_WIDTH);
		ray_x = MINI_WIDTH / 2;
		ray_y = MINI_HEIGHT / 2;

		while (!touch2(ray_x + cos(ray_angle), ray_y, game) \
		&& !touch2(ray_x, ray_y + sin(ray_angle), game))
		{
			if (pow(ray_x + cos(ray_angle) - MINI_WIDTH / 2, 2) + pow(ray_y + sin(ray_angle) - MINI_HEIGHT / 2, 2) <= pow(RADIUS, 2))
				put_pixel(ray_x + cos(ray_angle), ray_y + sin(ray_angle), 0xFF0000, game);
			ray_x += cos(ray_angle);
			ray_y += sin(ray_angle);
		}
		x++;
	}
}


int	draw_loop(t_game *game)
{
	move_player(game);
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
	
	if (ac == 2)
		parse(&game.config, av[1]);
	init_game(&game);
	mlx_hook(game.win, 2, 1L << 0, key_press, &game);
	mlx_hook(game.win, 3, 1L << 1, key_release, &game);
	mlx_hook(game.win, 6, 1L << 6, mouse_move, &game);
	mlx_loop_hook(game.mlx, draw_loop, &game);
	mlx_mouse_hide(game.mlx, game.win);

	mlx_loop(game.mlx);
}
