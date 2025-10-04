/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahakki <ahakki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 12:35:00 by ahakki            #+#    #+#             */
/*   Updated: 2025/10/04 12:35:00 by ahakki           ###   ########.fr       */
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

int	get_texture_color(t_ray *r, int tex_y, t_game *game)
{
	int			tex_index;
	int			tex_x;
	float		wall_hit;
	t_texture	*texture;

	tex_index = get_texture_index(r);
	texture = &game->textures[tex_index];
	if (r->side == 0)
		wall_hit = r->py + r->perpWallDist * r->rayDirY;
	else
		wall_hit = r->px + r->perpWallDist * r->rayDirX;
	wall_hit -= (int)wall_hit;
	tex_x = (int)(wall_hit * (float)texture->width);
	if ((r->side == 0 && r->rayDirX > 0) || (r->side == 1 && r->rayDirY < 0))
		tex_x = texture->width - tex_x - 1;
	if (tex_x < 0)
		tex_x = 0;
	if (tex_x >= texture->width)
		tex_x = texture->width - 1;
	if (tex_y < 0)
		tex_y = 0;
	if (tex_y >= texture->height)
		tex_y = texture->height - 1;
	return (texture->data[tex_y * texture->width + tex_x]);
}
