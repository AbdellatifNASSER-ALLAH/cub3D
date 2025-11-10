/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdnasse <abdnasse@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 16:50:41 by abdnasse          #+#    #+#             */
/*   Updated: 2025/10/03 15:48:49 by abdnasse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	is_player(char c);
static void	get_item(int line, int i, t_config *cfg, char *item);
static int	is_invalid_neighbor(char ch);
static void	validate_member_map(char c, char *item, t_config *cfg);
static void	validate_player(char *item, t_config *cfg);
static void	validate_door(char *item, t_config *cfg);

void	fill_map(char **map, int start, int end, t_config *cfg)
{
	char	item[5];
	int		line;
	int		i;

	cfg->player_count = 0;
	line = start;
	while (line < end)
	{
		i = 0;
		while (map[line][i])
		{
			get_item(line, i, cfg, item);
			validate_member_map(map[line][i], item, cfg);
			i++;
		}
		line++;
	}
	if (cfg->player_count != 1)
		exit_err("Map must contain exactly one player", 1, cfg);
	cfg->map = cfg->lines + cfg->map_start;
}

static int	is_player(char c)
{
	return (c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

static void	get_item(int line, int i, t_config *cfg, char *item)
{
	char	**map;

	map = cfg->lines;
	item[ME] = map[line][i];
	if (line == cfg->map_start || (int)ft_strlen(map[line - 1]) <= i)
		item[U] = '\0';
	else
		item[U] = map[line - 1][i];
	if (line == cfg->map_end || (map[line + 1] && (int)ft_strlen(map[line
			+ 1]) <= i))
		item[Do] = '\0';
	else if (map[line + 1])
		item[Do] = map[line + 1][i];
	else
		item[Do] = 0;
	if (i == 0)
		item[L] = '\0';
	else
		item[L] = map[line][i - 1];
	if (map[line][i + 1] == '\0')
		item[R] = '\0';
	else
		item[R] = map[line][i + 1];
}

static int	is_invalid_neighbor(char ch)
{
	return (ch == '\0' || ch == ' ' || ch == '\t');
}

static void	validate_player(char *item, t_config *cfg)
{
	if (is_invalid_neighbor(item[U]) || is_invalid_neighbor(item[Do])
		|| is_invalid_neighbor(item[L]) || is_invalid_neighbor(item[R]))
		exit_err("Player cannot touch space or map edge", 1, cfg);
}

static void	validate_door(char *item, t_config *cfg)
{
	cfg->has_door_in_map = 1;
	if (is_invalid_neighbor(item[U]) || is_invalid_neighbor(item[Do])
		|| is_invalid_neighbor(item[L]) || is_invalid_neighbor(item[R]))
		exit_err("Door cannot touch space or map edge", 1, cfg);
	if (!((item[U] == '1' && item[Do] == '1') || (item[L] == '1'
				&& item[R] == '1')))
		exit_err("Door must be between two walls", 1, cfg);
}

static void	validate_member_map(char c, char *item, t_config *cfg)
{
	if (c == ' ' || c == '\t')
		return ;
	if (c == '1')
		return ;
	else if (c == '\n' && !item[R])
		return ;
	else if (is_player(c))
	{
		cfg->player_count++;
		validate_player(item, cfg);
	}
	else if (c == '0')
	{
		if (is_invalid_neighbor(item[U]) || is_invalid_neighbor(item[Do])
			|| is_invalid_neighbor(item[L]) || is_invalid_neighbor(item[R]))
			exit_err("Floor cannot touch space or map edge", 1, cfg);
	}
	else if (c == 'D')
		validate_door(item, cfg);
	else
		exit_err("Invalid character in map", 1, cfg);
}
