/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_vision.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahakki <ahakki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 09:15:50 by ahakki            #+#    #+#             */
/*   Updated: 2025/09/26 09:55:02 by ahakki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	draw_stripe(int x, t_ray *r, t_game *game)
{
	int	y;

	y = 0;
	while (y < r->start_y)
		put_pixel(x, y++, 0x87CEEB, game);
	while (y < r->end_y && y < HEIGHT)
		put_pixel(x, y++, r->color, game);
	while (y < HEIGHT)
		put_pixel(x, y++, 0x654321, game);
}


void	calc_dist_and_height(t_ray *r, t_player *player)
{
	if (r->side == 0)
		r->perpWallDist = (r->mapX - r->px + (1 - r->stepX) / 2) / r->rayDirX;
	else
		r->perpWallDist = (r->mapY - r->py + (1 - r->stepY) / 2) / r->rayDirY;
	r->dist = r->perpWallDist * BLOCK * cos(r->ray_angle - player->angle);
	if (r->dist < 0.01f)
		r->dist = 0.01f;
	r->wall_height = (BLOCK / r->dist) * (WIDTH / 2);
	r->start_y = (HEIGHT - r->wall_height) * player->z_eye;
	r->end_y = r->start_y + r->wall_height;
}

void	select_color(t_ray *r, t_game *game)
{
	if (game->map[r->wallY][r->wallX] == 'D')
		r->color = NB_TEX; // Use NB_TEX or define a specific enum for DOOR if needed
	else if (r->side == 0)
	{
		if (r->rayDirX > 0)
			r->color = WEST;
		else
			r->color = EAST;
	}
	else
	{
		if (r->rayDirY > 0)
			r->color = SOUTH;
		else
			r->color = NORTH;
	}
}

void	draw_vision(t_game *game)
{
	t_player	*player;
	int			x;
	t_ray		r;

	x = 0;
	player = &game->player;
	while (x < WIDTH)
	{
		init_ray(&r, player, x);
		perform_dda(&r, game);
		calc_dist_and_height(&r, player);
		select_color(&r, game);
		draw_stripe(x, &r, game);
		x++;
	}
}