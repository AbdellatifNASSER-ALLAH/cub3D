/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahakki <ahakki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 09:28:31 by abdnasse          #+#    #+#             */
/*   Updated: 2025/11/05 17:41:14 by ahakki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static	void	load_texture(t_game *game, int index, char *path);

void	load_textures(t_game *game)
{
	load_texture(game, NORTH, game->config.tex[NORTH]);
	load_texture(game, SOUTH, game->config.tex[SOUTH]);
	load_texture(game, EAST, game->config.tex[EAST]);
	load_texture(game, WEST, game->config.tex[WEST]);
	load_texture(game, TORCH, "./textures/torch.xpm");
	load_texture(game, TORCH_ATTACK, "./textures/attack_torch.xpm");
	if (game->config.door_found)
		load_texture(game, DOOR, game->config.tex[DOOR]);
}

static void	load_texture(t_game *game, int index, char *path)
{
	t_texture	*tex;
	char		*trimmed_path;
	char		*clean_path;
	int			i;
	int			start;

	tex = &game->textures[index];
	start = 0;
	while (path[start] && (path[start] == ' ' || path[start] == '\t'))
		start++;
	trimmed_path = &path[start];
	i = 0;
	while (trimmed_path[i] && trimmed_path[i] != '\n' && trimmed_path[i] != ' '
		&& trimmed_path[i] != '\t')
		i++;
	clean_path = ft_substr(trimmed_path, 0, i);
	if (!clean_path)
		exit_err("Failed to allocate memory for texture path", 1, &game->config);
	tex->img = mlx_xpm_file_to_image(game->mlx, clean_path, &tex->width,
			&tex->height);
	if (!tex->img)
	{
		printf("Error loading texture: %s\n", clean_path);
		free(clean_path);
		exit(1);
	}
	free(clean_path);
	tex->data = (int *)mlx_get_data_addr(tex->img, &i, &i, &i);
}

static int	get_texture_index(t_ray *r, t_game *game)
{
	int	tex_index;

	if (game->map[r->wally][r->wallx] == 'D' && game->config.door_found)
		tex_index = DOOR;
	else if (r->side == 0)
	{
		tex_index = WEST;
		if (r->ray_dirx > 0)
			tex_index = EAST;
	}
	else
	{
		tex_index = NORTH;
		if (r->ray_diry > 0)
			tex_index = SOUTH;
	}
	return (tex_index);
}

static int	calculate_tex_x(t_ray *r, t_texture *tex)
{
	float	wall_x;
	int		tex_x;

	if (r->side == 0)
		wall_x = r->py + r->perp_wall_dist * r->ray_diry;
	else
		wall_x = r->px + r->perp_wall_dist * r->ray_dirx;
	wall_x -= (int)wall_x;
	tex_x = (int)(wall_x * (float)tex->width);
	if ((r->side == 0 && r->ray_dirx < 0) || (r->side == 1 && r->ray_diry > 0))
		tex_x = tex->width - tex_x - 1;
	return (tex_x);
}

int	get_texture_color(t_ray *r, int tex_y, t_game *game)
{
	t_texture	*tex;
	int			tex_index;
	int			tex_x;

	tex_index = get_texture_index(r, game);
	tex = &game->textures[tex_index];
	tex_x = calculate_tex_x(r, tex);
	return (tex->data[tex_y * tex->width + tex_x]);
}
