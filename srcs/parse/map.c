/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdnasse <abdnasse@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 16:50:41 by abdnasse          #+#    #+#             */
/*   Updated: 2025/09/25 16:25:53 by abdnasse         ###   ########.fr       */
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
enum { UP, DOWN, LEFT, RIGHT, ME };

/* Check if a character is a player symbol */
static int is_player(char c)
{
    return (c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

/* Collect current cell and its neighbors */
static void get_item(char **map, int line, int i, t_config *cfg, char *item)
{
    item[ME] = map[line][i];

    // UP neighbor
    if (line == cfg->map_start || (int)ft_strlen(map[line - 1]) <= i)
        item[UP] = '\0';
    else
        item[UP] = map[line - 1][i];

    // DOWN neighbor
    if (line == cfg->map_end || (int)ft_strlen(map[line + 1]) <= i)
        item[DOWN] = '\0';
    else
        item[DOWN] = map[line + 1][i];

    // LEFT neighbor
    if (i == 0)
        item[LEFT] = '\0';
    else
        item[LEFT] = map[line][i - 1];

    // RIGHT neighbor
    if (map[line][i + 1] == '\0')
        item[RIGHT] = '\0';
    else
        item[RIGHT] = map[line][i + 1];
}

/* Validate if the current map member is correct */
static void validate_member_map(char c, char *item, t_config *cfg)
{
    if (c == '1') // wall
        return;

    else if (is_player(c))
    {
        cfg->player_count++;
        if (item[UP] == '\0' || item[DOWN] == '\0'
            || item[LEFT] == '\0' || item[RIGHT] == '\0')
            exit_err("Player cannot touch space or map edge", 1, cfg);
    }

    else if (c == '0') // floor
    {
        if (item[UP] == '\0' || item[DOWN] == '\0'
            || item[LEFT] == '\0' || item[RIGHT] == '\0')
            exit_err("Floor cannot touch space or map edge", 1, cfg);
    }

    else if (c == 'D') // door
    {
        if (item[UP] == '\0' || item[DOWN] == '\0'
            || item[LEFT] == '\0' || item[RIGHT] == '\0')
            exit_err("Door cannot touch space or map edge", 1, cfg);

        if (!((item[UP] == '1' && item[DOWN] == '1')
           || (item[LEFT] == '1' && item[RIGHT] == '1')))
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
    char item[5]; // UP, DOWN, LEFT, RIGHT, ME

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
