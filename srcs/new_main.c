/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahakki <ahakki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 07:40:48 by ahakki            #+#    #+#             */
/*   Updated: 2025/10/02 20:05:13 by ahakki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



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