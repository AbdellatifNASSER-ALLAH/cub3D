/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahakki <ahakki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 11:19:21 by abdnasse          #+#    #+#             */
/*   Updated: 2025/11/23 23:53:47 by abdnasse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// [*] - init the data
// [*] - validate the file
// [*] - get all lines
// [*] - extract textures and colors
// [*] - parse and fill the map

static void	init_config(t_config *cfg, char *path);

void	parse(t_config *cfg, char *path)
{
	init_config(cfg, path);
	valid_file(path, ".cub", cfg);
	read_file(cfg);
	extract_configs(cfg, NULL);
	fill_map(cfg->lines, cfg->map_start, cfg->map_end, cfg);
	if (cfg->has_door_in_map && !cfg->door_found)
		exit_err("Map contains door but no door texture defined", 1, cfg);
	return ;
}

static void	init_config(t_config *cfg, char *path)
{
	int	i;

	cfg->path = path;
	cfg->lines = NULL;
	cfg->nb_lines = 0;
	i = 0;
	while (i < 4)
		cfg->tex_found[i++] = 0;
	i = 0;
	while (i < NB_TEX)
		cfg->tex[i++] = NULL;
	cfg->door_found = 0;
	cfg->c_found = 0;
	cfg->f_found = 0;
	cfg->map_start = -1;
	cfg->has_door_in_map = 0;
	return ;
}
