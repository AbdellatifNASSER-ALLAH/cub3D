/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdnasse <abdnasse@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 16:50:41 by abdnasse          #+#    #+#             */
/*   Updated: 2025/09/25 17:22:57 by abdnasse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// TODO: 
// 	[ ] - get the char and the up, down, right and left char next to him
// 		if its the first line form the map the up should be null-terminator
// 		and for the last line donw should be null-terminator
// 	[ ] - 1 can have any neighbors 
// 	[ ] - 0 can have any neighbors except space or null-terminator
// 	[ ] - Player if char is N or S or W or E : 
// 		can have any neighbors except space or null-terminator
// 	[ ] - D for the door should be between tow 1s right and left or up and down
// 		can have any neighbors except space or null-terminator


#include "cub3d.h"

/* Directions around a cell */
enum { U, Do, L, R, ME };

/* Check if a character is a player symbol */
static int is_player(char c)
{
    return (c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

/* Collect current cell and its neighbors */
static void get_item(char **map, int line, int i, t_config *cfg, char *item)
{
    item[ME] = map[line][i];

    // U neighbor
    if (line == cfg->map_start || (int)ft_strlen(map[line - 1]) <= i)
        item[U] = '\0';
    else
        item[U] = map[line - 1][i];

    // Do neighbor
    if (line == cfg->map_end || (int)ft_strlen(map[line + 1]) <= i)
        item[Do] = '\0';
    else
        item[Do] = map[line + 1][i];

    // L neighbor
    if (i == 0)
        item[L] = '\0';
    else
        item[L] = map[line][i - 1];

    // R neighbor
    if (map[line][i + 1] == '\0')
        item[R] = '\0';
    else
        item[R] = map[line][i + 1];
}

/* Validate if the current map member is correct */
static void validate_member_map(char c, char *item, t_config *cfg)
{
    if (c == '1') // wall
        return;

    else if (is_player(c))
    {
        cfg->player_count++;
        if (item[U] == '\0' || item[Do] == '\0'
            || item[L] == '\0' || item[R] == '\0')
            exit_err("Player cannot touch space or map edge", 1, cfg);
    }

    else if (c == '0') // floor
    {
        if (item[U] == '\0' || item[Do] == '\0'
            || item[L] == '\0' || item[R] == '\0')
            exit_err("Floor cannot touch space or map edge", 1, cfg);
    }

    else if (c == 'D') // door
    {
        if (item[U] == '\0' || item[Do] == '\0'
            || item[L] == '\0' || item[R] == '\0')
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
    char item[5]; // U, Do, L, R, ME

    cfg->player_count = 0;

    for (line = start; line <= end; line++)
    {
        for (i = 0; map[line][i]; i++)
        {
            get_item(map, line, i, cfg, item);
            validate_member_map(map[line][i], item, cfg);
        }
    }

    if (cfg->player_count != 1)
        exit_err("Map must contain exactly one player", 1, cfg);
}
