/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahakki <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 10:12:22 by ahakki            #+#    #+#             */
/*   Updated: 2024/11/01 10:12:30 by ahakki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_counter(long i)
{
	int	c;

	c = 0;
	if (i < 0)
	{
		i = -i;
		c = 1;
	}
	if (i == 0)
		c++;
	while (i > 0)
	{
		c++;
		i = i / 10;
	}
	return (c);
}

char	*ft_itoa(int nb)
{
	int		i;
	char	*ch;
	long	n;	

	n = nb;
	i = ft_counter(n);
	ch = (char *)malloc(i + 1);
	if (!ch)
		return (NULL);
	if (n == 0)
		ch[0] = '0';
	ch[i] = '\0';
	i = i - 1;
	if (n < 0)
	{
		n = -n;
		ch[0] = '-';
	}
	while (n > 0)
	{
		ch[i--] = (n % 10) + 48;
		n /= 10;
	}
	return (ch);
}
