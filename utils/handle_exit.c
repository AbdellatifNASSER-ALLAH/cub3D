/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_exit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahakki <ahakki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 14:31:09 by abdnasse          #+#    #+#             */
/*   Updated: 2025/11/11 11:51:00 by abdnasse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	free_game(t_game *game)
{
	int	i;

	i = -1;
	while (++i < NB_TEX)
		if (game->textures[i].img)
			mlx_destroy_image(game->mlx, game->textures[i].img);
	if (game->img)
		mlx_destroy_image(game->mlx, game->img);
	if (game->win)
		mlx_destroy_window(game->mlx, game->win);
	if (game->mlx)
	{
		mlx_destroy_display(game->mlx);
		free(game->mlx);
	}
}

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
		if (cfg->tex_found[i])
			free(cfg->tex[i]);
	if (cfg->has_door_in_map)
			free(cfg->tex[DOOR]);
}

void	exit_err(const char *msg, int st, t_config *cfg)
{
	if (cfg)
		free_configs(cfg);
	printf(RED"%s\n"RESET, msg);
	exit(st);
}

void	handle_exit(t_game *game)
{
	free_configs(&game->config);
	free_game(game);
	exit(0);
}
