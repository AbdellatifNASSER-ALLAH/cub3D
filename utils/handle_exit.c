/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_exit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdnasse <abdnasse@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 14:31:09 by abdnasse          #+#    #+#             */
/*   Updated: 2025/08/07 06:31:39 by abdnasse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	free_configs(t_config *cfg)
{
	int	i;
	i = 0;
	while (cfg->lines && cfg->lines[i])
		free(cfg->lines[i++]);
	free(cfg->lines);
	cfg->lines = NULL;
	i = -1;
	while (++i < 4)
		if (cfg->tex[i])
			free(cfg->tex[i]);
}

void	exit_err(const char *msg, int st, t_config *cfg)
{
	if (cfg)
		free_configs(cfg);
	printf(RED"%s\n"RESET, msg);
	exit(st);
}

