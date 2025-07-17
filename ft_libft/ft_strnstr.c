/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahakki <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 16:15:53 by ahakki            #+#    #+#             */
/*   Updated: 2024/11/04 16:15:59 by ahakki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *str, const char *sub, size_t n)
{
	size_t	i;
	size_t	j;
	char	*st1;
	char	*st2;

	i = 0;
	j = 0;
	st1 = (char *)str;
	st2 = (char *)sub;
	if (!st1 && n == 0)
		return (NULL);
	if (*st2 == '\0')
		return (st1);
	while (st1[i] && i < n)
	{
		while (st1[i + j] == st2[j] && (i + j) < n)
		{
			j++;
			if (st2[j] == '\0')
				return (st1 + i);
		}
		j = 0;
		i++;
	}
	return (NULL);
}
