/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahakki <ahakki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 18:43:15 by ahakki            #+#    #+#             */
/*   Updated: 2025/11/16 19:30:56 by abdnasse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	door_interaction(t_game *game)
{
	t_player	*player;
	int			x;
	int			y;
	char		b;

	player = &game->player;
	x = (player->x + cos(player->angle) * BLOCK) / BLOCK;
	y = (player->y + sin(player->angle) * BLOCK) / BLOCK;
	if (game->map[y][(int)(player->x / BLOCK)] == '1' && \
game->map[(int)(player->y / BLOCK)][x] == '1')
		return ;
	b = game->map[y][x];
	if (b == 'D' || b == 'O')
		game->map[y][x] = 'O' * (b == 'D') + 'D' * (b == 'O');
}

void	key_press2(int key, t_game *game)
{
	t_player	*player;

	player = &game->player;
	if (key == 65307)
		handle_exit(game);
	if (key == 'i')
		player->angle = 3 * PI / 2;
	if (key == 'k')
		player->angle = PI / 2;
	if (key == 'l')
		player->angle = 0;
	if (key == 'j')
		player->angle = PI;
	if (key == '=' && player->speed < 10)
		player->speed++;
	if (key == '-' && player->speed > 1)
		player->speed--;
	if (key == ' ')
		door_interaction(game);
}
