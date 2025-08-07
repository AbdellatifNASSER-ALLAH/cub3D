/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdnasse <abdnasse@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 11:19:21 by abdnasse          #+#    #+#             */
/*   Updated: 2025/08/07 06:39:45 by abdnasse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// [*] - init the data
// [*] - validate the file
// [*] - get all lines
// [*] - extract textures and colors TODO: need to be TESTED
// [ ] - parse map

static	void init_config(t_config *cfg, char *path);

void	parse(t_config *cfg, char *path)
{
	init_config(cfg, path);
	valid_file(path, ".cub", cfg);
	read_file(cfg);
	extract_configs(cfg);
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
