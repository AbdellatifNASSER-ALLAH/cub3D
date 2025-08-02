/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdnasse <abdnasse@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 14:00:20 by abdnasse          #+#    #+#             */
/*   Updated: 2025/08/02 15:06:38 by abdnasse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// Here we are checking for a valide .cub file 
// 	The extension .cub
// 	Shouldn't be a directory 
// 	The existence of the file
// 	Shouldn't be empty file 

void	valid_file(char *path)
{
	char	*tmp;
	int	fd;

	tmp = ft_strchr(path, '.');
	if (!tmp || ft_strncmp(tmp, ".cub", 4) != 0)
		exit_err("Invalid extension", 1);
	fd = open(path, O_DIRECTORY);
	if (fd >= 0)
	{
		if (-1 == close(fd))
			exit_err("close: failed ", 1);
		exit_err("No such file", 1);
	}
	fd = open(path, O_RDONLY);
	if (fd < 0)
		exit_err("No such file", 1);
	if (read(fd, tmp, 1) <= 0)
		exit_err("Empty file or read failed", 1);
	if (-1 == close(fd))
		exit_err("close: failed ", 1);
	return ;
}
