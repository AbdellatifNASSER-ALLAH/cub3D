/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahakki <ahakki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 11:19:21 by abdnasse          #+#    #+#             */
/*   Updated: 2025/09/17 11:18:24 by abdnasse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// [*] - init the data
// [*] - validate the file
// [*] - get all lines
// [*] - extract textures and colors
// [ ] - parse and fill the map

static	void init_config(t_config *cfg, char *path);

void	parse(t_config *cfg, char *path)
{
	init_config(cfg, path);
	valid_file(path, ".cub", cfg);
	read_file(cfg);
	extract_configs(cfg);
//	fill_map(cfg->map, cfg->map_start, cfg->map_end, cfg);
	return ;
}

static	void init_config(t_config *cfg, char *path)
{
	int	i;

	cfg->path = path;
	cfg->lines = NULL;
	cfg->nb_lines = 0;
	i = 0;
	while (i < 4)
		cfg->tex_found[i++] = 0;
	cfg->c_found = 0;
	cfg->f_found = 0;
	cfg->map_start = 0;
	return ;
}
