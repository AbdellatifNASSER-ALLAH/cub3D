/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahakki <ahakki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 07:41:45 by ahakki            #+#    #+#             */
/*   Updated: 2025/10/13 23:55:37 by ahakki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"


bool	touch2(int px, int py, t_game *game)
{
	int	block_x;
	int	block_y;

	block_x = (px + (game->player.x / BLOCK * MINI_BLOCK)
			- MINI_WIDTH / 2) / MINI_BLOCK;
	block_y = (py + (game->player.y / BLOCK * MINI_BLOCK)
			- MINI_HEIGHT / 2) / MINI_BLOCK;
	if (game->map[block_y][block_x] == '1' || \
		game->map[block_y][block_x] == 'D' || \
		game->map[block_y][block_x] == 'T')
		return (true);
	return (false);
}


void	draw_map_block(int x, int y, int dx, int dy, t_game *game)
{
	int	block_x;
	int	block_y;
	int	dist_x;
	int	dist_y;

	block_x = x * MINI_BLOCK + MINI_BLOCK / 2;
	block_y = y * MINI_BLOCK + MINI_BLOCK / 2;
	dist_x = block_x - dx - MINI_WIDTH / 2;
	dist_y = block_y - dy - MINI_WIDTH / 2;
	if (dist_x * dist_x + dist_y * dist_y <= RADIUS * RADIUS)
	{
		if (game->map[y][x] == '1')
			draw_full_squar((x * MINI_BLOCK) - dx,
				(y * MINI_BLOCK) - dy, MINI_BLOCK, game);
	}
}

void	draw_map(t_game *game)
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
			draw_map_block(x, y, dx, dy, game);
			x++;
		}
		y++;
	}
}

void	draw_minimap_rays(t_game *game, t_player *player)
{
	int		x;
	float	ray_angle;
	float	ray_x;
	float	ray_y;

	x = 0;
	while (x < MINI_WIDTH)
	{
		ray_angle = player->angle - (PI / 6) + (x * PI / 3 / MINI_WIDTH);
		ray_x = MINI_WIDTH / 2;
		ray_y = MINI_HEIGHT / 2;
		while (!touch2(ray_x + cos(ray_angle), ray_y, game)
			&& !touch2(ray_x, ray_y + sin(ray_angle), game))
		{
			if (pow(ray_x + cos(ray_angle) - MINI_WIDTH / 2, 2)
				+ pow(ray_y + sin(ray_angle) - MINI_HEIGHT / 2, 2)
				<= pow(RADIUS, 2))
				put_pixel(ray_x + cos(ray_angle),
					ray_y + sin(ray_angle), 0xFF0000, game);
			ray_x += cos(ray_angle);
			ray_y += sin(ray_angle);
		}
		x += MINI_WIDTH - 1;
	}
}

void	draw_minimap(t_game *game)
{
	t_player	*player;
	int	x;
	int	y;

	player = &game->player;

	x = 0;
	while (x < MINI_HEIGHT)
	{
		y = 0;
		while (y < MINI_WIDTH)
		{
			if (sqrt(pow(x - MINI_WIDTH / 2, 2)
					+ pow(y - MINI_HEIGHT / 2, 2)) <= MINI_WIDTH / 2)
				put_pixel(y, x, 0x000000, game);
			y++;
		}
		x++;
	}
	draw_minimap_rays(game, player);
}
