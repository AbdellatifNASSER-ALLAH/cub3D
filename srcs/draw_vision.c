/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_vision.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahakki <ahakki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 09:01:41 by ahakki            #+#    #+#             */
/*   Updated: 2025/09/14 09:04:25 by ahakki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

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