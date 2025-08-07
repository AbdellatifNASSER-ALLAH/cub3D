/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdnasse <abdnasse@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 02:47:53 by abdnasse          #+#    #+#             */
/*   Updated: 2025/08/07 06:29:00 by abdnasse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	read_file(t_config *cfg)
{
	char	**lines;
	int	fd;
	
	lines = cfg->lines;	
	fd = open(cfg->path, O_RDONLY);
	if (fd < 0)
		exit_err("read_file: open failed !", 1, NULL);
	while (1)
	{
		lines[cfg->nb_lines++] = get_next_line(fd);
		if (!lines[cfg->nb_lines])
			break ;
	}
	if (-1 == close(fd))
		exit_err("close: failed ", 1, cfg);
}
