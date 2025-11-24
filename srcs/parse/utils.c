/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdnasse <abdnasse@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 11:19:17 by abdnasse          #+#    #+#             */
/*   Updated: 2025/11/24 17:54:56 by abdnasse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	is_player(char c)
{
	return (c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

int	is_invalid_neighbor(char ch)
{
	return (ch == '\n' || ch == '\0' || ch == ' ' || ch == '\t');
}

char	*skip_ws(char *s)
{
	while (s && (*s == 32 || *s == '\t'))
		s++;
	return (s);
}

int	rgb(char *s, int *rgb)
{
	int	i;

	i = 0;
	while (i < 3)
	{
		s = skip_ws(s);
		if (!ft_isdigit(*s))
			return (0);
		rgb[i++] = ft_atoi(s);
		while (ft_isdigit(*s))
			s++;
		s = skip_ws(s);
		if (i < 2 && *s && *s != ',')
			return (0);
		s++;
	}
	s = skip_ws(s);
	if (*s && *s != '\n')
		return (0);
	if (rgb[0] < 0 || rgb[0] > 255 || rgb[1] < 0 || rgb[1] > 255 || rgb[2] < 0
		|| rgb[2] > 255)
		return (0);
	return (1);
}
