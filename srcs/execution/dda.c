/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahakki <ahakki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 09:50:24 by ahakki            #+#    #+#             */
/*   Updated: 2025/09/26 09:51:53 by ahakki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	init_ray_dir_and_map(t_ray *r, t_player *player, int x)
{
	r->ray_angle = player->angle - (FOV / 2) + (x * (FOV / WIDTH));
	r->rayDirX = cos(r->ray_angle);
	r->rayDirY = sin(r->ray_angle);
	r->mapX = (int)(player->x) / BLOCK;
	r->mapY = (int)(player->y) / BLOCK;
	r->px = player->x / BLOCK;
	r->py = player->y / BLOCK;
}

void	init_ray_delta(t_ray *r)
{
	if (r->rayDirX == 0)
		r->deltaDistX = 1e30;
	else
		r->deltaDistX = fabs(1.0f / r->rayDirX);
	if (r->rayDirY == 0)
		r->deltaDistY = 1e30;
	else
		r->deltaDistY = fabs(1.0f / r->rayDirY);
}

void	init_ray_steps(t_ray *r)
{
	if (r->rayDirX < 0)
	{
		r->stepX = -1;
		r->sideDistX = (r->px - r->mapX) * r->deltaDistX;
	}
	else
	{
		r->stepX = 1;
		r->sideDistX = (r->mapX + 1.0f - r->px) * r->deltaDistX;
	}
	if (r->rayDirY < 0)
	{
		r->stepY = -1;
		r->sideDistY = (r->py - r->mapY) * r->deltaDistY;
	}
	else
	{
		r->stepY = 1;
		r->sideDistY = (r->mapY + 1.0f - r->py) * r->deltaDistY;
	}
}

void	init_ray(t_ray *r, t_player *player, int x)
{
	r->side = -1;
	r->wallX = 0;
	r->wallY = 0;
	init_ray_dir_and_map(r, player, x);
	init_ray_delta(r);
	init_ray_steps(r);
}

void	perform_dda(t_ray *r, t_game *game)
{
	r->hit = 0;
	while (!r->hit)
	{
		if (r->sideDistX < r->sideDistY)
		{
			r->sideDistX += r->deltaDistX;
			r->mapX += r->stepX;
			r->side = 0;
		}
		else
		{
			r->sideDistY += r->deltaDistY;
			r->mapY += r->stepY;
			r->side = 1;
		}
		if (game->map[r->mapY][r->mapX] == '1' || \
			game->map[r->mapY][r->mapX] == 'D')
		{
			r->hit = 1;
			r->wallX = r->mapX;
			r->wallY = r->mapY;
		}
	}
}
