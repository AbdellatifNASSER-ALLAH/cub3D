/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahakki <ahakki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 23:19:02 by ahakki            #+#    #+#             */
/*   Updated: 2025/07/17 00:05:21 by ahakki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_player(t_player *player)
{
	player->x = WIDTH / 2;
	player->y = HEIGHT / 2;
	player->key_down = false;
	player->key_left = false;
	player->key_right = false;
	player->key_up = false;
}

int key_press(int key, t_player *player)
{
	if (key == W)
		player->key_up = true;
	if (key == S)
		player->key_down = true;
	if (key == A)
		player->key_left = true;
	if (key == D)
		player->key_right = true;
	if (key == 65307)
		exit(0);
	return (0);
}

int key_release(int key, t_player *player)
{
	if (key == W)
		player->key_up = false;
	if (key == S)
		player->key_down = false;
	if (key == A)
		player->key_left = false;
	if (key == D)
		player->key_right = false;
	return (0);
}
int move_player(t_player *player)	
{
	int	speed;

	speed = 5;
	if (player->key_up)
		player->y -= speed;
	if (player->key_down)
		player->y += speed;
	if (player->key_left)
		player->x -= speed;
	if (player->key_right)
		player->x += speed;
	return (0);
}



