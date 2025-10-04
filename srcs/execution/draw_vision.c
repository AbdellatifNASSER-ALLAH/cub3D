/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_vision.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahakki <ahakki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 09:15:50 by ahakki            #+#    #+#             */
/*   Updated: 2025/10/02 20:13:57 by ahakki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	draw_stripe(int x, t_ray *r, t_game *game)
{
	int		y;
	int		tex_y;
	float	step;
	int		color;
	int		ceiling_color;
	int		floor_color;
	int		wall_center;

	ceiling_color = (game->config.c_rgb[0] << 16) 
		| (game->config.c_rgb[1] << 8) | game->config.c_rgb[2];
	floor_color = (game->config.f_rgb[0] << 16) 
		| (game->config.f_rgb[1] << 8) | game->config.f_rgb[2];
	y = 0;
	while (y < r->start_y)
		put_pixel(x, y++, ceiling_color, game);
	step = (float)TEXTURE_HEIGHT / r->wall_height;
	wall_center = HEIGHT / 2;
	while (y < r->end_y && y < HEIGHT)
	{
		tex_y = (int)((y - wall_center + r->wall_height / 2) * step);
		if (tex_y < 0)
			tex_y = 0;
		if (tex_y >= TEXTURE_HEIGHT)
			tex_y = TEXTURE_HEIGHT - 1;
		color = get_texture_color(r, tex_y, game);
		put_pixel(x, y++, color, game);
	}
	while (y < HEIGHT)
		put_pixel(x, y++, floor_color, game);
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
		r->color = 0xFFFFFF; // Door
	else if (r->side == 0)
	{
		if (r->rayDirX > 0)
			r->color = 0xA52A2A; // East wall (right)
		else
			r->color = 0x008080; // West wall (left)
	}
	else
	{
		if (r->rayDirY > 0)
			r->color = 0xDEB887; // South wall (down)
		else
			r->color = 0x8A2BE2; // North wall (up)
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
		draw_stripe(x, &r, game);
		x++;
	}
}
