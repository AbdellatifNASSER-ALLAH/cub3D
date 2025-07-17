/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahakki <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 16:15:01 by ahakki            #+#    #+#             */
/*   Updated: 2024/11/04 16:15:04 by ahakki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t n)
{
	size_t	i;
	size_t	len;
	char	*s;

	i = 0;
	len = ft_strlen(src);
	s = (char *)src;
	if (n == 0)
		return (len);
	while (i < (n - 1) && s[i])
	{
		dst[i] = s[i];
		i++;
	}
	dst[i] = '\0';
	return (len);
}
