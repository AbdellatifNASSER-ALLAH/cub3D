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

static int	rgb_to_color(int *rgb)
{
	return ((rgb[0] << 16) | (rgb[1] << 8) | rgb[2]);
}

void	draw_stripe(int x, t_ray *r, t_game *game)
{
	int		y;
	int		tex_y;
	float	step;
	float	tex_pos;
	int		color;
	int		ceiling_color;
	int		floor_color;

	ceiling_color = rgb_to_color(game->config.c_rgb);
	floor_color = rgb_to_color(game->config.f_rgb);
	y = 0;
	while (y < r->start_y)
		put_pixel(x, y++, ceiling_color, game);
	step = (float)TEXTURE_HEIGHT / r->wall_height;
	tex_pos = (r->start_y - (HEIGHT - r->wall_height) * game->player.z_eye) * step;
	while (y < r->end_y && y < HEIGHT)
	{
		tex_y = (int)tex_pos;
		tex_pos += step;
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
		prepare_texture_rendering(&r, player);
		draw_stripe(x, &r, game);
		x++;
	}
}
