/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahakki <ahakki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 23:19:02 by ahakki            #+#    #+#             */
/*   Updated: 2025/08/03 15:09:07 by ahakki           ###   ########.fr       */
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
	player-> right_rotate = false;
	player->up_rotate = false;
	player->down_rotate = false;
	player->speed = 5;
	player->z_eye = 0.5;
}

int key_press(int key, t_game *game)
{
	t_player *player;

	player = &game->player;
	if (key == W)
		player->key_up = true;
	if (key == S)
		player->key_down = true;
	if (key == A)
		player->key_left = true;
	if (key == D)
		player->key_right = true;
	if (key == LEFT)
		player->left_rotate = true;
	if (key == RIGHT)
		player->right_rotate = true;
	if (key == UP)
		player->up_rotate = true;
	if (key == DOWN)
		player->down_rotate = true;
	if (key == 65307)
		exit(0);
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
	return (0);
}

int	mouse_move(int x, int y, t_game *game)
{
	t_player *player = &game->player;

	(void)y;
	if (x >=0 && x<= (WIDTH / 10) * 4)
		player->left_rotate = true;
	else if (x >= WIDTH - ((WIDTH / 10) * 4) && x <= WIDTH)
		player->right_rotate = true;
	else
	{
		player->left_rotate = false;
		player->right_rotate = false;
	}
	player->prev_mouse_x = x;
	return (0);
}

int key_release(int key, t_game *game)
{
	t_player *player;

	player = &game->player;
	if (key == W)
		player->key_up = false;
	if (key == S)
		player->key_down = false;
	if (key == A)
		player->key_left = false;
	if (key == D)
		player->key_right = false;
	if (key == LEFT)
		player->left_rotate = false;
	if (key == RIGHT)
		player->right_rotate = false;
	if (key == UP)
		player->up_rotate = false;
	if (key == DOWN)
		player->down_rotate = false;
	return (0);
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
	if (player->up_rotate && player->z_eye < 1)
		player->z_eye += z_eye_speed;
	if (player->down_rotate && player->z_eye > 0)
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


