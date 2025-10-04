/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copilot <copilot@github.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 00:00:00 by copilot           #+#    #+#             */
/*   Updated: 2025/10/04 00:00:00 by copilot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static void	load_texture(t_game *game, int index, char *path)
{
	t_texture	*tex;
	char		*trimmed_path;
	int			i;

	tex = &game->textures[index];
	i = 0;
	while (path[i] && (path[i] == ' ' || path[i] == '\t'))
		i++;
	trimmed_path = &path[i];
	i = 0;
	while (trimmed_path[i] && trimmed_path[i] != '\n' 
		&& trimmed_path[i] != ' ' && trimmed_path[i] != '\t')
		i++;
	trimmed_path[i] = '\0';
	tex->img = mlx_xpm_file_to_image(game->mlx, trimmed_path,
			&tex->width, &tex->height);
	if (!tex->img)
	{
		printf("Error loading texture: %s\n", trimmed_path);
		exit(1);
	}
	tex->data = (int *)mlx_get_data_addr(tex->img, &i, &i, &i);
}

void	load_textures(t_game *game)
{
	load_texture(game, NORTH, game->config.tex[NORTH]);
	load_texture(game, SOUTH, game->config.tex[SOUTH]);
	load_texture(game, EAST, game->config.tex[EAST]);
	load_texture(game, WEST, game->config.tex[WEST]);
}

int	get_texture_color(t_ray *r, int tex_y, t_game *game)
{
	t_texture	*tex;
	int			tex_index;
	int			tex_x;
	float		wall_x;

	if (r->side == 0)
	{
		if (r->rayDirX > 0)
			tex_index = EAST;
		else
			tex_index = WEST;
		wall_x = r->py + r->perpWallDist * r->rayDirY;
	}
	else
	{
		if (r->rayDirY > 0)
			tex_index = SOUTH;
		else
			tex_index = NORTH;
		wall_x = r->px + r->perpWallDist * r->rayDirX;
	}
	wall_x -= (int)wall_x;
	tex = &game->textures[tex_index];
	tex_x = (int)(wall_x * (float)tex->width);
	if ((r->side == 0 && r->rayDirX < 0) || (r->side == 1 && r->rayDirY > 0))
		tex_x = tex->width - tex_x - 1;
	return (tex->data[tex_y * tex->width + tex_x]);
}
