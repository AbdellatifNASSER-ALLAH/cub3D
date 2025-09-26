/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahakki <ahakki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 23:47:58 by ahakki            #+#    #+#             */
/*   Updated: 2025/09/26 09:27:26 by ahakki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void    put_pixel(int x, int y, int color, t_game *game);
void    draw_aim(int cx, int cy, int radius, int color, t_game *game);
void    get_player_cord(t_game *game);
void    get_map(t_game *game);
void    init_game(t_game *game);
void    clear_img(t_game *game);
bool    touch(int px, int py, t_game *game);
void	draw_stripe(int x, t_ray *r, t_game *game);
void    draw_vision(t_game *game);
void    draw_map(t_game *game);
void    draw_minimap(t_game *game);
int     draw_loop(t_game *game);
int     main(int ac, char **av);


void	get_player_cord(t_game *game)
{
	int		y = 0;
	int		x;
	char	dir;

	while (game->map[y])
	{
		x = 0;
		while (game->map[y][x])
		{
			dir = game->map[y][x];
			if (dir == 'N' || dir == 'S' || dir == 'E' || dir == 'W')
			{
				game->player.x = x * BLOCK + BLOCK / 2;
				game->player.y = y * BLOCK + BLOCK / 2;
				if (dir == 'N')
					game->player.angle = 3 * PI / 2;
				else if (dir == 'S')
					game->player.angle = PI / 2;
				else if (dir == 'E')
					game->player.angle = 0;
				else if (dir == 'W')
					game->player.angle = PI;
				return ;
			}
			x++;
		}
		y++;
	}
}


void	get_map(t_game *game)
{
	char **map = malloc(sizeof(char*) * 20);
	map[0]  = "1111111111111111111";
	map[1]  = "1000000000000000001";
	map[2]  = "1000000000000010001";
	map[3]  = "100001N100000000001";
	map[4]  = "1000001000000000001";
	map[5]  = "1000000000011000001";
	map[6]  = "1000000000100100001";
	map[7]  = "1000000000100000001";
	map[8]  = "1000001000100100001";
	map[9]  = "1000000000011000001";
	map[10] = "1000100000000000001";
	map[11] = "1000000010000000001";
	map[12] = "1000000000000100001";
	map[13] = "1000010000000000001";
	map[14] = "1000000000000000001";
	map[15] = "1000000000010000001";
	map[16] = "1000000000000100001";
	map[17] = "1000000000000000001";
	map[18] = "1111111111111111111";
	map[19] = NULL;
	// char **map = malloc(sizeof(char*) * 11);
	// map[0]  = "111111111111";
	// map[1]  = "1N0000000001";
	// map[2]  = "100000000001";
	// map[3]  = "100000000001";
	// map[4]  = "100000000001";
	// map[5]  = "100000000001";
	// map[6]  = "100000N00001";
	// map[7]  = "100000000001";
	// map[8]  = "100000000001";
	// map[9]  = "111111111111";
	// map[10] = NULL;
	game->map = map;
	get_player_cord(game);
}


void	init_game(t_game *game)
{
	game->mlx = mlx_init();
	get_map(game);
	init_player(game);
	game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, "CUB3D");
	game->img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
	game->data = mlx_get_data_addr(game->img, &game->bpp, &game->size_line, &game->endian);
	mlx_put_image_to_window(game->mlx, game->win, game->img, 0 , 0);
}

bool	 touch(int px, int py, t_game *game)
{
	int block_x = px / BLOCK;
	int block_y = py / BLOCK;

	if (game->map[block_y][block_x] == '1' || game->map[block_y][block_x] == 'D')
		return (true);
	return (false);
}

int	draw_loop(t_game *game)
{
	move_player(game);
	draw_vision(game);
	draw_aim(WIDTH / 2, HEIGHT / 2, 7, 0x7FFF00, game);
	draw_minimap(game);
	draw_map(game);

	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
	return (0);
}


int main(int ac, char **av)
{
	t_game	game;
	
	if (ac == 2)
		parse(&game.config, av[1]);
	init_game(&game);
	mlx_hook(game.win, 2, 1L << 0, key_press, &game);
	mlx_hook(game.win, 3, 1L << 1, key_release, &game);
	mlx_hook(game.win, 6, 1L << 6, mouse_move, &game);
	mlx_loop_hook(game.mlx, draw_loop, &game);
	mlx_mouse_hide(game.mlx, game.win);

	mlx_loop(game.mlx);
}
