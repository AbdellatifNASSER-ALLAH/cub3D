/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdnasse <abdnasse@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 14:00:20 by abdnasse          #+#    #+#             */
/*   Updated: 2025/11/26 17:49:08 by abdnasse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static char	*find_last_dot(char *p);

void	valid_file(char *path, char *extension, t_config *cfg)
{
	char	*tmp;
	char	bf;
	int		fd;

	tmp = find_last_dot(path);
	if (*(tmp + 4) && *(tmp + 4) == '\n')
		*(tmp + 4) = 0;
	if (!tmp || *(tmp + 4) || ft_strncmp(tmp, extension, 4) != 0)
		exit_err("Invalid extension", 1, cfg);
	fd = open(path, O_DIRECTORY);
	if (fd >= 0)
	{
		if (-1 == close(fd))
			exit_err("close: failed ", 1, cfg);
		exit_err("No such file", 1, cfg);
	}
	fd = open(path, O_RDONLY);
	if (fd < 0)
		exit_err("No such file", 1, cfg);
	if (read(fd, &bf, 1) <= 0)
		exit_err("valid_file: Empty file or read failed", 1, cfg);
	if (-1 == close(fd))
		exit_err("close: failed ", 1, cfg);
}

static char	*find_last_dot(char *p)
{
	char	*tmp;

	tmp = ft_strchr(p, '/');
	while (tmp)
	{
		tmp = ft_strchr(p, '/');
		if (!tmp)
			break ;
		p = tmp + 1;
	}
	return (ft_strchr(p, '.'));
}
