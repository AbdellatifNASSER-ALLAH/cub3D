/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahakki <ahakki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 23:19:02 by ahakki            #+#    #+#             */
/*   Updated: 2025/08/09 09:26:34 by ahakki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_player(t_game *game)
{
	t_player *player;

	player = &game->player;
	player->key_down = false;
	player->key_left = false;
	player->key_right = false;
	player->key_up = false;
	player->left_rotate = false;
	player->right_rotate = false;
	player->up_rotate = false;
	player->down_rotate = false;
	player->speed = 5;
	player->z_eye = 0.5;
}



int	move_player(t_game *game)	
{
	t_player	*player = &game->player;
	float angle_speed = 0.05;
	float z_eye_speed = 0.05;

	// ROTATION left right
	if (player->left_rotate)
		player->angle -= angle_speed;
	if (player->right_rotate)
		player->angle += angle_speed;
	// ROTATION up down
	if (player->up_rotate && player->z_eye < 0.7)
		player->z_eye += z_eye_speed;
	if (player->down_rotate && player->z_eye > 0.3)
		player->z_eye -= z_eye_speed;

	// Normalize angle (keep between 0 and 2*PI)
	if (player->angle > 2 * PI)
		player->angle -= 2 * PI;
	if (player->angle < 0)
		player->angle += 2 * PI;

	// Compute keyection
	float new_x = 0;
	float new_y = 0;

	// FORWARD
	if (player->key_up)
	{
		new_x = player->x + cos(player->angle) * player->speed;
		new_y = player->y + sin(player->angle) * player->speed;
	}
	if (player->key_down)
	{
		new_x = player->x + cos(player->angle + PI) * player->speed;
		new_y = player->y + sin(player->angle + PI) * player->speed;
	}
	if (player->key_right)
	{
		new_x = player->x + cos(player->angle + PI / 2) * player->speed;
		new_y = player->y + sin(player->angle + PI / 2) * player->speed;
	}
	if (player->key_left)
	{
		new_x = player->x + cos(player->angle - PI / 2) * player->speed;
		new_y = player->y + sin(player->angle - PI / 2) * player->speed;
	}
	if (!touch(new_x, player->y, game) && !touch(player->x, new_y, game))
	{
		player->x = new_x;
		player->y = new_y;
	}
	else
	{
		if (!touch(new_x, player->y, game))
			player->x += (new_x - player->x) * 0.2;
		if (!touch(player->x, new_y, game))
			player->y += (new_y - player->y) * 0.2;
	}

	return (0);
}


