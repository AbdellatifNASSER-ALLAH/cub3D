/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahakki <ahakki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 13:51:53 by ahakki            #+#    #+#             */
/*   Updated: 2025/11/14 13:58:05 by ahakki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

bool	touch(int px, int py, t_game *game)
{
	int	block_x;
	int	block_y;

	block_x = px / BLOCK;
	block_y = py / BLOCK;
	if (game->map[block_y][block_x] == '1' || \
game->map[block_y][block_x] == 'D')
		return (true);
	return (false);
}

int	is_blocked(float x, float y, t_game *game)
{
	return (touch(x - BLOCK / 20, y - BLOCK / 20, game) || \
touch(x + BLOCK / 20, y - BLOCK / 20, game) || \
touch(x - BLOCK / 20, y + BLOCK / 20, game) || \
touch(x + BLOCK / 20, y + BLOCK / 20, game));
}
