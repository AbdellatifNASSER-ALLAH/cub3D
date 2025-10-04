/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahakki <ahakki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 09:50:24 by ahakki            #+#    #+#             */
/*   Updated: 2025/09/26 09:51:53 by ahakki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** DDA (Digital Differential Analyzer) Algorithm for Raycasting
** ============================================================
**
** The DDA algorithm is used to efficiently find where a ray intersects with
** walls in a grid-based map. It steps through the grid one square at a time,
** checking for wall collisions.
**
** Key Concept:
** Instead of checking every point along the ray, DDA jumps from grid line to
** grid line (either vertical or horizontal), always moving to the nearest
** intersection. This makes it very fast for grid-based raycasting.
**
** The algorithm works in steps:
** 1. Calculate ray direction based on player angle and screen position
** 2. Calculate delta distances (distance ray travels between grid lines)
** 3. Determine step direction (+1 or -1) and initial side distances
** 4. Loop: step to next grid intersection and check for walls
** 5. Stop when a wall is hit
**
** Variables explained:
** - rayDirX, rayDirY: Direction vector of the ray
** - mapX, mapY: Current grid square being checked
** - deltaDistX, deltaDistY: Distance ray travels to cross one grid square
** - sideDistX, sideDistY: Distance from current position to next grid line
** - stepX, stepY: Direction to step in grid (+1 or -1)
** - side: Which side of wall was hit (0 = vertical, 1 = horizontal)
*/

#include "../includes/cub3d.h"

/*
** Initialize ray direction and starting map position
** 
** For each screen column (x), we cast a ray from the player's position.
** The ray angle is calculated by:
** 1. Starting from player's viewing angle
** 2. Subtracting half the FOV (to start at left edge of view)
** 3. Adding angle offset based on screen position (x)
**
** This creates a panoramic view where:
** - Left side of screen (x=0) -> rays point left of player's direction
** - Center of screen (x=WIDTH/2) -> rays point in player's direction  
** - Right side of screen (x=WIDTH) -> rays point right of player's direction
**
** We also calculate:
** - rayDirX, rayDirY: The direction vector (using cos/sin of angle)
** - mapX, mapY: Which grid square the player is currently in
** - px, py: Player's exact position in grid coordinates (not grid square)
*/
void	init_ray_dir_and_map(t_ray *r, t_player *player, int x)
{
	r->ray_angle = player->angle - (FOV / 2) + (x * (FOV / WIDTH));
	r->rayDirX = cos(r->ray_angle);
	r->rayDirY = sin(r->ray_angle);
	r->mapX = (int)(player->x) / BLOCK;
	r->mapY = (int)(player->y) / BLOCK;
	r->px = player->x / BLOCK;
	r->py = player->y / BLOCK;
}

/*
** Calculate delta distances
**
** Delta distances represent how far the ray must travel to cross one grid
** square in each direction (X and Y).
**
** Mathematical explanation:
** - If the ray moves 1 unit in X direction, it travels 1/rayDirX in total
** - We take absolute value because distance is always positive
** - If rayDir is 0 (ray is parallel to that axis), we use a very large
**   number (1e30) to represent "infinite" distance
**
** Example:
** - If rayDirX = 0.5 (ray at 30° angle), deltaDistX = 1/0.5 = 2.0
**   This means the ray travels 2 units to cross one grid square horizontally
** - If rayDirX = 1.0 (ray is horizontal), deltaDistX = 1.0
**   The ray travels exactly 1 unit to cross one grid square
*/
void	init_ray_delta(t_ray *r)
{
	if (r->rayDirX == 0)
		r->deltaDistX = 1e30;
	else
		r->deltaDistX = fabs(1.0f / r->rayDirX);
	if (r->rayDirY == 0)
		r->deltaDistY = 1e30;
	else
		r->deltaDistY = fabs(1.0f / r->rayDirY);
}

/*
** Initialize step directions and initial side distances
**
** Step directions (stepX, stepY):
** - Determine which direction to move in the grid (+1 or -1)
** - Based on ray direction (positive or negative)
** - Example: if ray points right (rayDirX > 0), stepX = 1 (move right)
**
** Initial side distances (sideDistX, sideDistY):
** - Distance from player position to the FIRST grid line the ray will cross
** - Calculated differently based on ray direction:
**
** If ray points right (rayDirX > 0):
** - Need to reach the right edge of current grid square
** - Distance to edge: (mapX + 1.0 - px)
** - Total ray distance: (mapX + 1.0 - px) * deltaDistX
**
** If ray points left (rayDirX < 0):
** - Need to reach the left edge of current grid square  
** - Distance to edge: (px - mapX)
** - Total ray distance: (px - mapX) * deltaDistX
**
** Same logic applies for Y direction (up/down)
**
** These initial distances are crucial for the DDA loop to know where to
** start checking for walls.
*/
void	init_ray_steps(t_ray *r)
{
	if (r->rayDirX < 0)
	{
		r->stepX = -1;
		r->sideDistX = (r->px - r->mapX) * r->deltaDistX;
	}
	else
	{
		r->stepX = 1;
		r->sideDistX = (r->mapX + 1.0f - r->px) * r->deltaDistX;
	}
	if (r->rayDirY < 0)
	{
		r->stepY = -1;
		r->sideDistY = (r->py - r->mapY) * r->deltaDistY;
	}
	else
	{
		r->stepY = 1;
		r->sideDistY = (r->mapY + 1.0f - r->py) * r->deltaDistY;
	}
}

/*
** Initialize a ray for raycasting
**
** This is the main initialization function that sets up all parameters
** needed for the DDA algorithm. It:
** 1. Resets ray state (side, wallX, wallY)
** 2. Calculates ray direction and starting position
** 3. Calculates delta distances
** 4. Sets up step directions and initial side distances
**
** After this function, the ray is ready for the DDA loop.
*/
void	init_ray(t_ray *r, t_player *player, int x)
{
	r->side = -1;
	r->wallX = 0;
	r->wallY = 0;
	init_ray_dir_and_map(r, player, x);
	init_ray_delta(r);
	init_ray_steps(r);
}

/*
** Perform DDA algorithm - the main raycasting loop
**
** This is the core of the DDA algorithm. It steps through the grid square by
** square until it hits a wall.
**
** How it works:
** 1. Compare sideDistX and sideDistY to find which grid line is closer
** 2. Step to the closer grid line:
**    - If sideDistX < sideDistY: cross a vertical line
**      * Increase sideDistX by deltaDistX (move to next vertical line)
**      * Move to next grid square in X direction (mapX += stepX)
**      * Set side = 0 (vertical wall hit)
**    - Otherwise: cross a horizontal line
**      * Increase sideDistY by deltaDistY (move to next horizontal line)
**      * Move to next grid square in Y direction (mapY += stepY)
**      * Set side = 1 (horizontal wall hit)
** 3. Check if current grid square contains a wall ('1') or door ('D')
** 4. If wall found: mark as hit and store wall coordinates
** 5. If no wall: repeat from step 1
**
** Why this is efficient:
** - Only checks grid squares, not every point along the ray
** - Always moves to the nearest grid intersection
** - Guaranteed to find the first wall the ray hits
**
** The 'side' variable is important for rendering:
** - side = 0: ray hit a vertical wall (left/right face)
** - side = 1: ray hit a horizontal wall (top/bottom face)
** This is used later for texture mapping and shading.
*/
void	perform_dda(t_ray *r, t_game *game)
{
	r->hit = 0;
	while (!r->hit)
	{
		// Choose which grid line to cross next (closest one)
		if (r->sideDistX < r->sideDistY)
		{
			r->sideDistX += r->deltaDistX; // Move to next vertical grid line
			r->mapX += r->stepX; // Step to next grid square in X
			r->side = 0; // Vertical wall hit
		}
		else
		{
			r->sideDistY += r->deltaDistY; // Move to next horizontal grid line
			r->mapY += r->stepY; // Step to next grid square in Y
			r->side = 1; // Horizontal wall hit
		}
		// Check if we hit a wall ('1') or door ('D')
		if (game->map[r->mapY][r->mapX] == '1' || \
			game->map[r->mapY][r->mapX] == 'D')
		{
			r->hit = 1; // Wall found, stop the loop
			r->wallX = r->mapX; // Store wall grid coordinates
			r->wallY = r->mapY;
		}
	}
}
