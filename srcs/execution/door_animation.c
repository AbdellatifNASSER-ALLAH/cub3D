/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_animation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copilot <copilot@github.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 00:00:00 by copilot           #+#    #+#             */
/*   Updated: 2025/10/17 00:00:00 by copilot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_door_list(t_game *game)
{
	game->door_list.doors = NULL;
	game->door_list.count = 0;
	game->door_list.capacity = 0;
}

static void	add_door(t_game *game, int x, int y)
{
	t_door	*new_doors;
	int		new_capacity;

	if (game->door_list.count >= game->door_list.capacity)
	{
		new_capacity = game->door_list.capacity == 0 ? 10 : 
			game->door_list.capacity * 2;
		new_doors = malloc(sizeof(t_door) * new_capacity);
		if (!new_doors)
			return ;
		if (game->door_list.doors)
		{
			ft_memcpy(new_doors, game->door_list.doors, 
				sizeof(t_door) * game->door_list.count);
			free(game->door_list.doors);
		}
		game->door_list.doors = new_doors;
		game->door_list.capacity = new_capacity;
	}
	game->door_list.doors[game->door_list.count].x = x;
	game->door_list.doors[game->door_list.count].y = y;
	game->door_list.doors[game->door_list.count].state = DOOR_CLOSED;
	game->door_list.doors[game->door_list.count].anim_progress = 0.0f;
	game->door_list.doors[game->door_list.count].current_frame = 0;
	game->door_list.count++;
}

void	find_and_init_doors(t_game *game)
{
	int	y;
	int	x;

	y = 0;
	while (game->map[y])
	{
		x = 0;
		while (game->map[y][x])
		{
			if (game->map[y][x] == 'D')
				add_door(game, x, y);
			x++;
		}
		y++;
	}
}

t_door	*get_door_at(t_game *game, int x, int y)
{
	int	i;

	i = 0;
	while (i < game->door_list.count)
	{
		if (game->door_list.doors[i].x == x && game->door_list.doors[i].y == y)
			return (&game->door_list.doors[i]);
		i++;
	}
	return (NULL);
}

void	update_door_animations(t_game *game)
{
	int		i;
	t_door	*door;
	float	speed;

	speed = game->config.door_anim_speed;
	if (speed == 0.0f)
		speed = DOOR_ANIM_SPEED;
	i = 0;
	while (i < game->door_list.count)
	{
		door = &game->door_list.doors[i];
		if (door->state == DOOR_OPENING)
		{
			door->anim_progress += speed;
			if (door->anim_progress >= 1.0f)
			{
				door->anim_progress = 1.0f;
				door->state = DOOR_OPEN;
			}
			if (game->config.door_frame_count > 1)
				door->current_frame = (int)(door->anim_progress * 
					(game->config.door_frame_count - 1));
			else
				door->current_frame = 0;
		}
		i++;
	}
}

bool	is_door_passable(t_game *game, int x, int y)
{
	t_door	*door;

	door = get_door_at(game, x, y);
	if (!door)
		return (false);
	return (door->state == DOOR_OPEN);
}

static void	start_door_opening(t_door *door)
{
	if (door->state == DOOR_CLOSED)
	{
		door->state = DOOR_OPENING;
		door->anim_progress = 0.0f;
	}
}

void	interact_with_door(t_game *game)
{
	int		door_x;
	int		door_y;
	float	check_dist;
	float	dx;
	float	dy;
	t_door	*door;

	check_dist = BLOCK * 1.5f;
	dx = cos(game->player.angle) * check_dist;
	dy = sin(game->player.angle) * check_dist;
	door_x = (int)((game->player.x + dx) / BLOCK);
	door_y = (int)((game->player.y + dy) / BLOCK);
	if (game->map[door_y][door_x] == 'D')
	{
		door = get_door_at(game, door_x, door_y);
		if (door)
			start_door_opening(door);
	}
}
