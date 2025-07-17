/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahakki <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 16:12:53 by ahakki            #+#    #+#             */
/*   Updated: 2024/11/04 16:12:57 by ahakki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	size_t	len;
	size_t	i;
	char	*sor;
	char	*scopy;

	len = ft_strlen(s);
	i = 0;
	sor = (char *)s;
	scopy = malloc(len +1);
	if (scopy == NULL)
		return (NULL);
	while (i < len)
	{
		scopy[i] = sor[i];
		i++;
	}
	scopy[i] = '\0';
	return (scopy);
}
