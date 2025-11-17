/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_vision.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahakki <ahakki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 09:15:50 by ahakki            #+#    #+#             */
/*   Updated: 2025/11/17 06:26:46 by ahakki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	draw_stripe(int x, t_ray *r, t_game *game)
{
	int		y;
	int		tex_y;
	float	step;
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
		put_pixel(x, y++, get_texture_color(r, tex_y, game), game);
	}
	while (y < HEIGHT)
		put_pixel(x, y++, floor_color, game);
}

void	calc_dist_and_height(t_ray *r, t_player *player)
{
	if (r->side == 0)
		r->perp_wall_dist = (r->mapx - r->px
				+ (1 - r->stepx) / 2) / r->ray_dirx;
	else
		r->perp_wall_dist = (r->mapy - r->py
				+ (1 - r->stepy) / 2) / r->ray_diry;
	r->dist = r->perp_wall_dist * BLOCK * cos(r->ray_angle - player->angle);
	if (r->dist < 0.01f)
		r->dist = 0.01f;
	r->wall_height = (BLOCK / r->dist) * (HEIGHT / 2);
	r->start_y = (HEIGHT - r->wall_height) * player->z_eye;
	r->end_y = r->start_y + r->wall_height;
}

void	select_color(t_ray *r, t_game *game)
{
	if (game->map[r->wally][r->wallx] == 'D')
		r->color = 0xFFFFFF;
	else if (r->side == 0)
	{
		if (r->ray_dirx > 0)
			r->color = 0xA52A2A;
		else
			r->color = 0x008080;
	}
	else
	{
		if (r->ray_diry > 0)
			r->color = 0xDEB887;
		else
			r->color = 0x8A2BE2;
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

static void	draw_torch_line(t_texture *tex, int screen_x, int screen_y,
				int draw_y, t_game *game)
{
	int	draw_x;
	int	tex_x;
	int	tex_y;
	int	color;

	draw_x = 0;
	while (draw_x < TORCH_SIZE && screen_x + draw_x < WIDTH)
	{
		tex_x = (draw_x * tex->width) / TORCH_SIZE;
		tex_y = (draw_y * tex->height) / TORCH_SIZE;
		color = tex->data[tex_y * tex->width + tex_x];
		if (color != TRANSPARENT_COLOR)
			put_pixel(screen_x + draw_x, screen_y + draw_y, color, game);
		draw_x++;
	}
}

static void	update_attack_animation(t_game *game)
{
	if (game->player.is_attacking)
	{
		game->player.attack_frame++;
		if (game->player.attack_frame > ATTACK_ANIMATION_FRAMES)
		{
			game->player.is_attacking = false;
			game->player.attack_frame = 0;
		}
	}
}

void	draw_torch(t_game *game)
{
	t_texture	*torch_tex;
	int			screen_x;
	int			screen_y;
	int			draw_y;

	if (game->player.is_attacking)
		torch_tex = &game->textures[TORCH_ATTACK];
	else
		torch_tex = &game->textures[TORCH];
	screen_x = WIDTH - TORCH_SIZE - TORCH_OFFSET_X;
	screen_y = HEIGHT - TORCH_SIZE + TORCH_OFFSET_Y;
	draw_y = 0;
	while (draw_y < TORCH_SIZE && screen_y + draw_y < HEIGHT)
	{
		draw_torch_line(torch_tex, screen_x, screen_y, draw_y, game);
		draw_y++;
	}
	update_attack_animation(game);
}
