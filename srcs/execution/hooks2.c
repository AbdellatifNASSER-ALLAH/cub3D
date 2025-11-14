/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahakki <ahakki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 18:43:15 by ahakki            #+#    #+#             */
/*   Updated: 2025/11/13 18:49:33 by ahakki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

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
	if (key == 'x')
		player->mouse_captured = !player->mouse_captured;
}


