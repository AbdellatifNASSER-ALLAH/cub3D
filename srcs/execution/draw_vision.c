/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_vision.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahakki <ahakki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 09:15:50 by ahakki            #+#    #+#             */
/*   Updated: 2025/10/13 23:53:16 by ahakki           ###   ########.fr       */
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

	ceiling_color = (game->config.c_rgb[0] << 16) 
		| (game->config.c_rgb[1] << 8) | game->config.c_rgb[2];
	floor_color = (game->config.f_rgb[0] << 16) 
		| (game->config.f_rgb[1] << 8) | game->config.f_rgb[2];
	y = 0;
	while (y < r->start_y)
		put_pixel(x, y++, ceiling_color, game);
	step = (float)TEXTURE_HEIGHT / r->wall_height;
	while (y < r->end_y && y < HEIGHT)
	{
		tex_y = (int)((y - r->start_y) * step);
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
		r->perp_wall_dist = (r->mapx - r->px + (1 - r->stepx) / 2) / r->ray_dirx;
	else
		r->perp_wall_dist = (r->mapy - r->py + (1 - r->stepy) / 2) / r->ray_diry;
	r->dist = r->perp_wall_dist * BLOCK * cos(r->ray_angle - player->angle);
	if (r->dist < 0.01f)
		r->dist = 0.01f;
	r->wall_height = (BLOCK / r->dist) * (WIDTH / 2);
	r->start_y = (HEIGHT - r->wall_height) * player->z_eye;
	r->end_y = r->start_y + r->wall_height;
}

void	select_color(t_ray *r, t_game *game)
{
	if (game->map[r->wally][r->wallx] == 'D')
		r->color = 0xFFFFFF; // Door
	else if (r->side == 0)
	{
		if (r->ray_dirx > 0)
			r->color = 0xA52A2A; // East wall (right)
		else
			r->color = 0x008080; // West wall (left)
	}
	else
	{
		if (r->ray_diry > 0)
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

void	draw_torch(t_game *game)
{
	t_texture	*torch_tex;
	int			screen_x;
	int			screen_y;
	int			draw_x;
	int			draw_y;
	int			tex_x;
	int			tex_y;
	int			color;

	if (game->player.is_attacking)
		torch_tex = &game->textures[TORCH_ATTACK];
	else
		torch_tex = &game->textures[TORCH];
	screen_x = WIDTH - 256 - 50;
	screen_y = HEIGHT - 256 + 50;
	draw_y = 0;
	while (draw_y < 256 && screen_y + draw_y < HEIGHT)
	{
		draw_x = 0;
		while (draw_x < 256 && screen_x + draw_x < WIDTH)
		{
			tex_x = (draw_x * torch_tex->width) / 256;
			tex_y = (draw_y * torch_tex->height) / 256;
			color = torch_tex->data[tex_y * torch_tex->width + tex_x];
			if (color != 0x000000)
				put_pixel(screen_x + draw_x, screen_y + draw_y, color, game);
			draw_x++;
		}
		draw_y++;
	}
	if (game->player.is_attacking)
	{
		game->player.attack_frame++;
		if (game->player.attack_frame > 15)
		{
			game->player.is_attacking = false;
			game->player.attack_frame = 0;
		}
	}
}
