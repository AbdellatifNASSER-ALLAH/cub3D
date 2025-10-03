/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pixel_draw.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahakki <ahakki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 09:24:00 by ahakki            #+#    #+#             */
/*   Updated: 2025/10/02 20:15:15 by ahakki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	put_pixel(int x, int y, int color, t_game *game)
{
	int	index;
	
	if (x >= WIDTH || y >= HEIGHT || x < 0 || y < 0)
	return ;
	index = y * game->size_line + x * game->bpp / 8;
	game->data[index] = color & 0xFF;
	game->data[index + 1] = (color >> 8) & 0xFF;
	game->data[index + 2] = (color >> 16) & 0xFF;
}

void	draw_aim(int cx, int cy, int radius, int color, t_game *game)
{
    put_pixel(cx, cy, color, game);
    int i = 1;
    while (i <= radius)
    {
        put_pixel(cx + i, cy, color, game); // right
        put_pixel(cx - i, cy, color, game); // left
        put_pixel(cx, cy + i, color, game); // down
        put_pixel(cx, cy - i, color, game); // up
        i++;
    }
}
void	draw_circule(int cx, int cy, int radius, int color, t_game *game)
{
    int x, y;
    int r2 = radius * radius;

    for (y = -radius; y <= radius; y++)
    {
        for (x = -radius; x <= radius; x++)
        {
            if (x * x + y * y <= r2)
                put_pixel(cx + x, cy + y, color, game);
        }
    }
}


void	draw_square_pixel(int px, int py, int color, t_game *game)
{
	int	center_x;
	int	center_y;
	int	dist_x;
	int	dist_y;

	center_x = MINI_WIDTH / 2;
	center_y = MINI_HEIGHT / 2;
	dist_x = px - center_x;
	dist_y = py - center_y;
	if (dist_x * dist_x + dist_y * dist_y <= RADIUS * RADIUS)
		put_pixel(px, py, color, game);
}

void	draw_full_squar(int x, int y, int size, int color, t_game *game)
{
	int	i;
	int	j;

	i = 0;
	while (i < size)
	{
		j = 0;
		while (j < size)
		{
			draw_square_pixel(x + j, y + i, color, game);
			j++;
		}
		i++;
	}
}