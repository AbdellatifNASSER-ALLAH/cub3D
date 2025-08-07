/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdnasse <abdnasse@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 02:47:53 by abdnasse          #+#    #+#             */
/*   Updated: 2025/08/07 17:36:39 by abdnasse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	count_lines(t_config *cfg);

void	read_file(t_config *cfg)
{
	char	**lines;
	int	fd;
	int	i;

	lines = ft_calloc(sizeof(char *), count_lines(cfg));
	if (!lines)
		exit_err("read_file: ft_calloc failed !", 1, cfg);
	fd = open(cfg->path, O_RDONLY);
	if (fd < 0)
		exit_err("read_file: open failed !", 1, NULL);
	i = 0;
	while (1)
	{
		lines[i] = get_next_line(fd);
		if (!lines[i++])
			break ;
		printf("%s", lines[i]);
	}
	if (-1 == close(fd))
		exit_err("close: failed ", 1, cfg);
	cfg->lines = lines;
}

static int	count_lines(t_config *cfg)
{
	char	*line;
	int	fd;

	printf("=========count lines =======\n");
	fd = open(cfg->path, O_RDONLY);
	if (fd < 0)
		exit_err("read_file: open failed !", 1, cfg);
	while (1)
	{
		line = get_next_line(fd);
		cfg->nb_lines++;
		if (!line)
			break ;
		printf("%s", line);
		free(line);
	}
	if (-1 == close(fd))
		exit_err("close: failed ", 1, cfg);
	printf("=========count lines : [%d] =======\n", cfg->nb_lines);
	return (cfg->nb_lines);
}
