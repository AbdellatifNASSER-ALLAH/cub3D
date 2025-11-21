/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   torch.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdnasse <abdnasse@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 09:22:33 by abdnasse          #+#    #+#             */
/*   Updated: 2025/11/21 09:25:32 by abdnasse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	update_attack_animation(t_game *game);
static void	draw_torch_line(t_texture *tex, int screen_x, int draw_y,
				t_game *game);

void	draw_torch(t_game *game)
{
	t_texture	*torch_tex;
	int			screen_x;
	int			draw_y;

	if (game->player.is_attacking)
		torch_tex = &game->textures[TORCH_ATTACK];
	else
		torch_tex = &game->textures[TORCH];
	screen_x = WIDTH - TORCH_SIZE - TORCH_OFFSET_X;
	draw_y = -1;
	while (++draw_y < TORCH_SIZE)
		draw_torch_line(torch_tex, screen_x, draw_y, game);
	update_attack_animation(game);
}

static void	draw_torch_line(t_texture *tex, int screen_x, int draw_y,
		t_game *game)
{
	int	draw_x;
	int	tex_x;
	int	tex_y;
	int	color;
	int	screen_y;

	screen_y = HEIGHT - TORCH_SIZE;
	draw_x = 0;
	while (draw_x < TORCH_SIZE && screen_x + draw_x < WIDTH)
	{
		tex_x = (draw_x * tex->width) / TORCH_SIZE;
		tex_y = (draw_y * tex->height) / TORCH_SIZE;
		color = tex->data[tex_y * tex->width + tex_x];
		if (color != TRANSPARENT_COLOR)
			put_pixel(screen_x + draw_x, screen_y + draw_y, color, game);
		draw_x++;
	}
}

static void	update_attack_animation(t_game *game)
{
	if (game->player.is_attacking)
	{
		game->player.attack_frame++;
		if (game->player.attack_frame > ATTACK_ANIMATION_FRAMES)
		{
			game->player.is_attacking = false;
			game->player.attack_frame = 0;
		}
	}
}
