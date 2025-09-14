/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_line.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahakki <ahakki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 14:38:41 by abdnasse          #+#    #+#             */
/*   Updated: 2025/09/11 07:46:21 by ahakki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	wrap_line(t_line *line, char *s)
{
	line->val = s;
	line->i = 0;
	line->size = ft_strlen(s);
	return ;
}

int	peek_line(t_line *line)
{
	if (!line || !line->val || line->i >= line->size)
		return (-1);
	return ((unsigned char)line->val[line->i]);
}

int	next_char_line(t_line *line)
{
	int	c;
	c = peek_line(line);
	if (c != -1)
		line->i++;
	return (c);
}

// void	iter_line(t_line *line, 
