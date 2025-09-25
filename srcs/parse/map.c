/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdnasse <abdnasse@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 16:50:41 by abdnasse          #+#    #+#             */
/*   Updated: 2025/09/25 16:07:22 by abdnasse         ###   ########.fr       */
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

enum
{
    UP,
    DOWN,
    RIGHT,
    LEFT,
    ME
};
void    get_item(char **map, int line, int i, char *item);
void    validat_member_map(char c, char *item, t_config *cfg);
int    is_player(char c);

void    fill_map(char **map,int    start, int end, t_config *cfg)
{
    int    i;
    char    item[5];

    while (start < end)
    {
        i = -1;
        while(map[start][++i] != 0)
        {
            get_item(map, start, i, item);
            validat_member_map(item[ME], item, cfg);
        }
        start++;
    }
    return ;
}

void    validat_member_map(char c, char *item, t_config *cfg)
{
    if (c == '1')
        return ;
    else if (is_paler(c))
        // do player     
    else if (c == '0')
        // do 0
    else if (c == 'D')
        // do 0
    else 
        exit_err("Uknown member of map", 1, cfg);
    return ;
}
void    get_item(char **map, int line, int i, char *item)
{
    item[ME] = map[line][i];
    if (line == cfg->map_start || (int)ft_strlen(map[line - 1]) < i)
        item[UP] = 0;
    else
        item[UP] = map[line - 1][i];
    if (line == cfg->map_end || (int)ft_strlen(map[line + 1]) < i)
        item[DOWN] = 0;
    else
        item[DOWN] = map[line + 1][i];
    if (0 == i)
        item[RIGHT] = 0;
    else 
        item[RIGHT] = map[line][i - 1];
    if (map[line][i + 1] == 0)
        item[LEFT] = 0;
    else 
        item[LEFT] = map[line][i + 1];
    return ;
}
