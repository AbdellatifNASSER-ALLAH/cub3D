/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdnasse <abdnasse@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 10:22:21 by abdnasse          #+#    #+#             */
/*   Updated: 2025/09/09 10:22:28 by abdnasse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int is_player(char c) { return (c == 'N' || c == 'S' || c == 'E' || c == 'W'); }
static int is_valid_map_char(char c) { return (is_player(c) || c == '1' || c == '0' || c == 'D' || c == ' '); }

static int is_between_ones(char **map, int x, int y, int width, int height) {
    // Horizontal check
    if (x > 0 && x < width-1 && map[y][x-1] == '1' && map[y][x+1] == '1') return 1;
    // Vertical check
    if (y > 0 && y < height-1 && map[y-1][x] == '1' && map[y+1][x] == '1') return 1;
    return 0;
}

int validate_map(char **map, int height, int width) {
    int player_found = 0;
    for (int y = 0; y < height; ++y) {
        int reached_newline = 0;
        for (int x = 0; x < width; ++x) {
            char c = map[y][x];
            if (c == '\n') {
                reached_newline = 1;
                continue;
            }
            if (reached_newline) {
                if (c != '\n')
                    return 0; // Non-newline after a newline: invalid
                continue;
            }
            if (!is_valid_map_char(c))
                return 0;

            // Neighbors
            char up    = (y > 0)          ? map[y-1][x] : '\0';
            char down  = (y < height-1)   ? map[y+1][x] : '\0';
            char left  = (x > 0)          ? map[y][x-1] : '\0';
            char right = (x < width-1)    ? map[y][x+1] : '\0';

            if (is_player(c)) {
                if (up == ' ' || down == ' ' || left == ' ' || right == ' ' ||
                    up == '\0' || down == '\0' || left == '\0' || right == '\0')
                    return 0;
                player_found++;
            } else if (c == '0') {
                if (up == ' ' || down == ' ' || left == ' ' || right == ' ' ||
                    up == '\0' || down == '\0' || left == '\0' || right == '\0')
                    return 0;
            } else if (c == 'D') {
                if (up == ' ' || down == ' ' || left == ' ' || right == ' ' ||
                    up == '\0' || down == '\0' || left == '\0' || right == '\0')
                    return 0;
                if (!is_between_ones(map, x, y, width, height))
                    return 0;
            }
            // '1' can have any neighbors
        }
    }
    return (player_found == 1); // Must have exactly one player
}
