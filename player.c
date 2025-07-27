/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahakki <ahakki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 23:19:02 by ahakki            #+#    #+#             */
/*   Updated: 2025/07/27 18:25:25 by ahakki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_player(t_game *game)
{
	t_player *player;

	player = &game->player;
	// player->x = get_x();
	// player->y = get_y();
	// player->x = WIDTH / 2;
	// player->y = HEIGHT / 2;
	// player->angle = PI * -1 / 2;
	player->key_down = false;
	player->key_left = false;
	player->key_right = false;
	player->key_up = false;
	player->left_rotate = false;
	player-> right_rotate = false;
	player->speed = 5;
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
	if (key == '=')
		player->speed++;
	if (key == '-')
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
	return (0);
}
int	move_player(t_game *game)	
{
	t_player	*player = &game->player;
	float angle_speed = 0.05;

	// ROTATION
	if (player->left_rotate)
		player->angle -= angle_speed;
	if (player->right_rotate)
		player->angle += angle_speed;

	// Normalize angle (keep between 0 and 2*PI)
	if (player->angle > 2 * PI)
		player->angle -= 2 * PI;
	if (player->angle < 0)
		player->angle += 2 * PI;

	// Compute keyection
	float move_step = player->speed;
	float new_x;
	float new_y;

	// FORWARD
	if (player->key_up)
	{
		new_x = player->x + cos(player->angle) * move_step;
		new_y = player->y + sin(player->angle) * move_step;
		if (!touch(new_x + PLAYER_SIZE / 2.0f, new_y + PLAYER_SIZE / 2.0f, game))
		{
			player->x = new_x;
			player->y = new_y;
		}
	}
	// BACKWARD
	if (player->key_down)
	{
		new_x = player->x + cos(player->angle + PI) * move_step;
		new_y = player->y + sin(player->angle + PI) * move_step;
		if (!touch(new_x + PLAYER_SIZE / 2.0f, new_y + PLAYER_SIZE / 2.0f, game))
		{
			player->x = new_x;
			player->y = new_y;
		}
	}
	// STRAFE RIGHT
	if (player->key_right)
	{
		new_x = player->x + cos(player->angle + PI / 2) * move_step;
		new_y = player->y + sin(player->angle + PI / 2) * move_step;
		if (!touch(new_x + PLAYER_SIZE / 2.0f, new_y + PLAYER_SIZE / 2.0f, game))
		{
			player->x = new_x;
			player->y = new_y;
		}
	}
	// STRAFE LEFT
	if (player->key_left)
	{
		new_x = player->x + cos(player->angle - PI / 2) * move_step;
		new_y = player->y + sin(player->angle - PI / 2) * move_step;
		if (!touch(new_x + PLAYER_SIZE / 2.0f, new_y + PLAYER_SIZE / 2.0f, game))
		{
			player->x = new_x;
			player->y = new_y;
		}
	}

	return (0);
}


