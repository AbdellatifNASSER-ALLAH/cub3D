/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdnasse <abdnasse@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 16:50:41 by abdnasse          #+#    #+#             */
/*   Updated: 2025/09/05 17:14:56 by abdnasse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	fill_map(char **map,int	start, int end, t_config *cfg)
{
	t_line	l;

	(void)cfg;
	while(start < end)
	{
		wrap_line(&l, map[start]);
		start++;
	}
}
