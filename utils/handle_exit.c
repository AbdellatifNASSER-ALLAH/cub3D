/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_exit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdnasse <abdnasse@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 14:31:09 by abdnasse          #+#    #+#             */
/*   Updated: 2025/08/07 02:32:27 by abdnasse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	exit_err(const char *msg, int st)
{
	printf(RED"%s\n"RESET, msg);
	exit(st);
}

void	free_lines(char **lines)
{
    int i = 0;
    while (lines && lines[i])
        free(lines[i++]);
    free(lines);
}
