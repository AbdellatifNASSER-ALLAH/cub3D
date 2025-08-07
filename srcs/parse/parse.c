/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdnasse <abdnasse@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 11:19:21 by abdnasse          #+#    #+#             */
/*   Updated: 2025/08/07 03:19:03 by abdnasse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// [*] - validate the file
// [ ] - get all lines
// [ ] - validate the configuration
// [ ] - extract textures and colors
// [ ] - parse map

void	parse(t_config *cfg, char *path)
{
	cfg->path = path;
	valid_file(path, ".cub");
	read_file(cfg);
	extract_configs(cfg);
	return ;
}

