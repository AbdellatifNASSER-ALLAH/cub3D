/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_configs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdnasse <abdnasse@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 03:21:37 by abdnasse          #+#    #+#             */
/*   Updated: 2025/11/26 13:50:30 by abdnasse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	do_tex(int n, char *line, t_config *cfg);
static void	missing_textures(t_config *cfg);
static void	handle_texture(int n, char *line, t_config *cfg);
static void	handle_color(int n, char *line, t_config *cfg);

void	extract_configs(t_config *cfg, char *l)
{
	while (++cfg->map_start < cfg->nb_lines)
	{
		l = skip_ws(cfg->lines[cfg->map_start]);
		if (!l || (*l && (*l == '0' || *l == '1')))
			break ;
		else if (*l && *l == '\n')
			continue ;
		else if (ft_strncmp(l, "NO ", 3) == 0)
			do_tex(0, l, cfg);
		else if (ft_strncmp(l, "SO ", 3) == 0)
			do_tex(1, l, cfg);
		else if (ft_strncmp(l, "EA ", 3) == 0)
			do_tex(2, l, cfg);
		else if (ft_strncmp(l, "WE ", 3) == 0)
			do_tex(3, l, cfg);
		else if (ft_strncmp(l, "DO ", 3) == 0)
			do_tex(4, l, cfg);
		else if (ft_strncmp(l, "C ", 2) == 0)
			do_tex(5, l, cfg);
		else if (ft_strncmp(l, "F ", 2) == 0)
			do_tex(6, l, cfg);
		else
			exit_err("Unknown config line", 1, cfg);
	}
	missing_textures(cfg);
}

static void	missing_textures(t_config *cfg)
{
	int		i;
	char	*l;

	i = -1;
	while (++i < 4)
		if (!cfg->tex[i])
			exit_err("Missing texture config", 1, cfg);
	if (!cfg->f_found || !cfg->c_found)
		exit_err("Missing F or C config", 1, cfg);
	cfg->map_end = cfg->map_start;
	while (cfg->map_end < cfg->nb_lines)
	{
		l = skip_ws(cfg->lines[cfg->map_end]);
		if (l && *l && (*l == '0' || *l == '1'))
			cfg->map_end++;
		else
			break ;
	}
}

static void	handle_texture(int n, char *line, t_config *cfg)
{
	if (n < 4)
	{
		valid_file(line, ".xpm", cfg);
		if (cfg->tex_found[n]++)
			exit_err("Duplicated config", 1, cfg);
		cfg->tex[n] = ft_strdup(line);
	}
	else if (n == 4)
	{
		valid_file(line, ".xpm", cfg);
		if (cfg->door_found++)
			exit_err("Duplicated config", 1, cfg);
		cfg->tex[DOOR] = ft_strdup(line);
	}
}

static void	handle_color(int n, char *line, t_config *cfg)
{
	if (n == 5)
	{
		if (cfg->c_found++)
			exit_err("Duplicated Color C", 1, cfg);
		if (!rgb(line, cfg->c_rgb))
			exit_err("Parsing rgb colors", 1, cfg);
	}
	else if (n == 6)
	{
		if (cfg->f_found++)
			exit_err("Duplicated Color F", 1, cfg);
		if (!rgb(line, cfg->f_rgb))
			exit_err("Parsing rgb colors", 1, cfg);
	}
}

static void	do_tex(int n, char *line, t_config *cfg)
{
	if (n < 5)
		handle_texture(n, skip_ws(line + 3), cfg);
	else
		handle_color(n, skip_ws(line + 2), cfg);
}
