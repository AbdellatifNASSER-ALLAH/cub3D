/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahakki <ahakki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 11:30:00 by ahakki            #+#    #+#             */
/*   Updated: 2025/10/04 11:30:00 by ahakki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static int	get_texture_index(t_ray *r)
{
	if (r->side == 0)
	{
		if (r->rayDirX > 0)
			return (EAST);
		else
			return (WEST);
	}
	else
	{
		if (r->rayDirY > 0)
			return (SOUTH);
		else
			return (NORTH);
	}
}

static void	calculate_wall_hit_x(t_ray *r, t_player *player)
{
	if (r->side == 0)
		r->wallHitX = player->y + r->perpWallDist * r->rayDirY;
	else
		r->wallHitX = player->x + r->perpWallDist * r->rayDirX;
	r->wallHitX -= floor(r->wallHitX);
}

int	get_texture_color(t_ray *r, int tex_y, t_game *game)
{
	int			tex_index;
	int			tex_x;
	int			color;
	t_texture	*texture;

	tex_index = get_texture_index(r);
	texture = &game->textures[tex_index];
	tex_x = (int)(r->wallHitX * (float)texture->width);
	if ((r->side == 0 && r->rayDirX < 0) || (r->side == 1 && r->rayDirY > 0))
		tex_x = texture->width - tex_x - 1;
	if (tex_x < 0)
		tex_x = 0;
	if (tex_x >= texture->width)
		tex_x = texture->width - 1;
	if (tex_y < 0)
		tex_y = 0;
	if (tex_y >= texture->height)
		tex_y = texture->height - 1;
	color = texture->data[tex_y * texture->width + tex_x];
	return (color);
}

void	prepare_texture_rendering(t_ray *r, t_player *player)
{
	calculate_wall_hit_x(r, player);
}
