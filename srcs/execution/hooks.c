/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahakki <ahakki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 17:20:14 by ahakki            #+#    #+#             */
/*   Updated: 2025/08/09 09:26:00 by ahakki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

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
	float r;

	int center_x = WIDTH / 2;
	int center_y = HEIGHT / 2;
	int delta_x = x - center_x;
	int delta_y = y - center_y;

	float sensitivity = 0.0015;
	player->angle += delta_x * sensitivity;
	r = player->z_eye - delta_y * sensitivity;
	if (r >= 0 && r <= 1)
		player->z_eye -= delta_y * sensitivity;
	mlx_mouse_move(game->mlx, game->win, center_x, HEIGHT / 2);
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