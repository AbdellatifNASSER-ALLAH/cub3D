/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdnasse <abdnasse@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 09:28:31 by abdnasse          #+#    #+#             */
/*   Updated: 2025/10/16 09:40:29 by abdnasse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static char	*get_frame_path(char *base_path, int frame_num)
{
	char	*frame_path;
	char	*num_str;
	char	*temp;

	num_str = ft_itoa(frame_num);
	if (!num_str)
		return (NULL);
	if (frame_num < 10)
		temp = ft_strjoin(base_path, "_0");
	else
		temp = ft_strjoin(base_path, "_");
	if (!temp)
		return (free(num_str), NULL);
	frame_path = ft_strjoin(temp, num_str);
	free(temp);
	free(num_str);
	if (!frame_path)
		return (NULL);
	temp = ft_strjoin(frame_path, ".xpm");
	free(frame_path);
	return (temp);
}

static int	count_door_frames(char *base_path, void *mlx)
{
	char	*frame_path;
	void	*test_img;
	int		count;
	int		w;
	int		h;

	count = 0;
	while (count < MAX_DOOR_FRAMES)
	{
		frame_path = get_frame_path(base_path, count);
		if (!frame_path)
			break ;
		test_img = mlx_xpm_file_to_image(mlx, frame_path, &w, &h);
		free(frame_path);
		if (!test_img)
			break ;
		mlx_destroy_image(mlx, test_img);
		count++;
	}
	return (count > 0 ? count : 1);
}

void	load_door_textures(t_game *game)
{
	t_texture	*tex;
	char		*frame_path;
	char		*trimmed_base;
	int			i;
	int			bpp;
	int			j;

	tex = &game->textures[DOOR];
	i = 0;
	while (game->config.tex[DOOR][i] && (game->config.tex[DOOR][i] == ' ' 
		|| game->config.tex[DOOR][i] == '\t'))
		i++;
	trimmed_base = &game->config.tex[DOOR][i];
	j = 0;
	while (trimmed_base[j] && trimmed_base[j] != '\n' 
		&& trimmed_base[j] != ' ' && trimmed_base[j] != '\t')
		j++;
	trimmed_base[j] = '\0';
	game->config.door_frame_count = count_door_frames(trimmed_base, game->mlx);
	i = 0;
	while (i < game->config.door_frame_count)
	{
		frame_path = get_frame_path(trimmed_base, i);
		if (!frame_path && i == 0)
			frame_path = ft_strdup(trimmed_base);
		if (!frame_path)
			break ;
		tex->frames[i] = mlx_xpm_file_to_image(game->mlx, frame_path,
				&tex->width, &tex->height);
		free(frame_path);
		if (!tex->frames[i])
			break ;
		tex->frame_data[i] = (int *)mlx_get_data_addr(tex->frames[i],
				&bpp, &bpp, &bpp);
		i++;
	}
	tex->frame_count = i;
	if (tex->frame_count > 0)
	{
		tex->img = tex->frames[0];
		tex->data = tex->frame_data[0];
	}
}

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
	if (game->config.door_found)
		load_door_textures(game);
}

int	get_texture_color(t_ray *r, int tex_y, t_game *game)
{
	t_texture	*tex;
	int			tex_index;
	int			tex_x;
	float		wall_x;
	t_door		*door;

	tex_index = -1;
	if (game->map[r->wally][r->wallx] == 'D' && game->config.door_found)
	{
		tex_index = DOOR;
		door = get_door_at(game, r->wallx, r->wally);
		if (door && door->current_frame < game->textures[DOOR].frame_count)
		{
			tex = &game->textures[tex_index];
			if (r->side == 0)
				wall_x = r->py + r->perp_wall_dist * r->ray_diry;
			else
				wall_x = r->px + r->perp_wall_dist * r->ray_dirx;
			wall_x -= (int)wall_x;
			tex_x = (int)(wall_x * (float)tex->width);
			if ((r->side == 0 && r->ray_dirx < 0) || 
				(r->side == 1 && r->ray_diry > 0))
				tex_x = tex->width - tex_x - 1;
			return (tex->frame_data[door->current_frame]
				[tex_y * tex->width + tex_x]);
		}
		else if (game->textures[DOOR].frame_count > 0)
		{
			tex = &game->textures[tex_index];
			if (r->side == 0)
				wall_x = r->py + r->perp_wall_dist * r->ray_diry;
			else
				wall_x = r->px + r->perp_wall_dist * r->ray_dirx;
			wall_x -= (int)wall_x;
			tex_x = (int)(wall_x * (float)tex->width);
			if ((r->side == 0 && r->ray_dirx < 0) || 
				(r->side == 1 && r->ray_diry > 0))
				tex_x = tex->width - tex_x - 1;
			return (tex->frame_data[0][tex_y * tex->width + tex_x]);
		}
	}
	if (tex_index == -1)
	{
		if (r->side == 0)
		{
			if (r->ray_dirx > 0)
				tex_index = EAST;
			else
				tex_index = WEST;
		}
		else
		{
			if (r->ray_diry > 0)
				tex_index = SOUTH;
			else
				tex_index = NORTH;
		}
	}
	if (r->side == 0)
		wall_x = r->py + r->perp_wall_dist * r->ray_diry;
	else
		wall_x = r->px + r->perp_wall_dist * r->ray_dirx;
	wall_x -= (int)wall_x;
	tex = &game->textures[tex_index];
	tex_x = (int)(wall_x * (float)tex->width);
	if ((r->side == 0 && r->ray_dirx < 0) || (r->side == 1 && r->ray_diry > 0))
		tex_x = tex->width - tex_x - 1;
	return (tex->data[tex_y * tex->width + tex_x]);
}
