/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdnasse <abdnasse@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 11:19:21 by abdnasse          #+#    #+#             */
/*   Updated: 2025/08/03 11:45:09 by abdnasse         ###   ########.fr       */
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
	char	**lines;
	
	(void)cfg;
	valid_file(path, ".cub");
	lines = read_file(path);
}

char	**read_file(char *path)
{
	char	**lines;
	int	fd;
	int	i;
	
	fd = open(path, O_RDONLY);
	if (fd < 0)
		exit_err("read_file: open failed !", 1);
	i = 0;
	lines = NULL;
	while (1)
	{
		lines[i++] = get_next_line(fd);
		if (!lines[i])
			break ;
	}
	if (-1 == close(fd))
		exit_err("close: failed ", 1);
	return (lines);
}
