/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   torch_effect.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Copilot Agent                               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 00:00:00 by agent             #+#    #+#             */
/*   Updated: 2025/11/02 00:00:00 by agent            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

/* Torch configuration */
#define TORCH_RADIUS_PIXELS 200
#define TORCH_MIN_BRIGHTNESS 0.15f

/**
 * apply_torch_effect - Applies a centered torch/flashlight effect to the frame
 * 
 * This function darkens the edges of the screen while keeping the center bright,
 * creating a flashlight/torch effect. The effect uses a smooth radial falloff
 * from the screen center.
 * 
 * @game: Pointer to game structure containing image buffer
 * @width: Screen width in pixels
 * @height: Screen height in pixels
 * 
 * The torch is centered at (width/2, height/2) and pixels within TORCH_RADIUS_PIXELS
 * remain at full brightness. Outside that radius, brightness smoothly falls to
 * TORCH_MIN_BRIGHTNESS using cosine interpolation for a soft edge.
 */
void	apply_torch_effect(t_game *game, int width, int height)
{
	int		x;
	int		y;
	int		dx;
	int		dy;
	float	dist;
	float	multiplier;
	int		index;
	int		r;
	int		g;
	int		b;
	float	center_x;
	float	center_y;
	float	radius_sq;
	float	falloff_start_sq;

	center_x = width / 2.0f;
	center_y = height / 2.0f;
	radius_sq = TORCH_RADIUS_PIXELS * TORCH_RADIUS_PIXELS;
	falloff_start_sq = (TORCH_RADIUS_PIXELS * 0.7f) * (TORCH_RADIUS_PIXELS * 0.7f);
	y = 0;
	while (y < height)
	{
		x = 0;
		while (x < width)
		{
			dx = x - center_x;
			dy = y - center_y;
			dist = dx * dx + dy * dy;
			if (dist <= falloff_start_sq)
				multiplier = 1.0f;
			else if (dist >= radius_sq)
				multiplier = TORCH_MIN_BRIGHTNESS;
			else
			{
				/* Smooth cosine falloff */
				float t = (dist - falloff_start_sq) / (radius_sq - falloff_start_sq);
				multiplier = TORCH_MIN_BRIGHTNESS + (1.0f - TORCH_MIN_BRIGHTNESS) * 
					(0.5f * (1.0f + cosf(t * PI)));
			}
			if (multiplier < 1.0f)
			{
				index = y * game->size_line + x * game->bpp / 8;
				r = (unsigned char)game->data[index + 2];
				g = (unsigned char)game->data[index + 1];
				b = (unsigned char)game->data[index];
				r = (int)(r * multiplier);
				g = (int)(g * multiplier);
				b = (int)(b * multiplier);
				game->data[index] = b & 0xFF;
				game->data[index + 1] = g & 0xFF;
				game->data[index + 2] = r & 0xFF;
			}
			x++;
		}
		y++;
	}
}
