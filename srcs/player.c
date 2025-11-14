/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahakki <ahakki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 23:19:02 by ahakki            #+#    #+#             */
/*   Updated: 2025/11/12 15:41:07 by ahakki           ###   ########.fr       */
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
	player->is_attacking = false;
	player->attack_frame = 0;
	player->mouse_captured = true;
}
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
	return touch(x - BLOCK / 20, y - BLOCK / 20, game) ||
			touch(x + BLOCK / 20, y - BLOCK / 20, game) ||
			touch(x - BLOCK / 20, y + BLOCK / 20, game) ||
			touch(x + BLOCK / 20, y + BLOCK / 20, game);
}

static void	normalize_angle(t_player *player)
{
	if (player->angle > 2 * PI)
		player->angle -= 2 * PI;
	if (player->angle < 0)
		player->angle += 2 * PI;
}

static void	apply_rotation(t_player *player)
{
	float angle_speed = 0.05;
	float z_eye_speed = 0.05;

	if (player->left_rotate)
		player->angle -= angle_speed;
	if (player->right_rotate)
		player->angle += angle_speed;
	if (player->up_rotate && player->z_eye < 0.95)
		player->z_eye += z_eye_speed;
	if (player->down_rotate && player->z_eye > 0.05)
		player->z_eye -= z_eye_speed;
	normalize_angle(player);
}

static void	calc_move(t_player *player, float *new_x, float *new_y)
{
	if (player->key_up)
	{
		*new_x = player->x + cos(player->angle) * player->speed;
		*new_y = player->y + sin(player->angle) * player->speed;
	}
	else if (player->key_down)
	{
		*new_x = player->x + cos(player->angle + PI) * player->speed;
		*new_y = player->y + sin(player->angle + PI) * player->speed;
	}
	else if (player->key_right)
	{
		*new_x = player->x + cos(player->angle + PI / 2) * player->speed;
		*new_y = player->y + sin(player->angle + PI / 2) * player->speed;
	}
	else if (player->key_left)
	{
		*new_x = player->x + cos(player->angle - PI / 2) * player->speed;
		*new_y = player->y + sin(player->angle - PI / 2) * player->speed;
	}
}

int	move_player(t_game *game)
{
	t_player	*player = &game->player;
	float		new_x = player->x;
	float		new_y = player->y;

	apply_rotation(player);
	calc_move(player, &new_x, &new_y);

	if (!is_blocked(new_x, player->y, game) && !is_blocked(player->x, new_y, game))
	{
		player->x = new_x;
		player->y = new_y;
	}
	else
	{
		if (!is_blocked(new_x, player->y, game))
			player->x += (new_x - player->x) * 0.2;
		if (!is_blocked(player->x, new_y, game))
			player->y += (new_y - player->y) * 0.2;
	}
	return (0);
}
