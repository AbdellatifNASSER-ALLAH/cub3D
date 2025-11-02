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

static void	load_texture_direct(t_game *game, int index, const char *path)
{
	t_texture	*tex;
	int			i;

	tex = &game->textures[index];
	tex->img = mlx_xpm_file_to_image(game->mlx, (char *)path,
			&tex->width, &tex->height);
	if (!tex->img)
	{
		printf("Error loading texture: %s\n", path);
		exit(1);
	}
	tex->data = (int *)mlx_get_data_addr(tex->img, &i, &i, &i);
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
		load_texture(game, DOOR, game->config.tex[DOOR]);
	load_texture_direct(game, TORCH, "./textures/torch/torch1_wall.xpm");
}

int	get_texture_color(t_ray *r, int tex_y, t_game *game)
{
	t_texture	*tex;
	int			tex_index;
	int			tex_x;
	float		wall_x;

	if (game->map[r->wally][r->wallx] == 'D' && game->config.door_found)
		tex_index = DOOR;
	else if (r->side == 0)
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

void	draw_torch(t_game *game)
{
	t_texture	*torch;
	int			screen_x;
	int			screen_y;
	int			tex_x;
	int			tex_y;
	int			color;
	int			sprite_width;
	int			sprite_height;
	int			start_x;
	int			start_y;

	torch = &game->textures[TORCH];
	sprite_width = torch->width * 2;
	sprite_height = torch->height * 2;
	start_x = (WIDTH - sprite_width) / 2;
	start_y = HEIGHT - sprite_height;
	screen_y = 0;
	while (screen_y < sprite_height && (start_y + screen_y) < HEIGHT)
	{
		screen_x = 0;
		while (screen_x < sprite_width && (start_x + screen_x) < WIDTH)
		{
			tex_x = screen_x * torch->width / sprite_width;
			tex_y = screen_y * torch->height / sprite_height;
			if (tex_x >= 0 && tex_x < torch->width 
				&& tex_y >= 0 && tex_y < torch->height)
			{
				color = torch->data[tex_y * torch->width + tex_x];
				put_pixel(start_x + screen_x, start_y + screen_y, 
					color, game);
			}
			screen_x++;
		}
		screen_y++;
	}
}
