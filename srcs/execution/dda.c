/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahakki <ahakki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 09:50:24 by ahakki            #+#    #+#             */
/*   Updated: 2025/11/12 15:50:12 by ahakki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	init_ray_dir_and_map(t_ray *r, t_player *player, int x)
{
	r->ray_angle = player->angle - (FOV / 2) + (x * (FOV / WIDTH));
	r->ray_dirx = cos(r->ray_angle);
	r->ray_diry = sin(r->ray_angle);
	r->mapx = (int)(player->x) / BLOCK;
	r->mapy = (int)(player->y) / BLOCK;
	r->px = player->x / BLOCK;
	r->py = player->y / BLOCK;
}

void	init_ray_delta(t_ray *r)
{
	if (r->ray_dirx == 0)
		r->delta_distx = 1e30;
	else
		r->delta_distx = fabs(1.0f / r->ray_dirx);
	if (r->ray_diry == 0)
		r->delta_disty = 1e30;
	else
		r->delta_disty = fabs(1.0f / r->ray_diry);
}

void	init_ray_steps(t_ray *r)
{
	if (r->ray_dirx < 0)
	{
		r->stepx = -1;
		r->side_distx = (r->px - r->mapx) * r->delta_distx;
	}
	else
	{
		r->stepx = 1;
		r->side_distx = (r->mapx + 1.0f - r->px) * r->delta_distx;
	}
	if (r->ray_diry < 0)
	{
		r->stepy = -1;
		r->side_disty = (r->py - r->mapy) * r->delta_disty;
	}
	else
	{
		r->stepy = 1;
		r->side_disty = (r->mapy + 1.0f - r->py) * r->delta_disty;
	}
}

void	init_ray(t_ray *r, t_player *player, int x)
{
	r->side = -1;
	r->wallx = 0;
	r->wally = 0;
	init_ray_dir_and_map(r, player, x);
	init_ray_delta(r);
	init_ray_steps(r);
}

void	perform_dda(t_ray *r, t_game *game)
{
	r->hit = 0;
	while (!r->hit)
	{
		if (r->side_distx < r->side_disty)
		{
			r->side_distx += r->delta_distx;
			r->mapx += r->stepx;
			r->side = 0;
		}
		else
		{
			r->side_disty += r->delta_disty;
			r->mapy += r->stepy;
			r->side = 1;
		}
		if (game->map[r->mapy][r->mapx] == '1' || \
game->map[r->mapy][r->mapx] == 'D')
		{
			r->hit = 1;
			r->wallx = r->mapx;
			r->wally = r->mapy;
		}
	}
}
