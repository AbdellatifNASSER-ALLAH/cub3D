/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_configs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdnasse <abdnasse@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 03:21:37 by abdnasse          #+#    #+#             */
/*   Updated: 2025/09/25 17:09:09 by abdnasse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <stdio.h>

static	char	*skip_ws(char *s);
static	void	do_tex(int n, char *line, t_config *cfg);
static	int	rgb(char *s, int *rgb);
static	void	missing_textures(t_config *cfg);

void	extract_configs(t_config *cfg)
{
	char	*l;

	while (cfg->map_start < cfg->nb_lines)
	{
		l = skip_ws(cfg->lines[cfg->map_start++]);
		if (*l && ( *l == '0' || *l == '1'))
			break ;
		else if (*l && *l == '\n')
			continue;
		else if (ft_strncmp(l, "NO ", 3) == 0)
			do_tex(0, l, cfg);
		else if (ft_strncmp(l, "SO ", 3) == 0)
			do_tex(1, l, cfg);
		else if (ft_strncmp(l, "EA ", 3) == 0)
			do_tex(2, l, cfg);
		else if (ft_strncmp(l, "WE ", 3) == 0)
			do_tex(3, l, cfg);
		else if (ft_strncmp(l, "C ", 2) == 0)
			do_tex(4, l, cfg);
		else if (ft_strncmp(l, "F ", 2) == 0)
			do_tex(5, l, cfg);
		else 
			exit_err("Unknow config lien", 1, cfg);
	}
	missing_textures(cfg);
}

static	void	missing_textures(t_config *cfg)
{
	int	i;
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
		if (l && *l && ( *l == '0' || *l == '1'))
			cfg->map_end++;
		else
			break;
	}
}

static	int	rgb(char *s, int *rgb)
{
	int	i;

	i = 0;
	while (i < 3)
	{
		s = skip_ws(s);
		rgb[i++] = ft_atoi(s);
		while(ft_isdigit(*s))
			s++;
		s = skip_ws(s);
		if (i < 2 && *s && *s != ',')
			return 0;
		s++;
	}
	s = skip_ws(s);
	if (*s && *s != '\n')
		return 0;
	if (rgb[0] < 0 || rgb[0] > 255 || rgb[1] < 0 
			|| rgb[1] > 255 || rgb[2] < 0 || rgb[2] > 255)
		return 0;
	return (1);
}

static	void	do_tex(int n, char *line, t_config *cfg)
{
	if (n < 4)
	{
		valid_file(line + 3, ".xpm", cfg);
		if (cfg->tex_found[n]++)
			exit_err("Dubplicated config", 1, cfg);
		cfg->tex[n] = ft_strdup(line + 3);
	}
	if (n == 4)
	{
		if (cfg->c_found++)
			exit_err("Dubplicated Color C", 1, cfg);
		if (!rgb(skip_ws(line + 2), cfg->c_rgb))
			exit_err("Parsing rgb colors", 1, cfg);
	}
	if (n == 5)
	{
		if (cfg->f_found++)
			exit_err("Dubplicated Color F", 1, cfg);
		if (!rgb(skip_ws(line + 2), cfg->f_rgb))
			exit_err("Parsing rgb colors", 1, cfg);
	}
}

static	char	*skip_ws(char *s)
{
	while (s && (*s == 32 || *s == '\t'))
		s++;
	return (s);
}
