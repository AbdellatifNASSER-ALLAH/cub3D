/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahakki <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 16:45:30 by ahakki            #+#    #+#             */
/*   Updated: 2024/10/24 16:46:11 by ahakki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ftb_putchar(int fd, int c)
{
	c = c + 48;
	write(fd, &c, 1);
}

void	ft_putnbr_fd(int n, int fd)
{
	if (n == 0)
		ftb_putchar(fd, n);
	if (n == -2147483648)
	{
		ft_putnbr_fd((n / 10), fd);
		ftb_putchar(fd, 8);
	}
	else if (n < 0)
	{
		write(fd, "-", 1);
		ft_putnbr_fd(-n, fd);
	}
	else if (n > 0)
	{
		if (n <= 9)
			ftb_putchar(fd, n);
		else
		{
			ft_putnbr_fd((n / 10), fd);
			ftb_putchar(fd, n % 10);
		}
	}
}
