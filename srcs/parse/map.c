/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdnasse <abdnasse@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 16:50:41 by abdnasse          #+#    #+#             */
/*   Updated: 2025/09/30 16:26:46 by abdnasse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// TODO: 
// 	[ ] - get the char and the up, down, right and left char next to him
// 		if its the first line form the map the up should be null-terminator
// 		and for the last line donw should be null-terminator
// 	[ ] - 0 and Player cannot touch space or null-terminator
//	[ ] - door cannot touch space or null-terminator
//	[ ] - door must be between two walls : up-down or left-right
//	[ ] - 1 can have any neighbors

#include "cub3d.h"

/* Enum indices for neighbor array */
enum { U, Do, L, R, ME };

void	print_item(char *item)
{
	printf("  %c\n", item[U]);
	printf("%c-%c-%c\n", item[L], item[ME], item[R]);
	printf("  %c\n", item[Do]);
	return ;
}

/* Check if a character is a player symbol */
static int is_player(char c)
{
    return (c == 'N' || c == 'S' || c == 'E' || c == 'W');
}
/* Collect current cell and its neighbors */
static void get_item(char **map, int line, int i, t_config *cfg, char *item)
{
    item[ME] = map[line][i];

    /* U neighbor */
    if (line == cfg->map_start || (int)ft_strlen(map[line - 1]) <= i)
        item[U] = '\0';
    else
        item[U] = map[line - 1][i];

    /* Do neighbor */
    if (line == cfg->map_end || (int)ft_strlen(map[line + 1]) <= i)
        item[Do] = '\0';
    else
        item[Do] = map[line + 1][i];

    /* L neighbor */
    if (i == 0)
        item[L] = '\0';
    else
        item[L] = map[line][i - 1];

    /* R neighbor */
    if (map[line][i + 1] == '\0')
        item[R] = '\0';
    else
        item[R] = map[line][i + 1];
}

/* Helper: neighbor is invalid if it's end-of-string or a space (outside map) */
static int is_invalid_neighbor(char ch)
{
    return (ch == '\0' || ch == ' ');
}

/* Normalize tabs to spaces in the map */
static void normalize_map(char **map, int start, int end)
{
    int line;
    int i;

    line = start;
    while (line < end)
    {
        i = 0;
        while (map[line][i])
        {
            if (map[line][i] == '\t')
                map[line][i] = ' ';
            i++;
        }
        line++;
    }
}

/* Validate if the current map member is correct */
static void validate_member_map(char c, char *item, t_config *cfg)
{
    /* If the current cell is a space it represents outside-of-map; ignore */
    if (c == ' ' || c == '\t')
        return;

    if (c == '1') /* wall */
        return;

    else if (c == '\n' && !item[R])
	return ;
    else if (is_player(c))
    {
        cfg->player_count++;
        if (is_invalid_neighbor(item[U]) || is_invalid_neighbor(item[Do])
            || is_invalid_neighbor(item[L]) || is_invalid_neighbor(item[R]))
            exit_err("Player cannot touch space or map edge", 1, cfg);
    }

    else if (c == '0') /* floor */
    {
        if (is_invalid_neighbor(item[U]) || is_invalid_neighbor(item[Do])
            || is_invalid_neighbor(item[L]) || is_invalid_neighbor(item[R]))
	{
		print_item(item);
            exit_err("Floor cannot touch space or map edge", 1, cfg);
	}
    }

    else if (c == 'D') /* door */
    {
        if (is_invalid_neighbor(item[U]) || is_invalid_neighbor(item[Do])
            || is_invalid_neighbor(item[L]) || is_invalid_neighbor(item[R]))
            exit_err("Door cannot touch space or map edge", 1, cfg);

        if (!((item[U] == '1' && item[Do] == '1')
           || (item[L] == '1' && item[R] == '1')))
            exit_err("Door must be between two walls vertically or horizontally", 1, cfg);
    }

    else
        exit_err("Invalid character in map", 1, cfg);
}

/* Main function to validate the map */
void fill_map(char **map, int start, int end, t_config *cfg)
{
    int line;
    int i;
    char item[5]; /* U, Do, L, R, ME */

    cfg->player_count = 0;

    /* Normalize map before validation (replace tabs with spaces) */
    normalize_map(map, start, end);

    line = start;
    while (line < end)
    {
        i = 0;
        while (map[line][i])
        {
            get_item(map, line, i, cfg, item);
	    printf("line: %d, i = %d\n%s\n", line, i, map[line]);
            validate_member_map(map[line][i], item, cfg);
            i++;
        }
        line++;
    }

    if (cfg->player_count != 1)
        exit_err("Map must contain exactly one player", 1, cfg);
}
