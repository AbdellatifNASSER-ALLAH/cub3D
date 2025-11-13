/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahakki <ahakki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 17:20:14 by ahakki            #+#    #+#             */
/*   Updated: 2025/11/13 18:46:42 by ahakki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	key_press(int key, t_game *game)
{
	t_player	*player;

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
	key_press2(key, game);
	return (0);
}

static void	update_player_angle(t_game *game, int delta_x, int delta_y)
{
	t_player	*player;
	float		r;
	float		sensitivity;

	player = &game->player;
	sensitivity = 0.0015;
	player->angle += delta_x * sensitivity;
	r = player->z_eye - delta_y * sensitivity;
	if (r >= 0 && r <= 1)
		player->z_eye -= delta_y * sensitivity;
}

int	mouse_move(int x, int y, t_game *game)
{
	int	center_x;
	int	center_y;
	int	delta_x;
	int	delta_y;

	center_x = WIDTH / 2;
	center_y = HEIGHT / 2;
	delta_x = x - center_x;
	delta_y = y - center_y;
	update_player_angle(game, delta_x, delta_y);
	mlx_mouse_move(game->mlx, game->win, center_x, HEIGHT / 2);
	return (0);
}

int	mouse_button(int button, int x, int y, t_game *game)
{
	(void)x;
	(void)y;
	if (button == RIGHT_MOUSE_BUTTON)
	{
		game->player.is_attacking = true;
		game->player.attack_frame = 0;
	}
	return (0);
}

int	key_release(int key, t_game *game)
{
	t_player	*player;

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
