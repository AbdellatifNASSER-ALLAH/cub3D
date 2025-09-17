/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdnasse <abdnasse@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 16:50:41 by abdnasse          #+#    #+#             */
/*   Updated: 2025/09/17 11:22:43 by abdnasse         ###   ########.fr       */
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

void	fill_map(char **map,int	start, int end, t_config *cfg)
{
	while (start < end)
	{

		start++;
	}
	return ;
}
